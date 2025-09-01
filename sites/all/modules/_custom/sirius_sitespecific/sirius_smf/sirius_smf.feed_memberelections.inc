<?php

sirius_require('sirius_smf', 'sirius_smf.feed_members.fields.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.mapping.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.verify.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.process.inc');

sirius_require('sirius_smf', 'sirius_smf.feed_members.employer_mapping.inc');

function sirius_smf_feed_memberelections_info() {

  $items = array();
  $items['sirius_smf_feed_memberelections'] = array(
    'name' => 'sirius_smf_feed_memberelections',
    'title' => 'Member Election Import',
    'description' => 'A subset of the member feed, that only imports elections.',
    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_feed_wizard_upload'
      ),
      'map' => array(
        'label' => 'Mapping',
        'callback' => 'sirius_smf_feed_members_map'
      ),
      'verify' => array(
        'label' => 'Verify', 
        'callback' => 'sirius_feed_wizard_verify',
        'hide_limit' => TRUE
      ),
      'process' => array(
        'label' => 'Process', 
        'callback' => 'sirius_feed_wizard_process', 
        'hide_limit' => TRUE
      ),
      'review' => array(
        'label' => 'Review', 
        'callback' => 'sirius_feed_wizard_review'
      ),
    ),

    'feed' => array(
      'pks' => array(
        'ssn' => 'SSN',
      ),
      'callback_fields' => 'sirius_smf_feed_members_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_memberelections_row_verify',
      'callback_row_process' =>'sirius_smf_feed_memberelections_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_memberelections'),
  );

  return $items;
}


function sirius_smf_feed_memberelections_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $warnings = array();

  // Load the fields
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // SSN
  if (!sirius_ssn_validate($row['C'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN.'); }
  $row['ssn'] = sirius_ssn_format($row['C']);
  if ($precompute) {
    $result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $row['ssn']);
    if ($result['success']) {
      // We found exactly one. Use that NID.
      $row['worker_nid'] = $result['nid'];
      $msg = "An existing worker will be updated.";
    } else {
      // We can't create new records, or we have a different kind of error
      return $result;
    }
  }

  // Elections
  if ($precompute) {
    $result = sirius_smf_feed_members_row_verify_elections($feed_node, $info, $data, $row, $fields);
    if (!$result['success']) { $warnings[] = $result['msg']; }
  }

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE; 
  if ($precompute) {
    if ($row['worker_nid']) { $msg = 'An existing worker will be updated.'; } 
    else { $msg = 'A new worker will be created.'; }
  } else { $msg = 'Looks good!'; }
  $result['msg'] = $msg;
  return $result;
}

function sirius_smf_feed_memberelections_row_process($feed_node, $info, $data, $row) {

  $result = sirius_smf_feed_memberelections_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

  $domain_nid = sirius_domain_nid();
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // ////////////////////////////////////////////////////////
  // Worker 
  // ////////////////////////////////////////////////////////

  if ($row['worker_nid']) {
    $worker_is_new = FALSE;
    $worker_node = node_load($row['worker_nid']);
  } else {
    return array('success' => FALSE, 'msg' => "No worker_nid.");
  }
  if ($domain_nid) { $worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

  // ////////////////////////////////////////////////////////
  // Elections
  // ////////////////////////////////////////////////////////

  if ($row['elections']) {
    foreach ($row['elections'] as $date => $election_node) {
      if (!$election_node->needs_save) { continue; }
      $election_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;
      node_save($election_node);
    }
  }

  // ////////////////////////////////////////////////////////
  // Result
  // ////////////////////////////////////////////////////////

  if ($is_new) { $msg = 'Created a new worker.'; } else { $msg = 'Updated an existing worker.'; }
  return array('success' => TRUE, 'msg' => $msg, 'warnings' => $warnings);
}
