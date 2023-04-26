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
  // Create or update elections
  // 

  foreach ($prefixes as $prefix) {
  	if (preg_match('/MED/', $prefix)) { $is_med = TRUE; } else { $is_med = FALSE; }

  	// Raw data from the feed
		$election_benefits_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}DEF")] . 
			$row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}PLAN")];
		$election_type_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}COV")];
		$election_start_str = $row[sirius_smf_feed_members_field_name($fields, "5 SMCEE-${prefix}DATE")];
		if (!$election_benefits_str && !$election_type_str && !$election_start_str) { continue; }

		// Start date
		$year = substr($election_start_str, 0, 4);
		$month = substr($election_start_str, 4, 2);
		$day = '01';
		if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "Invalid election date for $prefix"); }
		$date = "$year-$month-$day 00:00:00";

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
			$row['elections'][$date] = $election_node;
			$election_node->field_sirius_date_start['und'][0]['value'] = $date;
			$election_node->needs_save = TRUE;
			$election_node->field_sirius_worker['und'][0]['target_id'] = $row['worker_nid'];
		}

		// Is this a medical or dental election (or both)?
		// We use this to carry forward, e.g. an elected dental benefit is added to all subsequent medical elections
		if ($is_med) {
			$row['elections'][$date]->is_medical = TRUE;
		} else {
			$row['elections'][$date]->is_dental = TRUE;
		}

		// Election type
		if ($election_type_str) {
			$election_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_election_type', $election_type_str);
			if (!$election_type_tid) { return array('succes' => FALSE, 'msg' => "Invalid election type for $prefix"); }
			if ($row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'] != $election_type_tid) {
				$row['elections'][$date]->field_sirius_trust_election_type['und'][0]['tid'] = $election_type_tid;
				$row['elections'][$date]->needs_save = TRUE; 
			}
		}

		// Benefits
  	if ($election_benefits_str) {
  		$strs = str_split($election_benefits_str);
  		foreach ($strs as $str) {
  			$benefit_nid = sirius_find_nid_by_id($str, 'sirius_trust_benefit');
  			if (!$benefit_nid) { return array('success' => FALSE, 'msg' => "Invalid benefit for $prefix: $str"); }
  			if (!in_array($benefit_nid, sirius_fieldvals($row['elections'][$date], 'field_sirius_trust_benefits', 'target_id'))) {
  				$row['elections'][$date]->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
  				$row['elections'][$date]->needs_save = TRUE;
  			}
  			if ($is_med) {
  				$row['elections'][$date]->carry_forward_medical[] = $benefit_nid;
  			} else {
  				$row['elections'][$date]->carry_forward_dental[] = $benefit_nid;
  			}
  		}
  	}
  }

	// 
  // Set end dates
  // 

  krsort($row['elections']);
  $end_date = '';
  foreach ($row['elections'] as $date => $ignore) {
  	if ($end_date) {
  		if ($row['elections'][$date]->field_sirius_date_end['und'][0]['value'] != $end_date) {
  			$row['elections'][$date]->field_sirius_date_end['und'][0]['value'] = $end_date;
  			$row['elections'][$date]->needs_save = TRUE;
  		}
  	}
  	$end_date = date('Y-m-d 00:00:00', strtotime('-1 day', strtotime($date)));
  }

  // 
  // Carry forward
  //

  $carry_forward_dental = $carry_forward_medical = array();
  ksort($row['elections']);
  foreach ($row['elections'] as $date => $ignore) {
  	// Carry forward medical into dental-only elections
  	if ($carry_forward_medical && $row['elections'][$date]->is_dental && !$row['elections'][$date]->is_medical) {
  		foreach ($carry_forward_medical as $benefit_nid) {
  			if (!in_array($benefit_nid, sirius_fieldvals($row['elections'][$date], 'field_sirius_trust_benefits', 'target_id'))) {
  				drupal_set_message("Carry forward medial: $benefit_nid");
  				$row['elections'][$date]->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
  				$row['elections'][$date]->needs_save = TRUE;
  			}
  		}
  	}

  	// Carry forward dental into medical-only elections
  	if ($carry_forward_dental && $row['elections'][$date]->is_medical && !$row['elections'][$date]->is_dental) {
  		foreach ($carry_forward_dental as $benefit_nid) {
  			if (!in_array($benefit_nid, sirius_fieldvals($row['elections'][$date], 'field_sirius_trust_benefits', 'target_id'))) {
  				drupal_set_message("Carry forward dental: $benefit_nid");
  				$row['elections'][$date]->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
  				$row['elections'][$date]->needs_save = TRUE;
  			}
  		}
  	}

  	// If it's a medical election, then carry forward our medical choices
  	if ($row['elections'][$date]->is_medical) {
  		$carry_forward_medical = $row['elections'][$date]->carry_forward_medical;
  	}

  	// If it's a dental election, then carry forward our dental choices.
  	if ($row['elections'][$date]->is_dental) {
  		$carry_forward_dental = $row['elections'][$date]->carry_forward_dental;
  	}

  	// It it's neither, then we didn't create this election -- which means it resets *everything*.
  	if (!$row['elections'][$date]->is_medical && !$row['elections'][$date]->is_dental) {
  		$carry_forward_medical = $carry_forward_dental = array();
  	}
  }

  return array('success' => TRUE);
}