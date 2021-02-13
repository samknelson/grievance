<?php

function sirius_office_node_tab_operator_users($operator_node) {
	return drupal_get_form('sirius_office_node_tab_operator_users_form', $operator_node);
}

function sirius_office_node_tab_operator_users_form($form, &$form_state, $operator_node) {
	$form_state['operator_nid'] = $operator_node->nid;

	// List of users with the given domain and permission
	$domain_nid = $operator_node->field_sirius_domain['und'][0]['target_id'];
	$args = array();
	$sql = "select distinct users.uid, users.name, users.mail from users ";
	$sql .= "left join users_roles on users_roles.uid = users.uid ";
	$sql .= "left join role_permission ON users_roles.rid = role_permission.rid ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
	}
	$sql .= "where users.uid != 0 ";
	$sql .= "and users.uid != 1 ";
	$sql .= "and role_permission.permission = :permission ";
	$args[':permission'] = 'sirius office operator user';
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	$sql .= "order by users.name limit 1000 ";
	$stmt = db_query($sql, $args);
	$user_options = array(NULL => t('-- Select --'));
	while ($hr = $stmt->fetchAssoc()) {
		$user_options[$hr['uid']] = $hr['name'] . ' [' . $hr['mail'] . ']';
	}

	$max = 2;
	if ($operator_node->field_sirius_users['und'][0]['target_id']) {
		$max = count($operator_node->field_sirius_users['und']) + 2;
	}

	for ($delta=0; $delta<$max; ++$delta) {
		$form["user_$delta"] = array(
			'#title' => t('User #@i', array('@i' => $delta+1)),
			'#type' => 'select',
			'#options' => $user_options,
			'#default_value' => $operator_node->field_sirius_users['und'][$delta]['target_id'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_office_node_tab_operator_users_form_submit($form, &$form_state) {
	$operator_node = node_load($form_state['operator_nid']);
	$delta = 0;
	$values = array();
	while ($form_state['values']["user_$delta"]) {
		$values[] = array('target_id' => $form_state['values']["user_$delta"]);
		++$delta;
	}
	$operator_node->field_sirius_users['und'] = $values;
	node_save($operator_node);
	drupal_set_message(t('Users saved.'));
}

