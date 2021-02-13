<?php

function sirius_office_node_access_operator($node, $op, $account) {
	if (user_access('sirius admin')) { return NODE_ACCESS_IGNORE; }
	if (user_access('sirius office operator admin')) { return NODE_ACCESS_IGNORE; }
	return NODE_ACCESS_DENY;
}

function sirius_office_form_alter_operator_node_form(&$form, &$form_state, $form_id) {
	// Certain fields should only be set on specific tabs.
	$form['field_sirius_users']['#access'] = FALSE;
	$form['field_sirius_json']['#access'] = FALSE;
}

function sirius_office_operator_list($params = array()) {
	$domain_nid = sirius_domain_nid();

	$status = $params['status'];

	$args = array();
	$sql = "select node.nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_operator' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	if ($params['status']) {
		$safe = sirius_jsonfield_util_quote($params['status']);
		$sql .= "and json_contains(field_sirius_json_value, $safe, '$.status.status') ";
	}
	$sql .= "order by node.title ";
	$operator_nids = array();
	$stmt = db_query($sql, $args);
	while ($hr = $stmt->fetchAssoc()) {
		$operator_nids[] = $hr['nid'];
	}
	return node_load_multiple($operator_nids);
}