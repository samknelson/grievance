<?php

function grievance_uh226_report_violation() {
  return drupal_get_form('grievance_uh226_report_violation_form');
}

function grievance_uh226_report_violation_form($form, &$form_state) {
  $report_type = 'grievance-uh226-report-violation';
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

function grievance_uh226_report_violation_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_uh226_report_violation_fields() {
  return array(
    'grievance_nid' => 'Grievance NID',
    'id' => 'Grievance ID',
    'specialist_name' => 'Specialist Name',
    'employer_name' => 'Employer Name',
    'rslvd' => 'RSLVD',
    'nfa' => 'NFA',
    'arb' => 'ARB Submitted',
    'ww' => 'Written Warning',
    'fw' => 'Final Warning',
    'suspension' => 'Suspension',
    'spi' => 'SPI',
    'term' => 'Term',
    '8.01' => '8.01',
    'cv' => 'CV',
    'general' => 'General',
	);
}

function grievance_uh226_report_violation_pks(&$form_state) {
  $from_ts = strtotime($form_state['values']['from_date']);
  $to_ts = strtotime($form_state['values']['to_date']);

  $lookups = grievance_uh226_report_violation_lookups();

  $sql = "select distinct node.nid ";
  $sql .= "from node ";
  $sql .= "where node.type = 'grievance' ";
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

function grievance_uh226_report_violation_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

  $lookups = grievance_uh226_report_violation_lookups();
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_uh226_report_violation_nid($grievance_nid, $lookups, $values);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_uh226_report_violation_nid($grievance_nid, $lookups, $values) {
  // Our result row
  $row = array();
  $row['grievance_nid'] = $grievance_nid;

	// Load the grievance
	$grievance_node = node_load($grievance_nid);
  $row['id'] = $grievance_node->field_grievance_id['und'][0]['value'];

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

  // resolved
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
  );
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['rslvd_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['rslvd'] = TRUE;
  } 

  // NFA
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['nfa_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['nfa'] = TRUE;
  } 

  // Arb
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['arb_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['arb'] = TRUE;
  } 

  // SPI?
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['spi_log_type_tid']);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['spi'] = TRUE;
  }

  // Term?
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['term_log_type_tid']);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['term'] = TRUE;
  }

  // Term?
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['suspension_log_type_tid']);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['suspension'] = TRUE;
  }

  // WW?
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['written_warning_log_type_tid']);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['ww'] = TRUE;
  }

  // FW?
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['final_warning_log_type_tid']);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['fw'] = TRUE;
  }

  // Is general?
  if ($grievance_node->field_grievance_classaction['und'][0]['value'] == 'Yes') {
    $row['general'] = TRUE;
  }

  // CV?
  if (!$row['general'] && $grievance_node->field_grievance_category['und'][0]['tid'] == 421) {
    $row['cv'] = TRUE;
  }

  // 8.01?
  foreach ($grievance_node->field_grievance_clauseref['und'] as $tmp) {
    if (in_array($tmp['target_id'], $lookups['discrimination_nids'])) {
      $row['8.01'] = TRUE;
    }
  }

  // Must fall into one of the categories
  if (!$row['ww'] && !$row['suspension'] && !$row['spi'] && !$row['term'] && !$row['cv'] && !$row['general'] && !$row['8.01']) {
    return NULL;
  }

  return $row;
}

function grievance_uh226_report_violation_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $rslvd_status_tids = array(-999);
  $nfa_status_tids = array(-999);
  $won_status_tids = array(-999);
  $lost_status_tids = array(-999);
  $arb_status_tids = array(-999);
  $boa_held_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/resolved/i', $term->name)) {
      $rslvd_status_tids[] = $tid;
    }
    if (preg_match('/nfa/i', $term->name)) {
      $nfa_status_tids[] = $tid;
    }
    if (preg_match('/BOA Held/i', $term->name)) {
      $boa_held_status_tids[] = $tid;
    }
    if (preg_match('/Referred/i', $term->name)) {
      $arb_tids[] = $tid;
    }
    if (preg_match('/Won/i', $term->name)) {
      $won_tids[] = $tid;
    }
    if (preg_match('/Lost/i', $term->name)) {
      $lost_tids[] = $tid;
    }
  }

  // Discrimination CBAs clauses.  I shouldn't do it this way.  
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%discrimination%', 'like');
  $stmt = $query->execute();
  $discrimination_nids = array();
  if ($stmt['node']) {
    $discrimination_nids = array_keys($stmt['node']);
  } 

  $result = array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
		'arbitration_status_tids'					=> $arbitration_status_tids,
    'rslvd_status_tids'               => $rslvd_status_tids,
    'nfa_status_tids'                 => $nfa_status_tids,
    'arb_tids'                        => $arb_tids,
    'won_tids'                        => $won_tids,
    'lost_tids'                       => $lost_tids,
		'boa_held_status_tids' 						=> $boa_held_status_tids,
    'spi_log_type_tid'                => variable_get('grievance_log_spi_type', ''),
    'term_log_type_tid'               => variable_get('grievance_log_term_type', ''),
    'suspension_log_type_tid'         => variable_get('grievance_log_suspension_type', ''),
    'written_warning_log_type_tid'    => variable_get('grievance_log_written_warning_type', ''),
    'final_warning_log_type_tid'      => variable_get('grievance_log_final_warning_type', ''),
    'discrimination_nids'             => $discrimination_nids,
  );

  return $result;
}

function grievance_uh226_report_violation_render_summary($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  // Construct the page title
  $html = '';
  $html .= '<div class="uh-226-violation-report-summary-render">';
  $html .= '<h2>';
  $html .= t('Grievances from @from_date to @to_date', array(
      '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
      '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
    )
  );
  $html .= '</h2>';

  $data = array();
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();

  $totals = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    foreach (array('ww', 'fw', 'suspension', 'spi', 'term', '8.01', 'cv', 'general') as $key) {
      if ($hr[$key]) {
        $totals[$key]['all']++;
        if ($hr['rslvd']) { $totals[$key]['rslvd']++; }
        if ($hr['nfa']) { $totals[$key]['nfa']++; }
        if ($hr['arb']) { $totals[$key]['arb']++; }
      }
    }
  }

  $rows = array();
  foreach (array('ww', 'fw', 'suspension', 'spi', 'term', '8.01', 'cv', 'general') as $key) {
    $row = array();
    $row[] = $key;
    $row[] = $totals[$key]['all'];
    $row[] = $totals[$key]['rslvd'];
    $row[] = $totals[$key]['nfa'];
    $row[] = $totals[$key]['arb'];
    $rows[] = $row;
  }

  $header = array(
    'Type',
    'All',
    'Resolved',
    'NFA',
    'Arb',
  );
  
  $html .= theme_table(array('header' => $header, 'attributes' => array(), 'rows' => $rows));

  return $html;
}

