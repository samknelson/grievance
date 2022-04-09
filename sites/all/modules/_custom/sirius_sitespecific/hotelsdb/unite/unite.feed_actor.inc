<?php

function unite_feed_actor_info() {
  $items = array();
  $items['unite_feed_actor'] = array(
    'name' => 'unite_feed_actor',
    'title' => 'Actors',
    'description' => 'Import new or existing actors.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      // 'options'   => array('label' => 'Options',    'callback' => 'unite_feed_actor_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Internal ID (NID)',
        'title' => 'Canonical Name',
        // 'id' => 'External ID',
        // 'addr' => 'Address',
      ),
      'create_new_records' => 'optional',
      'fields' => array(
        'nid' => array('group' => 'pk', 'title' => 'Internal ID (NID)'),
        'title' => array('group' => 'pk', 'title' => 'Title'),

        'actor_type' => array('group' => 'core', 'type' => 'term_reference', 'taxonomy' => 'actor_type'),

        'address_thoroughfare' => array('group' => 'location', 'title' => 'Address 1',),
        'address_premise' => array('group' => 'location', 'title' => 'Address 2',),
        'address_locality' => array('group' => 'location', 'title' => 'City',),
        'address_administrative_area' => array('group' => 'location', 'title' => 'State',),
        'address_postal_code' => array('group' => 'location', 'title' => 'Zip',),
        'address_country' => array('group' => 'location', 'title' => 'Country',),
        'phone' => array('group' => 'location', 'type' => 'text'),
        'notes' => array('group' => 'core', 'type' => 'long'),
     ),
      'batch_size' => 10,
      'callback_row_verify' => 'unite_feed_actor_row_verify',
      'callback_row_process' =>'unite_feed_actor_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_actor'),
  );

  $groups = array(
    'pk' => 'Primary Key',
    'core' => 'Core',
    'location' => 'Location',
  );
  foreach ($items['unite_feed_actor']['feed']['fields'] as $field_name => $field_info) {
    $group = $field_info['group'];
    $group_name = $groups[$group];

    if (!$field_info['title']) {
      $structure_info = field_info_instance('node', "field_$field_name", 'actor');
      if ($structure_info) { $items['unite_feed_actor']['feed']['fields'][$field_name]['title'] = $structure_info['label']; }
    }

    $items['unite_feed_actor']['feed']['fields'][$field_name]['title'] = $group_name . ': ' . $items['unite_feed_actor']['feed']['fields'][$field_name]['title'];
  }

  return $items;
}

function unite_feed_actor_options($info, $data, $feed_node, $current_step) { return drupal_get_form('unite_feed_actor_options_form', $info, $data, $feed_node, $current_step); }

function unite_feed_actor_options_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  $form['#tree'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function unite_feed_actor_options_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['charge_settings'] = $form_state['values']['charge_settings'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Payment selection saved.");
}

function unite_feed_actor_handle_list($field_name, $value) {
  $structure_info = field_info_field("field_$field_name");
  $allowed_values = list_allowed_values($structure_info);
  $allowed_values_flip = array_flip($allowed_values);

  // Match case insensitive
  foreach ($allowed_values as $a => $b) {
    if (trim(strtolower($a)) == trim(strtolower($value))) { return $a; }
    if (trim(strtolower($b)) == trim(strtolower($value))) { return $a; }

    if (trim(strtolower($value)) == 'yes') {
      if (trim(strtolower($a)) == 'y') { return $a; }
    }

    if (trim(strtolower($value)) == 'no') {
      if (trim(strtolower($a)) == 'n') { return $a; }
    }
  }

  return NULL;
}

function unite_feed_actor_handle_entity_reference($entity_type, $value) {
  $sql = "select min(nid) as mn, max(nid) as mx from node ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
  $sql .= "where nid = :v1 ";
  $sql .= "or title = :v2 ";
  $sql .= "or field_sirius_id_value = :v3 ";
  $sql .= "or field_sirius_name_short_value = :v4 ";
  $stmt = sirius_sql_query($sql, array(':v1' => $value, ':v2' => $value, ':v3' => $value, ':v4' => $value));
  $hr = $stmt->fetchAssoc();
  if (!$hr['mn']) { return NULL; }
  if ($hr['mn'] != $hr['mx']) { return NULL; }
  return $hr['mn'];
}

function unite_feed_actor_row_verify($feed_node, $info, $data, &$row, $precompute) {
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
      $value = unite_feed_actor_handle_list($field_name, $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the allowed values."); }
      $row[$field_name] = $value;
    }

    // Validate term_reference
    if ($field_info['type'] == 'term_reference') {
      $value = sirius_taxonomy_lookup_by_anything($field_info['taxonomy'], $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the values in the taxonomy: $field_info[taxonomy]."); }
      $row[$field_name] = $value;
    }
  }

  if ($precompute) {
    if ($pk == 'nid') {
      $nid = $row['nid'];
      if (!$nid) { return array('success' => FALSE, 'msg' => 'If the primary key is NID, all rows must have a node ID.'); }
      $probe = node_load($nid);
      if (!$probe) { return array('success' => FALSE, 'msg' => 'No node was found with this NID.'); }
      if ($probe->type != 'actor') { return array('success' => FALSE, 'msg' => 'The node for this NID was found, but it is not a actor.'); }

      $msg = "An existing actor will be updated.";
    } else if ($pk == 'title') {
      if (!$row[$pk]) { return array('success' => FALSE, 'msg' => "This record has no value for primary key $pk."); }
      $result = sirius_feed_lookup_by_field('actor', 'field_data_field_title_canon', 'field_title_canon_value', unite_actor_canonicalize_title($row[$pk]));

      if ($result['success']) {
        // We found exactly one. Use that NID.
        $row['nid'] = $result['nid'];
        $msg = "An existing actor will be updated.";
      } else {
        if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
          // Not found, but we can create new records. Do nothing
          $msg = "A new actor will be created.";
        } else {
          // We can't create new records, or we have a different kind of error
          return $result;
        }
      }
    }

    foreach ($info['feed']['fields'] as $field_name => $field_info) {
      if (!$row[$field_name]) { continue; }

      // Validate entity_reference
      if ($field_info['type'] == 'entity_reference') {
        $value = unite_feed_actor_handle_entity_reference($field_info['entity_type'], $row[$field_name]);
        if (!$value) { return array('success' => FALSE, 'msg' => "No value found (or multiple values found) for $field_name in the entity type: $field_info[entity_type]."); }
        $row[$field_name] = $value;
      }
    }
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function unite_feed_actor_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_actor_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  if ($row['nid']) {
    $is_new = FALSE;
    $actor_node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $actor_node = new stdClass();
    $actor_node->type = 'actor';
    $actor_node->language = LANGUAGE_NONE;
    $actor_node->uid = $user->uid;
    $actor_node->status = 1;
    $actor_node->promote = 0;
    $actor_node->comment = 0;
  }

  if ($row['title']) { $actor_node->title = $row['title']; }

  $need_country = FALSE;
  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    if ($row["address_$col"]) {
      $need_country = TRUE;
      $actor_node->field_address['und'][0][$col] = $row["address_$col"];
    }
  }
  if ($need_country && !$row['address_country']) { $actor_node->field_address['und'][0]['country'] = 'US'; }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long'))) {
        $actor_node->{"field_$field_name"}['und'][0]['value'] = $row[$field_name];
      }
      if ($field_info['type'] == 'term_reference') {
        $actor_node->{"field_$field_name"}['und'][0]['tid'] = $row[$field_name];
      }
      if ($field_info['type'] == 'entity_reference') {
        $actor_node->{"field_$field_name"}['und'][0]['target_id'] = $row[$field_name];
      }
    }
  }

  $actor_node = node_submit($actor_node);
  node_save($actor_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new actor.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing actor.');
  }
}
