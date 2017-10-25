<?php

function grievance_uh23_report_summary() {
  return drupal_get_form('grievance_uh23_report_summary_form');
}

function grievance_uh23_report_summary_form($form, &$form_state) {
  $report_type = 'grievance-uh23-report-summary';
  report_cache_form_prepare($report_type, $form, $form_state);

  /*
  $form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Start Date',
    '#date_format' => 'm/d/Y',
  );

  $form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date',
    '#date_format' => 'm/d/Y',
  );

  $form['open_only'] = array(
    '#type' => 'select',
    '#title' => 'Open Grievances Only?',
    '#required' => TRUE,
    '#options' => array(
      'yes' => 'Yes - Include only open grievances', 
      'no' => 'No - Include both open and closed grievances'
    ),
  );

  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Specialist'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
      '#description' => t('Leave blank to select all specialists.'),
    '#prefix' => '<div class="form-start">',
  );
  */

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_uh23_report_summary_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_uh23_report_summary_fields() {
  return array(
    'organizer' => 'Organizer Name',
    'employer' => 'Employer Name',
    'open' => 'Open',
    'step_1' => 'Step 1',
    'step_2' => 'Step 2',
    'step_3' => 'Step 3',
    'mediation' => 'Mediation',
    'arbitration' => 'Arbitration',
    'old' => '90+ Days Old',
    'overdue' => 'Past Deadline',
    'closed' => 'Closed Last Week',
  );
}

function grievance_uh23_report_summary_pks(&$form_state) {
  $values = array();

  $lookups = grievance_uh23_report_summary_lookups();

  // All open grievances
  $sql = "select distinct node.nid ";
  $sql .= "from field_data_field_grievance, node, field_data_field_grievance_status ";
  $sql .= "where field_data_field_grievance.entity_type = 'node' ";
  $sql .= "and node.type = 'grievance' ";
  $sql .= "and field_data_field_grievance_status.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_status.entity_id = node.nid ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (" . join(',', $lookups['open_status_tids']) . ") ";
  $sql .= "order by node.nid ";
  $stmt = db_query($sql, $values);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }

  // All grievances closed in the last week
  $sql = "select distinct node.nid ";
  $sql .= "from node, field_data_field_grievance, field_data_field_grievance_log_type, field_data_field_grievance_status, field_data_field_grievance_status_date ";

  $sql .= "where node.type = 'grievance' ";

  $sql .= "and field_data_field_grievance.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance.bundle = 'grievance_log' ";
  $sql .= "and field_data_field_grievance.field_grievance_target_id = node.nid ";

  $sql .= "and field_data_field_grievance_status.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_status.bundle = 'grievance_log' ";
  $sql .= "and field_data_field_grievance_status.entity_id = field_data_field_grievance.entity_id ";

  $sql .= "and field_data_field_grievance_status_date.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_status_date.bundle = 'grievance_log' ";
  $sql .= "and field_data_field_grievance_status_date.entity_id = field_data_field_grievance.entity_id ";

  $sql .= "and field_data_field_grievance_log_type.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_log_type.bundle = 'grievance_log' ";
  $sql .= "and field_data_field_grievance_log_type.entity_id = field_data_field_grievance.entity_id ";

  $sql .= "and field_data_field_grievance_status_date.field_grievance_status_date_value >= '" . date('Y-m-d', strtotime('-7 days')) . "' ";
  $sql .= "and field_data_field_grievance_status_date.field_grievance_status_date_value <= '" . date('Y-m-d', strtotime('+1 day')) . "' ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (" . join(',', $lookups['closed_status_tids']) . ") ";
  // $sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid in () "; // Whatever, any time we set the status field.
  $stmt = db_query($sql, $values);
  while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }

	return array_chunk($nids, 100, TRUE);
}

function grievance_uh23_report_summary_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

  $lookups = grievance_uh23_report_summary_lookups();
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_uh23_report_summary_nid($grievance_nid, $lookups, $values);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_uh23_report_summary_nid($grievance_nid, $lookups, $values) {
  // Our result row
  $row = array();
  $row['grievance_nid'] = $grievance_nid;

	// Load the grievance
	$grievance_node = node_load($grievance_nid);

  // Specialist name
  // @todo user_load is expensive.
  if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
    $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    $row['organizer'] = $user->name;
  }

  // Employer name
  // @todo node_load is expensive
  if ($grievance_node->field_grievance_shop['und'][0]['target_id']) {
    $employer_node = node_load($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $row['employer'] = $employer_node->title;
  }

  // Chapter
  $chapter_nid = $employer_node->field_grievance_chapter['und'][0]['target_id'];
  $chapter_node = node_load($chapter_nid);
  $row['chapter'] = $chapter_node->title;


  // Open or closed?
  $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
  if (in_array($status_tid, $lookups['open_status_tids'])) {
    $row['open'] = TRUE;
  } else {
    $row['closed'] = TRUE;
  }

  // Step 1, 2, 3, mediation, arbitration
  $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
  if (in_array($status_tid, $lookups['step_1_status_tids'])) { $row['step_1'] = TRUE; }
  if (in_array($status_tid, $lookups['step_2_status_tids'])) { $row['step_2'] = TRUE; }
  if (in_array($status_tid, $lookups['step_3_status_tids'])) { $row['step_3'] = TRUE; }
  if (in_array($status_tid, $lookups['arbitration_status_tids'])) { $row['arbitration'] = TRUE; }
  if (in_array($status_tid, $lookups['mediation_status_tids'])) { $row['mediation'] = TRUE; }

  // Old?
  if (time() - $grievance_node->created > 60*60*24*90) {
    $row['old'] = TRUE;
  }

  // Overdue?
  if ($grievance_node->field_grievance_alert_date['und'][0]['value'] && (strtotime($grievance_node->field_grievance_alert_date['und'][0]['value']) < time())) {
    $row['overdue'] = TRUE;
  }

  return $row;

  /*
  // SPI date
  if ($values['from_date']) {
    $from_date = date('Y-m-d 00:00:00', strtotime($values['from_date']));
  }
  if ($values['to_date']) {
    $to_date = date('Y-m-d 00:00:00', strtotime($values['to_date']));
  }
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['spi_log_type_tid']);
  if ($from_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, '>=');
  }
  if ($to_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, '<=');
  }
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['spi_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }
  */

}

function grievance_uh23_report_summary_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $step_1_status_tids = array(-999);
  $step_2_status_tids = array(-999);
  $step_3_status_tids = array(-999);
  $arbitration_status_tids = array(-999);
  $mediation_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/step\s+1/i', $term->name)) {
      $step_1_status_tids[] = $tid;
    }
    if (preg_match('/step\s+2/i', $term->name)) {
      $step_2_status_tids[] = $tid;
    }
    if (preg_match('/step\s+3/i', $term->name)) {
      $step_3_status_tids[] = $tid;
    }
    if (preg_match('/arbitra/i', $term->name)) {
      $arbitration_status_tids[] = $tid;
    }
    if (preg_match('/mediat/i', $term->name)) {
      $mediation_status_tids[] = $tid;
    }
  }

  $result = array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
    'step_1_status_tids'              => $step_1_status_tids,
    'step_2_status_tids'              => $step_2_status_tids,
    'step_3_status_tids'              => $step_3_status_tids,
    'mediation_status_tids'           => $mediation_status_tids,
    'arbitration_status_tids'         => $arbitration_status_tids,
    'status_terms'                    => $status_terms,
  );

  return $result;
}

function grievance_uh23_report_summary_render_specialist($report_type, $report_id) {
  return grievance_uh23_report_summary_render($report_type, $report_id, 'specialist');
}

function grievance_uh23_report_summary_render_employer($report_type, $report_id) {
  return grievance_uh23_report_summary_render($report_type, $report_id, 'employer');
}

function grievance_uh23_report_summary_render($report_type, $report_id, $which) {
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
  $html .= '<h2>';
  $html .= 'Summary Report';
  $html .= '</h2>';

  // Our columns
  $header = array(
    'organizer' => 'Name',
    'open' => 'Open',
    'step_1' => 'Step 1',
    'step_2' => 'Step 2',
    'step_3' => 'Step 3',
    'mediation' => 'Mediation',
    'arbitration' => 'Arbitration',
    'old' => '90+ Days Old',
    'overdue' => 'Past Deadline',
    'closed' => 'Closed Last Week',
  );$attributes= array();

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();

  $data = array();
  $totals = array();
  $grand_total = array('organizer' => '<strong>GRAND TOTAL</strong>');
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    if (!$hr['organizer']) { $hr['organizer'] = '(no organizer)'; }
    if (!$hr['chapter']) { $hr['chapter'] = '(no chapter)'; }

    $data[$hr['organizer']]['organizer'] = $hr['organizer'];
    $totals[$hr['chapter']]['organizer'] = '<strong>' . $hr['chapter'] . '</strong>';

    foreach ($header as $key => $ignore) {
      if ($key == 'organizer') { continue; }
      if ($key == 'chapter') { continue; }

      if ($hr[$key]) {
        $data[$hr['organizer']][$key]++;
        $totals[$hr['chapter']][$key]++;
        $grand_total[$key]++;
      }
    }
  }
  ksort($data);
  ksort($totals);

  $rows = array();

  foreach ($data as $organizer => $hr) {
    $row = array();
    foreach ($header as $key => $ignore) {
      $row[] = $hr[$key];
    }
    $rows[] = $row;
  }

  foreach ($totals as $chapter => $hr) {
    $row = array();
    foreach ($header as $key => $ignore) {
      $row[] = $hr[$key];
    }
    $rows[] = $row;
  }
  $row = array();
  foreach ($header as $key => $ignore) {
    $row[] = $grand_total[$key];
  }
  $rows[] = $row;

  return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
}
