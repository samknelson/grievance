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

  // Member Status
  if ($row['member_status']) {
    $member_status_tid = sirius_taxonomy_lookup_by_anything('sirius_member_status', $row['member_status']);
    if (!$member_status_tid) { return array('success' => FALSE, 'msg' => 'Member status [' . $row['member_status'] . '] does not match the TID, name, short name, or ID of any member status taxonomy term.'); }
    if ($precompute) {
      $row['member_status_tid'] = $member_status_tid;
    }
  }

  // Job NID
  if ($row['dispatch_job_nid']) {
    $sql = "select type from node where nid = :dispatch_job_nid";
    $stmt = db_query($sql, array(':dispatch_job_nid' => $row['dispatch_job_nid']));
    $hr = $stmt->fetchAssoc();
    if ($hr['type'] != 'sirius_dispatch_job') {
      return array('success' => FALSE, 'msg' => 'Dispatch to Job [' . $row['dispatch_job_nid'] . '] does not correspond to a node of type "sirius_dispatch_job".');
    }
    if ($precompute) {
      $row['job_node'] = node_load($row['dispatch_job_nid']);
    }
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
  }

  if ($row['id'])  { $worker_node->field_sirius_id['und'][0]['value'] = $row['id']; }
  if ($row['id3']) { $worker_node->field_sirius_id3['und'][0]['value'] = $row['id3']; }
  if ($row['ssn']) { $worker_node->field_sirius_ssn['und'][0]['value'] = $row['ssn']; }

  if ($row['name_given'])  { $worker_node->field_sirius_name['und'][0]['given'] = $row['name_given']; }
  if ($row['name_middle']) { $worker_node->field_sirius_name['und'][0]['middle'] = $row['name_middle']; }
  if ($row['name_family']) { $worker_node->field_sirius_name['und'][0]['family'] = $row['name_family']; }

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

  if ($row['member_status_tid']) {
    $member_status_term = taxonomy_term_load($row['member_status_tid']);
    sirius_worker_member_status_set($worker_node, $member_status_term, TRUE, 'Feed upload');
  }

  if ($row['dispatch_job_nid']) {
    $result = sirius_dispatch_actions_create($row['job_node'], $worker_node->nid, 'No', 'accepted');
  }

  if ($is_new) {
  	return array('success' => TRUE, 'msg' => 'Created a new worker.');
  } else {
  	return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
  }
}
