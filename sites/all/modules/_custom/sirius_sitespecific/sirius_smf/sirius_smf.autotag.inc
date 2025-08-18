<?php

function sirius_smf_autotag($denorm_item) {
	$args = json_decode($denorm_item['denorm_trigger_args'], TRUE);
	$worker_nid = $denorm_item['denorm_target_nid'];
	sirius_smf_autotag_worker(node_load($worker_nid), $args['asof_ts'], 'live');
}

function sirius_smf_autotag_worker($worker_node, $asof_ts = NULL, $mode = 'test') {
	// Make sure we're in the correct domain
	sirius_domain($worker_node->field_sirius_domain['und'][0]['target_id']);

	// Which nodes are we tagging?
	$entity_nodes = array();

	// Get the contact node
	$contact_node = sirius_worker_contact_get($worker_node);
	if (!$contact_node) { return array('success' => FALSE, 'msg' => 'The worker does not have a connected contact record.'); }

	// Are tagging as of "right now"? This means that we tag the contact node
	if (isset($asof_ts)) {
		$is_historical = TRUE;
	}
	if (!isset($asof_ts)) {
		$is_historical = FALSE;
		$entity_nodes[] = $contact_node;
		$asof_ts = time();
	}

	// Find out if the worker has any hours in the last 60 months
	list ($cutoff_y, $cutoff_m) = explode('-', date('Y-m', strtotime('-5 years', $asof_ts)));
  $sql = 'select sum(hours) as s '; 
  $sql .= 'from sirius_hours_cache ';
  $sql .= "where worker_nid = :worker_nid ";
  $sql .= "and ( ";
  $sql .= "  (year > :cutoff_y) ";
  $sql .= "  or (year = :cutoff_y and month > :cutoff_m) ";
  $sql .= ") ";
  $sql_args = array(
    ':worker_nid' => $worker_node->nid,
    ':cutoff_y' => $cutoff_y,
    ':cutoff_m' => $cutoff_m,
  );
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $hours = $hr['s'];

  if (!$hours) {
		// Fetch and delete the WYM node
		$wym_node = sirius_smf_wym_fetch(
			$worker_node->nid, 
			date('Y', $asof_ts),
			date('n', $asof_ts), 
			FALSE
		);
		if ($wym_node) { 
			node_delete($wym_node->nid);
		}

  	if ($entity_nodes) {
			// Make a copy of the original nodes, so we don't have to save if there are no changes.
			$original_nodes = array();
			foreach ($entity_nodes as $entity_node) { $original_nodes[] = clone $entity_node; }

	  	// Clear the worker's tags
			$result = SiriusResult::create(sirius_smf_autotag_worker_clear($worker_node, $entity_nodes, $asof_ts, $mode));
			if (!$result->success()) { return $result; }
		}

	} else {
		// The worker does have hours. Tag the worker-year-month node
		$wym_node = sirius_smf_wym_fetch(
			$worker_node->nid, 
			date('Y', $asof_ts),
			date('n', $asof_ts), 
			TRUE
		);
		$entity_nodes[] = $wym_node;

		// Make a copy of the original nodes, so we don't have to save if there are no changes.
		$original_nodes = array();
		foreach ($entity_nodes as $entity_node) { $original_nodes[] = clone $entity_node; }

		$result = SiriusResult::create(sirius_smf_autotag_worker_nonzero60($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }

		$result = SiriusResult::create(sirius_smf_autotag_worker_benefits($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }
		
		$result = SiriusResult::create(sirius_smf_autotag_worker_benefits_recent($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }
		
		$result = SiriusResult::create(sirius_smf_autotag_worker_election_type($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }

		$result = SiriusResult::create(sirius_smf_autotag_worker_buildup($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }

		$result = SiriusResult::create(sirius_smf_autotag_worker_home_employer_and_policies($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }

		$result = SiriusResult::create(sirius_smf_autotag_worker_disability($worker_node, $entity_nodes, $asof_ts, $mode));
		if (!$result->success()) { return $result; }
	}

	// Set autotag status
	foreach ($entity_nodes as $delta => $entity_node) {
		$original_node = $original_nodes[$delta];

		if (!sirius_revision_changed($original_node, $entity_node)) { continue; }

		$status = array();
		$status['run_ts'] = time();
		$status['asof_ts'] = $asof_ts;
		$json = sirius_json_get($entity_node);
		$json['smf']['autotag']['status'] = $status;
		sirius_json_set($entity_node, $json);
		node_save($entity_node);
	}

	return array(
		'success' => TRUE, 
	);
}

function sirius_smf_autotag_worker_clear($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	$lookups = &drupal_static(__FUNCTION__);

	if (!isset($lookups)) {
		$root_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_root');
		if (!$root_tid) { return array('success' => FALSE, 'msg' => 'Unable to find a term with id "smf_autotag_root".'); }

		$vid = taxonomy_vocabulary_machine_name_load('sirius_contact_tags')->vid;
 		$lookups['tree'] = taxonomy_get_tree($vid, $root_tid, NULL, TRUE);
	}

	foreach ($lookups['tree'] as $term) {
		foreach ($entity_nodes as $entity_node) {
			$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $term->tid, TRUE));
			if (!$result->success()) { return $result->data(); }
		}
	}

	return array('success' => TRUE);
}

function sirius_smf_autotag_worker_nonzero60($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	$lookups = &drupal_static(__FUNCTION__);
	$id = 'smf_autotag_hrs_nonzero60';
	if (!isset($lookups)) {
		$parent_id = "smf_autotag_hrs";
		$name = 'Hours: Nonzero 60 Months Previous';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup($name, $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$lookups['map'][$id] = $result->get('tid');
	}

	// Find out if we have any hours in the last 60 months
	list ($cutoff_y, $cutoff_m) = explode('-', date('Y-m', strtotime('-5 years', $asof_ts)));
  $sql = 'select sum(hours) as s '; 
  $sql .= 'from sirius_hours_cache ';
  $sql .= "where worker_nid = :worker_nid ";
  $sql .= "and ( ";
  $sql .= "  (year > :cutoff_y) ";
  $sql .= "  or (year = :cutoff_y and month > :cutoff_m) ";
  $sql .= ") ";
  
  $sql_args = array(
    ':worker_nid' => $worker_node->nid,
    ':cutoff_y' => $cutoff_y,
    ':cutoff_m' => $cutoff_m,
  );
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $hours = $hr['s'];

  // Tag
  $tid = $lookups['map']['smf_autotag_hrs_nonzero60'];
	foreach ($entity_nodes as $entity_node) {
		if ($hours) {
			$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tid, TRUE));
		} else {
			$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tid, TRUE));
		}
		if (!$result->success()) { return $result->data(); }
	}

	return array('success' => TRUE, 'hours' => $hours);
}

function sirius_smf_autotag_worker_home_employer_and_policies($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Build our autotags for policies
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$policies = sirius_ledger_policy_list();
		$policy_tids = array();

		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Plans", 'smf_autotag_policy_root', 'smf_autotag_root', TRUE));
		if (!$result->success()) { return $result->data(); }

		foreach ($policies as $policy_nid => $policy_node) {
			$id = "smf_autotag_policy_$policy_nid";
			$parent_id = "smf_autotag_policy_root";
			$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Plan: " . $policy_node->title, $id, $parent_id, TRUE));
			if (!$result->success()) { return $result->data(); }
			$policy_tids[$policy_nid] = $result->get('tid');
		}
		$lookups['policies'] = $policies;
		$lookups['policy_tids'] = $policy_tids;
	}

	$dispatch_nodes = sirius_dispatch_search(
		array(
			'worker_nid' => $worker_node->nid,
			'types' => array('Primary'),
			'ts' => $asof_ts + 24*60*60, // Avoid timezone silliness
		)
	);
	$dispatch_node = reset($dispatch_nodes);

	if ($dispatch_node) {
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$home_employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	}

	if ($mode == 'live') {
		foreach ($entity_nodes as $entity_node) {
			// Don't touch the worker node
			if ($entity_node->type != 'smf_worker_month') { continue; }

			if ($home_employer_nid) {
				$entity_node->field_grievance_shop['und'][0]['target_id'] = $home_employer_nid;
			} else {
				unset($entity_node->field_grievance_shop['und']);
			}
		}
	}

	if ($home_employer_nid) {
		$home_employer_node = node_load($home_employer_nid);
		$policy_nid = sirius_ledger_policy($home_employer_node, array('nid_only' => TRUE), $asof_ts);

		if ($mode == 'live') {
			foreach ($entity_nodes as $entity_node) {
				foreach ($lookups['policy_tids'] as $probe_policy_nid => $tid) {
					if ($probe_policy_nid == $policy_nid) {
						$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tid, TRUE));
					} else {
						$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tid, TRUE));
					}

					if (!$result->success()) { return $result->data(); }
				}
			}
		}
	}

	return array('success' => TRUE, 'home_employer_nid' => $home_employer_nid);
}

function sirius_smf_autotag_worker_policy($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Hack: we assume that the home employer has already been set
}

function sirius_smf_autotag_worker_benefits($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Get the list of term IDs for each benefit node. Save it statically in case we're called many times.
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$benefits = sirius_trust_benefits(FALSE);
		$benefit_tids = array();

		foreach ($benefits as $benefit_nid => $benefit_node) {
			$id = "smf_autotag_benefit_" . $benefit_node->nid;
			$parent_id = "smf_autotag_benefit_root";
			$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Benefit: " . $benefit_node->title, $id, $parent_id, TRUE));
			if (!$result->success()) { return $result->data(); }
			$benefit_tids[$benefit_nid] = $result->get('tid');
		}
		$lookups['benefits'] = $benefits;
		$lookups['benefit_tids'] = $benefit_tids;
	}

	// Find the worker's benefits
	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	$params['subscriber_nid'] = $worker_node->nid;
	$params['ts'] = $asof_ts;
	$wb_nodes = sirius_trust_worker_benefits_search($params);
	$benefit_nids = array();
	foreach ($wb_nodes as $wb_nid => $wb_node) { $benefit_nids[] = $wb_node->field_sirius_trust_benefit['und'][0]['target_id']; }

	// Tag
	foreach ($entity_nodes as $entity_node) {
		foreach ($lookups['benefit_tids'] as $benefit_nid => $tid) {
			if (in_array($benefit_nid, $benefit_nids)) {
				$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tid, TRUE));
			} else {
				$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tid, TRUE));
			}

			if (!$result->success()) { return $result->data(); }
		}
	}

	return array('success' => TRUE);
}

function sirius_smf_autotag_worker_benefits_recent($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Get the list of term IDs for each benefit node. Save it statically in case we're called many times.
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$map = array();

		$id = 'smf_autotag_benefit_any_recent_benefit';
		$parent_id = 'smf_autotag_benefit_root';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Any Recent Benefit", $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$map[$id] = $result->get('tid');

		$id = 'smf_autotag_benefit_any_recent_benefit_1';
		$parent_id = 'smf_autotag_benefit_any_recent_benefit';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Any Benefit 1 Month Previous", $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$map[$id] = $result->get('tid');

		$id = 'smf_autotag_benefit_any_recent_benefit_2';
		$parent_id = 'smf_autotag_benefit_any_recent_benefit';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Any Benefit 2 Month Previous", $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$map[$id] = $result->get('tid');

		$lookups['map'] = $map;
	}

	$my_tag_ids = array();
	for ($i=1; $i<=2; ++$i) {
		// Find the worker's benefits
		$params = array();
		$params['worker_nid'] = $worker_node->nid;
		$params['subscriber_nid'] = $worker_node->nid;
		$params['ts'] = strtotime("-$i months", $asof_ts);
		$params['nids_only'] = TRUE;
		$wb_nids = sirius_trust_worker_benefits_search($params);
		if ($wb_nids) {
			$my_tag_ids[] = 'smf_autotag_benefit_any_recent_benefit';
			$my_tag_ids[] = "smf_autotag_benefit_any_recent_benefit_$i";
		}
	}

	foreach ($entity_nodes as $entity_node) {
		foreach ($lookups['map'] as $id => $tag_tid) {
			if (in_array($id, $my_tag_ids)) {
				$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tag_tid, TRUE));
			} else {
				$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tag_tid, TRUE));
			}

			if (!$result->success()) { return $result->data(); }
		}
	}

	return array('success' => TRUE);
}


function sirius_smf_autotag_worker_election_type($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Get the list of term IDs for each benefit node. Save it statically in case we're called many times.
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$parent_id = "smf_autotag_election_types_root";

		$election_types = sirius_taxonomy_load('sirius_trust_election_type', 'tid', 'name');
		foreach ($election_types as $election_type_tid => $election_type_name) {
			$id = "smf_autotag_election_type_" . $election_type_tid;
			$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Election Type: " . $election_type_name, $id, $parent_id, TRUE));
			if (!$result->success()) { return $result->data(); }
			$election_type_map[$election_type_tid] = $result->get('tid');
		}

		$id = "smf_autotag_election_type_no_election_type";
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Election Type: [No Election Type]", $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$election_type_map['no_election_type'] = $result->get('tid');

		$id = "smf_autotag_election_type_no_election";
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup("Election Type: [No Election]", $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$election_type_map['no_election'] = $result->get('tid');

		$lookups['election_types'] = $election_types;
		$lookups['election_type_map'] = $election_type_map;
	}

	// Find the worker's benefits
	$election_node = sirius_trust_worker_election($worker_node, $asof_ts);
	$election_type_tid = NULL;
	if ($election_node) {
		$election_type_tid = $election_node->field_sirius_trust_election_type['und'][0]['tid'];
		if (!$election_type_tid) {
			$election_type_tid = 'no_election_type';
		}
	} else {
		$election_type_tid = 'no_election';
	}

	// Tag
	foreach ($entity_nodes as $entity_node) {
		foreach ($lookups['election_type_map'] as $probe_election_type_tid => $tag_tid) {
			if ($election_type_tid == $probe_election_type_tid) {
				$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tag_tid, TRUE));
			} else {
				$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tag_tid, TRUE));
			}

			if (!$result->success()) { return $result->data(); }
		}
	}

	return array('success' => TRUE);
}

















function sirius_smf_autotag_worker_disability($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Get the list of term IDs for each benefit node. Save it statically in case we're called many times.
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$parent_id = "smf_autotag_status";
		$parent_name = 'Status';

		$id = "smf_autotag_status_disability";
		$name = 'Status: Disability';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup($name, $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$lookups['disability'] = $result->get('tid');

		$id = "smf_autotag_status_fmla";
		$name = 'Status: FMLA';
		$result = SiriusResult::create(sirius_smf_autotag_tag_lookup($name, $id, $parent_id, TRUE));
		if (!$result->success()) { return $result->data(); }
		$lookups['fmla'] = $result->get('tid');

		$lookups['hour_types'] = sirius_taxonomy_load('sirius_hour_type', 'tid', 'full');
	}

	// Find any payperiods
	$payperiod_nodes = sirius_hour_payperiod_load_multiple(
		sirius_domain_nid(), 
		$worker_node->nid, 
		date('Y', $asof_ts), 
		date('n', $asof_ts),
		1
	);

	// Logic
	$disability = $fmla = FALSE;
	foreach ($payperiod_nodes as $payperiod_nid => $payperiod_node) {
		$payperiod_json = sirius_json_get($payperiod_node);
		if ($payperiod_json['smf']['status']['disability']) { $disability = TRUE; }
		if ($payperiod_json['smf']['status']['fmla']) { $fmla = TRUE; }

		if (!$payperiod_json['entries']) { continue; }
		$entry = reset($payperiod_json['entries']);
		if (!$entry) { continue; }
		if (!$entry['rows']) { continue; }
		$row = reset($entry['rows']);
		if (!$row) { continue; }
		$hour_type_tid = $row['hour_type'];
		if ($lookups['hour_types'][$hour_type_tid]->field_sirius_id['und'][0]['value'] == 'fmla') { $fmla = TRUE; }
		if ($lookups['hour_types'][$hour_type_tid]->field_sirius_id['und'][0]['value'] == 'disability') { $disability = TRUE; }
	}

	// Tag
	foreach ($entity_nodes as $entity_node) {
		if ($disability) {
			$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $lookups['disability'], TRUE));
		} else {
			$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $lookups['disability'], TRUE));
		}
		if (!$result->success()) { return $result->data(); }

		if ($fmla) {
			$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $lookups['fmla'], TRUE));
		} else {
			$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $lookups['fmla'], TRUE));
		}
		if (!$result->success()) { return $result->data(); }
	}

	return array('success' => TRUE);
}











function sirius_smf_autotag_worker_buildup($worker_node, &$entity_nodes, $asof_ts, $mode = 'test') {
	// Get the list of term IDs for each benefit node. Save it statically in case we're called many times.
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) {
		$parent_id = "smf_autotag_hrs";

		$ids = array(
			"smf_autotag_hrs_buildup" => 'Hours: In Buildup',
			"smf_autotag_hrs_buildup_1" => 'Hours: In Buildup 1 Month',
			"smf_autotag_hrs_buildup_2" => 'Hours: In Buildup 2 Months',
			// "smf_autotag_hrs_buildup_3" => 'Hours: In Buildup 3 Months',
			// "smf_autotag_hrs_buildup_4" => 'Hours: In Buildup 4 Months',

			"smf_autotag_hrs_break" => 'Hours: In Break',
			"smf_autotag_hrs_break_1" => 'Hours: In Break 1 Month',
			"smf_autotag_hrs_break_2" => 'Hours: In Break 2 Months',
			"smf_autotag_hrs_break_3" => 'Hours: In Break 3 Months',
			"smf_autotag_hrs_break_4" => 'Hours: In Break 4 Months',
			"smf_autotag_hrs_break_5" => 'Hours: In Break 5 Months',
			"smf_autotag_hrs_break_6" => 'Hours: In Break 6 Months',
			"smf_autotag_hrs_break_7" => 'Hours: In Break 7 Months',
			"smf_autotag_hrs_break_8" => 'Hours: In Break 8 Months',
			"smf_autotag_hrs_break_9" => 'Hours: In Break 9 Months',
			"smf_autotag_hrs_break_10" => 'Hours: In Break 10 Months',
			"smf_autotag_hrs_break_11" => 'Hours: In Break 11 Months',
			"smf_autotag_hrs_break_12" => 'Hours: In Break 12 Months',

			"smf_autotag_hrs_buildup_complete" => 'Hours: Buildup Complete',

			"smf_autotag_hrs_incomplete_data" => 'Hours: Incomplete Data',

			"smf_autotag_hrs_nonzero" => 'Hours: Nonzero Three Months Previous',
			"smf_autotag_hrs_elig" => 'Hours: Eligible Three Months Previous',
		);

		$parents = array(
			"smf_autotag_hrs_buildup" => 'smf_autotag_hrs',
			"smf_autotag_hrs_buildup_1" => 'smf_autotag_hrs_buildup',
			"smf_autotag_hrs_buildup_2" => 'smf_autotag_hrs_buildup',
			// "smf_autotag_hrs_buildup_3" => 'smf_autotag_hrs_buildup',
			// "smf_autotag_hrs_buildup_4" => 'smf_autotag_hrs_buildup',

			"smf_autotag_hrs_break" => 'smf_autotag_hrs',
			"smf_autotag_hrs_break_1" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_2" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_3" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_4" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_5" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_6" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_7" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_8" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_9" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_10" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_11" => 'smf_autotag_hrs_break',
			"smf_autotag_hrs_break_12" => 'smf_autotag_hrs_break',

			"smf_autotag_hrs_buildup_complete" => 'smf_autotag_hrs',

			"smf_autotag_hrs_incomplete_data" => 'smf_autotag_hrs',

			"smf_autotag_hrs_nonzero" => 'smf_autotag_hrs',
			"smf_autotag_hrs_elig" => 'smf_autotag_hrs',
		);

		foreach ($ids as $id => $name) {
			$result = SiriusResult::create(sirius_smf_autotag_tag_lookup($name, $id, $parents[$id], TRUE));
			if (!$result->success()) { return $result->data(); }
			$map[$id] = $result->get('tid');
		}

		$lookups['ids'] = $ids;
		$lookups['map'] = $map;
	}

	// Check the buildup status. We should really be pulling settings and params
	// from various other places, i.e. the policy. But buildup has hardcoded values and no
	// settings. So we're just coercing.
	$elig_plugin_info = sirius_trust_eligibility_plugin_info('smf-buildup');
	$settings = array();
	$params = array();
	$params['ts'] = $asof_ts;
	$params['subscriber_worker_node'] = $worker_node;
	
	$result = SiriusResult::create($elig_plugin_info['instance']->check($settings, $params));

	$my_tag_ids = array();
	if ($result->success()) {
		$my_tag_ids[] = 'smf_autotag_hrs_buildup_complete';
	} else {
		$buildup = $result->get('current_buildup_count');
		if ($buildup) {
			$my_tag_ids[] = 'smf_autotag_hrs_buildup';
			$my_tag_ids[] = 'smf_autotag_hrs_buildup_' . $buildup;
		}

		$incomplete_data = $result->get('incomplete_data');
		if ($incomplete_data) {
			$my_tag_ids[] = 'smf_autotag_hrs_incomplete_data';
		}
	}
	if ($result->get('threemonthsprev_elig')) { $my_tag_ids[] = 'smf_autotag_hrs_elig'; }
	if ($result->get('threemonthsprev_nonzero')) { $my_tag_ids[] = 'smf_autotag_hrs_nonzero'; }

	$break = $result->get('current_break_count');
	if ($break) {
		$my_tag_ids[] = 'smf_autotag_hrs_break';
		$my_tag_ids[] = 'smf_autotag_hrs_break_' . $break;
	}


	// Tag
	foreach ($entity_nodes as $entity_node) {
		foreach ($lookups['map'] as $id => $tag_tid) {
			if (in_array($id, $my_tag_ids)) {
				$result = SiriusResult::create(sirius_contact_tag_add($entity_node, $tag_tid, TRUE));
			} else {
				$result = SiriusResult::create(sirius_contact_tag_remove($entity_node, $tag_tid, TRUE));
			}

			if (!$result->success()) { return $result->data(); }
		}
	}

	return array('success' => TRUE);
}

function sirius_smf_autotag_status_render($node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	$json = sirius_json_get($node);
	$status = $json['smf']['autotag']['status'];

	$html = '<div class="sirius_smf_autotag_status sirius_callout_basic">';
	$html .= '<h2>' . t('Autotag Status') . '</h2>';
	if (!$status) {
		$html .= t('Autotag has never been run.');
	} else {
		$html .= sirius_fakerender_field(
			'Last Run:', 
			date('l, F d Y - g:ia', $status['run_ts']) . " (" . format_interval(time() - $status['run_ts']) . " ago)"
		);
		$html .= sirius_fakerender_field('As Of:', date('l, F d Y - g:ia', $status['asof_ts']));
		//$html .= sirius_fakerender_field('Changed:', $status['change_count']);
		//$html .= sirius_fakerender_field('Added:', $status['add_count']);
		//$html .= sirius_fakerender_field('Removed:', $status['remove_count']);
	}
	$html .= '</div>'; 
	return $html;
} 

function sirius_smf_autotag_tag_lookup($name, $id, $parent_id, $create = FALSE) {
	$tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $id);
	if ($tid) { return array('success' => TRUE, 'tid' => $tid); }

	if (!$create) { return array('success' => FALSE, 'msg' => "Tag not found: $name (id = $id, parent_id = $parent_id)"); }

	sirius_minilog("Tag not found with id $id. Creating with parent $parent_id.");
	$vid = taxonomy_vocabulary_machine_name_load('sirius_contact_tags')->vid;

	$parent_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $parent_id);
	if (!$parent_tid) {
		return array('success' => FALSE, 'msg' => "Parent tag not found. Please create a tag with the ID $parent_id.");
	}

	$term = new stdClass();
	$term->vid = $vid;
	$term->field_sirius_id['und'][0]['value'] = $id;
	$term->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
	$term->name = $name;
	$term->parent = array($parent_tid);
	taxonomy_term_save($term);

	return array('success' => TRUE, 'tid' => $term->tid, 'term' => $term);
}
