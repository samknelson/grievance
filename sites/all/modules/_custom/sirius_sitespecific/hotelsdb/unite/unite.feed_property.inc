<?php

function unite_feed_property_info() {
  $items = array();
  $items['unite_feed_property'] = array(
    'name' => 'unite_feed_property',
    'title' => 'Properties',
    'description' => 'Import new or existing properties.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      // 'options'   => array('label' => 'Options',    'callback' => 'unite_feed_property_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Internal ID (NID)',
        'pk_addr' => 'Address',
        // 'id' => 'External ID',
        // 'addr' => 'Address',
      ),
      'create_new_records' => 'optional',
      'fields' => array(
        'nid' => array('group' => 'pk', 'title' => 'Internal ID (NID)'),

        'title' => array('group' => 'core', 'title' => 'Title'),
        'rooms' => array('group' => 'core', 'type' => 'integer'),
        'brand' => array('group' => 'core', 'type' => 'term_reference', 'taxonomy' => 'brand'),
        'niche' => array('group' => 'core', 'type' => 'term_reference', 'taxonomy' => 'hdb_niche'),
        'development_status' => array('group' => 'core', 'type' => 'list'),
        'owner_of_record' => array('group' => 'core', 'type' => 'text'),
        'notes' => array('group' => 'core', 'type' => 'long'),
        'legacy_id' => array('group' => 'core', 'type' => 'text'),

        'address_thoroughfare' => array('group' => 'location', 'title' => 'Address 1',),
        'address_premise' => array('group' => 'location', 'title' => 'Address 2',),
        'address_locality' => array('group' => 'location', 'title' => 'City',),
        'address_administrative_area' => array('group' => 'location', 'title' => 'State',),
        'address_postal_code' => array('group' => 'location', 'title' => 'Zip',),
        'address_country' => array('group' => 'location', 'title' => 'Country',),
        'on_public_land' => array('group' => 'location', 'type' => 'list'),
        'county' => array('group' => 'location', 'type' => 'text'),
        'neighborhood' => array('group' => 'location', 'type' => 'text'),
        'pin' => array('group' => 'core', 'location' => 'text'),
        'phone' => array('group' => 'location', 'type' => 'text'),
        'fax' => array('group' => 'location', 'type' => 'text'),
        'msa' => array('group' => 'location', 'type' => 'term_reference', 'taxonomy' => 'msa'),

        'local' => array('group' => 'contract', 'type' => 'entity_reference', 'entity_type' => 'local'),
        'hotel_contract_covered' => array('group' => 'contract', 'type' => 'integer'),
        'rest_contract_covered' => array('group' => 'contract', 'type' => 'integer'),
        'union_status' => array('group' => 'contract', 'type' => 'list'),
        'union_other' => array('group' => 'contract', 'type' => 'text',),
        'hotel_contract_start_date' => array('group' => 'contract', 'type' => 'date'),
        'hotel_contract_end_date' => array('group' => 'contract', 'type' => 'date'),
        'rest_contract_start_date' => array('group' => 'contract', 'type' => 'date'),
        'rest_contract_end_date' => array('group' => 'contract', 'type' => 'date'),

        'convention_center' => array('group' => 'convention', 'type' => 'entity_reference', 'entity_type' => 'hdb_convention_center'),
        'convention_center_attached' => array('group' => 'convention', 'type' => 'list'),

        'fh_meeting_room_count' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_meeting_room_capacity' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_meeting_room_largest' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_event_space' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_ballroom_count' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_ballroom_capacity' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_outdoor_area' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_parking_capacity' => array('group' => 'amenities', 'type' => 'integer'),
        'fh_outdoor' => array('group' => 'amenities', 'type' => 'list'),
        'fh_parking' => array('group' => 'amenities', 'type' => 'list'),

        'csno_type' => array('group' => 'casino', 'title' => 'Casino Type', 'type' => 'list'),
        'csno_slots_count' => array('group' => 'casino', 'title' => 'Slots count', 'type' => 'integer'),
        'csno_tables_count' => array('group' => 'casino', 'title' => 'Tables count', 'type' => 'integer'),
        'is_casino' => array('group' => 'casino', 'type' => 'list'),
        'csno_book' => array('group' => 'casino', 'type' => 'list'),
        'csno_horse' => array('group' => 'casino', 'type' => 'list'),
        'csno_online' => array('group' => 'casino', 'type' => 'list'),
        'csno_regulator' => array('group' => 'casino', 'type' => 'text'),
        'csno_source' => array('group' => 'casino', 'type' => 'long'),
        'csno_slots_date' => array('group' => 'casino', 'type' => 'date'),
        'csno_tables_date' => array('group' => 'casino', 'type' => 'date'),

        'labor_activity_index' => array('group' => 'fairhotel', 'type' => 'list'),
        'fh_notes' => array('group' => 'fairhotel', 'type' => 'long'),
        'fh_nonunion' => array('group' => 'fairhotel', 'type' => 'long'),
        'fh_sources' => array('group' => 'fairhotel', 'type' => 'long'),
        'fh_last_updated' => array('group' => 'fairhotel', 'type' => 'date'),
     ),
      'batch_size' => 10,
      'callback_row_verify' => 'unite_feed_property_row_verify',
      'callback_row_process' =>'unite_feed_property_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_property'),
  );

  $groups = array(
    'pk' => 'Primary Key',
    'core' => 'Core',
    'amenities' => 'Amenities',
    'casino' => 'Casino',
    'fairhotel' => 'FairHotel',
    'airport' => 'Airport',
    'convention' => 'Convention',
    'location' => 'Location',
    'contract' => 'Contract',

  );
  foreach ($items['unite_feed_property']['feed']['fields'] as $field_name => $field_info) {
    $group = $field_info['group'];
    $group_name = $groups[$group];

    if (!$field_info['title']) {
      $structure_info = field_info_instance('node', "field_$field_name", 'property');
      if ($structure_info) { $items['unite_feed_property']['feed']['fields'][$field_name]['title'] = $structure_info['label']; }
    }

    $items['unite_feed_property']['feed']['fields'][$field_name]['title'] = $group_name . ': ' . $items['unite_feed_property']['feed']['fields'][$field_name]['title'];
  }

  return $items;
}

function unite_feed_property_options($info, $data, $feed_node, $current_step) { return drupal_get_form('unite_feed_property_options_form', $info, $data, $feed_node, $current_step); }

function unite_feed_property_options_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
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

function unite_feed_property_options_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['charge_settings'] = $form_state['values']['charge_settings'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Payment selection saved.");
}

function unite_feed_property_handle_list($field_name, $value) {
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

function unite_feed_property_handle_date($field_name, $value) {
  $ts = strtotime($value);
  if (!$ts) { return NULL; }
  return gmdate('Y-m-d', $ts);
}

function unite_feed_property_handle_entity_reference($entity_type, $value) {
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

function unite_feed_property_row_verify($feed_node, $info, $data, &$row, $precompute) {
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
      $value = unite_feed_property_handle_list($field_name, $row[$field_name]);
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
      $value = unite_feed_property_handle_date($field_name, $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name does not look like a valid date."); }
      $row[$field_name] = $value;
    }
  }

  if ($precompute) {
    if ($pk == 'nid') {
      $nid = $row['nid'];
      if (!$nid) { return array('success' => FALSE, 'msg' => 'If the primary key is NID, all rows must have a node ID.'); }
      $probe = node_load($nid);
      if (!$probe) { return array('success' => FALSE, 'msg' => 'No node was found with this NID.'); }
      if ($probe->type != 'property') { return array('success' => FALSE, 'msg' => 'The node for this NID was found, but it is not a property.'); }
    } else if ($pk = 'pk_addr') {
      $parts = array('thoroughfare', 'locality', 'administrative_area', 'postal_code', 'country');
      $addr = array();
      foreach ($parts as $part) {
        if ( ($part != 'locality') && !$row["address_$part"]) {
          return array('success' => FALSE, 'msg' => "Please include all the address parts.");
        }
        $addr[$part] = $row["address_$part"];
      }

      // if ($rowcount > 5) { return array('success' => FALSE, 'msg' => 'Be kind.'); }

      $result = sirius_addr_validate_full($addr);
      if (!$result['success']) { return $result; }
      if (!$result['point']) { return array('success' => FALSE, 'msg' => 'No point in the address result?'); }

      $addr_canon = $result['point']->data['geocoder_formatted_address'];
      if (!$addr_canon) { return array('success' => FALSE, 'msg' => 'No geocoder_formatted_address in the data for the point returned from Google.'); }

      $result = sirius_feed_lookup_by_field(
        'property', 
        'field_data_field_address_canon', 
        'field_address_canon_value', 
        $addr_canon
      );

      if ($result['success']) {
        // We found exactly one. Use that NID.
        $row['nid'] = $result['nid'];
        $msg = "An existing property will be updated.";
      } else {
        if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
          // Not found, but we can create new records. Do nothing
          $msg = "A new property will be created.";
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
        $value = unite_feed_property_handle_entity_reference($field_info['entity_type'], $row[$field_name]);
        if (!$value) { return array('success' => FALSE, 'msg' => "No value found (or multiple values found) for $field_name in the entity type: $field_info[entity_type]."); }
        $row[$field_name] = $value;
      }
    }

    // Clean postal codes
    $row['address_postal_code'] = sirius_addr_zip_zeropad($row['address_postal_code']);
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function unite_feed_property_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_property_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  if ($row['nid']) {
    $is_new = FALSE;
    $property_node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $property_node = new stdClass();
    $property_node->type = 'property';
    $property_node->language = LANGUAGE_NONE;
    $property_node->uid = $user->uid;
    $property_node->status = 1;
    $property_node->promote = 0;
    $property_node->comment = 0;
  }

  if ($row['title']) { $property_node->title = $row['title']; }

  $need_country = FALSE;
  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    if ($row["address_$col"]) {
      $need_country = TRUE;
      $property_node->field_address['und'][0][$col] = $row["address_$col"];
    }
  }
  if ($need_country && !$row['address_country']) { $property_node->field_address['und'][0]['country'] = 'US'; }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long', 'date'))) {
        $property_node->{"field_$field_name"}['und'][0]['value'] = $row[$field_name];
      }
      if ($field_info['type'] == 'term_reference') {
        $property_node->{"field_$field_name"}['und'][0]['tid'] = $row[$field_name];
      }
      if ($field_info['type'] == 'entity_reference') {
        $property_node->{"field_$field_name"}['und'][0]['target_id'] = $row[$field_name];
      }
    }
  }

  $property_node = node_submit($property_node);
  node_save($property_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new property.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing property.');
  }
}
