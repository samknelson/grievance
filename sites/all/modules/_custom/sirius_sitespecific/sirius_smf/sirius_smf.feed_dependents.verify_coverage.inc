<?php

function sirius_smf_feed_dependents_row_verify_coverage($feed_node, $info, $data, &$row, $fields) {
  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;

  // Must have worker, subscriber, relationship. If the relationship feed hasn't been run yet, these won't exist.
  $worker_nid = $row['worker_nid'];
  if (!$worker_nid) { 
    return array('success' => FALSE, 'msg' => 'No worker NID found. Maybe the relationship feed hasn\'t been run yet on this record?', 'warnings' => $warnings);
  }

  $subscriber_worker_nid = $row['subscriber_worker_nid'];
  if (!$subscriber_worker_nid) {
    return array('success' => FALSE, 'msg' => 'No subscriber worker NID found. Maybe the relationship feed hasn\'t been run yet on this record?', 'warnings' => $warnings);
  }
  
  $relationship_node = $row['relationship_node'];
  $relationship_nid = $relationship_node->nid;
  if (!$relationship_nid) {
    return array('success' => FALSE, 'msg' => 'No relationship NID found. Maybe the relationship feed hasn\'t been run yet on this record?', 'warnings' => $warnings);
  }

  $result = sirius_smf_feed_dependents_date($fields, $row, '05 BPA-DEP2-EFF');
  $eff = $result['date'];

  $result = sirius_smf_feed_dependents_date($fields, $row, '05 BPA-DEP2-EFF2');
  $eff2 = $result['date'];

  $result = sirius_smf_feed_dependents_date($fields, $row, '05 BPA-DEP2-TERM');
  $term = $result['date'];

  $result = sirius_smf_feed_dependents_date($fields, $row, '05 BPA-DEP2-TERM2');
  $term2 = $result['date'];
  // drupal_set_message("Here with $eff/$term, $eff2/$term2");

  // Coverage 1 Start:
  // BPA-DEP2-EFF
  $start[0] = $eff;

  // Coverage 1 End:
  if ($term >= $eff) { $end[0] = $term; }

  // Coverage 2 Start:
  $start[1] = $eff2;

  // Coverage 2 End:
  // if BPA-DEP2-TERM2 >= BPA-DEP2-EFF2 then BPA-DEP2-TERM2;
  // else if (BPA-DEP2-TERM >= BPA-DEP2-EFF2 AND BPA-DEP2-TERM < BPA-DEP2-EFF) then BPA-DEP2-TERM;
  // else 1 day before BPA-DEP2-EFF
  if ($term2 > $eff2) { $end[1] = $term2; }
  else if ( ($term > $eff2) && ($term < $eff) ) { $end[1] = $term; }
  else if ($eff) { $end[1] = date('Y-m-d 00:00:00', strtotime('-1 day', strtotime($eff))); }

  for ($i=0; $i<2; ++$i) {
    if ($end[$i]) { $end[$i] = date('Y-m-t 00:00:00', strtotime($end[$i])); }
    if ($start[$i] && $end[$i] && ($end[$i] < $start[$i])) { return array('success' => FALSE, 'msg' => "Coverage end #" . ($i+1) . " is after coverage start."); }
  }

  if (!$start[0] && !$start[1]) {return array('success' => FALSE, 'msg' => 'No coverage start date 1 and no coverage start date 2.'); }

  // Get subscriber's coverage records
  $subscriber_wb_nodes = sirius_trust_worker_benefits_search(array('worker_nid' => $subscriber_worker_nid));
  if (!$subscriber_wb_nodes) { return array('success' => FALSE, 'msg' => 'No coverage records found for the subscriber.'); }

  // Delete all the existing coverage nodes for this worker and relationship
  $row['wb_nodes_to_delete'] = sirius_trust_worker_benefits_search(array('worker_nid' => $worker_nid, 'relationship_nid' => $relationship_nid));

  // Benefit lookup table
  $lookups = &drupal_static(__FUNCTION__);
  if (!isset($lookups)) {
    $lookups['benefits'] = sirius_trust_benefits(FALSE);
    $lookups['benefit_types'] = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'name');
  }

  // Walk the subscriber benefits
  $row['wb_nodes'] = array();
  global $user;
  foreach ($subscriber_wb_nodes as $subscriber_wb_node) {
    // Skip life insurance
    $benefit_nid = $subscriber_wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
    $benefit_type_tid = $lookups['benefits'][$benefit_nid]->field_sirius_trust_benefit_type['und'][0]['tid'];
    $benefit_type_name = $lookups['benefit_types'][$benefit_type_tid];
    if (in_array($benefit_type_name, array('Life Insurance'))) { continue; }
    // drupal_set_message("$worker_nid: $benefit_type_name: $subscriber_start_date => $subscriber_end_date");

    // Generate our own coverage records
    for ($i=0; $i<2; ++$i) {
      if (!$start[$i]) { continue; }

      // Find start and end dates
      $wb_start = $subscriber_wb_node->field_sirius_date_start['und'][0]['value'];
      $wb_end = $subscriber_wb_node->field_sirius_date_end['und'][0]['value'];
      if ($wb_start < $start[$i]) { $wb_start = $start[$i];  }
      if ($wb_end && $end[$i] && ($end[$i] < $wb_end)) { $wb_end = $end[$i]; }
      if ($wb_end < $wb_start) { continue; }

      // Clone the node
      $wb_node = clone $subscriber_wb_node;
      $keys = array_keys((array)$wb_node);
      foreach ($keys as $key) {
        if (preg_match('/field_/', $key)) { continue; }
        unset($wb_node->{$key});
      }
      
      $wb_node->type = 'sirius_trust_worker_benefit';
      $wb_node->language = LANGUAGE_NONE;
      $wb_node->uid = $user->uid;
      $wb_node->status = 1;
      $wb_node->promote = 0;
      $wb_node->comment = 0;
 
       $wb_node->field_sirius_date_start['und'][0]['value'] = $wb_start;
      if ($wb_end) { $wb_node->field_sirius_date_end['und'][0]['value'] = $wb_end; }
      $wb_node->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
      $wb_node->field_sirius_contact_relation['und'][0]['target_id'] = $relationship_nid;

      $row['wb_nodes'][$benefit_nid][$wb_start] = $wb_node; 
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

      // Only if they have the same employer
      if ($wb_node->field_grievance_shop['und'][0]['target_id'] != $prev_wb_node->field_grievance_shop['und'][0]['target_id']) { $can_merge = FALSE; }

      // Only if the previous one ended just a day or so ago
      if ($prev_wb_node->field_sirius_date_end['und'][0]['value'] < date('Y-m-d 00:00:00', strtotime('-5 days', strtotime($date)))) { $can_merge = FALSE;}

      if ($can_merge) {
        // Set the prevous node's end date to our end date
        $prev_date = $prev_wb_node->field_sirius_date_start['und'][0]['value'];
        $row['wb_nodes'][$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'] = max(
          $row['wb_nodes'][$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'],
          $row['wb_nodes'][$benefit_nid][$date]->field_sirius_date_end['und'][0]['value'],
        );

        // Get rid of the current node
        unset($row['wb_nodes'][$benefit_nid][$date]);
      } else {
        $prev_wb_node = $wb_node;
      }
    }
  }

  return array('success' => TRUE);
}