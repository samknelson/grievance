<?php

function sirius_smf_feed_members_row_verify_coverage($feed_node, $info, $data, &$row, $fields) {
  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;

  $row['wb_nodes'] = array();

  // Get worker's coverage records
  if ($row['worker_nid']) {
    $wb_nodes = sirius_trust_worker_benefits_search(array('worker_nid' => $row['worker_nid']));
    if ($wb_nodes) {
      foreach ($wb_nodes as $wb_nid => $wb_node) {
        $date = $wb_node->field_sirius_date_start['und'][0]['value'];
        if (!$date) { return array('success' => FALSE, 'msg' => 'This worker has a prior worker benefit with no start date.'); }
        // if ($row['elections'][$date]) { return array('success' => FALSE, 'msg' => 'This worker has multiple prior elections with the same start date.'); }
        // $row['elections'][$date] = $election_node;

        $benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
        if (!$benefit_nid) { continue; }
        $row['wb_nodes'][$benefit_nid][$date] = $wb_node;
      }
    }
  }

  for ($coverage_month = 1; $coverage_month <= 16; ++$coverage_month) {
    // Load the coverage record
    $coverage_record = sirius_smf_feed_members_coverage_record($fields, $row, $coverage_month);
    if (!$coverage_record['10 SMCEE-CWELCODE'] && !$coverage_record['10 SMCEE-CDENCODE']) { continue; }

    // Start date
    $year = substr($coverage_record['10 SMCEE-CELIGDATE'], 0, 4);
    $month = substr($coverage_record['10 SMCEE-CELIGDATE'], 4, 2);
    $day = '01';
    if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, invalid date."); }
    $month += 3;
    if ($month > 12) { $month -= 12; ++$year; }
    if (strlen($month) < 2) { $month = "0$month"; }
    $date = "$year-$month-$day 00:00:00";

    $linked_benefits = sirius_smf_feed_members_linked_benefits();

    // Look up the benefits
    $election_benefits_str = $coverage_record['10 SMCEE-CWELCODE'] . $coverage_record['10 SMCEE-CDENCODE'];
    $strs = str_split($election_benefits_str);
    $benefit_nids = array();
    foreach ($strs as $str) {
      $benefit_nid = sirius_find_nid_by_id($str, 'sirius_trust_benefit');
      if (!$benefit_nid) { return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, benefit not found: $str"); }
      $benefit_nids[] = $benefit_nid;

      if ($linked_benefits[$benefit_nid]) {
        foreach ($linked_benefits[$benefit_nid] as $linked_benefit_nid) {
          $benefit_nids[] = $linked_benefit_nid;
        }
      }
    }
    $benefit_nids = array_unique($benefit_nids);

    // Get the employer
    $employer_nid = NULL;
    if ($coverage_record['10 SMCEE-CEMPR']) {
      $employer_id_from = $coverage_record['10 SMCEE-CEMPR'];
      $employer_record = sirius_smf_feed_members_employer_mapping($employer_id_from);
      if (!isset($employer_record)) {
        return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, don't know which employer ID corresponds to the incoming ID $employer_id_from");
      }
      $employer_id_to = $employer_record['employer_id'];
      if ($employer_id_to) {
        $employer_nid = sirius_find_nid_by_id($employer_id_to, 'grievance_shop');
        if (!$employer_nid) {
          return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, employer not found: $employer_id_to");
        }
      }
      $employer_note = $employer_record['note'];
    }

    // For each benefit
    foreach ($benefit_nids as $benefit_nid) {

      // Find out if we're already covered by an existing wb_node
      $already_covered = FALSE;
      if ($row['wb_nodes'][$benefit_nid]) {
        foreach ($row['wb_nodes'][$benefit_nid] as $key => $wb_node) {
          // If this wb already covers our date, we can stop now.
          $start_date = $wb_node->field_sirius_date_start['und'][0]['value'];
          $end_date = $wb_node->field_sirius_date_end['und'][0]['value'];
          if (!$end_date) { $end_date = '9999-99-99 00:00:00'; }
          if (($start_date <= $date) && ($end_date >= $date) && ($wb_node->field_grievance_shop['und'][0]['target_id'] == $employer_nid)) {
            $already_covered = TRUE;
            break;
          } 
        }
      }
      if ($already_covered) { continue; }

      // Create a new wb_node
      $wb_node = new stdClass();
      $wb_node->type = 'sirius_trust_worker_benefit';
      $wb_node->language = LANGUAGE_NONE;
      $wb_node->uid = $user->uid;
      $wb_node->status = 1;
      $wb_node->promote = 0;
      $wb_node->comment = 0;
      if ($domain_nid) { $wb_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
      $wb_node->field_sirius_date_start['und'][0]['value'] = $date;
      $wb_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-t 00:00:00', strtotime($date));
      $wb_node->needs_save = TRUE;
      $wb_node->field_sirius_worker['und'][0]['target_id'] = $row['worker_nid'];
      $wb_node->field_sirius_trust_benefit['und'][0]['target_id'] = $benefit_nid;
      if ($employer_nid) { $wb_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid; }

      $wb_node->field_sirius_notes['und'][0]['value'] = $employer_note;

      $row['wb_nodes'][$benefit_nid][$date] = $wb_node;
    }

    // Now, find the election corresponding to this coverage record.
    foreach ($row['elections'] as $election_date => $election_node) {
      $election_start_date = $election_node->field_sirius_date_start['und'][0]['value'];
      $election_end_date = $election_node->field_sirius_date_end['und'][0]['value'];
      if (!$election_end_date) { $election_end_date = '9999-99-99 00:00:00'; }
      if ($date >= $election_start_date && $date <= $election_end_date) {
        // drupal_set_message("$election_start_date <= $date <= $election_end_date");
      }
    }
  }

  // We may have just created a bunch of adjacent coverage records. If so, we should consolidate them.
  foreach ($row['wb_nodes'] as $benefit_nid => $ignore) {
    ksort($row['wb_nodes'][$benefit_nid]);
    $prev_wb_node = NULL;
    foreach ($row['wb_nodes'][$benefit_nid] as $date => $wb_node) {
      // Can we merge wb_node into prev_wb_ndoe?
      $can_merge = TRUE; 

      // Only if there's a previous one
      if (!$prev_wb_node) { $can_merge = FALSE; }

      // Only if we haven't already saved this one.
      if ($wb_node->nid) { $can_merge = FALSE; }

      // Only if they have the same employer
      if ($wb_node->field_grievance_shop['und'][0]['target_id'] != $prev_wb_node->field_grievance_shop['und'][0]['target_id']) { $can_merge = FALSE; }

      // Only if the previous one ended just a day or so ago
      if ($prev_wb_node->field_sirius_date_end['und'][0]['value'] < date('Y-m-d 00:00:00', strtotime('-5 days', strtotime($date)))) { $can_merge = FALSE;}

      if ($can_merge) {
        // Set the prevous node's end date to our end date
        $prev_date = $prev_wb_node->field_sirius_date_start['und'][0]['value'];
        $row['wb_nodes'][$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'] = max(
          $row['wb_nodes'][$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'],
          $row['wb_nodes'][$benefit_nid][$date]->field_sirius_date_end['und'][0]['value']
        );
        $row['wb_nodes'][$benefit_nid][$prev_date]->needs_save = TRUE;

        // Get rid of the current node
        unset($row['wb_nodes'][$benefit_nid][$date]);
      } else {
        $prev_wb_node = $wb_node;
      }
    }
  }

  return array('success' => TRUE);
}

function sirius_smf_feed_members_linked_benefits() {
  // Get the "linked" benefits
  // - MLK should create:
  // - - VSP Enhanced
  // - - Express Scripts
  // - - Hinge PT
  // - - Life Insurance
  // - - Carelon EAP
  // - - Carelon Behavioral Health
  // - - AD&D
  // 
  // - Healthnet, Kaiser & Kaiser Enhanced should create:
  // - - VSP
  // - - Express Scripts
  // - - Life Insurance
  // - - Carelon EAP
  // - - AD&D

  $linked_benefits = &drupal_static(__FUNCTION__);
  if (!$linked_benefits) {
    $map_by_id = array();
    $map_by_id['M'] = array('3E', 'EXPRESSSCRIPTS', 'HINGEPT', '2', 'CARELONEAP', 'CARELONBEHAVIORALHEALTH', 'ADD');
    $map_by_id['H'] = array('3', 'EXPRESSSCRIPTS', '2', 'CARELONEAP', 'ADD');
    $map_by_id['K'] = $map_by_id['KE'] = $map_by_id['H'];

    foreach ($map_by_id as $from_id => $to_ids) {
      $from_nid = sirius_find_nid_by_id($from_id, 'sirius_trust_benefit');
      if (!$from_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$from_id]"); }

      foreach ($to_ids as $to_id) {
        $to_nid = sirius_find_nid_by_id($to_id, 'sirius_trust_benefit');
        if (!$to_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$to_id]"); }

        $linked_benefits[$from_nid][] = $to_nid;
      }
    }
  }

  return $linked_benefits;
}