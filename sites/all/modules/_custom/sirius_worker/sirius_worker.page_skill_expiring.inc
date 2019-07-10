<?php

function sirius_worker_page_skill_expiring($days = NULL) {
	return drupal_get_form('sirius_worker_page_skill_expiring_form', $days);
}

function sirius_worker_page_skill_expiring_form($form, &$form_state, $days = NULL) {
	$form['days'] = array(
		'#type' => 'textfield',
		'#default_value' => $days,
		'#title' => t('Days'),
		'#description' => t('Search for skills expiring within this many days.'),
		'#size' => 4,
		'#required' => TRUE,
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

	$args = array();
	$args[':start'] = gmdate('Y-m-d');
	$args[':end'] = gmdate('Y-m-d', strtotime("+$days days"));

	$sql = "select title, name, nid, field_sirius_skill_expire_value, field_sirius_id_value ";
	$sql .= "from node ";
	$sql .= "join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_skill on field_data_field_sirius_skill.entity_type = 'node' and field_data_field_sirius_skill.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_skill_expire on field_data_field_sirius_skill_expire.entity_type = 'node' and field_data_field_sirius_skill_expire.entity_id = node.nid ";
	$sql .= "join taxonomy_term_data on taxonomy_term_data.tid = field_sirius_skill_tid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill.delta = field_data_field_sirius_skill_expire.delta ";
	$sql .= "and field_sirius_skill_expire_value >= :start ";
	$sql .= "and field_sirius_skill_expire_value <= :end ";
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
	$sql .= "order by field_sirius_skill_expire_value, node.nid ";
	$sql .= "limit 5000 ";

	$stmt = db_query($sql, $args);
	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$row[] = $hr['title'];
		$row[] = $hr['field_sirius_id_value'];
		$row[] = $hr['name'];
		$row[] = date('Y-m-d', strtotime($hr['field_sirius_skill_expire_value']));
		$row[] = l(t('Manage Skills'), 'node/' . $hr['nid'] . '/sirius_worker_skill', array('attributes' => array('class' => 'tool-button')));
		$rows[] = $row;
	}

	$header = array('Worker', 'Badge #', 'Skill', 'Expiration', 'NID');
	$form_state['result'] = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}