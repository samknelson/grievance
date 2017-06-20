<?php

function grievance_uh226_report_spi_term_summary() {
  return drupal_get_form('grievance_uh226_report_spi_term_summary_form');
}

function grievance_uh226_report_spi_term_summary_form($form, &$form_state) {
  $report_type = 'grievance-uh226-report-spi-term-summary';
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

function grievance_uh226_report_spi_term_summary_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_uh226_report_spi_term_summary_fields() {
  return array(
    'grievance_nid' => 'Grievance NID',
    'specialist_name' => 'Specialist Name',
    '1st_call' => '1st Call',
    'no_activity' => 'No Activity',
    'no_boa_held' => 'No BOA Held',
    'boa_held' => 'BOA Held',
    'rslvd' => 'RSLVD',
    'nfa' => 'NFA',
    'arb_submitted' => 'ARB Submitted',
    'won' => 'Won',
    'lost' => 'Lost',
  );
}

function grievance_uh226_report_spi_term_summary_pks(&$form_state) {
  $from_ts = strtotime($form_state['values']['from_date']);
  $to_ts = strtotime($form_state['values']['to_date']);

  $spi_log_type_tid = variable_get('grievance_log_spi_type', '');
  $term_log_type_tid = variable_get('grievance_log_term_type', '');

  if (!$spi_log_type_tid || !$term_log_type_tid) {
    drupal_set_message(t('Unable to generate this report.  Please go to the configuration screen and set the SPI Log Type and Termination Log Type.'), 'error');
    return array();
  }

  $sql = "select distinct node.nid ";
  $sql .= "from field_data_field_grievance_log_type, field_data_field_grievance, node ";
  $sql .= "where field_data_field_grievance.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_log_type.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance.entity_id = field_data_field_grievance_log_type.entity_id ";
  $sql .= "and node.nid = field_data_field_grievance.field_grievance_target_id ";
  $sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid in ($spi_log_type_tid, $term_log_type_tid) ";
  $sql .= "and node.type = 'grievance' ";
  $sql .= "and node.created >= :from_ts ";
  $sql .= "and node.created <= :to_ts ";
  $sql .= "order by node.nid ";

  $stmt = db_query($sql, 
    array(
      ':from_ts' => $from_ts,
      ':to_ts' => $to_ts,
    )
  );
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }
	return array_chunk($nids, 100, TRUE);
}

function grievance_uh226_report_spi_term_summary_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_uh226_report_spi_term_summary_nid($grievance_nid, $values, $report_id, $report_action, $context);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_uh226_report_spi_term_summary_nid($grievance_nid, $lookups) {
  $spi_log_type_tid = variable_get('grievance_log_spi_type', '');
  $term_log_type_tid = variable_get('grievance_log_term_type', '');

  // Our result row
  $row = array();
  $row['grievance_nid'] = $grievance_nid;

	// Load the grievance
	$grievance_node = node_load($grievance_nid);

  // Specialist name
  if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
    $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    $row['specialist_name'] = $user->name;
  }

  // 1st call?
  $first_call_log_type_tid = variable_get('grievance_log_first_call_type', '');
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $first_call_log_type_tid);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['1st_call'] = TRUE;
  }

  // no_activity @todo what does no activity mean?

  // no_boa_held
  // boa_held
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
  );
  $boa_status_tid = 112;
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $boa_status_tid);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['boa_held'] = TRUE;
  } else {
    $row['no_boa_held'] = TRUE;
  }

  return $row;

	// Load the log
	$found = FALSE;
	foreach ($grievance_node->field_grievance_clauseref['und'] as $tmp) {
		$clause_nid = $tmp['target_id'];
		$clause = node_load($clause_nid);
	}

	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
	if (!$shop_nid) { return NULL; }
	$shop_node = node_load($shop_nid);
	if (!$shop_node) { return NULL; }

	$row = array();
	$row[] = $shop_node->title;
	$row[] = $grievance_node->field_grievance_last_name['und'][0]['value'] . ', ' . $grievance_node->field_grievance_first_name['und'][0]['value'];
	$row[] = $grievance_node->field_grievance_id['und'][0]['value'];

	return $row;
}

function grievance_uh226_report_spi_term_summary_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $arbitration_status_tids = array(-999);
  $fmcs_status_tids = array(-999);
  $adr_status_tids = array(-999);
  $formal_arb_held_status_tids = array(-999);
  $expedited_arb_held_status_tids = array(-999);
  $victory_status_tids = array(-999);
  $boa_held_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/arbitration/i', $term->name)) {
      $arbitration_status_tids[] = $tid;
    }
    if (preg_match('/FMCS Held/i', $term->name)) {
      $fmcs_status_tids[] = $tid;
    }
    if (preg_match('/ADR Held/i', $term->name)) {
      $adr_status_tids[] = $tid;
    }
    if (preg_match('/Formal Arbitration Held/i', $term->name)) {
      $formal_arb_held_status_tids[] = $tid;
    }
    if (preg_match('/Expedited Arbitration Held/i', $term->name)) {
      $expedited_arb_held_status_tids[] = $tid;
    }
    if (preg_match('/(resolved|won)/i', $term->name)) {
      $victory_status_tids[] = $tid;
    }
    if (preg_match('/BOA Held/i', $term->name)) {
      $boa_held_status_tids[] = $tid;
    }
  }

  // Get the list of "SPI" nids
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%Suspension Pending Investigation%', 'like');
  $stmt = $query->execute();
  $spi_nids = array(-999);
  if ($stmt['node']) {
    $spi_nids = array_keys($stmt['node']);
  }

  // Get the list of "Term" nids
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%Discharge%', 'like');
  $stmt = $query->execute();
  $term_nids = array(-999);
  if ($stmt['node']) {
    $term_nids = array_keys($stmt['node']);
  }

  return array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
		'arbitration_status_tids'					=> $arbitration_status_tids,
		'fmcs_status_tids' 								=> $fmcs_status_tids,
		'adr_status_tids' 								=> $adr_status_tids,
		'formal_arb_held_status_tids' 		=> $formal_arb_held_status_tids,
		'expedited_arb_held_status_tids' 	=> $expedited_arb_held_status_tids,
		'victory_status_tids' 						=> $victory_status_tids,
		'boa_held_status_tids' 						=> $boa_held_status_tids,
		'spi_nids' 												=> $spi_nids,
		'term_nids' 											=> $term_nids,
  );
}

function grievance_uh226_report_spi_term_summary_render($report_type, $report_id) {
  return 'Render goes here...';
}


