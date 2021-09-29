<?php

function sirius_worker_page_skill_expiring($days = NULL) {
	return drupal_get_form('sirius_worker_page_skill_expiring_form', $days);
}

function sirius_worker_page_skill_expiring_form($form, &$form_state, $days = NULL) {

	$form['days'] = array(
		'#type' => 'textfield',
		'#default_value' => $days,
		'#title' => t('Expires Before'),
		'#description' => t('Enter a date or a number of days (positive or negative).'),
		'#size' => 12,
	);

	$form['days2'] = array(
		'#type' => 'textfield',
		'#title' => t('Expires After'),
		'#description' => t('Search for skills expiring AT LEAST this many days out.'),
	);

	$skills = sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE);
	$skills[NULL] = t('-- All Skills --');
	$form['tid'] = array(
		'#type' => 'select',
		'#options' => $skills,
		'#title' => t('Skill'),
	);

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker Name'),
		'#description' => t('Substing ok; leave blank to search all workers.'),
	);

	$form['id'] = array(
		'#type' => 'textfield',
		'#title' => t('ID (Badge Number)'),
		'#size' => 6,
	);

	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$form['ms'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Member Status'),
		'#options' => $ms_options,
		'#checkall' => TRUE,
		'#prefix' => "<div class=\"checkboxes-inline\">",
		'#suffix' => '</div><br clear="left" />',
	);

	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$form['ws'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Work Status'),
		'#options' => $ws_options,
		'#checkall' => TRUE,
		'#prefix' => "<div class=\"checkboxes-inline\">",
		'#suffix' => '</div><br clear="left" />',
	);


	$form['output'] = array(
		'#type' => 'select',
		'#title' => t('Output'),
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('CSV'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['result']) {
		if ($form_state['result']['rows']) {

			$form['worker_action'] = array(
				'#type' => 'select',
				'#title' => t('Action for Selected Workers'),
				'#options' => array(
					NULL => t('-- Select --'),
					'event_register' => t('Register for an Event'),
				),
			);

			// Upcoming events
			$event_options = sirius_event_upcoming();
			$form['class_nid'] = array(
				'#type' => 'select',
				'#title' => 'Event',
				'#options' => $event_options,
				'#states' => array('visible' => array(':input[name="worker_action"]' => array('value' => 'event_register'))),
			);

			$form['worker_nids'] = array(
				'#type' => 'tableselect',
				'#title' => 'Results',
				'#header' => $form_state['result']['header'],
				'#options' => $form_state['result']['rows'],
			);

		} else {
			drupal_set_message("No workers match your search criteria.");
		}
	}

	return $form;
}

function sirius_worker_page_skill_expiring_form_submit($form, &$form_state) {
	// Handle redirect to register
	if ($form_state['values']['worker_action'] == 'event_register') {
		$worker_nids = array_filter(array_values($form_state['values']['worker_nids']));
		$event_nid = $form_state['values']['class_nid'];
		if (!$event_nid) { 
			drupal_set_message("Please select a class.", 'error');
		} else if (!$worker_nids) { 
			drupal_set_message("Please select at least one worker", 'error');
		} else {
			$widget = 'worker_byname';
			$result = sirius_worker_stupid_event_widget_finder($event_nid, $widget);
			if (!$result['success']) {
				drupal_set_message($result['msg'], 'error');
			} else {
				$path = 'node/' . $event_nid . '/sirius_event_participants_register';
				$query = array(
					'delta' => $result['delta'],
					'widget' => $widget,
					'worker_nids' => join(',', $worker_nids),
				);
				drupal_goto($path, array('query' => $query));
			}
		}
	}

	$form_state['rebuild'] = TRUE;
	$days = $form_state['values']['days'];
	$days2 = $form_state['values']['days2'];

	$args = array();
	if (is_numeric($days)) {
		$args[':end'] = gmdate('Y-m-d', strtotime("+$days days"));
	} else if ($days) {
		$args[':end'] = gmdate('Y-m-d', strtotime($days));
	} else {
		$args[':end'] = gmdate('Y-m-d', strtotime("+10000 days"));
	}
	if (is_numeric($days2)) {
		$args[':start'] = gmdate('Y-m-d', strtotime("+$days2 days"));
	} else if ($days2) {
		$args[':start'] = gmdate('Y-m-d', strtotime($days2));
	}

	$output = $form_state['values']['output'];

	$ms = array_filter(array_values($form_state['values']['ms']));
	$ws = array_filter(array_values($form_state['values']['ws']));

	$domain_nid = sirius_domain_nid();
	$sql = "select title, name, nid, field_sirius_skill_expire_value, field_sirius_id_value, field_sirius_email_value, field_sirius_phone_value ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_skill on field_data_field_sirius_skill.entity_type = 'node' and field_data_field_sirius_skill.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_skill_expire on field_data_field_sirius_skill_expire.entity_type = 'node' and field_data_field_sirius_skill_expire.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_email on field_data_field_sirius_email.entity_type = 'node' and field_data_field_sirius_email.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
	if ($domain_nid) { $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid "; }
	if ($ms) { $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid "; }
	if ($ws) { $sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid "; }
	$sql .= "left join taxonomy_term_data on taxonomy_term_data.tid = field_sirius_skill_tid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill.delta = field_data_field_sirius_skill_expire.delta ";
	if ($args[':start']) {
		$sql .= "and field_sirius_skill_expire_value >= :start ";
	}
	if ($args[':end']) {
		$sql .= "and field_sirius_skill_expire_value <= :end ";
	}
	if ($form_state['values']['name']) {
		$sql .= "and node.title like :name ";
		$args['name'] = '%' . $form_state['values']['name'] . '%';
	}
	if ($form_state['values']['tid']) {
		$sql .= "and tid = :tid ";
		$args['tid'] = $form_state['values']['tid'];
	}
	if ($form_state['values']['id']) {
		$sql .= "and field_sirius_id_value = :id ";
		$args['id'] = $form_state['values']['id'];
	}
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	if ($ms) {
		$sql .= "and field_sirius_member_status_tid in (:ms) ";
		$args[':ms'] = $ms;
	}
	if ($ws) {
		$sql .= "and field_sirius_work_status_tid in (:ws) ";
		$args[':ws'] = $ws;
	}
	if ($ms) {
		$sql .= "group by field_data_field_sirius_member_status.entity_id ";
	}
	$sql .= "order by field_sirius_skill_expire_value, node.nid ";

	$limit = 1000;
	if ($output == 'csv') { $limit = 100000; }

	$header = array(t('Worker'), t('Badge #'), t('Skill'), t('Phone'), t('Email'), t('Expiration'), t('Tools'));

	if ($output == 'csv') {
		$filename = "skill_expiration_report_" . date('YmdHis') . '_' . $days . '_' . $days2 . '.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		$fp = fopen('php://output', 'w');
		fputcsv($fp, $header);
	}


	$stmt = db_query($sql, $args);
	$rows = array();
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		++$count;
		$row = array();
		$row[] = $hr['title'];
		$row[] = $hr['field_sirius_id_value'];
		$row[] = $hr['name'];
		$row[] = $hr['field_sirius_phone_value'];
		$row[] = $hr['field_sirius_email_value'];
		$row[] = date('Y-m-d', strtotime($hr['field_sirius_skill_expire_value']));
		if ($output == 'csv') {
			$row[] = $hr['nid'];
		} else {
			$row[] = l(t('Manage Skills'), 'node/' . $hr['nid'] . '/sirius_worker_skill', array('attributes' => array('class' => 'tool-button')));
		}

		if ($output == 'csv') {
			fputcsv($fp, $row);
		} else {
			$rows[$hr['nid']] = $row;
		}
	}

	if ($output == 'csv') {
		drupal_exit();
	}

	if ($count >= $limit-1) { drupal_set_message("Only showing the first $limit records. Select a CSV export to download the full list."); }

	$form_state['result'] = array('header' => $header, 'rows' => $rows);
}


















function sirius_worker_stupid_event_widget_finder($event_nid, $widget_name) {
	$event_node = node_load($event_nid);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$all_rules = $json['register']['rules'];
	if (user_access('sirius event admin')) {
		$rules = $all_rules;
	} else {
		$rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE);
	}
	$widgets = sirius_event_register_widgets();
	if (!$rules) {
		return array('success' => FALSE, 'msg' => 'You do not have access to any registration rules for this event type.');
	}
	foreach ($rules as $delta => $rule) {
		if ($rule['widget'][$widget_name]) {
			return array('success' => TRUE, 'delta' => $delta);
		}
	}
	return array('success' => FALSE, 'msg' => "You do not have access to use the registration tool [$widget_name] in the specied event.");
}