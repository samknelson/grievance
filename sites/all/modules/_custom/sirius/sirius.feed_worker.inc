<?php

function sirius_feed_worker_info() {
  $items = array();
  $items['feed_worker'] = array(
    'name' => 'feed_worker',
    'title' => 'Workers',
    'description' => 'Create or update workers. Workers are generally union members or other individuals who may be dispatched to a job.',
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
        'id' => 'ID',
        'ssn' => 'SSN',
        'nid' => 'NID',
      ),
      'tag_taxonomy' => 'sirius_contact_tags',
      'batch_size' => 50,
      'callback_fields' => 'sirius_feed_worker_fields',
      'callback_row_verify' => 'sirius_feed_worker_row_verify',
      'callback_row_process' =>'sirius_feed_worker_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_worker'),
  );
  return $items;
} 

function sirius_feed_worker_fields($info, $data, $feed_node, $current_step) {
  // Core fields
  $fields = array(
    'ssn' => array('title' => 'SSN'),
    'id' => array('title' => 'ID'),
    'id2' => array('title' => 'ID2'),
    'id3' => array('title' => 'ID3'),
    'nid' => array('title' => 'NID'),
    'name_given' => array('title' => 'First Name'),
    'name_family' => array('title' => 'Last Name'),
    'name_middle' => array('title' => 'Middle Name'),
    'address_thoroughfare' => array('title' => 'Address 1'),
    'address_premise' => array('title' => 'Address 2'),
    'address_locality' => array('title' => 'City'),
    'address_administrative_area' => array('title' => 'State'),
    'address_postal_code' => array('title' => 'Zip'),
    'email' => array('title' => 'Email'),
    'phone' => array('title' => 'Phone'),
    'phone_alt' => array('title' => 'Phone (Alt)'),
    'phone_mobile' => array('title' => 'Phone (Mobile)'),
    'shop_name' => array('title' => 'Current Employer Name'),
    'member_status' => array('title' => 'Member Status'),
    'dispatch_job_nid' => array('title' => 'Dispatch to Job (NID)'),
    'level' => array('title' => 'Training Center Level Code'),
  );

  // Json fields
  $contact_definition_json = sirius_contact_definition();
  $contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
  $paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));
  foreach ($paths as $path) {
    $fields["CONTACT_JSON:$path"] = array('title' => "contact:$path");
  }

  return $fields;
}

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
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  $row['ssn'] = sirius_ssn_format($row['ssn']);
  if ($pk == 'id') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  } else if ($pk == 'nid') {
   if (!preg_match('/^[0-9]+$/', $row['nid'])) { return array('success' => FALSE, 'msg' => 'Invalid NID'); }
  }

  if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
  if ($row['phone_alt'] && !sirius_phone_validate($row['phone_alt'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Alt)'); } else { $row['phone_alt'] = sirius_phone_format($row['phone_alt']); }
  if ($row['phone_mobile'] && !sirius_phone_validate($row['phone_mobile'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Mobile)'); } else { $row['phone_mobile'] = sirius_phone_format($row['phone_mobile']); }

  if ($row['email'] && !sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }

  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

  // Worker
  if ($precompute) {
    if ($pk == 'id') {
      $sql_args = array();
      $sql_args[':id'] = $row['id'];

      $sql = "select max(field_data_field_sirius_id.entity_id) as a, min(field_data_field_sirius_id.entity_id) as b from field_data_field_sirius_id ";
      if ($domain_nid) {
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
      }
      $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
      $sql .= "and field_data_field_sirius_id.bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_id_value = :id ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid";
        $sql_args[':domain_nid'] = $domain_nid;
      }
      $stmt = db_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this ID.');
      }
      $row['worker_nid'] = $nid;
    } else if ($pk == 'ssn') {
      $sql_args = array();
      $sql_args[':ssn'] = $row['ssn'];

      $sql = "select max(field_data_field_sirius_ssn.entity_id) as a, min(field_data_field_sirius_ssn.entity_id) as b from field_data_field_sirius_ssn ";
      if ($domain_nid) {
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_ssn.entity_id ";
      }
      $sql .= "where field_data_field_sirius_ssn.entity_type = 'node' ";
      $sql .= "and field_data_field_sirius_ssn.bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_ssn_value = :ssn ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid";
        $sql_args[':domain_nid'] = $domain_nid;
      }
      $stmt = db_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this SSN.');
      }
      $row['worker_nid'] = $nid;
    } else if ($pk == 'nid') {
      $args = array();
      $args[':nid'] = $row['nid'];
      $sql = "select nid from node ";
      if ($domain_nid) {
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      }
      $sql .= "where node.type = 'sirius_worker' ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $args[':domain_nid'] = $domain_nid;
      }
      $sql .= "and node.nid = :nid ";

      $stmt = sirius_sql_query($sql, $args);
      $hr = $stmt->fetchAssoc();
      $nid = $hr['nid'];
      if (!$nid) { return array('success' => FALSE, 'msg' => 'Worker not found in this domain.'); }
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

  // Level
  if ($row['level']) {
    static $all_levels_by_code;
    if (!$all_levels_by_code && function_exists('sirius_training_levels_fetch_all')) {
      $all_levels = sirius_training_levels_fetch_all();
      foreach ($all_levels as $ms_tid => $levels) {
        foreach ($levels as $level_code => $level) {
          $all_levels_by_code[$level_code] = $ms_tid;
        }
      }
    }

    $level_ms_tid = $all_levels_by_code[$row['level']];
    if (!$level_ms_tid) { return array('success' => FALSE, 'msg' => "Unable to find level: $row[level]"); }
    if ($precompute) { $row['level_ms_tid'] = $level_ms_tid; }
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

  $worker_node->field_sirius_domain = $feed_node->field_sirius_domain;

  if ($row['id'])  { $worker_node->field_sirius_id['und'][0]['value'] = $row['id']; }
  if ($row['id2']) { $worker_node->field_sirius_id2['und'][0]['value'] = $row['id2']; }
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

  if ($row['level'] && $row['level_ms_tid']) {
    $level = $row['level'];
    $ms_tid = $row['level_ms_tid'];
    $json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);

    if ($json['training']['levels'][$ms_tid]['code'] != $level) {
      $json['training']['levels'][$ms_tid] = array(
        'code' => $level,
        'ts' => time(),
      );
      $worker_node->field_sirius_json['und'][0]['value'] = json_encode($json);

      // Log
      if ($worker_node->nid) {
        sirius_log(
          'worker:level', 
          'set', 
          $worker_node->title . "'s level has been imported as " . $level, 
          '', 
          '', 
          array($worker_node->nid),
          array(),
          array('ms_tid' => $ms_tid, 'level' => $level)
        );
      }
    }
  }

  try {
    node_save($worker_node);
  } catch (exception $e) {
    return array('success' => FALSE, 'msg' => 'node_save() failed.');
  }

  if ($row['member_status_tid']) {
    $member_status_term = taxonomy_term_load($row['member_status_tid']);
    sirius_worker_member_status_set($worker_node, $member_status_term, TRUE, 'Feed upload');
  }

  if ($row['dispatch_job_nid']) {
    $result = sirius_dispatch_actions_create($row['job_node'], $worker_node->nid, 'No', 'accepted');
  }

  //
  // Contact
  //

  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
  $need_save_contact = FALSE;
  if ($data['tag']) { sirius_contact_tag_add($contact_node, $data['tag']); }
  if ($contact_node->field_sirius_source['und'][0]['value'] != $data['source']) {
    $contact_node->field_sirius_source['und'][0]['value'] = $data['source'];
    $need_save_contact = TRUE;
  }

  $contact_json = sirius_jsonfield_util_node_json_get($contact_node);
  $contact_results_json = sirius_jsonfield_util_results_get($contact_json, 'default');
  foreach ($row as $key => $value) {
    if (preg_match('/CONTACT_JSON:/', $key)) {
      sirius_jsonfield_util_set_from_path($contact_results_json, preg_replace('/CONTACT_JSON:/', '', $key), $value);
      $need_save_contact = TRUE;
    }
  }
  sirius_jsonfield_util_results_set($contact_json, 'default', $contact_results_json);
  sirius_jsonfield_util_node_json_set($contact_node, $contact_json);

  if ($need_save_contact) { 
    try {
      node_save($contact_node);
    } catch (exception $e) {
      return array('success' => FALSE, 'msg' => 'contact node_save() failed.');
    }
  }

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new worker.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
  }
}
