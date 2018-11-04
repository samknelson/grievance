<?php

function sirius_document_retention_policy_list() {
	$policy_infos = module_invoke_all('sirius_document_retention_policy_info');
	return $policy_infos;
}

function sirius_document_retention_policy_info($key) {
	$list = sirius_document_retention_policy_list();
	return $list[$key];
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
	$date = date('Y-m-d 00:00:00', strtotime("-$days"));
	sirius_document_retention_log("$key - Searching for grievances closed at least $days ago (prior to $date).");

  // Get the list of closed statuses
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

  // Get the list of log entries closing a grievance prior to $date
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $date, '<');
  $query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 25);
  $stmt = $query->execute();
  if (!$stmt['node']) {
		sirius_document_retention_log("$key - No log entries found.");
		return;
  }
  $log_nids = array_keys($stmt['node']);

  // Walk the log entries, looking for grievances that they reference
  $grievance_nids = array();
  foreach ($log_nids as $log_nid) {
		sirius_document_retention_log("$key - Looking at log entry $log_nid.");
		$log_node = node_load($log_nid);
		if (!$log_node) {
			sirius_document_retention_log("$key - Log entry $log_nid not found. Skipping.", 'warning');
			continue;
		}
		$grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
		$grievance_nids[$grievance_nid] = TRUE;
  }

  // Walk the grievances, deleting
  $purged_count = 0;
  foreach ($grievance_nids as $grievance_nid => $ignore) {
		sirius_document_retention_log("$key - Looking at grievance $grievance_nid.");
		$grievance_node = node_load($grievance_nid);
		if (!$log_node) {
			sirius_document_retention_log("$key - Log entry $log_nid not found. Skipping.", 'warning');
			continue;
		}
		$status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
		if (!in_array($status_tid, $closed_status_tids)) {
			sirius_document_retention_log("$key - Grievance #$grievance_nid has a non-closed status. Skipping.", 'warning');
			continue;
		}

		if ($mode == 'live') {
			node_delete($grievance_nid);
		}
		++$purged_count;
  }

	sirius_document_retention_log("$key - Purged $purged_count records. (Mode=$mode)");
}