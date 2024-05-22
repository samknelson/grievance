<?php

function sirius_teamsters631_apprentice_fetch($mode = 'test') {

	$count = 0;

	sirius_minilog("Fetching apprentices");

	$result = sirius_service_client_request('sirius_teamsters631_webservice', 'apprentice_list_all');
	if (!$result['success']) {
		return array('success' => FALSE, 'msg' => "Apprentice fetch failed: " . $result['msg']);
	}

	$fetch_result = $result['data'];
	if (!$fetch_result) { 
		return array('success' => FALSE, 'msg' => "Apprentice fetch did not return the expected variable ('data')");
	}

	if (!$fetch_result['success']) { 
		return array('success' => FALSE, 'msg' => "Apprentice fetch returned an error result: " . $fetch_result['msg'], 'warning');
	}

	$records = $fetch_result['data'];

	// /////////////////////////////////////////////////////////////////////////////////////
	// Create new dispatches or update existing dispatches
	// /////////////////////////////////////////////////////////////////////////////////////

	$count_dispatch = $count_error = $count_success = $count_existing = $count_new = 0;
	$active_by_uuid = array();
	foreach ($records as $record) {
		++$count_dispatch;
		sirius_minilog("Processing incoming record for worker " . $record['worker_name'] . ' (' . $record['worker_id'] . ') at employer ' . $record['employer_name'] . ' (' . $record['employer_id'] . ')');

		$uuid = $record['dispatch_uuid'];
		if (!$uuid) {
			++$count_error;
			sirius_minilog("... No uuid.", 'warning');
			continue;
		}
		$active_by_uuid[$uuid] = TRUE;

		$worker_nid = sirius_worker_get_by_id($record['worker_id'], TRUE);
		if (!$worker_nid) {
			++$count_error;
			sirius_minilog("... Worker not found. Skipping.", 'warning');
			continue;
		}

		// Fetch the log node
		$log_category = 'teamsters631:apprentice';

		$log_node = NULL;

		$sql_args = array();
	  $sql = "select nid ";
	  $sql .= "from node ";
	  $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	  $sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
	  $sql .= "where node.type = 'sirius_log' ";
	  $sql .= "and field_sirius_category_value = :category ";
	  $sql_args[':category'] = $log_category;
	  $sql .= "and field_sirius_notes_value = :id ";
	  $sql_args[':id'] = $uuid;
	  $sql .= "order by created desc limit 1";
	  $stmt = db_query($sql, $sql_args);
	  $hr = $stmt->fetchAssoc();
	  if ($hr['nid']) {
	  	++$count_existing;
	  	$log_node = node_load($hr['nid']);
	  }

	  if (!$log_node) {
	  	++$count_new;
			$log_node = sirius_log(
				$log_category,
				$record['dispatch_status'],
				"Apprentice dispatch to " . $record['employer_name'] . " on " . $record['start_date'],
				'',
				$uuid,
				array($worker_nid),
				NULL,
				$record
			);
		}

		// Update the log entry
		$changed = FALSE;
		$log_json = sirius_json_get($log_node);

		$record['start_ts'] = strtotime($record['start_date']);
		if ($record['employer_id']) {
			$employer_nid = sirius_employer_get_by_id($record['employer_id'], TRUE);
			if ($employer_nid) { $record['employer_nid'] = $employer_nid; }
		}

		foreach ($record as $key => $value) {
			if ($log_json[$key] != $record[$key]) {
				$log_json[$key] = $record[$key];
				$changed = TRUE;
			}
		}

		if ($log_node->field_sirius_type['und'][0]['value'] != $record['dispatch_status']) {
			$log_node->field_sirius_type['und'][0]['value'] = $record['dispatch_status'];
			$changed = TRUE;
		}

		$title = $record['employer_name'] . ' / ' . $record['dispatch_status'] . ' / ' . date('Y-m-d', strtotime($record['start_date']));
		if ($record['end_date']) {
			$title .= ' / ' . date('Y-m-d', strtotime($record['end_date']));
		}
		if ($title != $log_node->title) {
			$log_node->title = $title;
			$changed = TRUE;
		}

		if ($changed) {
			sirius_json_set($log_node, $log_json);
			node_save($log_node);
		}

		++$count_success;
	}

	sirius_minilog("Active fetch complete. Processed $count_dispatch incoming dispatch records, with $count_error errors and $count_success successes ($count_new new dispatches, $count_existing existing dispatches).");

	// /////////////////////////////////////////////////////////////////////////////////////
	// Terminate any existing dispatches which we didn't find.
	// /////////////////////////////////////////////////////////////////////////////////////

	sirius_minilog("Terminating existing dispatches ...");
	$count_layoff = 0;

	$sql_args = array();
  $sql = "select nid, field_sirius_notes_value ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_category_value = :category ";
  $sql_args[':category'] = $log_category;
  $sql .= "and field_sirius_type_value in ('accepted', 'accepted_secondary') ";
  $sql .= "order by created desc limit 1";
  $stmt = db_query($sql, $sql_args);
  $count_terminated = 0;
  while ($hr = $stmt->fetchAssoc()) {
  	$uuid = $hr['field_sirius_notes_value'];
  	if ($active_by_uuid[$uuid]) { continue; }
  	$log_node = node_load($hr['nid']);
  	$log_node->field_sirius_type['und'][0]['value'] = 'layoff';
  	$log_json = sirius_json_get($log_node);
  	$log_json['end_date'] = date('Y-m-d H:i:s');
  	$log_json['end_ts'] = time();

		$title = $log_json['employer_name'] . ' / ' . $log_json['dispatch_status'] . ' / ' . date('Y-m-d', strtotime($log_json['start_date']));
		if ($log_json['end_date']) {
			$title .= ' / ' . date('Y-m-d', strtotime($log_json['end_date']));
		}
		$log_node->title = $title;

  	sirius_json_set($log_node, $log_json);
  	node_save($log_node);
  	sirius_minilog("Terminating dispatch nid " . $log_node->nid);
  	++$count_layoff;
  }

  sirius_minilog("Terminated $count_layoff dispatches.");

	return array('success' => TRUE, 'count' => $count);
}



