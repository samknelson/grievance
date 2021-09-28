<?php

function grievance_uhmigrate_feed_timss_to_sepuede_info() {
  $items = array();
  $items['timss_to_sepuede'] = array(
    'name' => 'timss_to_sepuede',
    'title' => 'UNITE HERE - TIMSS to SePuede',
    'description' => 'Migrate grievance IDs from TIMSS to SePuede',
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
        'timss_id' => 'TIMSS ID',
      ),
      'fields' => array(
        'timss_id' => array('title' => 'TIMSS id'),
        'sepuede_id' => array('title' => 'SePuede ID'),
      ),
      'batch_size' => 500,
      'callback_row_verify' => 'grievance_uhmigrate_feed_timss_to_sepuede_row_verify',
      'callback_row_process' =>'grievance_uhmigrate_feed_timss_to_sepuede_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('timss_to_sepuede'),
  );

  return $items;
}

function grievance_uhmigrate_feed_timss_to_sepuede_row_verify($feed_node, $info, $data, &$row, $precompute) {
  if (!$row['timss_id']) { return array('success' => FALSE, 'msg' => 'No TIMSS ID.'); }
  if (!preg_match('/^[a-zA-Z][a-zA-Z]\d\d+$/', $row['timss_id'])) { return array('success' => FALSE, 'msg' => 'TIMSS ID must be two letters followed by several integers, i.e. AA123456'); }
  if (!$row['sepuede_id']) { return array('success' => FALSE, 'msg' => 'No SePuede ID.'); }
  if (!preg_match('/^\d+$/', $row['sepuede_id'])) { return array('success' => FALSE, 'msg' => 'SePuede ID must be numeric.'); }
  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function grievance_uhmigrate_feed_timss_to_sepuede_row_process($feed_node, $info, $data, $row) {
	$result = grievance_uhmigrate_feed_timss_to_sepuede_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  $sql_args = array(':timss_id' => $row['timss_id'], ':sepuede_id' => $row['sepuede_id']);
  $sql = "update field_data_field_grievance_min ";
  $sql .= "set field_grievance_min_value = :sepuede_id ";
  $sql .= "where entity_type = 'node' and bundle = 'grievance' and field_grievance_min_value = :timss_id ";
  $stmt = sirius_sql_query($sql, $sql_args);
  $rowcount = $stmt->rowCount();

  $sql = "update field_revision_field_grievance_min ";
  $sql .= "set field_grievance_min_value = :sepuede_id ";
  $sql .= "where entity_type = 'node' and bundle = 'grievance' and field_grievance_min_value = :timss_id ";
  $stmt = sirius_sql_query($sql, $sql_args);

  return array('success' => TRUE, 'msg' => 'Updated ' . $rowcount . ' records.');
}
