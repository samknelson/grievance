<?php

function sirius_trust_worker_elections_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_election' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = $domain_nid;
  if ($params['worker_nid']) {
  	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
  	$sql_args[':worker_nid'] = $params['worker_nid'];
  }
  if ($params['active']) {
  	$sql .= "and field_sirius_active_value = :active ";
  	$sql_args[':active'] = $params['active'];
  }
  if ($params['ts']) {
  	$sql .= "and field_sirius_date_start_value <= :date ";
  	$sql .= "and (field_sirius_date_end_value >= :date or field_sirius_date_end_value is null) ";
  	$sql_args[':date'] = gmdate('Y-m-d H:i:s', $params['ts']);
  }
  $order = "field_sirius_date_start_value desc";
  if ($params['order']) {
  	$order = $params['order'];
  }
  $sql .= "order by $order ";
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $election_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$election_nids[] = $hr['nid'];
  }
  if ($params['nids_only']) { return $election_nids; }
	return node_load_multiple($election_nids);
}

function sirius_trust_worker_election($worker_node, $ts = NULL) {
	if (!$worker_node) { return array(); }
	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	if ($ts) {
		$params['ts'] = $ts; 
	} else {
		$params['active'] = 'Yes';
	}
	$params['limit'] = 1;
	$nodes = sirius_trust_worker_elections_search($params);
	if (!$nodes) { return NULL; }
	return reset(array_values($nodes));
}

function sirius_trust_worker_election_node_form_alter(&$form, &$form_state, $form_id) {
	// Admins can see the full form
	if (user_access('sirius admin')) {
		drupal_set_message("Administrative user: Full form is displayed. Please use caution.", 'warning');
		return;
	}

	// "Active" field is computed, so don't edit manually
	if ($form['field_sirius_active']) { $form['field_sirius_active']['#access'] = FALSE;  }

	// New record or existing record?
	$election_node = $form['#entity'];
	if ($election_node && $election_node->nid) {
		$is_edit = TRUE;
	}

	// You can never change the worker or the relationship
	$form['field_sirius_worker']['#disabled'] = TRUE;
	$form['field_grievance_shop']['#disabled'] = TRUE;

	if ($is_edit) {
		// Existing record: You can't change the type or employer
		$form['field_sirius_trust_benefits']['#access'] = FALSE;
		$form['field_grievance_shop']['#access'] = FALSE;
		$form['field_sirius_trust_election_type']['#access'] = FALSE;
		$form['field_sirius_trust_policy']['#access'] = FALSE;
	} else {
		// New record: Benefit is required
		$form['field_sirius_trust_benefits']['#required'] = TRUE;
		$form['field_sirius_trust_election_type']['und']['#required'] = TRUE;
		$form['field_sirius_trust_policy']['und']['#required'] = TRUE;
	}
}

function sirius_trust_worker_election_node_presave($election_node) {
	// Relationship is active only if it started before now, and it either hasn't ended yet, or it ended after now.
	$start_ts = sirius_fieldval($election_node, 'field_sirius_date_start', 'ts');
	$end_ts = sirius_fieldval($election_node, 'field_sirius_date_end', 'ts');
	$now_ts = time();

	if ($start_ts <= $now_ts && ($end_ts < 1 || $end_ts >= $now_ts)) {
		$election_node->field_sirius_active['und'][0]['value'] = 'Yes';
	} else {
		$election_node->field_sirius_active['und'][0]['value'] = 'No';
	}

	$worker_name = sirius_node_title($election_node->field_sirius_worker['und'][0]['target_id']);
	$date = date('m/d/Y', strtotime($election_node->field_sirius_date_start['und'][0]['value']));
	$election_node->title = "$worker_name - Election $date";
}

/**
 * If we have multiple active elections, expire any duplciates (i.e. give them an end date just before 
 * the start date of the next one.)
 */

function sirius_trust_worker_election_cleanup_multiple_active($mode, $worker_nid) {
  if (!$worker_nid) { return array('success' => FALSE, 'msg' => "No worker."); }
  $params = array();
  $params['order'] = 'field_sirius_date_start_value asc';
  $params['worker_nid'] = $worker_nid;
  // $params['active'] = 'Yes';
  $elections = array_values(sirius_trust_worker_elections_search($params));
  $today_date = date('Y-m-d 00:00:00');

  $count_expired = 0;
  for ($i = 0; $i<count($elections)-1; ++$i) {
  	$election_node = $elections[$i];
    $next_election_node = $elections[$i+1];

    if ($election_node->field_sirius_active['und'][0]['value'] != 'Yes') { continue; }

		$next_start_date = $next_election_node->field_sirius_date_start['und'][0]['value'];
    $next_start_ts = strtotime($next_start_date);

    if ($next_start_ts == 0) {
    	return array('success' => FALSE, 'msg' => 'It looks like one of the elections doesn\'t have a start date.');
    }

    $end_date = date('Y-m-d 00:00:00', strtotime('-1 day', $next_start_ts));
    sirius_minilog("Setting the end date for election " . $election_node->title . " (" . $election_node->nid . ") to $end_date, one day before the next election's start date of $next_start_date.");

    $election_node->field_sirius_date_end['und'][0]['value'] = $end_date;
    if ($mode == 'live') {
    	node_save($election_node);
    } else {
    	sirius_minilog("(Skipping save because mode = $mode.)");
    }
    ++$count_expired;
  }
  return array('success' => TRUE, 'count_expired' => $count_expired);
}

function sirius_trust_worker_election_scan($mode = 'test') {
	sirius_minilog("Scanning for workers with multiple active elections (mode = $mode)");

	// Get all workers with multiple active elections.
	// (SQL for speed)
	$sql = "select field_sirius_worker_target_id as worker_nid, count(nid) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_election' ";
	$sql .= "and field_sirius_active_value = 'Yes' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = sirius_domain_nid();
	$sql .= "group by field_sirius_worker_target_id ";
	$sql .= "having c > 1 ";
	$sql .= "order by field_sirius_worker_target_id ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['worker_nid'];
	}

	if (!$worker_nids) {
		sirius_minilog('-- No workers found.');
		return array('success' => TRUE);
	}

	$count_scan = $count_success = $count_failure = $count_expired = 0;

	foreach ($worker_nids as $worker_nid) {
		++$count_scan;
		sirius_minilog("-- Cleaning up worker $worker_nid");
		$result = sirius_trust_worker_election_cleanup_multiple_active($mode, $worker_nid);
		if ($result['success']) { 
			sirius_minilog("-- Success: $result[msg]");
			++$count_success;
			$count_expired += $result['count_expired'];
		} else {
			sirius_minilog("-- Failure: $result[msg]", 'warning');
			++$count_failure;
		}
	}

	return array('success' => TRUE, 'msg' => "Scanned $count_scan workes with multiple active eletions. Cleaned up $count_success, expiring $count_expired expired records with $count_failure failures.");
}

function sirius_trust_election_wizard_search($params) {
	$sql_args = array();
	$sql = "select nid, field_sirius_type_value, field_sirius_notes_value from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	if ($params['election_nid']) {
		$sql .= "and field_sirius_log_handler_target_id = :election_nid ";
		$sql_args[':election_nid'] = $params['election_nid'];
	}
	if ($params['ssn']) {
		$sql .= "and field_sirius_notes_value = :ssn ";
		$sql_args[':ssn'] = $params['ssn'];
	}
	if ($params['status']) {
		$sql .= "and field_sirius_type_value = :status ";
		$sql_args[':status'] = $params['status'];
	}
	$sql .= "and field_sirius_category_value = 'trust:election_wizard' ";
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}

	if ($params['order']) {
		$sql .= "order by " . $params['order'] . ' ';
	} else {
		$sql .= "order by nid desc ";
	}

	if ($params['limit']) {
		$sql .= "limit " . $params['limit'] . ' ';
	} else {
		$sql .= "limit 100 ";
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$results = array();
	while ($hr = $stmt->fetchAssoc()) {
		$results[$hr['nid']] = $hr;
	}
	return $results;
}

function sirius_trust_election_wizard_status_options() {
	return array(
		'draft' => t('Draft'),
		'submitted' => t('Submitted'),
		'posted' => t('Posted'),
	);
}