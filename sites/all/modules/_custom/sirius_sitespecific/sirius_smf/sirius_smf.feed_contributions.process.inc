<?php

function sirius_smf_feed_contributions_row_process($feed_node, $info, $data, $row) {

	$result = sirius_smf_feed_contributions_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

	$domain_nid = sirius_domain_nid();
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  if (!$row['payperiod_node']) {
    return array('success' => FALSE, 'msg' => 'No payperiod node found. Probably this is a self-pay record, not yet implemented.');
  }

  $payperiod_node = $row['payperiod_node'];
  $payperiod_node = node_submit($payperiod_node);
  node_object_prepare($payperiod_node);
  node_save($payperiod_node);

  return array('success' => TRUE, 'msg' => "Payperiod node saved.", 'warnings' => $warnings);
}
