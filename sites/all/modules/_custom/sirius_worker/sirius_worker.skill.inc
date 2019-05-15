<?php

/**
 * Delete a skill term
 */

function sirius_worker_skill_term_delete() {
	return drupal_get_form('sirius_worker_skill_term_delete_form');
}

function sirius_worker_skill_term_delete_form($form, &$form_state) {
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Use this form to delete a skill permanently and globally. This action cannot be undone.') . '</div>');

	$form['tid'] = array(
		'#title' => t('Skill'),
		'#type' => 'select',
		'#options' => sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE),
		'#required' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, permanently and completely remove this skill from the system.',
		'#value' => 1,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_worker_skill_term_delete_form_submit($form, &$form_state) {
	// Init
	$tid = $form_state['values']['tid'];

	$operations = array();
	$sql = "select entity_id from field_data_field_sirius_skill ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_worker' ";
	$sql .= "and field_sirius_skill_tid = :tid ";
	$sql .= "order by entity_id ";
	$stmt = db_query($sql, array(':tid' => $tid));
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['entity_id'];
	}

	$count = 0;
	foreach ($worker_nids as $worker_nid) {
		$operations[] = array('sirius_worker_skill_set', array($worker_nid, array($tid), NULL, 'remove', 'Skill was deleted', array()));
		++$count;
		// if ($count > 5) { break; }
	}
  $operations[] = array('taxonomy_term_delete', array($tid));

	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
  );
  batch_set($batch);

	return;
}

function sirius_worker_skill_check($worker_node, $skill_term) {
	if (!$worker_node->field_sirius_skill['und']) { return FALSE; }

	foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
		if ($tmp['tid'] == $skill_term->tid) { return TRUE; }
	}

	return FALSE;
}

function sirius_worker_skill_set($worker_node, $tids, $expires, $action, $reason, $files) {
	if (is_numeric($worker_node)) { $worker_node = node_load($worker_node); }
	if ($expires) {
		$expires_display = date('m/d/Y', strtotime($expires));
	} else {
		$expires = date('Y-m-d', strtotime('+100 years'));
		$expires_display = 'Never';
	} 

	// Adding an expired skill does nothing. But we still log the attempt
	if ($action == 'add' && (strtotime($expires) < time())) {
		$msg = 'Skipping add for skills, because they expired on ' . $expires . ': ';
		foreach ($tids as $tid) {
			$term = taxonomy_term_load($tid);
			$msg .= $term->name;
		}
		sirius_log('worker:skill', 'add_expired', $msg, $reason, '', array($worker_node->nid), $files);
		return;
	}

	$orig = array();
	if ($worker_node->field_sirius_skill['und']) {
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			$tid = $tmp['tid'];
			if (!isset($orig[$tid])) {
				$orig[$tid] = $worker_node->field_sirius_skill_expire['und'][$delta]['value'];
			}
		}
	}

	$new = array();
	foreach ($tids as $tid) {
		if (!isset($new[$tid])) {
			$new[$tid] = $expires;
		}
	}

	$msg = '';
	if ($action == 'add') {
		foreach ($new as $tid => $expires) {
			$orig[$tid] = $expires;
			$term = taxonomy_term_load($tid);
			$msg .= "Add or renew $term->name (expires $expires_display); ";
		}
	} else {
		foreach ($new as $tid => $ignore) {
			unset($orig[$tid]);
			$term = taxonomy_term_load($tid);
			$msg .= "Remove $term->name; ";
		}
	}

	$worker_node->field_sirius_skill['und'] = array();
	$worker_node->field_sirius_skill_expire['und'] = array();
	foreach ($orig as $tid => $exp) {
		$worker_node->field_sirius_skill['und'][] = array('tid' => $tid);
		$worker_node->field_sirius_skill_expire['und'][] = array('value' => date('Y-m-d', strtotime($exp)));
	}

	node_save($worker_node);
	sirius_log('worker:skill', $action, $msg, $reason, '', array($worker_node->nid), $files);

	return array('success' => TRUE, 'msg' => $msg);
}


function sirius_worker_skills($worker_node) {
	$skill_tids = array();

	// First add all the entries from the worker node
	if ($worker_node->field_sirius_skill) {
	foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_tids[$skill_tid] = TRUE;
		}
	}

  // Load all and return
  return taxonomy_term_load_multiple(array_keys($skill_tids));
}

function sirius_worker_skills_cron($force = FALSE) {
	// Run once a day
	$today = date('Y-m-d');
	$last_run = variable_get('sirius_worker_skills_last_date', 0);
	if ($last_run == $today && !$force) { return; }
	variable_set('sirius_worker_skills_last_date', $today);

	// Don't run before a specific time
	$hour = variable_get('sirius_worker_skills_start_hr', 1);
	$min = variable_get('sirius_worker_skills_start_min', 30);
	if (time() < strtotime("$today $hour:$min") && !$force) { return; }

	// For skills that will be expiring soon, send workers notifications.
	$warnings = array(
		'first' => array(date('Y-m-d', strtotime('26 days')), date('Y-m-d', strtotime('+30 days'))),
		'second' => array(date('Y-m-d', strtotime('+4 days')), date('Y-m-d', strtotime('+7 days'))),
	);
	foreach ($warnings as $which => $cutoffs) {
		$start = $cutoffs[0];
		$end = $cutoffs[1];
		$sql = "select * from field_data_field_sirius_skill, field_data_field_sirius_skill_expire  "; 
		$sql .= "where field_data_field_sirius_skill.entity_type = 'node' and field_data_field_sirius_skill.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_skill_expire.entity_type = 'node' and field_data_field_sirius_skill_expire.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_skill.entity_id = field_data_field_sirius_skill_expire.entity_id ";
		$sql .= "and field_data_field_sirius_skill.delta = field_data_field_sirius_skill_expire.delta ";
		$sql .= "and field_sirius_skill_expire_value <= :end ";
		$sql .= "and field_sirius_skill_expire_value >= :start ";
		$stmt = db_query($sql, array(':end' => $end, ':start' => $start));
		$to_remove = array();
		while ($hr = $stmt->fetchAssoc()) {
			$date = date('Y-m-d', strtotime($hr['field_sirius_skill_expire_value']));
			$to_remove[$hr['entity_id']][$date][] = $hr['field_sirius_skill_tid'];
		}
		$notification_count = 0;
		foreach ($to_remove as $worker_nid => $dates) {
			$worker_node = node_load($worker_nid);

			foreach ($dates as $date => $tids) {
				$terms = taxonomy_term_load_multiple($tids);
				$subject = "Dispatch system notification: skills expiring on $date";
				$body = "The following skills will be expiring on $date:<ul>";
				foreach ($terms as $tid => $term) {
					$body .= '<li>' . $term->name . '</li>';
				}
				$body .= '</ul>';
				$body .= 'Please contact the dispatch office to find out how to renew your certifications.';

				$sms = "Dispatch system notification: skills expiring on $date";
				$sms = "The following skills will be expiring on $date:\n";
				foreach ($terms as $tid => $term) {
					$sms .= $term->name . "\n";
				}
				$sms .= "\n";
				$sms .= 'Please contact the dispatch office to find out how to renew your certifications.';

				$unique_id = "UNIQUE/sirius-skill-expire-warning-$which/$worker_nid/$date";

				$result = sirius_worker_notify(
					$worker_node, 
					array(
						'subject' => $subject,
						'body' => $body,
						'sms' => $sms,
						'X-Sirius-Id' => $unique_id,
					)
				);

				if ($result['success']) { 
					++$notification_count;
				} 
			}
		}
	}

	// For skills that expired yesterday, remove them from the worker record.
	$cutoff = date('Y-m-d', strtotime('-1 day'));
	$sql = "select * from field_data_field_sirius_skill, field_data_field_sirius_skill_expire  "; 
	$sql .= "where field_data_field_sirius_skill.entity_type = 'node' and field_data_field_sirius_skill.bundle = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill_expire.entity_type = 'node' and field_data_field_sirius_skill_expire.bundle = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill.entity_id = field_data_field_sirius_skill_expire.entity_id ";
	$sql .= "and field_data_field_sirius_skill.delta = field_data_field_sirius_skill_expire.delta ";
	$sql .= "and field_sirius_skill_expire_value <= :cutoff ";
	$stmt = db_query($sql, array(':cutoff' => $cutoff));
	$to_remove = array();
	$to_remove_dates = array();
	$skill_count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$to_remove[$hr['entity_id']][] = $hr['field_sirius_skill_tid'];
		$to_remove_dates[$hr['entity_id']][] = date('Y-m-d', strtotime($hr['field_sirius_skill_expire_value']));
		++$skill_count;
	}

	$worker_count = 0;
	foreach ($to_remove as $worker_nid => $tids) {
		++$worker_count;
		$worker_node = node_load($worker_nid);
		sirius_worker_skill_set($worker_node, $tids, NULL, 'remove', 'Expiration date for the skills has passed.', array());

		$terms = taxonomy_term_load_multiple($tids);
		$subject = "Dispatch system notification: skills expired";
		$body = "The following skills have expired<ul>";
		foreach ($terms as $tid => $term) {
			$body .= '<li>' . $term->name . '</li>';
		}
		$body .= '</ul>';
		$body .= 'Please contact the dispatch office to find out how to renew your certifications.';

		$sms = "Dispatch system notification: the following skills have expired\n";
		foreach ($terms as $tid => $term) {
			$sms .= $term->name . "\n";
		}
		$sms .= "\n";
		$sms .= 'Please contact the dispatch office to find out how to renew your certifications.';

		$result = sirius_worker_notify(
			$worker_node, 
			array(
				'subject' => $subject,
				'body' => $body,
				'sms' => $sms,
			)
		);
	}

	watchdog('sirius_worker_skills', "Scanned for expired skills; removed $skill_count skills from $worker_count workers. Sent $notification_count notifications of upcoming skill expiration.");
}
