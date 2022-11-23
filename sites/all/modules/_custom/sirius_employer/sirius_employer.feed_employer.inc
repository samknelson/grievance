<?php

function sirius_employer_feed_employer_info() {
  $items = array();
  $items['sirius_employer_feed_employer'] = array(
    'name' => 'sirius_employer_feed_employer',
    'title' => 'Employers',
    'description' => 'Import new or existing employers.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      // 'options'   => array('label' => 'Options',    'callback' => 'sirius_employer_feed_employer_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Internal ID (NID)',
        'sirius_id' => 'External ID',
      ),
      'create_new_records' => 'optional',
      'fields' => array(
        'nid' => array('group' => 'pk', 'title' => 'Internal ID (NID)'),
        'sirius_id' => array('group' => 'pk', 'title' => 'External ID'),

        'title' => array('group' => 'core', 'title' => 'Title'),
        'rooms' => array('group' => 'core', 'type' => 'integer'),
        'brand' => array('group' => 'core', 'type' => 'term_reference', 'taxonomy' => 'brand'),
        'niche' => array('group' => 'core', 'type' => 'term_reference', 'taxonomy' => 'hdb_niche'),
        'development_status' => array('group' => 'core', 'type' => 'list'),
        'owner_of_record' => array('group' => 'core', 'type' => 'text'),
        'notes' => array('group' => 'core', 'type' => 'long'),


     ),
      'batch_size' => 10,
      'callback_row_verify' => 'sirius_employer_feed_employer_row_verify',
      'callback_row_process' =>'sirius_employer_feed_employer_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_employer_feed_employer'),
  );

  $groups = array(
    'pk' => 'Primary Key',
    'core' => 'Core',
  );

  foreach ($items['sirius_employer_feed_employer']['feed']['fields'] as $field_name => $field_info) {
    $group = $field_info['group'];
    $group_name = $groups[$group];

    if (!$field_info['title']) {
      $structure_info = field_info_instance('node', "field_$field_name", 'property');
      if ($structure_info) { $items['sirius_employer_feed_employer']['feed']['fields'][$field_name]['title'] = $structure_info['label']; }
    }

    $items['sirius_employer_feed_employer']['feed']['fields'][$field_name]['title'] = 
      $group_name . ': ' . $items['sirius_employer_feed_employer']['feed']['fields'][$field_name]['title'];
  }

  return $items;
}

function sirius_employer_feed_employer_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }

    // Validate integer fields
    if ($field_info['type'] == 'integer') {
      $row[$field_name] = preg_replace('/\,/', '', $row[$field_name]);
      if (!preg_match('/^[0-9]+$/', $row[$field_name])) { return array('success' => FALSE, 'msg' => "$field_name must be an integer."); }
    }

    // Validate list fields
    if ($field_info['type'] == 'list') {
      $value = sirius_feed_handle_list($field_name, $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the allowed values."); }
      $row[$field_name] = $value;
    }

    // Validate term_reference
    if ($field_info['type'] == 'term_reference') {
      $value = sirius_taxonomy_lookup_by_anything($field_info['taxonomy'], $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the values in the taxonomy: $field_info[taxonomy]."); }
      $row[$field_name] = $value;
    }

    // Validate date
    if ($field_info['type'] == 'date') {
      $value = sirius_feed_handle_date($field_name, $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name does not look like a valid date."); }
      $row[$field_name] = $value;
    }
  }

  $content_type = 'grievance_shop';
  if ($precompute) {
    if ($pk == 'nid') {
      $pk_result = sirius_feed_handle_pk_nid($content_type, $row['nid']);
      if (!$pk_result['success']) { return $pk_result; }
      $nid = $pk_result['nid'];
    } else if ($pk = 'sirius_id') {
      $result = sirius_feed_lookup_by_field(
        'grievance_shop', 
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

    foreach ($info['feed']['fields'] as $field_name => $field_info) {
      if (!$row[$field_name]) { continue; }

      // Validate entity_reference
      if ($field_info['type'] == 'entity_reference') {
        $value = sirius_feed_handle_entity_reference($field_info['entity_type'], $row[$field_name]);
        if (!$value) { return array('success' => FALSE, 'msg' => "No value found (or multiple values found) for $field_name in the entity type: $field_info[entity_type]."); }
        $row[$field_name] = $value;
      }
    }
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_employer_feed_employer_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = sirius_employer_feed_employer_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  if ($row['nid']) {
    $is_new = FALSE;
    $node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $node = new stdClass();
    $node->type = 'grievance_shop';
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_sirius_domain = $feed_node->field_sirius_domain;
  }

  if ($row['title']) { $node->title = $row['title']; }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long', 'date'))) {
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

  $node = node_submit($node);
  node_save($node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new property.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing property.');
  }
}
