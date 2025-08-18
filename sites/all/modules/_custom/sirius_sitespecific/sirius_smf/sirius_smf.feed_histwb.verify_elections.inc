<?php

function sirius_smf_feed_histwb_row_verify_elections($feed_node, $info, $data, &$row, $fields) {
  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;
  if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'Worker not found.'); }
  if (!$row['employer']) { return array('success' => FALSE, 'msg' => 'Employer not found.'); }

  // Get the date
	$year = substr($row['ym'], 0, 4);
	$month = substr($row['ym'], 4, 2);
	$day = '01';
	if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "Invalid election date."); }
	$date = "$year-$month-$day 00:00:00";

	// Get the benefit
	$benefit_nids = array_filter(array($row['med_plan'], $row['dental_plan']));

  // Did we have an election as of the date. Remember, we might not have created it yet.
  if (!$prior_election_node) {
	  $existing_elections = array();
	  $search_params = array();
	  $search_params['worker_nid'] = $row['worker_nid'];
	  $search_params['ts'] = strtotime($date) + 24*60*60*6;
		$election_nodes = sirius_trust_worker_elections_search($search_params);
		$prior_election_node = reset(array_values($election_nodes));
	}

	if ($prior_election_node) {
		$prior_benefit_nids = sirius_fieldvals($prior_election_node, 'field_sirius_trust_benefits', 'target_id');
		if (!array_diff($prior_benefit_nids, $benefit_nids) && 
			!array_diff($benefit_nids, $prior_benefit_nids) &&
			($prior_election_node->field_grievance_shop['und'][0]['target_id'] == $row['employer']) ) {
			return array('success' => FALSE, 'msg' => "Prior election already has all the benefits.");
		}
	}

	// If we don't already have an election, create a new one.
  $election_node = new stdClass();
  $election_node->type = 'sirius_trust_worker_election';
  $election_node->language = LANGUAGE_NONE;
  $election_node->uid = $user->uid;
  $election_node->status = 1;
  $election_node->promote = 0;
  $election_node->comment = 0;
  if ($domain_nid) { $election_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	$row['elections'][$date] = $election_node;
	$election_node->field_sirius_date_start['und'][0]['value'] = $date;
	$election_node->field_sirius_worker['und'][0]['target_id'] = $row['worker_nid'];
	foreach ($benefit_nids as $benefit_nid) {
		$election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
	}
	$election_node->field_grievance_shop['und'][0]['target_id'] = $row['employer'];

	$row['election_node'] = $election_node;

  return array('success' => TRUE);
}
