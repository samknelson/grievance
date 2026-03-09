<?php

function sirius_smf_feed_rdr_row_process($feed_node, $info, $data, $row) {
	$result = sirius_smf_feed_rdr_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	$domain_nid = sirius_domain_nid();

  $worker_nid = $row['worker_nid'];

  $benefit_nids = &drupal_static(__FUNCTION__);
  if (!$benefit_nids) {
    $benefit_nids[] = sirius_find_nid_by_id('K', 'sirius_trust_benefit');
    $benefit_nids[] = sirius_find_nid_by_id('KE', 'sirius_trust_benefit');
    $benefit_nids = array_filter($benefit_nids);
  }
  if (!$benefit_nids) { return array('success' => FALSE, 'msg' => 'Please make sure that there are benefits with IDs "K" and "KE".'); }

  $search_params = array();
  $search_params['subscriber_nid'] = $row['worker_nid'];
  $search_params['benefit_nids'] = $benefit_nids;
  $search_params['ts'] = strtotime($row['date']);
  $search_params['nids_only'] = TRUE;
  $probe = sirius_trust_worker_benefits_search($search_params);
  if ($probe) { $covered_in_month = 'TRUE'; } else { $covered_in_month = 'FALSE'; }

  $count = count($probe);
  if ($count == 1)  { $tier = 'single'; }
  if ($count == 2)  { $tier = 'single+1'; }
  if ($count > 2)   { $tier = 'family'; }


  if ($row['discrepancy_category_canon'] == 'Billed But Not Paid') {
    if ($probe) { $status = 'VALID'; } else { $status = 'DISPUTE'; }
  }

  if ($row['discrepancy_category_canon'] == 'Paid But Not Billed') {
    if ($probe) { $status = 'NEEDS REVIEW'; } else { $status = 'VALID'; }
  }

  if ($row['discrepancy_category_canon'] == 'Rate Variance') {
    $status = 'NEEDS REVIEW';
  }

  return array(
    'success' => TRUE, 
    'msg' => 'Good Input', 
    'covered_in_month' => $covered_in_month, 
    'tier' => $tier,
    'status' => $status
  );
}
