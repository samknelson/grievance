<?php

function unite_feed_convention_info() {
  $items = array();
  $items['unite_feed_convention'] = array(
    'name' => 'unite_feed_convention',
    'title' => 'conventions',
    'description' => 'Import new or existing conventions.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        // 'nid' => 'Internal ID (NID)',
        // 'id' => 'External ID',
        'field_sirius_name_short' => 'Abbreviation  ',
      ),
      'fields' => array(
        // 'nid' => array('title' => 'Internal ID (NID)'),
        'field_sirius_name_short' => array('title' => 'Abbreviation'),
        'title' => array('title' => 'Title'),
        'address_thoroughfare' => array(
          'title' => 'Address 1',
        ),
        'address_premise' => array(
          'title' => 'Address 2',
        ),
        'address_locality' => array(
          'title' => 'City',
        ),
        'address_administrative_area' => array(
          'title' => 'State',
        ),
        'address_postal_code' => array(
          'title' => 'Zip',
        ),
        'address_country' => array(
          'title' => 'Country',
        ),
        'fh_booth_capacity' => array('title' => 'Booth Capacity'),
        'notes' => array('title' => 'Notes'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'unite_feed_convention_row_verify',
      'callback_row_process' =>'unite_feed_convention_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_convention'),
  );

  return $items;
}

function unite_feed_convention_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  // Verify country
  if ($row['address_country']) {
    if (strlen($row['address_country']) != 2) {
      return array('success' => FALSE, 'msg' => 'Enter a two-letter country code.');
    }
  }

  if ($precompute) {
    $row['address_country'] = strtoupper($row['address_country']);

    if ($pk == 'field_sirius_name_short') { 
      $table = 'field_data_field_sirius_name_short';
      $column = 'field_sirius_name_short_value';
      $value = $row['field_sirius_name_short'];
    }

    $sql = "select max(nid) as a, min(nid) as b from node ";
    $sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
    $sql .= "where node.type = 'hdb_convention_center' ";
    $sql .= "and $column = :value ";
    $sql_args[':value'] = $value;
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $convention_nid = $hr['a'];
    $other_nid = $hr['b'];
    if ($convention_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple conventions found with this ID ($table, $column)."); }

    $row['convention_nid'] = $convention_nid;
    if ($row['convention_nid']) { 
      return array('success' => TRUE, 'msg' => "An existing convention was found and will be updated.");
    } else {
      return array('success' => TRUE, 'msg' => "No existing convention was found. A new convention will be created.");
    }
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function unite_feed_convention_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_convention_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  if ($row['convention_nid']) {
    $is_new = FALSE;
    $convention_node = node_load($row['convention_nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $convention_node = new stdClass();
    $convention_node->type = 'hdb_convention_center';
    $convention_node->language = LANGUAGE_NONE;
    $convention_node->uid = $user->uid;
    $convention_node->status = 1;
    $convention_node->promote = 0;
    $convention_node->comment = 0;
  }

  if ($row['title']) { $convention_node->title = $row['title']; }
  if ($row['field_sirius_name_short']) { $convention_node->field_sirius_name_short['und'][0]['value'] = $row['field_sirius_name_short']; }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    if ($row["address_$col"]) {
      $convention_node->field_address['und'][0][$col] = $row["address_$col"];
    }
  }

  foreach (array('fh_booth_capacity', 'notes') as $field) {
    if ($row[$field]) {
      $convention_node->{"field_$field"}['und'][0]['value'] = $row[$field];
    }
  }

  $convention_node = node_submit($convention_node);
  node_save($convention_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new convention.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing convention.');
  }
}
