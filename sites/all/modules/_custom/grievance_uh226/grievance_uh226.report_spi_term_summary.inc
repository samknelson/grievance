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
    'arb' => 'ARB Submitted',
    'won' => 'Won',
    'lost' => 'Lost',
  );
}

function grievance_uh226_report_spi_term_summary_pks(&$form_state) {
  $from_ts = strtotime($form_state['values']['from_date']);
  $to_ts = strtotime($form_state['values']['to_date']);

  $lookups = grievance_uh226_report_spi_term_summary_lookups();

  $sql = "select distinct node.nid ";
  $sql .= "from field_data_field_grievance_log_type, field_data_field_grievance, node ";
  $sql .= "where field_data_field_grievance.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_log_type.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance.entity_id = field_data_field_grievance_log_type.entity_id ";
  $sql .= "and node.nid = field_data_field_grievance.field_grievance_target_id ";
  $sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid in ($lookups[spi_log_type_tid], $lookups[term_log_type_tid]) ";
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

  $lookups = grievance_uh226_report_spi_term_summary_lookups();
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_uh226_report_spi_term_summary_nid($grievance_nid, $lookups);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_uh226_report_spi_term_summary_nid($grievance_nid, $lookups) {

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
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['boa_held_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['boa_held'] = TRUE;
  } else {
    $row['no_boa_held'] = TRUE;
  }

  // resolved
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
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['arb_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['arb'] = TRUE;
  } 

  // Won
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['won_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['won'] = TRUE;
  } 

  // Lost
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['lost_status_tids'], 'in');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $row['lost'] = TRUE;
  } 

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
    if (preg_match('/arbitration/i', $term->name)) {
      $arbitration_status_tids[] = $tid;
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
    if (preg_match('/Arbitration/i', $term->name)) {
      $arb_held_status_tids[] = $tid;
    }
    if (preg_match('/Won/i', $term->name)) {
      $won_held_status_tids[] = $tid;
    }
    if (preg_match('/Lost/i', $term->name)) {
      $lost_held_status_tids[] = $tid;
    }
  }

  return array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
		'arbitration_status_tids'					=> $arbitration_status_tids,
    'rslvd_status_tids'               => $rslvd_status_tids,
    'nfa_status_tids'                 => $nfa_status_tids,
    'arb_status_tids'                 => $arb_status_tids,
    'won_status_tids'                 => $won_status_tids,
    'lost_status_tids'                => $lost_status_tids,
		'boa_held_status_tids' 						=> $boa_held_status_tids,
    'spi_log_type_tid'                => variable_get('grievance_log_spi_type', ''),
    'term_log_type_tid'               => variable_get('grievance_log_term_type', ''),
  );
}

function grievance_uh226_report_spi_term_summary_render($report_type, $report_id) {
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
  $html .= '<h2>' .
    t('Grievances from @from_date to @to_date', array(
        '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
        '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
      )
    ) .
    '</h2>';

  $data = array();
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $sub_totals = array();
  $totals = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $data[$hr['specialist_name']]['total']++;

    foreach (
      array(
        '1st_call',
        'no_activity',
        'no_boa_held',
        'boa_held',
        'rslvd',
        'nfa',
        'arb',
        'won',
        'lost') as $field) {

      if ($hr[$field]) {
        $data[$hr['specialist_name']][$field]++;
        $sub_totals[$field]++;
      }
    }
  }

  ksort($data);

  $header = array(
    'Specialist',
    'Total SPIs/Terms',
    '1st Calls',
    'No Activity',
    'No BOA Held',
    'BOAs Held',
    'RSLVD',
    'NFA',
    'Arbs Submitted',
    'Won', 
    'Lost'
  );

  $rows = array();
  foreach ($data as $specialist_name => $hr) {
    $row = array();
    $row[] = $specialist_name;
    $row[] = $hr['total'];
    $row[] = $hr['1st_call'];
    $row[] = $hr['no_activity'];
    $row[] = $hr['no_boa_held'];
    $row[] = $hr['boa_held'];
    $row[] = $hr['rslvd'];
    $row[] = $hr['nfa'];
    $row[] = $hr['arb'];
    $row[] = $hr['won'];
    $row[] = $hr['lost'];
    $rows[] = $row;
  }

  $row = array();
  $row[] = '<strong>' . t('Sub-Total') . '</strong>';
  $row[] = '<strong>' . $sub_totals['total'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['1st_call'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['no_activity'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['no_boa_held'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['boa_held'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['rslvd'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['nfa'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['arb'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['won'] . '</strong>';
  $row[] = '<strong>' . $sub_totals['lost'] . '</strong>';
  $rows[] = array('data' => $row, 'no_striping' => TRUE);

  $row = array();
  $row[] = '<strong>' . t('Total') . '</strong>';
  $row[] = '<strong>' . $sub_totals['total'] . '</strong>';
  $row[] = array(
    'data' => '<strong>' . ($sub_totals['1st_call'] + $sub_totals['no_activity']) . '</strong>',
    'colspan' => 2,
  );
  $row[] = array(
    'data' => '<strong>' . ($sub_totals['no_boa_held'] + $sub_totals['boa_held']) . '</strong>',
    'colspan' => 2
  );
  $row[] = array(
    'data' => '<strong>' . ($sub_totals['rslvd'] + $sub_totals['nfa'] + $sub_totals['arb'] + $sub_totals['won'] + $sub_totals['lost'] ) . '</strong>',
    'colspan' => 5
  );
  $rows[] = array('data' => $row, 'no_striping' => TRUE);

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));

  return $html;
}

