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
		'#size' => 12,
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
		'#value' => 'Submit',
	);

	if ($form_state['result']) {
		$form['result'] = array('#markup' => $form_state['result']);
	}

	return $form;
}

function sirius_worker_page_skill_expiring_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$days = $form_state['values']['days'];
	$days2 = $form_state['values']['days2'];

	$args = array();
	if (is_numeric($days)) {
		$args[':end'] = gmdate('Y-m-d', strtotime("+$days days"));
	} else if ($days) {
		$args[':end'] = gmdate('Y-m-d', strtotime($days));
	}
	if (is_numeric($days2)) {
		$args[':start'] = gmdate('Y-m-d', strtotime("+$days2 days"));
	} else if ($days2) {
		$args[':end'] = gmdate('Y-m-d', strtotime($days));
	}

	$output = $form_state['values']['output'];

	$ms = array_filter(array_values($form_state['values']['ms']));

	$domain_nid = sirius_domain_nid();
	$sql = "select title, name, nid, field_sirius_skill_expire_value, field_sirius_id_value ";
	$sql .= "from node ";
	$sql .= "join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_skill on field_data_field_sirius_skill.entity_type = 'node' and field_data_field_sirius_skill.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_skill_expire on field_data_field_sirius_skill_expire.entity_type = 'node' and field_data_field_sirius_skill_expire.entity_id = node.nid ";
	if ($domain_nid) { $sql .= "join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid "; }
	if ($domain_nid) { $sql .= "join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid "; }
	$sql .= "join taxonomy_term_data on taxonomy_term_data.tid = field_sirius_skill_tid ";
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
	if ($ms) {
		$sql .= "group by field_data_field_sirius_member_status.entity_id ";
	}
	$sql .= "order by field_sirius_skill_expire_value, node.nid ";

	$limit = 1000;
	if ($output == 'csv') { $limit = 100000; }

	$stmt = db_query($sql, $args);
	$rows = array();
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		++$count;
		$row = array();
		$row[] = $hr['title'];
		$row[] = $hr['field_sirius_id_value'];
		$row[] = $hr['name'];
		$row[] = date('Y-m-d', strtotime($hr['field_sirius_skill_expire_value']));
		if ($output == 'csv') {
			$row[] = $hr['nid'];
		} else {
			$row[] = l(t('Manage Skills'), 'node/' . $hr['nid'] . '/sirius_worker_skill', array('attributes' => array('class' => 'tool-button')));
		}
		$rows[] = $row;
	}

	if ($count >= $limit-1) { drupal_set_message("Only showing the first $limit records. Select a CSV export to download the full list."); }

	$header = array('Worker', 'Badge #', 'Skill', 'Expiration', 'NID');

	if ($output == 'csv') {
		$filename = "skill_expiration_report_" . date('YmdHis') . '_' . $days . '_' . $days2 . '.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		$fp = fopen('php://output', 'w');
		fputcsv($fp, $header);
		foreach ($rows as $row) {
			fputcsv($fp, $row);
		}
		drupal_exit();
	}

	$form_state['result'] = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}