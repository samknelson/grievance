<?php

function unite_feed_airport_info() {
  $items = array();
  $items['unite_feed_airport'] = array(
    'name' => 'unite_feed_airport',
    'title' => 'Airports',
    'description' => 'Import new or existing airports.',
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
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'unite_feed_airport_row_verify',
      'callback_row_process' =>'unite_feed_airport_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_airport'),
  );

  return $items;
}

function unite_feed_airport_row_verify($feed_node, $info, $data, &$row, $precompute) {
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
    $sql .= "where node.type = 'hdb_airport' ";
    $sql .= "and $column = :value ";
    $sql_args[':value'] = $value;
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $airport_nid = $hr['a'];
    $other_nid = $hr['b'];
    if ($airport_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple airports found with this ID ($table, $column)."); }

    $row['airport_nid'] = $airport_nid;
    if ($row['airport_nid']) { 
      return array('success' => TRUE, 'msg' => "An existing airport was found and will be updated.");
    } else {
      return array('success' => TRUE, 'msg' => "No existing airport was found. A new airport will be created.");
    }
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function unite_feed_airport_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_airport_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  if ($row['airport_nid']) {
    $is_new = FALSE;
    $airport_node = node_load($row['airport_nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $airport_node = new stdClass();
    $airport_node->type = 'hdb_airport';
    $airport_node->language = LANGUAGE_NONE;
    $airport_node->uid = $user->uid;
    $airport_node->status = 1;
    $airport_node->promote = 0;
    $airport_node->comment = 0;
  }

  if ($row['title']) { $airport_node->title = $row['title']; }
  if ($row['field_sirius_name_short']) { $airport_node->field_sirius_name_short['und'][0]['value'] = $row['field_sirius_name_short']; }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    if ($row["address_$col"]) {
      $airport_node->field_address['und'][0][$col] = $row["address_$col"];
    }
  }

  $airport_node = node_submit($airport_node);
  node_save($airport_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new airport.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing airport.');
  }
}
