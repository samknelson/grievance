<?php

function sirius_edls_supervisor_list($employer_nid) {
	$supervisor_rid = sirius_domain_variable_get('sirius_edls_supervisor_role');
	if (!$supervisor_rid) { return array(); }

	$sql_args = array();

	$sql = "select users.uid, name, mail ";
	$sql .= "from users ";
	$sql .= "left join field_data_field_grievance_shops on field_data_field_grievance_shops.entity_type = 'user' and field_data_field_grievance_shops.entity_id = users.uid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
	$sql .= "left join users_roles on users_roles.uid = users.uid ";

	$sql .= "where users_roles.rid = :supervisor_rid ";
	$sql_args[':supervisor_rid'] = $supervisor_rid;

	if ($employer_nid) {
		$sql .= "and field_grievance_shops_target_id = :employer_nid ";
		$sql_args[':employer_nid'] = $employer_nid;
	}

	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}

	$sql .= "group by users.uid ";
	$sql .= "order by name";

	$stmt = sirius_sql_query($sql, $sql_args);
	$options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$options[$hr['uid']] = $hr['name'];
	}

	return $options;
}

function sirius_edls_supervisor_render($sheet_node) {
	if (! ($sheet_node->field_grievance_rep_assignee['und'][0]['target_id'])) { return ''; }

	$supervisor = user_load($sheet_node->field_grievance_rep_assignee['und'][0]['target_id']);
	$supervisor_name = $supervisor->name;
	$supervisor_name .= '<br />';
	if ($supervisor->field_grievance_phone['und'][0]['value']) {
		$supervisor_name .= sirius_phone_render($supervisor->field_grievance_phone['und'][0]['value']);
		$supervisor_name .= '<br />';
	}
	$supervisor_name .= sirius_email_render($supervisor->mail, array('maxlength' => 15));

	return $supervisor_name;
}