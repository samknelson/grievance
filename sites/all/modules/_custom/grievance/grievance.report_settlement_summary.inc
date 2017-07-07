<?php

function grievance_report_settlement_summary() {
  return drupal_get_form('grievance_report_settlement_summary_form');
}

function grievance_report_settlement_summary_form($form, &$form_state) {
  $report_type = 'grievance-report-settlement-summary';
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Start Date',
    '#required' => TRUE,
    '#date_format' => 'm/d/Y',
  );

  $form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date',
    '#required' => TRUE,
    '#date_format' => 'm/d/Y',
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_settlement_summary_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_settlement_summary_fields() {
  return array(
    'settlement_nid' => 'Settlement NID',
    'specialist_name' => 'Specialist',
    'employer_name' => 'Employer',
    'year' => 'Year',
    'amt' => 'Amount'
  );
}

function grievance_report_settlement_summary_pks(&$form_state) {
  $from_ts = strtotime($form_state['values']['from_date']);
  $to_ts = strtotime($form_state['values']['to_date']);
  $from_date = date('Y-m-d 00:00:00', $from_ts);
  $to_date = date('Y-m-d 00:00:00', $to_ts);

  $lookups = grievance_report_settlement_summary_lookups();

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_settlement');
  $query->fieldCondition('field_grievance_date_1', 'value', $from_date, '>=');
  $query->fieldCondition('field_grievance_date_1', 'value', $to_date, '<=');
  $query->fieldCondition('field_grievance_amt', 'value', 0, '>');
  $stmt = $query->execute();
  $nids = array();
  if ($stmt['node']) {
    $nids = array_keys($stmt['node']);
  }
	return array_chunk($nids, 100, TRUE);
}

function grievance_report_settlement_summary_batch($settlement_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

  $lookups = grievance_report_settlement_summary_lookups();
	foreach ($settlement_nids as $settlement_nid) {
		$row = grievance_report_settlement_summary_nid($settlement_nid, $lookups, $values);
		if ($row) {
			$rows['rows'][$settlement_nid] = $row;
		}
	}
	return $rows;
}

function grievance_report_settlement_summary_nid($settlement_nid, $lookups, $values) {

  // Our result row
  $row = array();
  $row['settlement_nid'] = $settlement_nid;

	// Load the grievance
	$settlement_node = node_load($settlement_nid);
  $grievance_node = node_load($settlement_node->field_grievance['und'][0]['target_id']);

  // Specialist name
  // @todo user_load is expensive.
  if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
    $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    $row['specialist_name'] = $user->name;
  }

  // Employer name
  // @todo node_load is expensive
  if ($grievance_node->field_grievance_shop['und'][0]['target_id']) {
    $employer_node = node_load($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $row['employer_name'] = $employer_node->title;
  }

  $row['amt'] = $settlement_node->field_grievance_amt['und'][0]['value'];
  $row['year'] = date('Y', strtotime($settlement_node->field_grievance_date_1['und'][0]['value']));

  return $row;
}

function grievance_report_settlement_summary_lookups() {
  return array();
}

function grievance_report_settlement_summary_render_specialist($report_type, $report_id) {
  return grievance_report_settlement_summary_render($report_type, $report_id, 'specialist');
}

function grievance_report_settlement_summary_render_employer($report_type, $report_id) {
  return grievance_report_settlement_summary_render($report_type, $report_id, 'employer');
}

function grievance_report_settlement_summary_render($report_type, $report_id, $which) {
  // Iterate through the results and build our aggregates
  /*
    'grievance_nid' => 'Grievance NID',
    'specialist_name' => 'Specialist Name',
    '
  */

  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  $html = '';
  $html .= '<div class="grievance-report-settlement-render">';
  $html .= '<h2>';
  $html .= t('Grievances from @from_date to @to_date', array(
      '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
      '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
    )
  );
  if ($which == 'specialist') { 
    $html .= ' -- By Specialist'; 
  } else {
    $html .= ' -- By Employer'; 
  }
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $total += $hr['amt'];

    $key1 = $hr[$which . '_name'];
    if (!$key1) { $key1 = '[Unknown]'; }
    $key2 = $hr['year'];

    $data[$key1][$key2] += $hr['amt'];
    $grand_total += $hr['amt'];
  }

  ksort($data);

  $header = array();
  if ($which == 'specialist') {
    $header[] = 'Specialist';
  } else {
    $header[] = 'Employer';
  }
  $header[] = 'Year';
  $header[] = 'Amount';

  $rows = array();
  foreach ($data as $specialist_name => $years) {
    ksort($years);
    foreach ($years as $year => $amt) {
      $row = array();
      $row[] = $specialist_name;
      $row[] = $year;
      $row[] = '$' . number_format($amt, 2);
      $rows[] = $row;
    }
  }

  $row = array();
  $row[] = '<strong>' . t('Total') . '</strong>';
  $row[] = '';
  $row[] = '<strong>' . '$' . number_format($grand_total, 2) . '</strong>';
  $rows[] = $row;

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  return $html;
}


