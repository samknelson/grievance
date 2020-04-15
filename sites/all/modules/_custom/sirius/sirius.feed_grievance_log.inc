<?php

function sirius_feed_grievance_log_info() {
  $items = array();
  $items['feed_grievance_log'] = array(
    'name' => 'feed_grievance_log',
    'title' => 'Grievances Log',
    'description' => 'Upload grievance log records.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'fields' => array(
        'log_id' => array('title' => 'Log ID', 'description' => 'Identifier for this row. Must be unique within a grievance, need not be unique universally.', 'required' => TRUE),
        'grievance_id' => array('title' => 'Grievance ID', 'required' => TRUE),
        'grievance_log_type_str' => array('title' => 'Log Type', 'description' => 'Log type lookup by name, TID, or ID', 'required' => TRUE),
        'grievance_status_str' => array('title' => 'Status', 'description' => 'Status lookup by name, TID, or ID'),
        'body' => array('title' => 'Body'),
        'created' => array('title' => 'Created', 'description' => 'Date the record was created'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_feed_grievance_log_row_verify',
      'callback_row_process' =>'sirius_feed_grievance_log_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_grievance_log'),
  );
  return $items;
}

function sirius_feed_grievance_log_row_verify($feed_node, $info, $data, &$row, $precompute) {
	// Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  $log_id = $row['log_id'];
  if (!$log_id) { return array('success' => FALSE, 'msg' => 'Missing log ID.'); } 

  $grievance_id = $row['grievance_id'];
  if (!$grievance_id) { return array('success' => FALSE, 'msg' => 'Missing grievance ID.'); }

  // pk
  $id = $grievance_id . '|' . $log_id;
  if ($precompute) { $row['id'] = $id; }

  // Taxonomy lookups
  $lookups = array(
    'grievance_log_type' => 'grievance_log_types',
    'grievance_status' => 'grievance_status',
  );
  foreach ($lookups as $field => $taxonomy) {
    if ($row["${field}_str"]) {
      $tid = sirius_taxonomy_lookup_by_anything($taxonomy, $row["${field}_str"]);
      if (!$tid) { return array('success' => FALSE, 'msg' => "Match not found for $taxonomy, $field"); }
      if ($precompute) { $row[$field] = $tid; }
    }
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
    $result = sirius_feed_node_search($grievance_id, 'grievance', array('nid', 'grievance_id'));
    if ($result['multiple']) { return array('success' => FALSE, 'msg' => 'Multiple grievance records found with this id.'); }
    if (!$result['nid']) { return $result; }
    $row['grievance_nid'] = $result['nid'];

    // Log NID
    $result = sirius_feed_node_search($id, 'grievance_log', array('nid', 'grievance_external_id'));
    if ($result['multiple']) { return array('success' => FALSE, 'msg' => 'Multiple log records found with this id.'); }
    $row['grievance_log_nid'] = $result['nid'];
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_grievance_log_row_process($feed_node, $info, $data, $row) {
  $result = sirius_feed_grievance_log_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  if ($row['grievance_log_nid']) {
    $is_new = FALSE;
    $grievance_log_node = node_load($row['grievance_log_nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $grievance_log_node = new stdClass();
    $grievance_log_node->type = 'grievance_log';
    $grievance_log_node->language = LANGUAGE_NONE;
    $grievance_log_node->uid = $user->uid;
    $grievance_log_node->status = 1;
    $grievance_log_node->promote = 0;
    $grievance_log_node->comment = 0;
  }

  if ($row['id'])                           { $grievance_log_node->field_grievance_external_id['und'][0]['value']         = $row['id']; }

  if ($row['grievance_nid'])                { $grievance_log_node->field_grievance['und'][0]['target_id']                 = $row['grievance_nid']; }

  if ($row['grievance_status'])             { $grievance_log_node->field_grievance_status['und'][0]['tid']                = $row['grievance_status']; }
  if ($row['grievance_log_type'])           { $grievance_log_node->field_grievance_log_type['und'][0]['tid']              = $row['grievance_log_type']; }


  $term = taxonomy_term_load($row['grievance_log_type']);
  $grievance_log_node->title = $term->name;

  $ts = strtotime($row['created']);
  if (!$ts || ($ts < 10)) { $ts = time(); }

  $grievance_log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s', $ts);

  $grievance_log_node->body['und'][0]['value'] = $row['body'];

  node_object_prepare($grievance_log_node);
  $grievance_log_node = node_submit($grievance_log_node);
  node_save($grievance_log_node);

  $sql = "update node set created = :ts where nid = :nid";
  $stmt = db_query($sql, array(':ts' => $ts, ':nid' => $grievance_log_node->nid));

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new grievance log entry.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing grievance log entry.');
  }
}
