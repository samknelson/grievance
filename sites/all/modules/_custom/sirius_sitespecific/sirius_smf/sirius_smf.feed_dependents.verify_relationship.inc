<?php

function sirius_smf_feed_dependents_row_verify_relationship($feed_node, $info, $data, &$row, $fields) {
  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;

  $row['relationship_node'] = array();

  // Don't import pending records
  if ($row['AD'] == 'P') {
    return array('success' => FALSE, 'msg' => 'This record is marked as "Pending" (Column AD = "P") and will not be imported.');
  }

  // Get the relationship type
  $reltype_id = $row['AB'];
  if ($row['AI'] == 'Y') { $reltype_id = 'QMSCO'; }
  $reltype_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_relationship_types', $reltype_id);
  if (!$reltype_tid) { return array('success' => FALSE, 'msg' => 'Relationship type not found: ' . $row['AB']); }

  // Get the "From" contact NID
  $from_worker_nid = $row['subscriber_worker_nid'];
  $from_worker_node = node_load($from_worker_nid);
  $from_contact_nid = $from_worker_node->field_sirius_contact['und'][0]['target_id'];
  if (!$from_contact_nid) { return array('success' => FALSE, 'msg' => 'The subscriber worker does not have a contact record.'); }

  // Get the "To" contact NID
  $to_worker_nid = $row['worker_nid'];
  if ($to_worker_nid) {
    $to_worker_node = node_load($to_worker_nid);
    $to_contact_nid = $to_worker_node->field_sirius_contact['und'][0]['target_id'];
    if (!$to_contact_nid) { return array('success' => FALSE, 'msg' => 'The dependent worker does not have a contact record.'); }
  }

  // Start date:
  // reltype = SP: BPA-DEP2-MARR-DATE / AF
  // reltype = ES: BPA-DEP2-MARR-DATE / AF
  // reltype = C:  BPA-DEP2-BD / Z
  // reltype = H:  BPA-DEP2-BD / Z
  // reltype = Y/QMSCO:  BPA-DEP2-BD / Z
  // If not found: earlier of BPA-DEP2-EFF / AC or BPA-DEP2-EFF2 / AY

  if (in_array($reltype_id, array('SP', 'ES'))) { $start_date_raw = $row['AF'] + 0; }
  if (in_array($reltype_id, array('C', 'H', 'Y', 'QMSCO')))   { $start_date_raw = $row['Z'] + 0; }
  if (!$start_date_raw) {
    $a = $row['AC'] + 0;
    $b = $row['AY'] + 0;
    if ($a && $b) { $start_date_raw = min($a, $b); }
    else if ($a)  { $start_date_raw = $a; }
    else if ($b)  { $start_date_raw = $b; }
    else          { 
      // return array('success' => FALSE, 'msg' => "No start date (nothing in columns AC or AY) for the relationship.");
      $start_date_raw = '19000101';
    }
  }
  $year = substr($start_date_raw, 0, 4);
  $month = substr($start_date_raw, 4, 2);

  $day = substr($start_date_raw, 6, 2);
  if (!$day) { $day = 1; }
  $day += 0;
  if (!$year || $year < 1900 || !$month || $month < 1 || $month > 12 || $day < 1 || $day > 31) {
    return array('success' => FALSE, 'msg' => "Invalid or missing relationship effective date.");
  }
  if (strlen($month) < 2) { $month = "0$month"; }
  if (strlen($day) < 2) { $day = "0$day"; }
  $start_date = "$year-$month-$day 00:00:00";

  // End date:
  // reltype = SP: BPA-DEP2-DIV-DATE / AH
  // reltype = ES: BPA-DEP2-DIV-DATE / AH if it exists; else later of BPA-DEP2-TERM / AL or BPA-DEP2-TERM2 / AX

  if (in_array($reltype_id, array('SP', 'ES'))) { $end_date_raw = $row['AH'] + 0; }
  if (!$end_date_raw && ($reltype_id == 'ES')) {
    $a = $row['AL'] + 0;
    $b = $row['AX'] + 0;
    if ($a && $b) { $end_date_raw = min($a, $b); }
    else if ($a)  { $end_date_raw = $a; }
    else if ($b)  { $end_date_raw = $b; }
  }
  if ($end_date_raw) {
    $year = substr($end_date_raw, 0, 4);
    $month = substr($end_date_raw, 4, 2);
    $day = '01';
    if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) {
      return array('success' => FALSE, 'msg' => "Invalid or missing relationship end date.");
    }
    if (strlen($month) < 2) { $month = "0$month"; }
    $end_date = "$year-$month-$day 00:00:00";
  }

  // Look for an existing relationship
  $relationship_nodes = sirius_contact_relationships(
    array(
      'contact_nid' => $from_contact_nid, 
      'contact_alt_nid' => $to_contact_nid,
      'reltype_tid' => $reltype_tid
    )
  );
  if (count($relationship_nodes) > 1) { return array('success' => FALSE, 'msg' => "Found multiple relationships of this type between these two contacts. Don't know which relationship to update."); }

  $relationship_node = reset($relationship_nodes);

  if (!$relationship_node) {
    // drupal_set_message("New relationship");
    // Create a new relationship_node
    $relationship_node = new stdClass();
    $relationship_node->type = 'sirius_contact_relationship';
    $relationship_node->language = LANGUAGE_NONE;
    $relationship_node->uid = $user->uid;
    $relationship_node->status = 1;
    $relationship_node->promote = 0;
    $relationship_node->comment = 0;
    if ($domain_nid) { $relationship_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
    // $relationship_node->field_sirius_date_start['und'][0]['value'] = $date;
    // $relationship_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-t 00:00:00', strtotime($date));
    $relationship_node->needs_save = TRUE;
    $relationship_node->field_sirius_contact['und'][0]['target_id'] = $from_contact_nid;
    $relationship_node->field_sirius_contact_alt['und'][0]['target_id'] = $to_contact_id;
    $relationship_node->field_sirius_contact_reltype['und'][0]['tid'] = $reltype_tid;
  } else {
    // drupal_set_message("Existing relationship");
  }

  if ($relationship_node->field_sirius_date_start['und'][0]['value'] != $start_date) {
    $relationship_node->field_sirius_date_start['und'][0]['value'] = $start_date;
    $relationship_node->needs_save = TRUE;
  }

  if ($relationship_node->field_sirius_date_end['und'][0]['value'] != $end_date) {
    if ($end_date) {
      $relationship_node->field_sirius_date_end['und'][0]['value'] = $end_date;
    } else {
      unset($relationship_node->field_sirius_date_end['und'][0]);
    }
    $relationship_node->needs_save = TRUE;
  }
  // drupal_set_message("$reltype_id: $start_date => $end_date");

  $row['relationship_node'] = $relationship_node;

  return array('success' => TRUE);
}
