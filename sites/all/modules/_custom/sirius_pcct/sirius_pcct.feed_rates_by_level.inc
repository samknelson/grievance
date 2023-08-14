<?php

function sirius_pcct_feed_rates_by_level_info() {

  $items = array();

  $items['sirius_pcct_feed_rates_by_level'] = array(
    'name' => 'sirius_pcct_feed_rates_by_level',
    'title' => 'Service Rates by Level',
    'description' => 'Import the rates for each service, by level of provider.',
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
      'content_type' => 'sirius_trust_service',
      'pks' => array(
        'id' => 'ID',
      ),
      'groups' => array(
        'pk' => 'Primary Key',
        'core' => 'Core',
      ),
      'fields' => array(
        'sirius_id' => array('group' => 'pk', 'type' => 'text', 'title' => 'CPT Code or Service ID'),
        'provider_level' => array('group' => 'core', 'title' => 'Provider Level', 'type' => 'term_reference', 'taxonomy' => 'sirius_trust_provider_level'),
        'rate' => array('group' => 'core', 'title' => 'Rate', 'type' => 'text'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_pcct_feed_rates_by_level_row_verify',
      'callback_row_process' =>'sirius_pcct_feed_rates_by_level_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_pcct_feed_rates_by_level'),
  );

  foreach ($items as $key => $item) { sirius_feed_info_set_titles($items[$key]); }
  return $items;
}


function sirius_pcct_feed_rates_by_level_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

  // Trim all
  foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }

    $result = sirius_feed_handle_field($field_name, $field_info, $row[$field_name], $precompute);
    if (!$result['success']) { return $result; }
  }

  $content_type = $info['feed']['content_type'];
  if ($precompute) {
    if ($pk == 'nid') {
      if (!$row['nid']) { return array('success' => FALSE, 'msg' => 'No NID'); }
      $pk_result = sirius_feed_handle_pk_nid($content_type, $row['nid']);
      if (!$pk_result['success']) { return $pk_result; }
      $nid = $pk_result['nid'];
    } else if ($pk = 'sirius_id') {
      if (!$row['sirius_id']) { return array('success' => FALSE, 'msg' => 'No ID.'); }
      $result = sirius_feed_lookup_by_field(
        $content_type, 
        'field_data_field_sirius_id', 
        'field_sirius_id_value', 
        $row['sirius_id']
      );

      if ($result['success']) {
        // We found exactly one. Use that NID.
        $row['nid'] = $result['nid'];
        $msg = "An existing record will be updated.";
      } else {
        if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
          // Not found, but we can create new records. Do nothing
          $msg = "A new record will be created.";
        } else {
          // We can't create new records, or we have a different kind of error
          return $result;
        }
      }
    } else {
      return array('success' => FALSE, 'msg' => 'Missing or unknown pk $pk.');
    }
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_pcct_feed_rates_by_level_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_pcct_feed_rates_by_level_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  if (!$row['nid']) { return; }

  $level_tid = $row['provider_level'];
  $rate = $row['rate'];

  $node = node_load($row['nid']);
  $json = sirius_json_get($node);
  $json['rates']['by_level'][$level_tid] = $rate;
  sirius_json_set($node, $json);
  $node = node_submit($node);
  node_save($node);

  return array('success' => TRUE, 'msg' => 'Rate set.');
}
