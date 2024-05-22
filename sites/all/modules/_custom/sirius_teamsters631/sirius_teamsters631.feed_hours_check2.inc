<?php

function sirius_teamsters631_feed_hours_check2_info() {
  $items = array();
  $items['teamsters631_feed_hours_check2'] = array(
    'name' => 'teamsters631_feed_hours_check2',
    'title' => 'Teamsters 631 Day / Hour Check NEW',
    'description' => 'Check a spreadsheet of employees by day and hour to make sure that all are currently dispatched.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'dates'     => array('label' => 'Payperiod Dates',  'callback' => 'sirius_teamsters631_feed_hours_check2_dates'),
      'verify'    => array(
        'label' => 'Verify Upload',
        'callback' => 'sirius_feed_wizard_verify',
        'hide_limit' => TRUE,
        // 'hide_precompute' => TRUE,
      ),
      'process'   => array(
        'label' => 'Check Dispatch Status',
        'callback' => 'sirius_feed_wizard_process',
        'submit_txt' => 'Check Now',
        'hide_limit' => TRUE,
      ),
      'review'    => array('label' => 'Results',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'id' => 'Badge ID',
        'ssn' => 'SSN',
        'ein' => 'Employee ID',
      ),
      'fields' => array(
        'ssn' => array('title' => 'SSN'),
        'id' => array('title' => 'Badge ID'),
        'ein' => array('title' => 'EIN'),
        'date' => array('title' => 'Date'),
      ),
      'batch_size' => 500,
      'callback_row_verify' => 'sirius_teamsters631_feed_hours_check2_row_verify',
      'callback_row_process' =>'sirius_teamsters631_feed_hours_check2_row_process',
      // 'callback_list' => 'sirius_teamsters631_feed_hours_check2_list',
      'employer_access' => TRUE,
      'results_fields' => array(
          'worker_nid' => 'Worker NID',
          'link' => 'Link',
          'worker_ws' => 'Current Work Status',
          'worker_ws_asof' => 'As Of Work Status',
          'worker_ms' => 'Member Status',
          'dispatch_nid' => 'Dispatch NID',
          'job_nid' => 'Job NID',
          'job_title' => 'Job',
          'job_report_date' => 'Job Report Date',
          'job_group_title' => 'Show Title',
      ),
      'results_details' => array(
        'active' => TRUE,
        'error_limit' => 1000,
        'success_limit' => 1000,
      ),
      'results_echo_input' => TRUE,
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('teamsters631_feed_hours_check2'),
  );

  $employer = sirius_employer_get();
  if ($employer) { unset($items['employer_feed_dispatch_check']['feed']['fields']['employer']); }
  return $items;
}

function sirius_teamsters631_feed_hours_check2_list($wizard_name) {
  $domain_nid = sirius_domain_nid();
  $info = sirius_wizard_info($wizard_name);
  if (!$info) { return "No such wizard."; }

  $employer = sirius_employer_get();

  $page_size = 50;
  $page = pager_find_page();
  $offset = $page_size * $page;

  $sql_count = "select count(*) as c ";
  $sql_fields = "select node.nid, node.created, field_sirius_feed_status_value, field_sirius_json_value, field_grievance_shop_target_id, shop_node.title as shop_title ";
  $sql = "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_feed_status on field_data_field_sirius_feed_status.entity_type = 'node' and field_data_field_sirius_feed_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
  $sql .= "left join node shop_node on shop_node.nid = field_grievance_shop_target_id ";
  $sql .= "where node.type = 'sirius_feed' ";
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $sql_args[':domain_nid'] = $domain_nid;
  $sql .= "and field_sirius_type_value = :wizard_name ";
  $sql_args[':wizard_name'] = $wizard_name;
  if ($employer) {
    $sql .= "and field_grievance_shop_target_id = :employer_nid ";
    $sql_args[':employer_nid'] = $employer->nid;
  }
  $sql .= "order by node.created desc ";
  $sql_limit = "limit $page_size offset $offset ";

  $stmt = sirius_sql_query($sql_count . $sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $pager = pager_default_initialize($count, $page_size);

  $stmt = sirius_sql_query($sql_fields . $sql . $sql_limit, $sql_args);
  $rows = array();
  while ($hr = $stmt->fetchAssoc()) {
    $json = json_decode($hr['field_sirius_json_value'], TRUE);

    // Source is:
    // - The source field from the JSON (if it's not auto-generated)
    // - Followed by the original file name (if any)
    $source = '';
    if (!preg_match('/^Import \d\d\d\d/', $json['wizard'][$wizard_name]['source'])) {
      $source .= $json['wizard'][$wizard_name]['source'];
    }
    if ($json['wizard'][$wizard_name]['origname']) {
      if ($source) { $source .= ' / '; }
      $source .= $json['wizard'][$wizard_name]['origname'];
    }

    $row = array();
    $row[] = date('l, F j, Y h:i a', $hr['created']);
    $row[] = $hr['field_sirius_feed_status_value'];
    $row[] = $source;
    $row[] = $hr['shop_title'];
    $row[] = l('Wizard', 'node/' . $hr['nid'] . '/sirius_feed_wizard', array('attributes' => array('class' => array('tool-button'))));
    $rows[] = $row;
  }

  $header = array(
    t('Date'),
    t('Status'),
    t('Source'),
    t('Shop'),
    t('Tools')
  );

  $html .= l('Add a New Entry', 'sirius/feeds/launch/' . $wizard_name, array('attributes' => array('class' => array('tool-button', 'action-button'))));

  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

  $html .= theme('pager');

  return $html;
}

function sirius_teamsters631_feed_hours_check2_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

  static $rowcount;
  $rowcount++;
  // if ($rowcount > 1000) { return array('success' => FALSE, 'msg' => 'Discounting subsequent rows.'); }

  // Trim all
  foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
  }

  if ($row['ssn']) { $row['ssn'] = sirius_ssn_format($row['ssn']); }

  if (!$row['date']) { return array('success' => FALSE, 'msg' => 'Please enter a date.'); }
  if (strtotime($row['date'] < 1)) { return array('success' => FALSE, 'msg' => 'Please enter a valid date.'); }

  if ($pk == 'id') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Badge ID'); }
  } else if ($pk == 'id3') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id3'])) { return array('success' => FALSE, 'msg' => 'Invalid ID3'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  } else if ($pk == 'ein') {
    if (!$row['ein']) { return array('success' => FALSE, 'msg' => 'No EIN'); }
  } else if ($pk) {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; unknown pk $pk.");
  } else {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; no pk has been set for this upload.");
  }

  $employer_nid = $feed_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) { 
    return array('success' => FALSE, 'msg' => 'Employer not found.'); 
  }

  // Worker
  if ($precompute) {
    $row['employer_nid'] = $employer_nid;

    $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

    if ($pk == 'ein') {
      $worker_nid = sirius_worker_ein_lookup($employer_nid, $row['ein']);
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => 'Worker not found with this EIN.'); }
    } else {
      if ($pk == 'ssn') { 
        $table = 'field_data_field_sirius_ssn';
        $column = 'field_sirius_ssn_value';
        $value = $row['ssn'];
      } else if ($pk == 'id') { 
        $table = 'field_data_field_sirius_id';
        $column = 'field_sirius_id_value';
        $value = $row['id'];
      } else if ($pk == 'id2') { 
        $table = 'field_data_field_sirius_id2';
        $column = 'field_sirius_id2_value';
        $value = $row['id2'];
      } else if ($pk == 'id3') { 
        $table = 'field_data_field_sirius_ssn';
        $column = 'field_sirius_ssn_value';
        $value = $row['ssn'];
      } 

      $sql = "select max(nid) as a, min(nid) as b from node ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
      $sql .= "where node.type = 'sirius_worker' ";
      $sql .= "and $column = :value ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id IS NULL ";
      }
      $sql_args[':value'] = $value;
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $worker_nid = $hr['a'];
      $other_nid = $hr['b'];
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => "Worker not found with this ID...($table, $column)."); }
      if ($worker_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple workers found with this ID ($table, $column)."); }
    }

    $sql = "select title, field_sirius_id_value ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
    $sql .= "where node.nid = :worker_nid ";
    $sql_args = array();
    $sql_args[':worker_nid'] = $worker_nid;
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();

    $row['worker_nid'] = $worker_nid;
    $row['worker_name'] = $hr['title'];
    $row['worker_id'] = $hr['field_sirius_id_value'];
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_teamsters631_feed_hours_check2_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_teamsters631_feed_hours_check2_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'No worker.'); }

  $result = array();

  $employer_nid = $feed_node->field_grievance_shop['und'][0]['target_id'];
  $worker_nid = $row['worker_nid'];
  // if ($worker_nid != 116143) { return array('success' => FALSE, 'msg' => 'Looking for Dawn.'); }
  $worker_node = node_load($worker_nid);
  if (!$worker_node) {
    return array('success' => FALSE, 'msg' => 'Worker not found.', 'worker_nid' => $row['worker_nid']);
  }

  global $base_url;
  $result['link'] = $base_url . "/node/$worker_nid/sirius_worker_dispatches/all";
  $start_offset = $data['teamsters631_feed_hours_check2']['payperiod_start_offset'];
  $end_offset = $data['teamsters631_feed_hours_check2']['payperiod_end_offset'];

  $ts = strtotime($row['date']);
  if ( ($ts === NULL) || ($ts == 0)) {
    return array('success' => FALSE, 'msg' => "The pay date is not in a recognized format.");
  }

  if ($start_offset > 0) {
    $start_ts = strtotime("-$start_offset days", $ts);
  } else if ($start_offset < 0) {
    $start_offset *= -1;
    $start_ts = strtotime("$start_offset days", $ts);
  } else {
    $start_ts = $ts;
  }

  if ($end_offset > 0) {
    $end_ts = strtotime("-$end_offset days", $ts);
  } else if ($end_offset < 0) {
    $end_offset *= -1;
    $end_ts = strtotime("$end_offset days", $ts);
  } else {
    $end_ts = $ts;
  }

  $dispatch_node = sirius_teamsters631_dispatched_as_of2($worker_nid, $employer_nid, $start_ts, $end_ts);

  $result['worker_nid'] = $worker_node->nid;
  $ws_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];
  if ($ws_tid) { $result['worker_ws'] = sirius_term_title($ws_tid); }

  $ws_tid_asof = sirius_worker_history_asof($worker_node, 'worker:work_status', $ts);
  if ($ws_tid_asof) { $result['worker_ws_asof'] = sirius_term_title($ws_tid); }

  $employer_node = node_load($employer_nid);
  $industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
  foreach ($industry_tids as $industry_tid) {
    $ms_tid = sirius_worker_ms_by_industry($worker_node, $industry_tid); 
    if ($ms_tid) { break; }
  }
  if ($ms_tid) { $result['worker_ms'] = sirius_term_title($ms_tid); }

  if (!$dispatch_node) {
    $result['success'] = FALSE;
    $result['msg'] = 'Not dispatched.';
    return $result;
  }

  $result['dispatch_nid'] = $dispatch_node->nid;

  $job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);

  if ($job_node) {
    $result['job_nid'] = $job_node->nid;
    $result['job_title'] = $job_node->title;
    $result['job_report_date'] = $job_node->field_sirius_datetime['und'][0]['value'];

    $job_group_node = node_load($job_node->field_sirius_dispatch_job_group['und'][0]['target_id']);
    if ($job_group_node) {
      $result['job_group_title'] = $job_group_node->title;
    }
  }

  $result['success'] = TRUE;
  $result['msg'] = 'Dispatch found.';
  $result['dispatch_nid'] = $dispatch_node->nid;
  return $result;
}

function sirius_teamsters631_dispatched_as_of2($worker_nid, $employer_nid, $start_ts, $end_ts) {
  $sql = "select node.nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";

  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";

  $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
  $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
  
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
  $sql .= "where node.type = 'sirius_dispatch' ";
  $sql .= "and field_sirius_worker_target_id = :worker_nid ";
  $sql .= "and field_grievance_shop_target_id = :employer_nid ";

  $sql .= "and ( ";
  $sql .= "     (:start_date <= field_sirius_date_start_value and field_sirius_date_start_value <= :end_date) ";  // The start date of the event falls within the interval
  $sql .= "  or (field_sirius_date_start_value <= :start_date and :start_date <= field_sirius_date_end_value) ";  // The start date of the interval falls within the event
  $sql .= "  or (field_sirius_date_start_value <= :start_date and field_sirius_date_end_value is null) ";         // The end date is null
  $sql .= ") ";

  $sql .= "order by node.created desc ";
  $sql_args[':start_date'] = gmdate('Y-m-d 00:00:00', $start_ts);
  $sql_args[':end_date'] = gmdate('Y-m-d 00:00:00', $end_ts);
  $sql_args[':worker_nid'] = $worker_nid;
  $sql_args[':employer_nid'] = $employer_nid;
  $stmt = sirius_sql_query($sql, $sql_args);

  $dispatch_nids = array();
  $hr = $stmt->fetchAssoc();
  if (!$hr) { return NULL; }
  return node_load($hr['nid']);
}

function sirius_teamsters631_feed_hours_check2_dates($info, $data, $feed_node, $current_step) { 
  return drupal_get_form('sirius_teamsters631_feed_hours_check2_dates_form', $info, $data, $feed_node, $current_step);
}

function sirius_teamsters631_feed_hours_check2_dates_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);

  $form['#tree'] = TRUE;

  $default = $data['teamsters631_feed_hours_check2']['payperiod_start_offset'];
  if (!$default) { $default = 14; }
  $form['teamsters631_feed_hours_check2']['payperiod_start_offset'] = array(
    '#title' => t('Payperiod Start'),
    '#type' => 'textfield',
    '#default_value' => $default,
    '#required' => TRUE,
    '#description' => t('Number of days before the pay date that the payperiod starts.'),
  );

  $default = $data['teamsters631_feed_hours_check2']['payperiod_end_offset'];
  if (!$default) { $default = 3; }
  $form['teamsters631_feed_hours_check2']['payperiod_end_offset'] = array(
    '#title' => t('Payperiod End'),
    '#type' => 'textfield',
    '#default_value' => $default,
    '#required' => TRUE,
    '#description' => t('Number of days before the pay date that the payperiod ends.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_teamsters631_feed_hours_check2_dates_form_validate($form, &$form_state) {
  $start = $form_state['values']['teamsters631_feed_hours_check2']['payperiod_start_offset'];
  $end = $form_state['values']['teamsters631_feed_hours_check2']['payperiod_end_offset'];

  /*
  if (!($start > 0)) {
    form_set_error('teamsters631_feed_hours_check2][payperiod_start_offset', t('Please enter a positive integer.'));
  }

  if (!($end > 0)) {
    form_set_error('teamsters631_feed_hours_check2][payperiod_end_offset', t('Please enter a positive integer.'));
  }
  */

  if (! ($start >= $end)) {
    form_set_error('teamsters631_feed_hours_check2][payperiod_start_offset', t('Start must be greater than end.'));
  }
}

function sirius_teamsters631_feed_hours_check2_dates_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];

  // Save the data for this step
  $data['teamsters631_feed_hours_check2'] = $form_state['values']['teamsters631_feed_hours_check2'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "File uploaded.");
}