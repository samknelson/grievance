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
 * the start date of the next active one.)
 */

function sirius_trust_worker_election_cleanup_multiple_active($mode, $worker_nid) {
  if (!$worker_nid) { return array('success' => FALSE, 'msg' => "No worker."); }
  $params = array();
  $params['order'] = 'field_sirius_date_start_value asc';
  $params['worker_nid'] = $worker_nid;
  $params['active'] = 'Yes';
  $elections = array_values(sirius_trust_worker_elections_search($params));
  $today_date = date('Y-m-d 00:00:00');

  $count_expired = 0;
  for ($i = 0; $i<count($elections)-1; ++$i) {
  	$election_node = $elections[$i];
    $next_election_node = $elections[$i+1];

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

function sirius_trust_worker_election_post_from_wizard($wizard_node) {
	drupal_set_message("Posting from " . $wizard_node->title . ' (' . $wizard_node->nid . ')');

	$wizard_json = sirius_json_get($wizard_node);
	$data = $wizard_json['wizard']['trust_election'];

	$worker_node = sirius_log_handler($wizard_node, 'sirius_worker');
	if (!$worker_node) { return array('success' => FALSE, 'msg' => 'No worker found for this election wizard.'); }

	$policy_nid = $data['policy']['policy_rnid'];
	$policy_node = node_load($policy_nid);
	if (!$policy_node) { return array('success' => FALSE, 'msg' => 'Unable to find the policy for this election.'); }

	$employer_nid = $data['policy']['employer_rnid'];
	$employer_node = node_load($policy_nid);
	if ($employer_nid && !$employer_node) { return array('success' => FALSE, 'msg' => 'Unable to find the employer for this election.'); }

	// Update the worker
	$need_save = FALSE;
	if ($data['phone']['change'] == 'Yes') {
		$worker_node->field_sirius_phone['und'][0]['value'] = sirius_phone_format($data['phone']['new']['phone']);
		$need_save = TRUE;
	}
	if ($data['email']['change'] == 'Yes') {
		$worker_node->field_sirius_email['und'][0]['value'] = $data['email']['new']['email'];
		$need_save = TRUE;
	}
	if ($data['addr']['change'] == 'Yes') {
		$worker_node->field_sirius_address['und'][0] = $data['addr']['new'];
		$need_save = TRUE;
	}
	if ($need_save) {
		node_save($worker_node);
		drupal_set_message("Updated worker's contact information.");
	}

	// Create the relationships
	drupal_set_message("Need to create dependents: ");
	dpm($data['dependents']);

	// Create the election
	$election_node = sirius_log_handler($wizard_node, 'sirius_trust_worker_election');
	if (!$election_node) {
		drupal_set_message("Creating a new election for " . $worker_node->title);
		global $user;
    $election_node = new stdClass();
    $election_node->type = 'sirius_trust_worker_election';
    $election_node->uid = $user->uid;
    $election_node->status = 1;
    $election_node->promote = 0;
    $election_node->comment = 0;
	  $election_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
    node_object_prepare($election_node);
    $election_node = node_submit($election_node);
	} else {
		drupal_set_message("Updating existing election for " . $worker_node->title);
	}

	// Set the policy
  $election_node->field_sirius_trust_policy['und'][0]['target_id'] = $policy_node->nid;

  // Set the worker
  $election_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;

  // Set the benefits
  $benefit_nids = array();
  if ($data['benefits']) {
  	foreach ($data['benefits'] as $benefit_nid => $benefit_election) {
  		if ($benefit_election['election'] == 'Yes') { $benefit_nids[] = $benefit_nid; }
  	}
  }
  $tmp = array();
  foreach ($benefit_nids as $benefit_nid) { $tmp[] = array('target_id' => $benefit_nid); }
  $election_node->field_sirius_trust_benefits['und'] = $tmp;

  // Save
  node_save($election_node);

  // Link the wizard to the election
  sirius_log_handler_set($wizard_node, $election_node->nid);
  node_save($wizard_node);

	return array('success' => TRUE, 'msg' => 'Posted.', 'election_node' => $election_node);
}

function sirius_trust_worker_election_wizard_push_remote($wizard_pnode) {
	$wizard_pjson = sirius_json_get($wizard_pnode);

	$result = array();
	if ($wizard_pjson['wizard']['trust_election']['push']['server_nid']) {
		$wizard_node = node_load($wizard_pjson['wizard']['trust_election']['push']['server_nid']);
		$result['msg'] = "Updating existing server node.";
	} 
	if (!$wizard_node) {
		$result['msg'] = "Creating new server node.";
		$wizard_node = sirius_log(
	    'trust:election_wizard', 
	    'draft', 
	    $wizard_pnode->field_sirius_message['und'][0]['value'], 
	    'Election',
	    $wizard_pnode->field_sirius_notes['und'][0]['value'], 
	    array(),
	    NULL,
	    $wizard_pjson
	  );
	}
	$result['wizard_node'] = $wizard_node;

	// Update the server fields from the proxy
	$wizard_node->field_sirius_json = $wizard_pnode->field_sirius_json;
	$wizard_node->field_sirius_message = $wizard_pnode->field_sirius_message;
	$wizard_node->field_sirius_notes = $wizard_pnode->field_sirius_notes;
	$wizard_node->field_sirius_type = $wizard_pnode->field_sirius_type;

	// Link the election wizard to the worker
	$ssn = $wizard_node->field_sirius_notes['und'][0]['value'];
	if (!$ssn) {
		$result['msg'] = "No SSN.";
		$result['success'] = FALSE;
		return $result;
	}
	$worker_node = sirius_worker_get_by_ssn($ssn);
	if (!$worker_node) {
		$result['msg'] = "No worker found with this SSN.";
		$result['success'] = FALSE;
		return $result;
	}
	sirius_log_handler_set($wizard_node, $worker_node->nid);

	// Update the server node
	node_save($wizard_node);
	$result['success'] = TRUE;
	return $result;
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