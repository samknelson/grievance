<?php

function sirius_employer_feed_dispatch_check_info() {
  $items = array();
  $items['employer_feed_dispatch_check'] = array(
    'name' => 'employer_feed_dispatch_check',
    'title' => 'Employer Dispatch Check',
    'description' => 'Check a spreadsheet of employees to make sure that all are currently dispatched.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'verify'    => array(
        'label' => 'Verify Upload',
        'callback' => 'sirius_feed_wizard_verify',
        'hide_limit' => TRUE,
        'hide_precompute' => TRUE,
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
      ),
      'batch_size' => 500,
      'callback_row_verify' => 'sirius_employer_feed_dispatch_check_row_verify',
      'callback_row_process' =>'sirius_employer_feed_dispatch_check_row_process',
      'callback_list' => 'sirius_employer_feed_dispatch_check_list',
      'employer_access' => TRUE,
      'results_fields' => array(
        'worker_name' => 'Employee Name',
        'worker_id' => 'Badge ID',
        'dispatch_accepted_date' => 'Accepted On',
      ),
      'results_details' => array(
        'active' => TRUE,
        'error_limit' => 1000,
        'success_limit' => 1000,
      ),
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('employer_feed_dispatch_check'),
  );

  $employer = sirius_employer_get();
  if ($employer) { unset($items['employer_feed_dispatch_check']['feed']['fields']['employer']); }
  return $items;
}

function sirius_employer_feed_dispatch_check_list($wizard_name) {
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

function sirius_employer_feed_dispatch_check_row_verify($feed_node, $info, $data, &$row, $precompute) {
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
        $table = 'field_data_field_sirius_id';
        $column = 'field_sirius_id_value';
        $value = $row['id'];
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
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => "Worker not found with this ID ($table, $column)."); }
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

function sirius_employer_feed_dispatch_check_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = sirius_employer_feed_dispatch_check_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'No worker.'); }

  $sql = "select node.nid, field_sirius_dispatch_status_value, field_sirius_datetime_completed_value from node ";
  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_datetime_completed on field_data_field_sirius_datetime_completed.entity_type = 'node' and field_data_field_sirius_datetime_completed.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
  $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
  $sql .= "where node.type = 'sirius_dispatch' ";
  $sql .= "and field_sirius_worker_target_id = :worker_nid ";
  $sql .= "and field_grievance_shop_target_id = :employer_nid ";
  $sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
  $sql .= "order by node.created desc ";
  $sql_args[':worker_nid'] = $row['worker_nid'];
  $sql_args[':employer_nid'] = $row['employer_nid'];
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $completed = $hr['field_sirius_datetime_completed_value'];
    if ($completed) {
      $completed_ts = strtotime("$completed GMT");
      if ($completed_ts > 1) {
        $completed = date('Y-m-d H:i:s e', $completed_ts);
      }
    }
    return array(
      'success' => TRUE, 
      'msg' => "Dispatched with status " . $hr['field_sirius_dispatch_status_value'], 
      'worker_name' => $row['worker_name'],
      'worker_id' => $row['worker_id'],
      'dispatch_accepted_date' => $completed,
    );
  }

  return array(
    'success' => FALSE, 
    'msg' => 'Not dispatched.',
    'worker_name' => $row['worker_name'],
    'worker_id' => $row['worker_id'],
  );
}
