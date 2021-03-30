<?php

function sirius_ledger_node_tab_auth_events_access($auth_node) {
	if ($auth_node->field_sirius_category['und'][0]['value'] != 'ledger:auth') { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_auth_events($auth_node) {
	return drupal_get_form('sirius_ledger_node_tab_auth_events_form', $auth_node);
}

function sirius_ledger_node_tab_auth_events_form($form, &$form_state, $auth_node) {
	$form_state['auth_nid'] = $auth_node->nid;
  $esignature_status = sirius_esignature_entity_status($auth_node);
  if ($esignature_status['status'] != 'unsigned') {
    $form['intro'] = array('#markup' => '<p><strong>Because this authorization has already been signed, no changes can be made. (Signature status: ' . $esignature_status['status_msg'] . '.)</strong></p>');
  }

	$default = array_keys(sirius_ledger_auth_entities($auth_node, 'sirius_event'));

	// Upcoming events
	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql_args[':cutoff'] = date("Y-m-d 00:00:00", strtotime('-1 days'));
	$sql = "select * from node ";
	$sql .= "left join field_data_field_sirius_daterepeat on field_data_field_sirius_daterepeat.entity_type = 'node' and field_data_field_sirius_daterepeat.entity_id = node.nid and field_data_field_sirius_daterepeat.delta = 0 ";
	$sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join taxonomy_term_data on taxonomy_term_data.tid = field_sirius_event_type_tid ";
	$sql .= "where node.type = 'sirius_event' ";
	$sql .= "and ( ";
	$sql .= "  field_sirius_daterepeat_value >= :cutoff ";
	if ($default) {
		$sql .= "  or nid in (:default) ";
		$sql_args[':default'] = $default;
	}
	$sql .= ") ";
	if ($domain_nid) {
		$sql_args['domain_nid'] = $domain_nid;
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}
	$sql .= "order by field_sirius_daterepeat_value, name, title limit 100 ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$options[$hr['nid']] = $hr['title'] . ' [' . date('Y-m-d', strtotime($hr['field_sirius_daterepeat_value'] . ' GMT')) . ']';
	}

	$form['events'] = array(
		'#type' => 'checkboxes',
		'#options' => $options,
		'#default_value' => $default,
		'#title' => 'Events',
	);

	if ($esignature_status['status'] == 'unsigned') {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	}

	return $form;
}

function sirius_ledger_node_tab_auth_events_form_submit($form, &$form_state) {
	$auth_node = node_load($form_state['auth_nid']);

	$event_nids = array_filter(array_values($form_state['values']['events']));
	sirius_ledger_auth_entities_set($auth_node, 'sirius_event', $event_nids);
	sirius_ledger_auth_set_title($auth_node);
}