<?php

function sirius_trust_feed_services_info() {

  $items = array();

  $items['sirius_trust_feed_services'] = array(
    'name' => 'sirius_trust_feed_services',
    'title' => 'Services',
    'description' => 'Import new or existing services.',
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
        'sirius_id' => array('group' => 'pk', 'type' => 'text',),

        'title' => array('group' => 'core', 'title' => 'Company Name'),
        'sirius_id2' => array('group' => 'core'),
        'sirius_active' => array('group' => 'core', 'type' => 'list',),
        'body' => array('group' => 'core', 'type' => 'text'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_trust_feed_services_row_verify',
      'callback_row_process' =>'sirius_trust_feed_services_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_trust_feed_services'),
  );

  foreach ($items as $key => $item) { sirius_feed_info_set_titles($items[$key]); }
  return $items;
}


function sirius_trust_feed_services_row_verify($feed_node, $info, $data, &$row, $precompute) {
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

function sirius_trust_feed_services_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_trust_feed_services_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  if ($row['nid']) {
    $is_new = FALSE;
    $node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $node = new stdClass();
    $node->type = $info['feed']['content_type'];
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_sirius_domain = $feed_node->field_sirius_domain;
  }

  if ($row['title']) { $node->title = $row['title']; }
  if ($row['sirius_id']) {
    $node->field_sirius_id['und'][0]['value'] = $row['sirius_id'];
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long', 'date', 'phone', 'ssn'))) {
        $node->{"field_$field_name"}['und'][0]['value'] = $row[$field_name];
      }
      if ($field_info['type'] == 'term_reference') {
        $node->{"field_$field_name"}['und'][0]['tid'] = $row[$field_name];
      }
      if ($field_info['type'] == 'entity_reference') {
        $node->{"field_$field_name"}['und'][0]['target_id'] = $row[$field_name];
      }
    }
  }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    if ($row["sirius_address_$col"]) {
      $node->field_sirius_address['und'][0][$col] = $row["sirius_address_$col"];
    }
  }

  $node = node_submit($node);
  node_save($node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new record.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing record.');
  }
}
