<?php

function sirius_training_levels_fetch_all() {
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

	$all_levels = array();
	foreach ($ms_terms as $ms_tid => $ms_term) {
		$levels = sirius_training_levels_fetch($ms_term);
		if ($levels) { $all_levels[$ms_tid] = $levels; }
	}

	return $all_levels;
}

function sirius_training_levels_fetch($ms_term_or_tid) {
	if (is_object($ms_term_or_tid)) { $ms_term = $ms_term_or_tid; } else { $ms_term = sirius_taxonomy_load($ms_term); }
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

function sirius_training_level_notification_emails($worker_node, $ms_term, $level) {
	// The worker's email
	$emails = $worker_node->field_sirius_email['und'][0]['value'];

	// The staff emails
	if ($level['notification_emails']) {
		if ($emails) { $emails .= "\n"; }
		$emails .= $level['notification_emails'];
	}

	// Employer emails. SQL for speed
	$sql = "select nid, field_sirius_json_value from node ";
	$sql .= "join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance_shop' ";
	$sql .= "and field_sirius_active_value = 'Yes' ";
	$sql .= "and json_contains_path(field_sirius_json_value, 'one', '$.training.ms_$ms_term->tid.$level[code].notification_emails') "; 
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$employer_json = json_decode($hr['field_sirius_json_value'], TRUE);
		$employer_emails = $employer_json['training']['ms_' . $ms_term->tid][$level['code']]['notification_emails'];
		if ($employer_emails) {
			if ($emails) { $emails .= "\n"; }
			$emails .= $employer_emails;
		}
	}

	return $emails;
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

		$sql = "select hours_type_tid, sum(hours) as s ";
		$sql .= "from sirius_hours_cache ";
		$sql .= "where worker_nid = :worker_nid ";
		$sql .= "group by hours_type_tid ";
		$stmt = db_query($sql, array(':worker_nid' => $worker_node->nid));
		$grand_total = 0;
		$worker_hours = array();
		while ($hr = $stmt->fetchAssoc()) {
			$worker_hours[$hr['hours_type_tid']] = $hr['s'];
			$worker_hours['TOTAL'] += $hr['s'];
		}

		foreach ($hour_requirements as $hour_requirements) {
			$qty = $hour_requirements['qty'];
			$type = $hour_requirements['type'];
			if (!$qty || !$type) { continue; }
			if ($worker_hours[$type] >= $qty) { continue; }

			if ($type == 'TOTAL') {
				return array('success' => FALSE, 'msg' => t('The worker must have at least @qty total hours.', array('@qty' => $qty)));
			} else {
				$hour_type_term = taxonomy_term_load($type);
				return array('success' => FALSE, 'msg' => t('The worker must have at least @qty hours of type @type.', array('@qty' => $qty, '@type' => $hour_type_term->name)));
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

	// Return
	return array('success' => TRUE);
}