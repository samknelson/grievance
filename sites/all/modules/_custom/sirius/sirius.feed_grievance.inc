<?php

function sirius_feed_grievance_row_verify($feed_node, $info, $data, &$row, $precompute) {
  // pk
  $pk = $data['pk'];
  if (!$pk) { $pk = 'nid'; }

	// Trim all
	foreach ($row as $key => $value) { $row[$key] = trim($value); }

  // Clean SSN
  if ($row['ssn']) {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); } else { $row['ssn'] = sirius_ssn_format($row['ssn']); }
  }

  // Clean phone
	if ($row['phone']) { 
    if (!sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
  }

  // Clean email
	if ($row['email']) {
    if (!sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); } else { $row['email'] = sirius_email_format($row['email']); }
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
    $employer_id = $row['employer'];
    if (!$employer_id) { return array('success' => FALSE, 'msg' => 'No employer specified.'); }
    $result = sirius_feed_node_search($employer_id, 'grievance_shop', array('title', 'sirius_id', 'grievance_external_id', 'nid'));
    if (!$result['nid']) { return array('success' => FALSE, 'msg' => 'Employer not found.'); }
    $row['employer_nid'] = $result['nid'];
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

  if ($row['id'])  { $grievance_node->field_grievance_id['und'][0]['value'] = $row['id']; }

  // node_save($grievance_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new grievance.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing grievance.');
  }
}
