<?php

function sirius_smf_elections_cleanup_policy() {
	sirius_denorm_realtime(FALSE);
	$limit = 100;

 	$sql = "select * from node ";
	$sql .= "left join field_data_field_sirius_trust_policy on field_data_field_sirius_trust_policy.entity_type = 'node' and field_data_field_sirius_trust_policy.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "where type = 'sirius_trust_worker_election' ";
	$sql .= "and field_grievance_shop_target_id is not null ";
	$sql .= "and field_sirius_trust_policy_target_id is null ";
	// $sql .= "and nid = 6683660 ";
	$sql .= "order by nid desc ";
	// $sql .= "limit $limit ";

	$stmt = sirius_sql_query($sql, array());

	$election_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$election_nids[] = $hr['nid'];
	}
	$total = count($election_nids);

	sirius_minilog_echo_active(TRUE);
	$count = 0;
	foreach ($election_nids as $election_nid) {
		++$count;
		$election_node = node_load($election_nid);
		sirius_minilog("Cleaning up $count / $total: " . $election_node->title . " ($election_nid)");
		sirius_minilog_indent();
		$employer_node = node_load($election_node->field_grievance_shop['und'][0]['target_id']);
		if (!$employer_node) {
			sirius_minilog_outdent();
			continue;
		}
		sirius_minilog("Found employer " . $employer_node->title);
		$policy_node = sirius_ledger_policy($employer_node);
		if (!$policy_node) {
			sirius_minilog_outdent();
			continue;
		}
		sirius_minilog("Setting policy " . $policy_node->title);
		$election_node->field_sirius_trust_policy['und'][0]['target_id'] = $policy_node->nid;
		node_save($election_node);
		sirius_minilog("Election saved.");
		sirius_minilog_outdent();
	}
}

function sirius_smf_elections_cleanup_employer() {
	sirius_denorm_realtime(FALSE);
	$limit = 100;

 	$sql = "select * from node ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "where type = 'sirius_trust_worker_election' ";
	$sql .= "and field_grievance_shop_target_id is null ";
	$sql .= "order by nid desc ";
	// $sql .= "limit $limit ";

	$stmt = sirius_sql_query($sql, array());

	$election_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$election_nids[] = $hr['nid'];
	}
	$total = count($election_nids);

	sirius_minilog_echo_active(TRUE);
	$count = 0;
	foreach ($election_nids as $election_nid) {
		++$count;
		$election_node = node_load($election_nid);
		sirius_minilog("Cleaning up $count / $total: " . $election_node->title . " ($election_nid)");
		sirius_minilog_indent();
		$employer_nid = sirius_smf_election_cleanup_guess_employer($election_node);
		sirius_minilog("Setting employer: $employer_nid");
		$election_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
		node_save($election_node);
		sirius_minilog("Election saved.");
		sirius_minilog_outdent();
	}
}

/**
 * 1. The most recent employer to grant coverage
 * 2. Or the most recent employer with hours
 * 3. Or any employer with an active dispatch
 */

function sirius_smf_election_cleanup_guess_employer($election_node) {
	// Default
	$default_employer_nid = &drupal_static(__FUNCTION__);
	if (!isset($default_employer_nid)) {
		$default_employer_nid = sirius_find_nid_by_id('UK', 'grievance_shop');
	}
	if (!$default_employer_nid) {
		sirius_minilog("No default employer found. Please create an employer with ID [UK].");
		return;
	}


	$worker_nid = $election_node->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) {
		sirius_minilog("No worker found.");
		return $default_employer_nid;
	}

	// The most recent employer to grant coverage
	$search_params = array();
	$search_params['subscriber_nid'] = $worker_nid;
	$wb_nodes = sirius_trust_worker_benefits_search($search_params);
	foreach ($wb_nodes as $wb_nid => $wb_node) {
		$employer_nid = $wb_node->field_grievance_shop['und'][0]['target_id'];
		if ($employer_nid) {
			sirius_minilog("Found employer from benefit.");
			return $employer_nid;
		}
	}

	// The most recent employer with hours
	$sql = "select employer_nid from sirius_hours_cache ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "and employer_nid is not null ";
	$sql .= "and employer_nid > 0 ";
	$sql .= "order by year desc, month desc, day desc ";
	$sql .= "limit 1 ";
	$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
	$hr = $stmt->fetchAssoc();
	if ($hr) {
		sirius_minilog("Found employer from hours");
		return $hr['employer_nid'];
	}

	// "Any employer with an active dispatch"
	// (Skipping this because dispatches are derived from hours)

	// Default
	sirius_minilog("Using default employer.");
	return $default_employer_nid;
}

