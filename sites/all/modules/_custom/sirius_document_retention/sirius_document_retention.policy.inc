<?php

function sirius_document_retention_policy_list() {
	$policy_infos = module_invoke_all('sirius_document_retention_policy_info');
	return $policy_infos;
}

function sirius_document_retention_policy_info($key) {
	$list = sirius_document_retention_policy_list();
	return $list[$key];
}

function sirius_document_retention_policy_execute($key, $mode) {
	sirius_document_retention_log(NULL, 'reset');
	sirius_document_retention_log('Starting: ' . $key . ' (MODE: ' . $mode . ')');
	$policy = sirius_document_retention_policy_info($key);
	if (!$policy) {
		sirius_document_retention_log('No policy found for ' . $key . ' (MODE: ' . $mode . ')', 'error');
	} else  {
		$status = $policy['purge_callback']($key, $mode);
	}
	sirius_document_retention_log('Finishing: ' . $key);

  global $user;
  $log_node = new stdClass();
  $log_node->title = "$key - $mode - " . date('c');
  $log_node->type = 'sirius_document_retention_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;
  $log_node->field_sirius_log['und'][0]['value'] = sirius_document_retention_log_render();
  $log_node->field_sirius_docret_key['und'][0]['value'] = $key;
  $log_node->field_sirius_docret_mode['und'][0]['value'] = $mode;
  node_save($log_node);
}

/**
 * Implements hook_sirius_document_retention_policy_info()
 */

function sirius_document_retention_sirius_document_retention_policy_info() {
  $items = array();

  $items['grievance-delete-after-close'] = array(
    'name' => t('Grievance - Delete After Close'),
    'description' => 'Delete grievances X days after they have been closed',
    'purge_callback' => 'grievance_document_retention_policy_delete_after_close',
    'configure_callback' => 'grievance_document_retention_policy_delete_after_close_configure',
  );

  return $items;
}

function grievance_document_retention_policy_delete_after_close_configure($form, &$form_state, $key) {
	$form["$key-days"] = array(
		'#type' => 'textfield',
		'#title' => t('Days after close'),
		'#description' => t('Enter the number of days after the grievance is closed that it should be deleted.'),
		'#size' => 6,
		'#default_value' => variable_get("$key-days", 365*3),
		'#required' => TRUE,
	);

	return system_settings_form($form);
}

function grievance_document_retention_policy_delete_after_close($key, $mode) {
	// Days
	$days = variable_get("$key-days");
	if (!$days) {
		sirius_document_retention_log("$key - No variable [$key-days] found. Don't know how many days back to purge. Aborting.", 'error');
	}
	if ($days < 30) {
		sirius_document_retention_log("$key - Variable [$key-days] is $days. Please set this variable to a value larger than 30.", 'error');
	}
	$ts = strtotime(-1 * $days . " days");
	$date = date('Y-m-d 00:00:00', $ts);
	sirius_document_retention_log("$key - Searching for grievances closed at least $days days ago (prior to $date).");

	//
  // Get the list of closed statuses
  //

  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $closed_status_tids = array(-999);
  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
  }


  //
  // Get the list of log entries closing a grievance prior to $date
  //

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $date, '<');
  $query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 10000);
  $stmt = $query->execute();
  if (!$stmt['node']) {
		sirius_document_retention_log("$key - No log entries found.");
		return;
  }
  $log_nids = array_keys($stmt['node']);

  //
  // Walk the log entries, looking for grievances that they reference
  // 

  $grievance_nids = array();
  foreach ($log_nids as $log_nid) {
		$sql = "select field_grievance_target_id from field_data_field_grievance where entity_type = 'node' and entity_id = :log_nid";
		$stmt = db_query($sql, array(':log_nid' => $log_nid));
		$hr = $stmt->fetchAssoc();
		$grievance_nid = $hr['field_grievance_target_id'];
		if ($grievance_nid && !$grievance_nids[$grievance_nid]) {
			$grievance_nids[$grievance_nid] = TRUE;
		}
  }

  //
  // Walk the grievances, deleting
  //

  $purged_count = 0;
  foreach ($grievance_nids as $grievance_nid => $ignore) {
		sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " - Scanning.");

		// Make sure that it's closed
		$sql = "select field_grievance_status_tid from field_data_field_grievance_status where entity_type = 'node' and entity_id = :grievance_nid";
		$stmt = db_query($sql, array(':grievance_nid' => $grievance_nid));
		$hr = $stmt->fetchAssoc();
		$status_tid = $hr['field_grievance_status_tid'];
		if (!in_array($status_tid, $closed_status_tids)) {
			sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " - has a non-closed status. Skipping.");
			continue;
		}

		if ($mode == 'live') {
			node_delete($grievance_nid);
		}

		++$purged_count;
		if ($purged_count >= 25) {
			sirius_document_retention_log("$key - Can only delete 25 grievances at a time. Terminating this run.");
			break;
		}
  }

	if ($mode == 'live') {
		sirius_document_retention_log("$key - Purged $purged_count records.");
	} else {
		sirius_document_retention_log("$key - If this had been a live run, I would have purged $purged_count records. (Mode=$mode)");
	}

	return;






















  //
  // Get the list of closed grievances that were created prior to the given date
  //

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->propertyCondition('created', $ts, '<');
  $query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if (!$stmt['node']) {
		sirius_document_retention_log("$key - No grievances found.");
		return;
  }
  $grievance_nids = array_keys($stmt['node']);

  //
  // Walk the grievances, deleting if appropriate
  //

  $purged_count = 0;
  foreach ($grievance_nids as $grievance_nid) {
		sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " - Scanning.");

		// Find out if there are any log entries after $date
		$sql = "select node.nid, count(*) as c ";
		$sql .= "from node, field_data_field_grievance ";
		$sql .= "where node.nid = field_data_field_grievance.entity_id ";
		$sql .= "and field_data_field_grievance.entity_type = 'node' ";
		$sql .= "and field_data_field_grievance.field_grievance_target_id = :grievance_nid ";
		$sql .= "and node.created > :ts ";
		$stmt = db_query($sql, array('grievance_nid' => $grievance_nid, ':ts' => $ts));
		$hr = $stmt->fetchAssoc();
		if ($hr['c']) {
			sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " - Has a more recent log entry (" . $hr['nid'] . "). Skipping.");
			continue;
		}

		sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " Preparing to delete.");
		if ($mode == 'live') {
			node_delete($grievance_nid);
			sirius_document_retention_log("$key - Grievance " . l($grievance_nid, "node/$grievance_nid") . " Deleted.");
		} 

		++$purged_count;
		if ($purged_count > 25) { 
			sirius_document_retention_log("$key - Can only deleted 25 grievances at a time. Terminating this run.");
			break;
		}
  }

	sirius_document_retention_log("$key - Purged $purged_count records. (Mode=$mode)");
}
