<?php

function sirius_feed_worker_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

	static $employer_map;
	if (!$employer_map) {
		$sql = "select title, nid from node where type = 'grievance_shop'";
		$stmt = db_query($sql, array());
		while ($hr = $stmt->fetchAssoc()) {
			$employer_map[$hr['title']] = $hr['nid'];
		}
	}

	// Trim all
	foreach ($row as $key => $value) {
		$row[$key] = trim($value);
	}

  $row['ssn'] = sirius_ssn_format($row['ssn']);
  if ($pk == 'id') {
	 if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  }

	if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
	if ($row['phone_alt'] && !sirius_phone_validate($row['phone_alt'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Alt)'); } else { $row['phone_alt'] = sirius_phone_format($row['phone_alt']); }
	if ($row['phone_mobile'] && !sirius_phone_validate($row['phone_mobile'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Mobile)'); } else { $row['phone_mobile'] = sirius_phone_format($row['phone_mobile']); }

	if ($row['email'] && !sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }

  // Worker
  if ($precompute) {
    if ($pk == 'id') {
    	$sql = "select max(entity_id) as a, min(entity_id) as b from field_data_field_sirius_id ";
    	$sql .= "where entity_type = 'node' ";
    	$sql .= "and bundle = 'sirius_worker' ";
    	$sql .= "and field_sirius_id_value = :id ";
    	$stmt = db_query($sql, array(':id' => $row['id']));
    	$hr = $stmt->fetchAssoc();
    	$nid = $hr['a'];
    	$other_nid = $hr['b'];
    	if ($nid != $other_nid) {
    		return array('success' => FALSE, 'msg' => 'Multiple workers found with this ID.');
    	}
    	$row['worker_nid'] = $nid;
    } else if ($pk == 'ssn') {
      $sql = "select max(entity_id) as a, min(entity_id) as b from field_data_field_sirius_ssn ";
      $sql .= "where entity_type = 'node' ";
      $sql .= "and bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_ssn_value = :ssn ";
      $stmt = db_query($sql, array(':ssn' => $row['ssn']));
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this SSN.');
      }
      $row['worker_nid'] = $nid;
    }
  } 

  // Current Employer
  if ($row['shop_name']) {
  	$shop_nid = $employer_map[$row['shop_name']];
  	if (!$shop_nid) { return array('success' => FALSE, 'msg' => 'Employer "' . $row['shop_name'] . '" not found.'); }
  	if ($precompute) { $row['shop_nid'] = $shop_nid; }
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_worker_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_worker_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	if ($row['worker_nid']) {
		$is_new = FALSE;
		$worker_node = node_load($row['worker_nid']);
	} else {
		$is_new = TRUE;
    global $user;
    $worker_node = new stdClass();
    $worker_node->type = 'sirius_worker';
    $worker_node->language = LANGUAGE_NONE;
    $worker_node->uid = $user->uid;
    $worker_node->status = 1;
    $worker_node->promote = 0;
    $worker_node->comment = 0;
    $worker_node->field_sirius_id['und'][0]['value'] = $row['id'];
  }

  $worker_node->field_sirius_id3['und'][0]['value'] = $row['id3'];
  $worker_node->field_sirius_ssn['und'][0]['value'] = $row['ssn'];

  $worker_node->field_sirius_name['und'][0]['given'] = $row['name_given'];
  $worker_node->field_sirius_name['und'][0]['middle'] = $row['name_middle'];
  $worker_node->field_sirius_name['und'][0]['family'] = $row['name_family'];

  $phone1 = $row['phone'];
  $phone2 = $row['phone_alt'];
  if ($row['phone_mobile']) {
  	if (!$phone1) { $phone1 = $row['phone_mobile']; } else { $phone2 = $row['phone_mobile']; }
  }
  if ($phone1) { $worker_node->field_sirius_phone['und'][0]['value'] = $phone1; }
  if ($phone2) { $worker_node->field_sirius_phone_alt['und'][0]['value'] = $phone2; }
  if ($row['email']) { $worker_node->field_sirius_email['und'][0]['value'] = $row['email']; }

  if ($row['shop_nid']) { $worker_node->field_grievance_shop['und'][0]['target_id'] = $row['shop_nid']; }


  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $col) {
  	$has_address = FALSE;
  	if ($row["address_$col"]) {
  		$has_address = TRUE;
  		$worker_node->field_sirius_address['und'][0][$col] = $row["address_$col"];
  	}
  	if ($has_address) {
  		$worker_node->field_sirius_address['und'][0]['country'] = 'US';
  	}
  }

  node_save($worker_node);

  if ($is_new) {
  	return array('success' => TRUE, 'msg' => 'Created a new worker.');
  } else {
  	return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
  }
}
