<?php

function grievance_report_status_summary() {
  return drupal_get_form('grievance_report_status_summary_form');
}

function grievance_report_status_summary_form($form, &$form_state) {
  $report_type = 'grievance-report-status-summary';
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['intake_start_date'] = array(
    '#title' => t('Intake Start Date'),
    '#date_format' => 'Y-m-d',
    '#type' => 'date_popup',
    '#description' => t('Limit to grievances with an intake date after this value'),
	);

  $form['intake_end_date'] = array(
    '#title' => t('Intake End Date'),
    '#date_format' => 'Y-m-d',
    '#type' => 'date_popup',
    '#description' => t('Limit to grievances with an intake date before this value'),
	);

  $form['log_start_date'] = array(
    '#title' => t('Status Change Start Date'),
    '#date_format' => 'Y-m-d',
    '#type' => 'date_popup',
    '#description' => t('Limit to grievances with an status change date after this value'),
	);

  $form['log_end_date'] = array(
    '#title' => t('Status Change End Date'),
    '#date_format' => 'Y-m-d',
    '#type' => 'date_popup',
    '#description' => t('Limit to grievances with an status change date before this value'),
	);

  $form['open'] = array(
    '#title' => t('Open Grievances Only?'),
    '#type' => 'select',
    '#options' => array(
    	'-1' => t('Both open and closed'),
    	'1' => t('Open (unresolved) grievances only'),
    	'0' => t('Closed (resolved) grievances only'),
    ),
	);

  $options = sirius_taxonomy_load('grievance_status', 'tid', 'name');
  $form['statuses'] = array(
    '#title' => t('Statuses'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => '<br clear="left" />' . t('Leave blank to select all.'),
		'#prefix' => "<div class=\"checkboxes-inline\">",
		'#suffix' => '</div><br clear="all" />',
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_status_summary_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_status_summary_fields() {
	return array(
		'company_nid',
		'company_name',
		'shop_nid',
		'shop_name',
		'specialist_uid',
		'specialist_name',
		'status_tid',
		'status_name',
		'count'
	);
}

function grievance_report_status_summary_pks() {
	return array('Single Batch');
}

function grievance_report_status_summary_batch($ignore, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

	$sql_values = array();
	$sql = "select count(node.nid) as c, ";
	$sql .= "field_grievance_status_tid as status_tid, ";
	$sql .= "taxonomy_term_data.name as status_name, ";
	$sql .= "field_grievance_shop_target_id as shop_nid, ";
	$sql .= "shop_node.title as shop_name, ";
	$sql .= "field_grievance_company_target_id as company_nid, ";
	$sql .= "company_node.title as company_name, ";
	$sql .= "field_grievance_rep_organizer_target_id as specialist_uid, ";
	$sql .= "users.name as specialist_name ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid and field_data_field_grievance_status.delta = 0 ";
	$sql .= "left join taxonomy_term_data on taxonomy_term_data.tid = field_grievance_status_tid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid and field_data_field_grievance_shop.delta = 0 ";
	$sql .= "left join node shop_node on shop_node.nid = field_grievance_shop_target_id ";
	$sql .= "left join field_data_field_grievance_company on field_data_field_grievance_company.entity_type = 'node' and field_data_field_grievance_company.entity_id = shop_node.nid and field_data_field_grievance_company.delta = 0 ";
	$sql .= "left join node company_node on company_node.nid = field_grievance_company_target_id ";
	$sql .= "left join field_data_field_grievance_rep_organizer on field_data_field_grievance_rep_organizer.entity_type = 'node' and field_data_field_grievance_rep_organizer.entity_id = node.nid and field_data_field_grievance_rep_organizer.delta = 0 ";
	$sql .= "left join users on users.uid = field_grievance_rep_organizer_target_id ";
	$sql .= "left join field_data_field_grievance_open on field_data_field_grievance_open.entity_type = 'taxonomy_term' and field_data_field_grievance_open.entity_id = taxonomy_term_data.tid ";
	$sql .= "left join field_data_field_grievance_status_date on field_data_field_grievance_status_date.entity_type = 'node' and field_data_field_grievance_status_date.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance' ";
	if ($values['intake_start_date']) {
		$sql .= "and node.created >= :intake_start_date ";
		$sql_values[':intake_start_date'] = strtotime($values['intake_start_date']);
	}
	if ($values['intake_end_date']) {
		$sql .= "and node.created <= :intake_end_date ";
		$sql_values[':intake_end_date'] = strtotime($values['intake_end_date']);
	}
	if ($values['log_start_date']) {
		$sql .= "and field_grievance_status_date_value >= :log_start_date ";
		$sql_values[':log_start_date'] = $values['log_start_date'];
	}
	if ($values['log_end_date']) {
		$sql .= "and field_grievance_status_date_value <= :log_end_date ";
		$sql_values[':log_end_date'] = $values['log_end_date'];
	}
	if ($values['open'] != -1) {
		$sql .= "and field_grievance_open_value = :open ";
		$sql_values[':open'] = $values['open'];
	}
	$status_tids = array_unique(array_values($values['statuses']));
	if (count($status_tids) > 1) {
		$sql .= "and field_grievance_status_tid in (:status_tids) ";
		$sql_values[':status_tids'] = $status_tids;
	}
	$sql .= "group by field_grievance_status_tid, field_grievance_shop_target_id ";
	$stmt = db_query($sql, $sql_values);
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$row[] = $hr['company_nid'];
		$row[] = $hr['company_name'];
		$row[] = $hr['shop_nid'];
		$row[] = $hr['shop_name'];
		$row[] = $hr['specialist_uid'];
		$row[] = $hr['specialist_name'];
		$row[] = $hr['status_tid']; // status
		$row[] = $hr['status_name'];
		$row[] = $hr['c'];
		$rows['rows'][] = $row;
	}
	return $rows;
}

function grievance_report_status_summary_render_company($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $status_tids = array_unique(array_values($parameters['statuses']));
  $status_terms = taxonomy_term_load_multiple($status_tids);

  $html = "<div class=\"sirius_report sirius_report_$report_type\">";
  $html .= '<h2>';
  $html .= t('Status Summary Report');
  $html .= '</h2>';
  if ($parameters['intake_start_date']) { 
  	$html .= "Intake date after: <strong>" . $parameters['intake_start_date'] . '</strong><br>';
  }
  if ($parameters['intake_end_date']) { 
  	$html .= "Intake date before: <strong>" . $parameters['intake_end_date'] . '</strong><br>';
  }
  if ($parameters['log_start_date']) { 
  	$html .= "Status change date after: <strong>" . $parameters['log_start_date'] . '</strong><br>';
  }
  if ($parameters['log_end_date']) { 
  	$html .= "Status change date before: <strong>" . $parameters['log_end_date'] . '</strong><br>';
  }
  if ($status_terms) {
  	$html .= "Showing only statuses: <ul>";
  	foreach ($status_terms as $status_tid => $status_term) {
  		$html .= '<li><strong>' . $status_term->name . '</strong></li>';
  	}
  	$html .= '</ul>';
  }
  if ($parameters['open'] == '1') {
  	$html .= '<strong>Open grievances only</strong><br>';
  }
  if ($parameters['open'] == '0') {
  	$html .= '<strong>Closed grievances only</strong>';
  }
  $html .= '<hr>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total = 0;
  $fields = grievance_report_status_summary_fields();
  $status_tids = array();
  while ($arr = $stmt->fetchAssoc()) {
    $raw = unserialize($arr['report_data']);
    $hr = array();
    foreach ($fields as $idx => $field) { $hr[$field] = $raw[$idx]; }
  	$company_name = $hr['company_name'];
  	if (!$company_name) { $company_name = "(no company)"; }
  	$shop_name = $hr['shop_name'];
  	if (!$shop_name) { $shop_name = "(no shop)"; }
  	$status_name = $hr['status_name'];
  	if (!$status_name) { $status_name = "(no status)"; }
  	$status_tid = $hr['status_tid'];
  	$count = $hr['count'];
  	$data[$company_name][$shop_name][$status_name] = $count;
  	$status_tids[$status_tid] = $status_name;
  }
  ksort($data);
  $status_terms = taxonomy_term_load_multiple(array_keys($status_tids));
  $status_weights  = array();
  foreach ($status_terms as $status_tid => $status_term) { $status_weights[$status_term->name] = $status_term->weight; }

  $header = array();
  $header[] = 'Company';
  $header[] = 'Shop';
  $header[] = 'Status';
  $header[] = 'Count';

  $rows = array();
  foreach ($data as $company_name => $company_data) {
  	$rows[] = array($company_name, '', '', '');

  	ksort($company_data);
  	$company_count = 0;
  	foreach ($company_data as $shop_name => $shop_data) {
  		$rows[] = array('', $shop_name, '', '');

  		uksort($shop_data, function($a, $b) use ($status_weights) {
  			$aw = $status_weights[$a]; 
  			$bw = $status_weights[$b];
  			if ($aw > $bw) { return 1; }
  			if ($aw < $bw) { return -1; }
  			return 0;
  		});
  		$shop_count = 0;
  		foreach ($shop_data as $status_name => $count) {
  			$rows[] = array('', '', $status_name, $count);
  			$shop_count += $count;
  			$company_count += $count;
  			$total_count += $count;
  		}
  		$rows[] = array('', '', '<strong>Shop Total</strong>', "<strong>$shop_count</strong>");
  	}
		$rows[] = array('', '<strong>Company Total</strong>', '', "<strong>$company_count</strong>");
  }
	$rows[] = array('<strong>Grand Total</strong>', '', '', "<strong>$total_count</strong>");

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  return $html;
}


