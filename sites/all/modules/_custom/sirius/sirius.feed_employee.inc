<?php

function sirius_feed_employee_info() {
  $items = array();
  $items['feed_employee'] = array(
    'name' => 'feed_employee',
    'title' => 'Employees',
    'description' => 'Upload employee records.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'worker_nid' => 'Worker NID',
        'worker_ssn' => 'SSN',
        'worker_id' => 'Badge ID',
      ),
      'fields' => array(
        'worker_nid' => array('title' => 'Worker NID'),
        'worker_ssn' => array('title' => 'SSN'),
        'worker_id' => array('title' => 'Badge ID'),
        'ein' => array('title' => 'EIN'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_feed_employee_row_verify',
      'callback_row_process' =>'sirius_feed_employee_row_process',
      'employer_access' => TRUE,
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_employee'),
  );
  return $items;
}

function sirius_feed_employee_row_verify($feed_node, $info, $data, &$row, $precompute) {
	// Must have an employer_nid
	$employer_nid = $feed_node->field_grievance_shop['und'][0]['target_id'];
	if (!$employer_nid) { return array('success' => FALSE, 'msg' => 'There is no employer associated with this feed.'); }

  // pk
  $pk = $data['pk'];
  if (!$pk) { $pk = 'worker_nid'; }

	// Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  // Clean SSN
  if ($row['worker_ssn']) {
    if (!sirius_ssn_validate($row['worker_ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); } else { $row['worker_ssn'] = sirius_ssn_format($row['worker_ssn']); }
  }

  // Taxonomy lookups
  $lookups = array(
    // 'grievance_class_tid' => 'grievance_job_classification',
  );
  foreach ($lookups as $field => $taxonomy) {
    if ($row["${field}_str"]) {
      $tid = sirius_taxonomy_lookup_by_anything($taxonomy, $row["${field}_str"]);
      if (!$tid) { return array('success' => FALSE, 'msg' => "Match not found for $taxonomy, $field"); }
      if ($precompute) { $row[$field] = $tid; }
    }
  }

  if ($precompute) {
    // Grievance NID
    $id = $row[$pk];
    if ($id) {
      $result = sirius_feed_node_search($id, 'sirius_worker', array('nid', 'sirius_ssn'));
      if ($result['multiple']) { return array('success' => FALSE, 'msg' => 'Multiple records found with this id.'); }
      if (!$result['nid']) { return array('success' => FALSE, 'msg' => 'No workers found with this id'); }
      $row['worker_nid'] = $result['nid'];
    }

    $row['employer_nid'] = $employer_nid;

    // Raw row
    $row['raw'] = "Imported record\n" . print_r($row['xxxrawxxx'], 1);
  } 

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_employee_row_process($feed_node, $info, $data, $row) {
  $result = sirius_feed_employee_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  $worker_nid = $row['worker_nid'];
  $ein = $row['ein'];
  $employer_nid = $row['employer_nid'];

	$employee_nid = sirius_worker_employee_lookup($worker_nid, $employer_nid, TRUE);
	if ($employee_nid) {
		$employee_node = node_load($employee_nid);
	} else {
		$is_new = TRUE;
		global $user;
		$employer_node = node_load($employer_nid);
		$worker_node = node_load($worker_nid);
		$employee_node = new stdClass();
		$employee_node->title = substr($worker_node->title . ' @ ' . $employer_node->title, 0, 250);
		$employee_node->type = 'sirius_employee';
		$employee_node->language = LANGUAGE_NONE;
		$employee_node->uid = $user->uid;
		$employee_node->status = 1;
		$employee_node->promote = 0;
		$employee_node->comment = 0;
		$employee_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
		$employee_node->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
		$employee_node->field_sirius_domain = $feed_node->field_sirius_domain;
	}

	$need_save = FALSE;

	if ($employee_node->field_sirius_id['und'][0]['value'] != $ein) { $need_save = TRUE; }
	$employee_node->field_sirius_id['und'][0]['value'] = $ein;

	if (!$need_save) { return array('success' => TRUE, 'msg' => 'Employee record is unchanged.'); }

  node_object_prepare($employee_node);
  $payment_node = node_submit($employee_node);
  node_save($employee_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new employee record.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing employee record.');
  }
}
