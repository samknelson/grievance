<?php

function sirius_event_register_user_select_form($form, &$form_state, $event_node, $rule) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['rule'] = $rule;
	$form_state['participant_role_tid'] = $participant_role_tid;

	$domain_nid = sirius_domain_nid();

	$rids = array_keys($rule['participant_user_role']);
	$args = array();
	$sql = "select node.nid, node.title ";
	$sql .= "from users ";
	$sql .= "join users_roles on users_roles.uid = users.uid and users_roles.rid in (:rids) ";
	$args[':rids'] = $rids;
	$sql .= "join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'user' and field_data_field_sirius_contact.entity_id = users.uid ";
	$sql .= "join node on field_sirius_contact_target_id = node.nid ";
	if ($domain_nid) {
		$sql .= "join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	$sql .= "where users.status = 1 ";
	$sql .= "group by users.uid ";
	$sql .= "order by users.name ";
	$sql .= "limit 1000 ";
	$stmt = db_query($sql, $args);
	$users = array();
	while ($hr = $stmt->fetchAssoc()) { $users[$hr['nid']] = $hr['title']; }
	if (count($users) > 999) {
		drupal_set_message(t('The user select widget is limited to 1000 records. Consider using the by-name widget instead.'), 'warning');
	}

	$pstatus_tids = sirius_event_participant_role_status_tids($prole_tid);

	$form['contact_nid'] = array(
		'#title' => 'Register this user',
		'#type' => 'select',
		'#options' => $users,
		'#required' => TRUE,
	);

	foreach ($rule['register_as'] as $key) {
		list ($prole_tid, $pstatus_tid) = explode('|', $key);
		$prole_term = taxonomy_term_load($prole_tid);
		$pstatus_term = taxonomy_term_load($pstatus_tid);
		$register_as_options[$key] = $prole_term->name . ': ' . $pstatus_term->name;
	}

	$form['register_as'] = array(
		'#title' => 'Register As',
		'#type' => 'select',
		'#options' => $register_as_options,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Register',
	);

	return $form;
}

function sirius_event_register_user_select_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$contact_nid = $form_state['values']['contact_nid'];
	$register_as = $form_state['values']['register_as'];
	list ($prole_tid, $pstatus_tid) = explode('|', $register_as);
	$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
	if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'error'); }
}
