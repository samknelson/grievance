<?php

function sirius_pcct_feed_rates_by_service_info() {

  $items = array();

  $items['sirius_pcct_feed_rates_by_service'] = array(
    'name' => 'sirius_pcct_feed_rates_by_service',
    'title' => 'Provider Rates by Service',
    'description' => 'Import the rates for each provider, broken out by the service.',
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
      'content_type' => 'sirius_trust_provider',
      'pks' => array(
        'ps' => 'Provider / Service',
      ),
      'groups' => array(
        'core' => 'Core',
      ),
      'fields' => array(
        'provider_sirius_id' => array('group' => 'core', 'type' => 'text', 'title' => 'Provider ID'),
        'service_sirius_id' => array('group' => 'core', 'type' => 'text', 'title' => 'Service ID'),
        'rate' => array('group' => 'core', 'title' => 'Rate', 'type' => 'text'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_pcct_feed_rates_by_service_row_verify',
      'callback_row_process' =>'sirius_pcct_feed_rates_by_service_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_pcct_feed_rates_by_service'),
  );

  foreach ($items as $key => $item) { sirius_feed_info_set_titles($items[$key]); }
  return $items;
}


function sirius_pcct_feed_rates_by_service_row_verify($feed_node, $info, $data, &$row, $precompute) {
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

  if (!$row['service_sirius_id']) { return array('success' => FALSE, 'msg' => 'No service ID.'); }
  if (!$row['provider_sirius_id']) { return array('success' => FALSE, 'msg' => 'No provider ID.'); }

  $content_type = $info['feed']['content_type'];
  if ($precompute) {

    // Look up provider
    $result = sirius_feed_lookup_by_field(
      'sirius_trust_provider', 
      'field_data_field_sirius_id', 
      'field_sirius_id_value', 
      $row['provider_sirius_id']
    );

    if ($result['success']) {
      // We found exactly one. Use that NID.
      $row['provider_nid'] = $result['nid'];
      $msg = "An existing record will be updated.";
    } else {
      $result['msg'] = "Provider not found: " . $result['msg'];
      return $result;
    }

    // Look up service
    $result = sirius_feed_lookup_by_field(
      'sirius_trust_service', 
      'field_data_field_sirius_id', 
      'field_sirius_id_value', 
      $row['service_sirius_id']
    );

    if ($result['success']) {
      // We found exactly one. Use that NID.
      $row['service_nid'] = $result['nid'];
    } else {
      $result['msg'] = "Service not found: " . $result['msg'];
      return $result;
    }

  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_pcct_feed_rates_by_service_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_pcct_feed_rates_by_service_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  $level_tid = $row['provider_level'];
  $rate = $row['rate'];

  $service_nid = $row['service_nid'];
  $provider_nid = $row['provider_nid'];
  $rate = $row['rate'];

  $provider_node = node_load($provider_nid);

  $json = sirius_json_get($provider_node);
  $json['rates']['by_service'][$service_nid] = $rate;
  sirius_json_set($provider_node, $json);
  $node = node_submit($provider_node);
  node_save($provider_node);

  return array('success' => TRUE, 'msg' => 'Rate set.');
}
