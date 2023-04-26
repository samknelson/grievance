<?php

function sirius_smf_feed_dependents_row_process_rel($feed_node, $info, $data, $row) {

	$result = sirius_smf_feed_dependents_row_verify_rel($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

	$domain_nid = sirius_domain_nid();
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

	// ////////////////////////////////////////////////////////
  // Worker 
	// ////////////////////////////////////////////////////////

  if ($row['worker_nid']) {
    $worker_is_new = FALSE;
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
  if ($domain_nid) { $worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

  // SSN
  $worker_node->field_sirius_ssn['und'][0]['value'] = $row['ssn'];
  $worker_node->field_sirius_id2['und'][0]['value'] = $row['id2'];

  // Name
  $worker_node->field_sirius_name['und'][0]['given'] = $row['I'];
  $worker_node->field_sirius_name['und'][0]['middle'] = $row['J'];
  $worker_node->field_sirius_name['und'][0]['family'] = $row['H'];

  // Address
  $need_country = FALSE;
  $addr_fields = array(
  	'K' => 'thoroughfare',
  	'L' => 'premise',
  	'M' => 'locality',
  	'N' => 'administrative_area',
  	'O' => 'postal_code',
  );
  foreach ($addr_fields as $field_name => $col) {
    if ($row["$field_name"]) {
      $need_country = TRUE;
      $worker_node->field_sirius_address['und'][0][$col] = $row[$field_name];
    }
  }
  if ($need_country) { $worker_node->field_sirius_address['und'][0]['country'] = 'US'; }

  // Phone
  if ($row['phone']) {
  	$worker_node->field_sirius_phone['und'][0]['value'] = $row['phone'];
  }

  // Email
  if ($row['email']) { 
  	$worker_node->field_sirius_email['und'][0]['value'] = $row['email'];
  }

  // Gender
  if ($row['gender_tid']) {
  	$worker_node->field_sirius_gender['und'][0]['tid'] = $row['gender_tid'];
  }

  // Birth date
  if ($row['dob']) {
  	$worker_node->field_sirius_dob['und'][0]['value'] = $row['dob'];
  }

	$worker_node = node_submit($worker_node);
  node_save($worker_node);

	// ////////////////////////////////////////////////////////
  // Log entry
	// ////////////////////////////////////////////////////////

  // 
  // Create a "raw import data" log entry.
  // - Pull all the fields that are flagged "smf_log" = "yes"
  // - Only create the log entry if we don't already have an existing log entry with the same data.
  // This means that if we import multiple times with different data, we'll have different log entries.
  // 

  $log_text = array();
  foreach ($fields as $field_name => $field_record) {
  	if (preg_match('/yes/i', $field_record['smf_log'])) {
  		if (isset($row[$field_name])) {
  			$log_text[] = "$field_name: $field_record[title]: $row[$field_name]";
  		}
  	}
  }
  $log_text = join("\n", $log_text);
  $log_key = "smf:import:" . $worker_node->nid . ':' . md5($log_text);
  $result = sirius_feed_lookup_by_field('sirius_log', 'field_data_field_sirius_notes', 'field_sirius_notes_value', $log_key);
  if ($result['not_found']) {
		sirius_log('smf:import', 'raw', 'Raw import data', $log_text, $log_key, array($worker_node->nid));
	}

	// ////////////////////////////////////////////////////////
  // Relationship
	// ////////////////////////////////////////////////////////

	if ($row['relationship_node']) {
    $relationship_node = $row['relationship_node'];
		if ($relationship_node->needs_save) { 
		  $relationship_node->field_sirius_contact_alt['und'][0]['target_id'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
      $relationship_node = node_submit($relationship_node);
      node_object_prepare($relationship_node);
      node_save($relationship_node);
		}
	}

  /*
  // ////////////////////////////////////////////////////////
  // Elections
  // ////////////////////////////////////////////////////////

  if ($row['elections']) {
    foreach ($row['elections'] as $date => $election_node) {
      if (!$election_node->needs_save) { continue; }
      $election_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;
      node_save($election_node);
    }
  }

  // ////////////////////////////////////////////////////////
  // Coverage (aka "WB or Worker Benefit")
  // ////////////////////////////////////////////////////////

  if ($row['wb_nodes']) {
    foreach ($row['wb_nodes'] as $benefit_nid => $wb_nodes) {
      foreach ($wb_nodes as $date => $wb_node) {
        // if (!$wb_node->needs_save) { continue; }
        $wb_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;
        node_save($wb_node);
      }
    }
  }
  // drupal_set_message("Worker NID: " . $worker_node->nid);

  // ////////////////////////////////////////////////////////
  // Member status (aka "MS"; this also assigns industries
  // ////////////////////////////////////////////////////////

  if ($row['ms_assignments']) {
    foreach ($row['ms_assignments'] as $ms_date => $ms_record) {
      $result = sirius_worker_member_status_set($worker_node, $ms_record['ms_term'], TRUE, $ms_record['msg'], NULL, strtotime($ms_date));
      if (!$result['success']) { $warnings[] = "Member status update failed: " . $result['msg']; }
    }
  }
  */

	// ////////////////////////////////////////////////////////
	// Result
	// ////////////////////////////////////////////////////////

  if ($is_new) { $msg = 'Created a new worker.'; } else { $msg = 'Updated an existing worker.'; }
  return array('success' => TRUE, 'msg' => $msg, 'warnings' => $warnings);
}

function sirius_smf_feed_dependents_row_process_cov($feed_node, $info, $data, $row) {

  $result = sirius_smf_feed_dependents_row_verify_cov($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

  $domain_nid = sirius_domain_nid();
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // Delete old coverage
  $to_delete = $row['wb_nodes_to_delete'];
  if ($to_delete) {
    foreach ($to_delete as $nid => $node) { node_delete($nid); }
  }

  // Create new coverage
  $count_new = 0;
  foreach ($row['wb_nodes'] as $benefit_nid => $wb_nodes) {
    foreach ($wb_nodes as $wb_node) {
      ++$count_new;
      $wb_node = node_submit($wb_node);
      node_object_prepare($wb_node);
      node_save($wb_node);
    }
  }

  // drupal_set_message("$row[worker_nid]: $count_new records creasted.");

  return array('success' => TRUE, 'msg' => 'Coverage records created or updated.', 'warnings' => $warnings);
}
