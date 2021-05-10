<?php

function sirius_ledger_config_transmittal($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_ledger_transmittal_extra_rows');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Blank Rows'),
    '#description' => t('Enter the number of blank rows to show on the paper transmittal forms.'),
    '#default_value' => variable_get($name, 5),
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

	$options = array(NULL => t('No Letterhead'));
	$letterheads = sirius_letterhead_list();
	foreach ($letterheads as $nid => $title) { $options[$nid] = $title; }
  $name = sirius_domain_variable_name('sirius_ledger_transmittal_letterhead');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Letterhead'),
    '#default_value' => variable_get($name, ''),
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

  $name = sirius_domain_variable_name('sirius_ledger_transmittal_address_left');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Left margin for address'),
    '#default_value' => variable_get($name, '0.41in'),
    '#description' => t('Enter the left margin that should be used for the address box so that it fits into an envelope display window. This should be 1in from the left of the page, but different letterheads have different left margins. If letterhead is not being used, this should be 1in - 0.59in = 0.41in.'),
  );

  $name = sirius_domain_variable_name('sirius_ledger_transmittal_address_top');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Top margin for address'),
    '#default_value' => variable_get($name, '1.5975in'),
    '#description' => t('Enter the top margin that should be used for the address box so that it fits into an envelope display window. This should be 2.1875in from the top of the page, but different letterheads have different left margins. If letterhead is not being used, this should be 2.1875in - 0.59in = 1.5975in.'),
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
	$sql .= "worker_node.nid as worker_nid, ";
	$sql .= "field_sirius_contact_target_id as contact_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_payrate on field_data_field_sirius_payrate.entity_type = 'node' and field_data_field_sirius_payrate.entity_id = node.nid ";
	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
	$sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = worker_node.nid ";
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
	$sql .= "order by field_sirius_name_family, field_sirius_name_given ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['nid']] = $hr;
	}
	return $dispatches;
}

function sirius_ledger_transmittal_list($params = array()) {
  $domain_nid = sirius_domain_nid();

  $sql_args = array();
  $sql = "select node.nid as transmittal_nid, ";
  $sql .= "field_sirius_type_value as status, ";
  $sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.year')) as year, ";
  $sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.month')) as month, ";
  if ($params['account_nid']) {
    $sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.accounts.\"" . $params['account_nid'] . "\".payment_nid')) as payment_nid, ";
    $sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.accounts.\"" . $params['account_nid'] . "\".dollar_amt')) as dollar_amt, ";
  } 
  $sql .= "employer_node.nid as employer_nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
  $sql .= "left join node employer_node on employer_node.nid = field_sirius_log_handler_target_id ";
   if ($domain_nid) {
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  }
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and employer_node.type = 'grievance_shop' ";
  $sql .= "and field_sirius_category_value = 'ledger:transmittal' ";
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  if ($params['year']) { 
    $sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.year')) = :year ";
    $sql_args[':year'] = $params['year'];
  }
  if ($params['month']) { 
    $sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.transmittal.month')) = :month ";
    $sql_args[':month'] = $params['month'];
  }
  if ($params['account_nid']) {
    $path = '$.transmittal.accounts."' . $params['account_nid'] . '".active';
    $sql .= "and json_unquote(json_extract(field_sirius_json_value, '$path')) = 'Yes' ";
  }
  $sql .= "order by node.nid desc limit 1000";
  $stmt = sirius_sql_query($sql, $sql_args);
  $transmittal_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $transmittal_nids[$hr['nid']] = $hr;
  }
  return $transmittal_nids;
}