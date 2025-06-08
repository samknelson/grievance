<?php

function sirius_smf_feed_members_row_verify_elections($feed_node, $info, $data, &$row, $fields) {
  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;

  $prefixes = array(
  	'CMED',
  	'PMED',
  	'P2MED',
  	'P3MED',
  	'CDEN',
  	'PDEN',
  	'P2DEN',
  	'P3DEN',
  );

  // Process oldest to latest, so that multiple elections in the same month overwrite in the correct sequence
  $prefixes = array_reverse($prefixes);

  $row['elections'] = array();

  // 
  // Load current elections
  // 

  if ($row['worker_nid']) {
  	$election_nodes = sirius_trust_worker_elections_search(array('worker_nid' => $row['worker_nid']));
  	if ($election_nodes) {
  		foreach ($election_nodes as $election_nid => $election_node) {
  			$date = $election_node->field_sirius_date_start['und'][0]['value'];
  			if (!$date) { return array('success' => FALSE, 'msg' => 'This worker has a prior election with no start date.'); }
  			if ($row['elections'][$date]) { return array('success' => FALSE, 'msg' => 'This worker has multiple prior elections with the same start date.'); }
  			$row['elections'][$date] = $election_node;
  		}
  	}
  }

  //
  // Figure out the medical and dental benefit for each date
  //

  $benefits = array();
  $med_benefit_nid = $den_benefit_nid = NULL;

  foreach ($prefixes as $prefix) {
  	if (preg_match('/MED/', $prefix)) { $is_med = TRUE; } else { $is_med = FALSE; }

  	// Election benefit is the PLAN...
  	$election_benefits_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}PLAN")];
		$election_type_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}COV")];
		$election_start_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}DATE")];
		if ($election_start_str == '000000') { $election_start_str = ''; }
		if (!$election_benefits_str && !$election_type_str && !$election_start_str) { continue; }

		// Start date
		$year = substr($election_start_str, 0, 4);
		$month = substr($election_start_str, 4, 2);
		$day = '01';
		if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "Invalid election date for $prefix"); }
		$date = "$year-$month-$day 00:00:00";

		$benefit_nid = NULL;
  	if ($election_benefits_str) {
			$benefit_nid = sirius_find_nid_by_id($election_benefits_str, 'sirius_trust_benefit');
		} else {
			$benefit_nid = 0;
		}

		if ($is_med) {
			$benefits[$date]['med'] = $benefit_nid;
		} else {
			$benefits[$date]['den'] = $benefit_nid;
		}
	}


	ksort($benefits);
	foreach ($benefits as $date => $ignore) {
		if ($last_date) {
			if (!isset($benefits[$date]['den'])) {
				$benefits[$date]['den'] = $benefits[$last_date]['den'];
			}
			if (!isset($benefits[$date]['med'])) {
				$benefits[$date]['med'] = $benefits[$last_date]['med'];
			}
		}

		$last_date = $date;
	}

  // 
  // Create or update elections
  // 

  foreach ($prefixes as $prefix) {
  	if (preg_match('/MED/', $prefix)) { $is_med = TRUE; } else { $is_med = FALSE; }

  	// Election benefit is the PLAN...
  	$election_benefits_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}PLAN")];

  	// ... but for medical, no PLAN means look in DEF.
  	//if (!$election_benefits_str && $is_med) {
  	//	$election_benefits_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}DEF")];
  	//}

		$election_type_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}COV")];
		$election_start_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}DATE")];
		if ($election_start_str == '000000') { $election_start_str = ''; }
		if (!$election_benefits_str && !$election_type_str && !$election_start_str) { continue; }

		// Start date
		$year = substr($election_start_str, 0, 4);
		$month = substr($election_start_str, 4, 2);
		$day = '01';
		if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "Invalid election date for $prefix"); }
		$date = "$year-$month-$day 00:00:00";

	  // if ($row['worker_nid'] == 2505821) { drupal_set_message("$prefix ($date) Benefit: $election_benefits_str"); }

		// If we don't already have an election, create a new one.
		if (!$row['elections'][$date]) { 
	    $election_node = new stdClass();
	    $election_node->type = 'sirius_trust_worker_election';
	    $election_node->language = LANGUAGE_NONE;
	    $election_node->uid = $user->uid;
	    $election_node->status = 1;
	    $election_node->promote = 0;
	    $election_node->comment = 0;
	    if ($domain_nid) { $election_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
			$election_node->field_sirius_date_start['und'][0]['value'] = $date;
			$election_node->field_sirius_worker['und'][0]['target_id'] = $row['worker_nid'];
			$row['elections'][$date] = $election_node;
		}

		// Election type
		if ($election_type_str) {
			$election_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_election_type', $election_type_str);
			if (!$election_type_tid) { return array('success' => FALSE, 'msg' => "Invalid election type for $prefix"); }
			if ($row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'] != $election_type_tid) {
				$row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'] = $election_type_tid;
			}
		}

		// Add the current benefit
		$b = array();
		$row['elections'][$date]->field_sirius_trust_benefits['und'] = array();
		if ($benefits[$date]) {
			foreach (array_unique(array_values($benefits[$date])) as $benefit_nid) {
				if (!$benefit_nid) { continue; }
				$row['elections'][$date]->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
			}
		}
		if (!$row['elections'][$date]->field_sirius_trust_benefits['und']) { $row['elections'][$date]->field_sirius_trust_benefits['und'] = NULL; }
		$row['elections'][$date]->needs_save = TRUE;
  }

  /*
  if ($row['worker_nid'] == 2792573) {
  	dpm($benefits);
  	dpm($row['elections']);
  }
  */

	//
  // Set end dates
  // 

  krsort($row['elections']);
  $end_date = '';
  foreach ($row['elections'] as $date => $ignore) {
  	if ($end_date) {
  		if ($row['elections'][$date]->field_sirius_date_end['und'][0]['value'] != $end_date) {
  			$row['elections'][$date]->field_sirius_date_end['und'][0]['value'] = $end_date;
  		}
  	}
  	$end_date = date('Y-m-d 00:00:00', strtotime('-1 day', strtotime($date)));
  }

	//
  // Carry forward election types
  // 

  $last_election_type_tid = NULL;
  ksort($row['elections']);
  foreach ($row['elections'] as $date => $ignore) {
  	if ($last_election_type_tid && (!$row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'])) {
  		$row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'] = $last_election_type_tid;
  	}
  	$last_election_type_tid = $row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'];
  }

  return array('success' => TRUE);
}