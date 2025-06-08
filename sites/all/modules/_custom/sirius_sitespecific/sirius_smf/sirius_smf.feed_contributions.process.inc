<?php

function sirius_smf_feed_contributions_row_process($feed_node, $info, $data, $row) {

	$result = sirius_smf_feed_contributions_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

	$domain_nid = sirius_domain_nid();
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  $msgs = array();
  if ($row['payperiod_unchanged']) {
    $msgs[] = 'Payperiod: Unchanged';
  } else if (!$row['payperiod_node']) {
    $msgs[] = 'Payperiod: Missing';
  } else {
    $payperiod_node = $row['payperiod_node'];
    $payperiod_node = node_submit($payperiod_node);
    node_object_prepare($payperiod_node);
    node_save($payperiod_node);
    $msgs[] = 'Payperiod: Saved';
  }

  /*
  if (!$row['ws_tid']) {
    $msgs[] = "Work status: Not set";
  } else {
    $worker_node = node_load($row['worker_nid']);
    if (!$worker_node || $worker_node->type != 'sirius_worker') {
      return array('success' => FALSE, 'msg' => 'Work not found.', 'warnings' => $warnings);
    }

    drupal_set_message("***** Setting for month: " . date('Y-m-15', $row['ts']));
    $result = sirius_worker_work_status_set_month($worker_node, $row['ws_tid'], 'Feed Import', array(), array(), date('Y-m-15', $row['ts']));

    if (!$result['success']) { 
      return array('success' => FALSE, 'msg' => 'Work status update failed: ' . $result['msg'], 'warnings' => $warnings);
    }
    $msgs[] = 'Work status: Set ' . sirius_term_title($row['ws_tid']);
  }
  */

  return array('success' => TRUE, 'msg' => join(' / ', $msgs), 'warnings' => $warnings);
}
