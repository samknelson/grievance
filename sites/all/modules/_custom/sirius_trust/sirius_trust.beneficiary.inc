<?php

function sirius_trust_worker_beneficiary_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_beneficiary' ";
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
  $beneficiary_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$beneficiary_nids[] = $hr['nid'];
  }
  if ($params['nids_only']) { return $beneficiary_nids; }
	return node_load_multiple($beneficiary_nids);
}

function sirius_trust_worker_beneficiary($worker_node, $ts = NULL) {
	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	if ($ts) {
		$params['ts'] = $ts; 
	} else {
		$params['active'] = 'Yes';
	}
	$params['limit'] = 1;
	$nodes = sirius_trust_worker_beneficiary_search($params);
	if (!$nodes) { return NULL; }
	return reset(array_values($nodes));
}

function sirius_trust_worker_beneficiary_node_form_alter(&$form, &$form_state, $form_id) {
	// Admins can see the full form
	if (user_access('sirius admin')) {
		drupal_set_message("Administrative user: Full form is displayed. Please use caution.", 'warning');
		return;
	}

	// "Active" field is computed, so don't edit manually
	if ($form['field_sirius_active']) { $form['field_sirius_active']['#access'] = FALSE;  }

	// New record or existing record?
	$beneficiary_node = $form['#entity'];
	if ($beneficiary_node && $beneficiary_node->nid) {
		$is_edit = TRUE;
	}

	// You can never change the worker or the relationship
	$form['field_sirius_worker']['#disabled'] = TRUE;
	$form['field_grievance_shop']['#disabled'] = TRUE;

	if ($is_edit) {
		// Existing record: You can't change the type or employer
		$form['field_sirius_trust_policy']['#access'] = FALSE;
	} else {
		// New record: Benefit is required
		$form['field_sirius_trust_policy']['und']['#required'] = TRUE;
	}
}

function sirius_trust_worker_beneficiary_node_presave($beneficiary_node) {
	// Relationship is active only if it started before now, and it either hasn't ended yet, or it ended after now.
	$start_ts = sirius_fieldval($beneficiary_node, 'field_sirius_date_start', 'ts');
	$end_ts = sirius_fieldval($beneficiary_node, 'field_sirius_date_end', 'ts');
	$now_ts = time();

	if ($start_ts <= $now_ts && ($end_ts < 1 || $end_ts >= $now_ts)) {
		$beneficiary_node->field_sirius_active['und'][0]['value'] = 'Yes';
	} else {
		$beneficiary_node->field_sirius_active['und'][0]['value'] = 'No';
	}

	$worker_name = sirius_node_title($beneficiary_node->field_sirius_worker['und'][0]['target_id']);
	$date = date('m/d/Y', strtotime($beneficiary_node->field_sirius_date_start['und'][0]['value']));
	$beneficiary_node->title = "$worker_name - Beneficiary $date";
}

/**
 * If we have multiple active beneficiaries, expire any duplciates (i.e. give them an end date just before 
 * the start date of the next active one.)
 */

function sirius_trust_worker_beneficiary_cleanup_multiple_active($mode, $worker_nid) {
  if (!$worker_nid) { return array('success' => FALSE, 'msg' => "No worker."); }
  $params = array();
  $params['order'] = 'field_sirius_date_start_value asc';
  $params['worker_nid'] = $worker_nid;
  $params['active'] = 'Yes';
  $beneficiaries = array_values(sirius_trust_worker_beneficiary_search($params));
  $today_date = date('Y-m-d 00:00:00');

  $count_expired = 0;
  for ($i = 0; $i<count($beneficiaries)-1; ++$i) {
  	$beneficiary_node = $beneficiaries[$i];
    $next_beneficiary_node = $beneficiaries[$i+1];

		$next_start_date = $next_beneficiary_node->field_sirius_date_start['und'][0]['value'];
    $next_start_ts = strtotime($next_start_date);

    if ($next_start_ts < 1) {
    	return array('success' => FALSE, 'msg' => 'It looks like one of the beneficiaries doesn\'t have a start date.');
    }

    $end_date = date('Y-m-d 00:00:00', strtotime('-1 day', $next_start_ts));
    sirius_minilog("Setting the end date for beneficiary " . $beneficiary_node->title . " (" . $beneficiary_node->nid . ") to $end_date, one day before the next beneficiary's start date of $next_start_date.");

    $beneficiary_node->field_sirius_date_end['und'][0]['value'] = $end_date;
    if ($mode == 'live') {
    	node_save($beneficiary_node);
    } else {
    	sirius_minilog("(Skipping save because mode = $mode.)");
    }
    ++$count_expired;
  }
  return array('success' => TRUE, 'count_expired' => $count_expired);
}

function sirius_trust_worker_beneficiary_scan($mode = 'test') {
	sirius_minilog("Scanning for workers with multiple active beneficiaries (mode = $mode)");

	// Get all workers with multiple active beneficiarys.
	// (SQL for speed)
	$sql = "select field_sirius_worker_target_id as worker_nid, count(nid) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_beneficiary' ";
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
		$result = sirius_trust_worker_beneficiary_cleanup_multiple_active($mode, $worker_nid);
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


































function sirius_trust_beneficiary_form($form, &$form_state, $worker_node, $benefit_node) {
	$benefit_json = sirius_json_get($benefit_node);
	$beneficiary_settings_json = $benefit_json['beneficiary_settings'];
	if ($beneficiary_settings_json['active'] != 'Yes') { return array(); }

	$reltype_tids = array_filter(array_values($beneficiary_settings_json['relationship_types']));
	if (!$reltype_tids) { 
		return sirius_form_error('There is a configuration error. This benefit is configured to allow beneficiaries to be designated, but no relationship types are supported.');
	}

	$relationship_options = sirius_trust_relationship_options($worker_node, array('reltype_tids' => $reltype_tids, 'active' => 'Yes'));
	if (!$relationship_options) {
		return sirius_form_error('No matching relationships available.');
	}

	$beneficiary_json = sirius_trust_beneficiary_get($worker_node, $benefit_node);

	$form['primary'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Primary Beneficiary',
		'#options' => $relationship_options,
		'#default_value' => sirius_form_array_default_value($beneficiary_json['primary']),
	);

	$form['whatever'] = array(
		'#type' => 'textfield',
		'#title' => 'Whatever',
		'#default_value' => $beneficiary_json['whatever'],
	);

	return $form;
}

function sirius_trust_beneficiary_get($worker_node, $benefit_node) {
	$worker_json = sirius_json_get($worker_node);
	return $worker_json['beneficiaries'][$benefit_node->nid]['beneficiary'];
}

function sirius_trust_beneficiary_set($worker_node, $benefit_node, $new_beneficiary_json) {
	$old_beneficiary_json = sirius_trust_beneficiary_get($worker_node, $benefit_node);
	$changed = sirius_revision_changed_impl($old_beneficiary_json, $new_beneficiary_json, 10);

	if (!$changed) {
		return array('success' => TRUE, 'msg' => $benefit_node->title . ': Beneficiary is unchanged.');
	}

	$worker_json = sirius_json_get($worker_node);
	$worker_json['beneficiaries'][$benefit_node->nid]['beneficiary'] = $new_beneficiary_json;
	sirius_json_set($worker_node, $worker_json);
	node_save($worker_node);

	return array(
		'success' => TRUE, 
		'msg' => $benefit_node->title . ': Beneficiary updated.',
		'old_beneficiary' => $old_beneficiary_json,
		'new_beneficiary' => $new_beneficiary_json
	);
}
