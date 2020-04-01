<?php

function sirius_feed_contact_fields($info, $data, $feed_node, $current_step) {
  // Core fields
  $fields = array(
    'id' => array(
      'title' => 'ID',
      'required' => TRUE,
    ),
    'name_given' => array(
      'title' => 'First Name',
      'required' => FALSE,
    ),
    'name_family' => array(
      'title' => 'Last Name',
      'required' => FALSE,
    ),
    'name_middle' => array(
      'title' => 'Middle Name',
      'required' => FALSE,
    ),
    'address_thoroughfare' => array(
      'title' => 'Address 1',
      'required' => FALSE,
    ),
    'address_premise' => array(
      'title' => 'Address 2',
      'required' => FALSE,
    ),
    'address_locality' => array(
      'title' => 'City',
      'required' => FALSE,
    ),
    'address_administrative_area' => array(
      'title' => 'State',
      'required' => FALSE,
    ),
    'address_postal_code' => array(
      'title' => 'Zip',
      'required' => FALSE,
    ),
    'email' => array(
      'title' => 'Email',
      'required' => FALSE,
    ),
    'phone' => array(
      'title' => 'Phone',
      'required' => FALSE,
    ),
    'phone_alt' => array(
      'title' => 'Phone (Alt)',
      'required' => FALSE,
    ),
    'phone_mobile' => array(
      'title' => 'Phone (Mobile)',
      'required' => FALSE,
    ),
    'shop_name' => array(
      'title' => 'Current Employer Name',
      'required' => FALSE,
    ),
  );

  // Json fields
  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];
  if ($domain_nid) {
    $definition = sirius_contact_definition($domain_nid);
    $result = array();
    _sirius_feed_contact_fields_recursive($definition, 'JSON', '', $result);
    $fields += $result;
  }

  return $fields;
}

function _sirius_feed_contact_fields_recursive($obj, $prefix, $prefix_label, &$result) {
  if (!$obj) { return; }
  if (!is_array($obj)) { return; }

  $type = $obj['#type'];
  if (!$type || $type == 'fieldset') {
    foreach ($obj as $key => $value) {
      if (!is_array($value)) { continue; }
      if ($key == '#options') { continue; }
      $title = $prefix_label;
      if ($title) { $title .= ': '; }
      $title .= $obj['#title'];
      _sirius_feed_contact_fields_recursive($value, "$prefix:$key", $title, $result);
    }
  } else {
    $title = $prefix_label;
    if ($title) { $title .= ': '; }
    $title .= $obj['#title'];
    $result["$prefix"] = array(
      'title' => $title,
      'required' => FALSE,
    );
  }

  return $result;
}

function sirius_feed_contact_row_verify($feed_node, $info, $data, &$row, $precompute) {
	// Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

	// if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }

	if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
	if ($row['phone_alt'] && !sirius_phone_validate($row['phone_alt'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Alt)'); } else { $row['phone_alt'] = sirius_phone_format($row['phone_alt']); }

	if ($row['email'] && !sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }

  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

  // Contact
  if ($precompute) {
    $args = array();
    $args[':id'] = $row['id'];

  	$sql = "select max(field_data_field_sirius_id.entity_id) as a, min(field_data_field_sirius_id.entity_id) as b from field_data_field_sirius_id ";
    if ($domain_nid) {
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
    }
  	$sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
  	$sql .= "and field_data_field_sirius_id.bundle = 'sirius_contact' ";
  	$sql .= "and field_data_field_sirius_id.field_sirius_id_value = :id ";
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid";
      $args[':domain_nid'] = $domain_nid;
    }

  	$stmt = db_query($sql, $args);
  	$hr = $stmt->fetchAssoc();
  	$nid = $hr['a'];
  	$other_nid = $hr['b'];
  	if ($nid != $other_nid) {
  		return array('success' => FALSE, 'msg' => 'Multiple contacts found with this ID.');
  	}
  	$row['contact_nid'] = $nid;
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_contact_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_contact_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	if ($row['contact_nid']) {
		$is_new = FALSE;
		$contact_node = node_load($row['contact_nid']);
	} else {
		$is_new = TRUE;
    global $user;
    $contact_node = new stdClass();
    $contact_node->type = 'sirius_contact';
    $contact_node->language = LANGUAGE_NONE;
    $contact_node->uid = $user->uid;
    $contact_node->status = 1;
    $contact_node->promote = 0;
    $contact_node->comment = 0;
    $contact_node->field_sirius_id['und'][0]['value'] = $row['id'];
  }

  $contact_node->field_sirius_name['und'][0]['given'] = $row['name_given'];
  $contact_node->field_sirius_name['und'][0]['middle'] = $row['name_middle'];
  $contact_node->field_sirius_name['und'][0]['family'] = $row['name_family'];
  $contact_node->field_sirius_source['und'][0]['value'] = $data['source'];

  $phone1 = $row['phone'];
  $phone2 = $row['phone_alt'];
  if ($row['phone_mobile']) {
  	if (!$phone1) { $phone1 = $row['phone_mobile']; } else { $phone2 = $row['phone_mobile']; }
  }
  if ($phone1) { $contact_node->field_sirius_phone['und'][0]['value'] = $phone1; }
  if ($phone2) { $contact_node->field_sirius_phone_alt['und'][0]['value'] = $phone2; }
  if ($row['email']) { $contact_node->field_sirius_email['und'][0]['value'] = $row['email']; }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $col) {
  	$has_address = FALSE;
  	if ($row["address_$col"]) {
  		$has_address = TRUE;
  		$contact_node->field_sirius_address['und'][0][$col] = $row["address_$col"];
  	}
  	if ($has_address) {
  		$contact_node->field_sirius_address['und'][0]['country'] = 'US';
  	}
  }

  $json = array();
  foreach ($row as $key => $value) {
    if (preg_match('/JSON:/', $key)) {
      sirius_jsonfield_util_set_from_path($json, preg_replace('/JSON:/', '', $key), $value);
    }
  }
  $contact_node->field_sirius_json['und'][0]['value'] = json_encode($json);
  $contact_node->field_sirius_domain = $feed_node->field_sirius_domain;

  node_save($contact_node);

  if ($is_new) {
  	return array('success' => TRUE, 'msg' => 'Created a new contact.');
  } else {
  	return array('success' => TRUE, 'msg' => 'Updated an existing contact.');
  }
}