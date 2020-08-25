<?php

function sirius_ledger_config_transmittal($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_ledger_transmittal_extra_rows');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Blank Rows'),
    '#description' => t('Enter the number of blank rows to show on the paper transmittal forms.'),
    '#default_value' => variable_get($name, 5),
  );

  $contact_type_options = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name', TRUE);
  $name = sirius_domain_variable_name('sirius_ledger_transmittal_contact_type');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Contact Type for Address'),
    '#description' => t('Select the employer contact type that will be used to fetch the address to display on the transmittal sheet.'),
    '#default_value' => variable_get($name, NULL),
    '#options' => $contact_type_options,
  );

  $name = sirius_domain_variable_name('sirius_ledger_transmittal_header');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Header'),
    '#description' => t('Enter the header as it should appear before the transmittal sheet. May include HTML.'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_ledger_transmittal_footer');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Footer'),
    '#description' => t('Enter the footer as it should appear after the transmittal sheet. May include HTML.'),
    '#default_value' => variable_get($name, ''),
  );

  return system_settings_form($form);
}

function sirius_ledger_transmittal_dispatches($employer_nid) {
	$sql_args = array();
	$sql = "select node.nid, ";
	$sql .= "field_sirius_dispatch_status_value, ";
	$sql .= "job_node.title as job_title, ";
	$sql .= "worker_node.title as worker_title, ";
	$sql .= "field_sirius_payrate_value,  ";
	$sql .= "industry_term.tid as industry_tid, ";
	$sql .= "field_sirius_ssn_value, ";
	$sql .= "worker_json.field_sirius_json_value as worker_json, ";
	$sql .= "group_concat(distinct field_sirius_member_status_tid separator ',') as ms_tids, ";
	$sql .= "field_sirius_contact_target_id as contact_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_payrate on field_data_field_sirius_payrate.entity_type = 'node' and field_data_field_sirius_payrate.entity_id = node.nid ";
	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
	$sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_json worker_json on worker_json.entity_type = 'node' and worker_json.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = job_node.nid ";
	$sql .= "left join taxonomy_term_data industry_term on field_sirius_industry_tid = industry_term.tid ";
	$sql .= "where node.type = 'sirius_dispatch' ";
	$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	$sql_args[':employer_nid'] = $employer_nid;
	$sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
	$sql .= "group by worker_node.nid ";
	$sql .= "order by node.nid ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['nid']] = $hr;
	}
	return $dispatches;
}

function sirius_ledger_transmittal_contact($employer_nid) {
	$contact_type_tid = sirius_domain_variable_get('sirius_ledger_transmittal_contact_type', NULL);
	if (!$contact_type_tid) { return NULL; }

	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'grievance_shop_contact');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->fieldCondition('field_grievance_shops', 'target_id', $employer_nid);
	$query->range(0,1);
	$query->propertyOrderBy('nid', 'desc');
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$contact_nid = array_keys($stmt['node'])[0];
	return node_load($contact_nid);
}
