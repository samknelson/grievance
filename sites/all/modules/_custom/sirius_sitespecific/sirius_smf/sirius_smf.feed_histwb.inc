<?php

sirius_require('sirius_smf', 'sirius_smf.feed_histwb.verify_elections.inc');

function sirius_smf_feed_histwb_info() {

  $items = array();
  $items['sirius_smf_feed_histwb'] = array(
    'name' => 'sirius_smf_feed_histwb',
    'title' => 'Historical Coverage Import',
    'description' => 'Import historical coverage records.',
    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_feed_wizard_upload'
      ),
      'map' => array(
        'label' => 'Mapping',
        'callback' => 'sirius_feed_wizard_map'
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

      'fields' => array(
        'ssn' => array('group' => 'pk', 'type' => 'ssn', 'title' => 'SSN',),

        'ym' => array('group' => 'core', 'title' => 'Eligibility Month', 'description' => 'Month / year, i.e. 201511. Must be six digits.'),
        'med_plan' => array('group' => 'core', 'title' => 'Medical Benefit', 'description' => 'Medical Benefit (lookup)','type' => 'entity_reference', 'entity_type' => 'node', 'bundle' => 'sirius_trust_benefit'),
        'dental_plan' => array('group' => 'core', 'title' => 'Dental Benefit', 'description' => 'Dental Benefit (lookup)','type' => 'entity_reference', 'entity_type' => 'node', 'bundle' => 'sirius_trust_benefit'),
        'medical_tier' => array('group' => 'core', 'title' => 'Election Type', 'description' => 'Election Type (lookup)', 'type' => 'term_reference', 'taxonomy' => 'sirius_trust_election_type'),
        'employer' => array('group' => 'core', 'title' => 'employer', 'type' => 'entity_reference', 'entity_type' => 'node', 'bundle' => 'grievance_shop',),
      ),

      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_histwb_row_verify',
      'callback_row_process' =>'sirius_smf_feed_histwb_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_histwb'),
  );

  return $items;
}

function sirius_smf_feed_histwb_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  // Trim all
  foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }

    $result = sirius_feed_handle_field($field_name, $field_info, $row[$field_name], $precompute);
    if (!$result['success']) { return $result; }
  }

  if ($precompute) {
    // Look up the member
    if (!$row['ssn']) { return array('success' => FALSE, 'msg' => 'No SSN.'); }
    $result = sirius_feed_lookup_by_field(
      'sirius_worker', 
      'field_data_field_sirius_ssn', 
      'field_sirius_ssn_value', 
      $row['ssn']
    );

    if ($result['success']) {
      // We found exactly one. Use that NID.
      $row['worker_nid'] = $result['nid'];
      $msg = "An existing member record will be updated.";
    } else {
      // We can't create new records, or we have a different kind of error
      return $result;
    }
  }

  if ($precompute) {
    $result = sirius_smf_feed_histwb_row_verify_elections($feed_node, $info, $data, $row, $fields);
    if (!$result['success']) { return $result; }
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg, 'warnings' => $warnings);
}









function sirius_smf_feed_histwb_row_process($feed_node, $info, $data, $row) {

  $result = sirius_smf_feed_histwb_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

  $domain_nid = sirius_domain_nid();
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // if ($row['xxxrowcountxxx'] > 6) { return array('success' => FALSE, 'msg' => 'Limit'); }

  if ($row['election_node']) {
    node_save($row['election_node']);
    $msgs[] = "Election saved.";
  }

  return array('success' => TRUE, 'msg' => join(' / ', $msgs), 'warnings' => $warnings);
}
