<?php

function sirius_training_levels_fetch_all() {
	$all_levels = &drupal_static(__FUNCTION__);
	if (!$all_levels) {
		$all_levels = array();
		$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

		foreach ($ms_terms as $ms_tid => $ms_term) {
			$levels = sirius_training_levels_fetch($ms_term);
			if ($levels) { $all_levels[$ms_tid] = $levels; }
		}
	}

	return $all_levels;
}

function sirius_training_levels_fetch_record($ms_tid, $level_code) {
	$all_levels = sirius_training_levels_fetch_all();
	return $all_levels[$ms_tid][$level_code];
}

function sirius_training_levels_fetch_title($ms_tid, $level_code) {
	$record = sirius_training_levels_fetch_record($ms_tid, $level_code);
	return $record['title'];
}

function sirius_training_levels_fetch($ms_term_or_tid) {
	if (is_object($ms_term_or_tid)) { $ms_term = $ms_term_or_tid; } else { $ms_term = taxonomy_term_load($ms_term_or_tid); }
	$json = json_decode($ms_term->field_sirius_json['und'][0]['value'], TRUE);
	if ($json['training']['levels_enabled'] != 'Yes') { return array(); }

	$levels = $json['training']['levels'];
	uasort($levels, function($a,$b) { return $a['weight'] <=> $b['weight']; });
	return $levels;
}

function sirius_training_levels_fetch_worker($worker_node) {
	$json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
	return $json['training']['levels'];
}

function sirius_training_level_notification_recipients($worker_node, $ms_term, $level) {
	// The staff emails
	$recipients = array();
	if (is_array($level['notification_emails'])) {
		foreach ($level['notification_emails'] as $candidate) {
			if ($candidate['email'] || $candidate['name']) {
				$candidate['disposition'] = 'to';
				$candidate['description'] = 'Staff contact';
				$recipients[] = $candidate;
			}
		}
	}

	// Employer emails. SQL for speed
	$sql = "select nid, node.title as shop_name, field_sirius_json_value from node ";
	$sql .= "join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance_shop' ";
	$sql .= "and field_sirius_active_value = 'Yes' ";
	$sql .= "and json_contains_path(field_sirius_json_value, 'one', '$.training.ms_$ms_term->tid.$level[code].notification_emails') "; 
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$employer_json = json_decode($hr['field_sirius_json_value'], TRUE);
		if (is_array($employer_json['training']['ms_' . $ms_term->tid][$level['code']]['notification_emails'])) {
			foreach ($employer_json['training']['ms_' . $ms_term->tid][$level['code']]['notification_emails'] as $candidate) {
				if ($candidate['email'] || $candidate['name']) { 
					$candidate['disposition'] = 'to';
					$candidate['description'] = "Shop contact for " . $hr['shop_name'];
					$recipients[] = $candidate;
				}
			}
		}
	}

	return $recipients;
}

function sirius_training_level_graduate_possible($worker_node, $ms_term, $level, $params) {
	// Can't change to the current level
	$current_levels = sirius_training_levels_fetch_worker($worker_node);
	if ($current_levels[$ms_term->tid]['code'] == $level['code']) {
		return array('success' => FALSE, 'msg' => 'The worker already has that level.');
	}

	// Check the requirements
	$json = json_decode($ms_term->field_sirius_json['und'][0]['value'], TRUE);
	$hour_requirements = $json['training']['levels'][$level['code']]['hour_requirements'];
	if ($hour_requirements) {

		$sql = "select department_tid, sum(hours) as s ";
		$sql .= "from sirius_hours_cache ";
		$sql .= "where worker_nid = :worker_nid ";
		$sql .= "group by department_tid ";
		$stmt = db_query($sql, array(':worker_nid' => $worker_node->nid));
		$grand_total = 0;
		$worker_hours = array();
		while ($hr = $stmt->fetchAssoc()) {
			$worker_hours[$hr['department_tid']] = $hr['s'];
			$worker_hours['TOTAL'] += $hr['s'];
		}

		foreach ($hour_requirements as $hour_requirements) {
			$qty = $hour_requirements['qty'];
			$dept_tid = $hour_requirements['dept'];
			if (!$qty || !$dept_tid) { continue; }
			if ($worker_hours[$dept_tid] >= $qty) { continue; }

			if ($dept_tid == 'TOTAL') {
				return array('success' => FALSE, 'msg' => t('The worker must have at least @qty total hours.', array('@qty' => $qty)));
			} else {
				$dept_term = taxonomy_term_load($dept_tid);
				return array('success' => FALSE, 'msg' => t('The worker must have at least @qty hours of type @type.', array('@qty' => $qty, '@type' => $dept_term->name)));
			}
		}
	}

	return array('success' => TRUE);
}

function sirius_training_level_graduate($worker_node, $ms_term, $level, $params) {
	// Possible?
	$possible = sirius_training_level_graduate_possible($worker_node, $ms_term, $level, $params);
	if (!$possible['success']) {
		return $possible;
	}

	// Set the level
	$json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
	$level_code = $level['code'];
	$json['training']['levels'][$ms_term->tid] = array(
		'code' => $level_code,
		'ts' => time(),
	);
	$worker_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($worker_node);

	// Log
	sirius_log(
		'worker:level', 
		'set', 
		$worker_node->title . "'s level has been set to " . $level['title'], 
		'', 
		'', 
		array($worker_node->nid),
		array(),
		array('ms_tid' => $ms_term->tid, 'level' => $level, 'params' => $params)
	);

	// Email
	

	// Return
	return array('success' => TRUE);
}

function sirius_training_render_levels($worker_node, $options = array()) {
	$is_admin = $options['is_admin'];

	$all_levels = sirius_training_levels_fetch_all();
	$worker_levels = sirius_training_levels_fetch_worker($worker_node);

	$html = '';
	foreach ($all_levels as $ms_tid => $levels) {
		if ($html) { $html .= '<br /><br />'; }
		$ms_term = taxonomy_term_load($ms_tid);
		$level_code = $worker_levels[$ms_tid]['code'];
		$level = $levels[$level_code];
		$worker_level = $worker_levels[$ms_tid];

		$html .= '<strong>' . $ms_term->name . '</strong><br />';
		if ($worker_level) {
			$html .= sirius_fakerender_field('Level:', $level['title']);
			$html .= sirius_fakerender_field('Date:', date('Y-m-d H:i:s', $worker_level['ts'])); 
			if ($payrate_skilled) { $html .= sirius_fakerender_field('Payrate (Skilled):', '$' . number_format($level['payrate_skilled'], 2)); }
			if ($payrate_unskilled) { $html .= sirius_fakerender_field('Payrate (Unskilled):', '$' . number_format($level['payrate_unskilled'], 2)); }
		} else {
			$html .= sirius_fakerender_field('Level:', t('(no level)'));
		}

		if ($is_admin) {
			$html .= '<strong>' . t('Change level to:') . '</strong> ';
			foreach ($levels as $level) {
				$html .= l($level['title'], current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('ms_tid' => $ms_tid, 'graduate' => $level['code'])));
				$html .= ' ';
			}
		}
	}

	return $html;
}