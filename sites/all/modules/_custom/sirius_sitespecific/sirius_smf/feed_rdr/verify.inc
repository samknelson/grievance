<?php

function sirius_smf_feed_rdr_row_verify($feed_node, $info, &$data, &$row, $precompute) {
  static $rowcount;
  $rowcount++;

  // if ($rowcount > 2) { return array('success' => FALSE, 'msg' => 'Throttle'); }

  if (!sirius_ssn_validate($row['ssn'])) {
    return array('success' => FALSE, 'msg' => 'Invalid SSN');
  }
  $row['ssn'] = sirius_ssn_format($row['ssn']);

  $worker_node = sirius_worker_get_by_ssn($row['ssn']);
  if (!$worker_node) {
    return array('success' => FALSE, 'msg' => 'Worker not found');
  }
  $row['worker_nid'] = $worker_node->nid;

  if (preg_match('/^\s*billed but not paid\s*$/i', $row['discrepancy_category'])) {
    $row['discrepancy_category_canon'] = 'Billed But Not Paid';
  }

  if (preg_match('/^\s*Paid But Not Billed\s*$/i', $row['discrepancy_category'])) {
    $row['discrepancy_category_canon'] = 'Paid But Not Billed';
  }

  if (preg_match('/^\s*Rate Variance\s*$/i', $row['discrepancy_category'])) {
    $row['discrepancy_category_canon'] = 'Rate Variance';
  }
  if (!$row['discrepancy_category_canon']) {
    return array('success' => FALSE, 'msg' => "Invalid discrepancy category.");
  }

  $ts = strtotime($row['date']);
  if ($ts < 1) { return array('success' => FALSE, 'msg' => 'Invalid date.'); }
  $row['date'] = date('Y-m-d', $ts);

  return array('success' => TRUE, 'msg' => 'Good Input');
}
