<?php

function sirius_feed_grievance_info() {
  $items = array();
  $items['feed_grievance'] = array(
    'name' => 'feed_grievance',
    'title' => 'Grievances',
    'description' => 'Upload grievance records.',
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
        'nid' => 'NID',
        'id' => 'ID',
      ),
      'fields' => array(
        'nid' => array('title' => 'NID'),
        'id' => array('title' => 'ID'),
        'ssn' => array('title' => 'SSN'),
        'grievance_status_str' => array('title' => 'Status', 'description' => 'Status lookup by name, TID, or ID'),
        'grievance_shop_str' => array('title' => 'Employer', 'description' => 'Enter the employer name exactly as it appears, or enter the id or external ID or the node ID (nid).', 'required' => TRUE),
        'grievance_first_name' => array('title' => 'First Name'),
        'grievance_last_name' => array('title' => 'Last Name'),
        'grievance_ein' => array('title' => 'Employee ID'),
        'grievance_min' => array('title' => 'Member ID'),
        'grievance_phone' => array('title' => 'Phone'),
        'grievance_email' => array('title' => 'Email'),
        'grievance_address' => array('title' => 'Address'),
        'grievance_address_2' => array('title' => 'Address 2'),
        'grievance_city' => array('title' => 'City'),
        'grievance_state' => array('title' => 'State or Province'),
        'grievance_zip' => array('title' => 'Zip'),
        'grievance_classification' => array('title' => 'Classification (Text)', 'description' => 'Plain-text version of the classification'),
        'grievance_class_tid_str' => array('title' => 'Classification (Lookup)', 'description' => 'Classification lookup by name, TID, or ID'),
        'grievance_category_str' => array('title' => 'Category', 'description' => 'Category lookup by name, TID, or ID'),
        'grievance_shift_str' => array('title' => 'Shift', 'description' => 'Shift lookup by name, TID, or ID'),
        'grievance_violation' => array('title' => 'Violation', 'description' => 'Full-text CBA clause violated.'),
        'grievance_rep_filed' => array('title' => 'Intake User'),
        'grievance_rep_organizer' => array('title' => 'Organizer / Grievance Specialist'),
        'grievance_st_name' => array('title' => 'Shop Steward Name'),
        'grievance_supervisor_name' => array('title' => 'Supervisor Name'),
        'grievance_department' => array('title' => 'Department'),
        'grievance_type_str' => array('title' => 'Complaint', 'description' => 'Complaint lookup by name, TID, or ID'),
        'grievance_type_other' => array('title' => 'Complaint (text)'),
        'grievance_remedy_other' => array('title' => 'Remedy (text)'),
        'grievance_daysoff_1' => array('title' => 'Days Off 1'),
        'grievance_daysoff_2' => array('title' => 'Days Off 2'),
        'grievance_daysoff_3' => array('title' => 'Days Off 3'),
        'grievance_work_status_str' => array('title' => 'Work Status', 'description' => 'Lookup by name, TID, or ID'),
        'grievance_status_date' => array('title' => 'Status Change Date', 'description' => 'Date the grievance entered the imported status'),
        'created' => array('title' => 'Created', 'description' => 'Date the record was created'),
      ),
      'batch_size' => 25,
      'callback_row_verify' => 'sirius_feed_grievance_row_verify',
      'callback_row_process' =>'sirius_feed_grievance_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_grievance'),
  );
  return $items;
}

function sirius_feed_grievance_row_verify($feed_node, $info, $data, &$row, $precompute) {
  // pk
  $pk = $data['pk'];
  if (!$pk) { $pk = 'nid'; }

	// Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  // Clean SSN
  if ($row['ssn']) {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); } else { $row['ssn'] = sirius_ssn_format($row['ssn']); }
  }

  // Clean phone
	if ($row['grievance_phone']) { 
    if (!sirius_phone_validate($row['grievance_phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['grievance_phone'] = sirius_phone_format($row['grievance_phone']); }
  }

  // Clean email
	if ($row['grievance_email']) {
    if (!sirius_email_validate($row['grievance_email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); } else { $row['grievance_email'] = sirius_email_format($row['grievance_email']); }
  }

  // Taxonomy lookups
  $lookups = array(
    'grievance_class_tid' => 'grievance_job_classification',
    'grievance_category' => 'grievance_category',
    'grievance_status' => 'grievance_status',
    'grievance_shift' => 'grievance_shift',
    'grievance_type' => 'grievance_types',
    'grievance_work_status' => 'grievance_work_status',
  );
  foreach ($lookups as $field => $taxonomy) {
    if ($row["${field}_str"]) {
      $tid = sirius_taxonomy_lookup_by_anything($taxonomy, $row["${field}_str"]);
      if (!$tid) { return array('success' => FALSE, 'msg' => "Match not found for $taxonomy, $field"); }
      if ($precompute) { $row[$field] = $tid; }
    }
  }

  // Days off
  $patterns = array(
    '/sun/i' => 'Sunday',
    '/mon/i' => 'Monday',
    '/tue/i' => 'Tuesday',
    '/wed/i' => 'Wednesday',
    '/thu/i' => 'Thursday',
    '/fri/i' => 'Friday',
    '/sat/i' => 'Saturday',
  );
  for ($i=1; $i<=3; ++$i) {
    if (!$row["grievance_daysoff_$i"]) { continue; }
    $match = '';
    foreach ($patterns as $pattern => $result) {
      if (preg_match($pattern, $row["grievance_daysoff_$i"])) {
        $match = $result;
      }
    }
    if (!$match) { return array('success' => FALSE, 'msg' => "No matching day found for grievance_daysoff_$i"); }
    $row["grievance_daysoff_$i"] = $match;
    // drupal_set_message("Set $i = " . $row["grievance_daysoff_$i"]);
  }

  // Dates
  $lookups = array('created');
  foreach ($lookups as $field) {
    if ($row[$field]) {
      $ts = strtotime($row[$field]);
      if ($ts <= 1) { return array('success' => FALSE, 'msg' => "The value for $field is not a valid date / time string."); }
    }
  }

  if ($precompute) {
    // Grievance NID
    $id = $row[$pk];
    if ($id) {
      $result = sirius_feed_node_search($id, 'grievance', array('nid', 'grievance_id'));
      if ($result['multiple']) { return array('success' => FALSE, 'msg' => 'Multiple records found with this id.'); }
      $row['grievance_nid'] = $result['nid'];
    }

    // Employer NID
    $grievance_shop_str = $row['grievance_shop_str'];
    if (!$grievance_shop_str) { return array('success' => FALSE, 'msg' => 'No employer specified.'); }
    $result = sirius_feed_node_search($grievance_shop_str, 'grievance_shop', array('title', 'sirius_id', 'grievance_external_id', 'nid'));
    if (!$result['nid']) { return array('success' => FALSE, 'msg' => 'Employer not found.'); }
    $row['grievance_shop'] = $result['nid'];

    // Various UID fields
    $uid_fields = array(
      'grievance_rep_filed', 'grievance_rep_organizer'
    );
    foreach ($uid_fields as $uid_field) {
      if (!$row[$uid_field]) { continue; }
      $result = sirius_feed_user_search($row[$uid_field]);
      // if (!$result['success']) { return array('success' => FALSE, 'msg' => "User not found for $uid_field"); }
      if ($result['success']) { $row[$uid_field . '_uid'] = $result['uid']; }
    }

    // Raw row
    $row['raw'] = "Imported record\n" . print_r($row['xxxrawxxx'], 1);
  } 

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_grievance_row_process($feed_node, $info, $data, $row) {
  $result = sirius_feed_grievance_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  if ($row['grievance_nid']) {
    $is_new = FALSE;
    $grievance_node = node_load($row['grievance_nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $grievance_node = new stdClass();
    $grievance_node->type = 'grievance';
    $grievance_node->language = LANGUAGE_NONE;
    $grievance_node->uid = $user->uid;
    $grievance_node->status = 1;
    $grievance_node->promote = 0;
    $grievance_node->comment = 0;
  }

  // ID
  if ($row['id'])                           { $grievance_node->field_grievance_id['und'][0]['value']                  = $row['id']; }

  // Entity reference fields
  if ($row['grievance_shop'])               { $grievance_node->field_grievance_shop['und'][0]['target_id']            = $row['grievance_shop']; }

  // Value fields
  $fields = array(
    'grievance_classification',
    'grievance_violation',
    'grievance_phone',
    'grievance_email',
    'grievance_address',
    'grievance_address_2',
    'grievance_city',
    'grievance_zip',
    'grievance_first_name',
    'grievance_last_name',
    'grievance_ein',
    'grievance_min',
    'grievance_st_name',
    'grievance_supervisor_name',
    'grievance_department',
    'grievance_type_other',
    'grievance_remedy_other',
  );
  foreach ($fields as $field) {
    if ($row[$field]) { $grievance_node->{"field_$field"}['und'][0]['value'] = substr($row[$field], 0, 255); }
  }
  if ($row['grievance_state']) {
    $grievance_node->{"field_grievance_state"}['und'][0]['value'] = substr($row['grievance_state'], 0, 2);
  }

  if ($row['grievance_status_date']) {
    $grievance_node->field_grievance_status_date['und'][0]['value'] = gmdate('Y-m-d', strtotime($row['grievance_status_date']));
  }

  if ($row['created']) {
    $grievance_node->field_grievance_date_1['und'][0]['value'] = gmdate('Y-m-d', strtotime($row['created']));
  }

  // Term reference fields
  if ($row['grievance_class_tid'])          { $grievance_node->field_grievance_class_tid['und'][0]['tid']             = $row['grievance_class_tid']; }
  if ($row['grievance_category'])           { $grievance_node->field_grievance_category['und'][0]['tid']              = $row['grievance_category']; }
  if ($row['grievance_status'])             { $grievance_node->field_grievance_status['und'][0]['tid']                = $row['grievance_status']; }
  if ($row['grievance_shift'])              { $grievance_node->field_grievance_shift['und'][0]['tid']                 = $row['grievance_shift']; }
  if ($row['grievance_type'])               { $grievance_node->field_grievance_type['und'][0]['tid']                  = $row['grievance_type']; }
  if ($row['grievance_work_status'])        { $grievance_node->field_grievance_work_status['und'][0]['tid']           = $row['grievance_work_status']; }

  // Days Off
  for ($i=1; $i<=3; ++$i) {
    if (!$row["grievance_daysoff_$i"]) { continue; }
    $grievance_node->field_grievance_daysoff['und'][($i-1)]['value'] = $row["grievance_daysoff_$i"];
  }

  $uid_fields = array(
    'grievance_rep_filed', 'grievance_rep_organizer'
  );
  foreach ($uid_fields as $uid_field) {
    $uid = $row[$uid_field . '_uid'];
    if ($uid) { 
      $grievance_node->{"field_$uid_field"}['und'][0]['target_id'] = $uid; 
    }
  }

  $comments = $row['raw'];
  $grievance_node->field_grievance_comments['und'][0]['value'] = $comments;

  $grievance_node = node_submit($grievance_node);
  node_save($grievance_node);

  if ($row['created']) { 
    $ts = strtotime($row['created']);
    $sql = "update node set created = :ts where nid = :nid";
    $stmt = db_query($sql, array(':ts' => $ts, ':nid' => $grievance_node->nid));
    for ($i=1; $i<=5; ++$i) {
      $grievance_node->{"grievance_intake_log_$i"} = gmdate('Y-m-d', $ts);
    }
  }

  // Oneoff: delete all log entries of type "Event", "Created", "Assigned", "Updated without status change"
  /*
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', array(120, 175, 179, 178), 'in');
  $stmt = $query->execute();
  $log_nids = array_keys($stmt['node']);
  node_delete_multiple($log_nids);
  grievance_log_update($grievance_node, 'insert');
  */

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new grievance.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing grievance.');
  }
}
