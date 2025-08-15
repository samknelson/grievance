<?php

function sirius_trust_wb_scan_worker_start($mode, $worker_node, $options = array()) {
	$change_count = 0;
	$election_node = sirius_trust_worker_election($worker_node, $options['ts']);
	if (!$election_node) {
		sirius_minilog("No election found.");
	} else {
		sirius_minilog("Using election " . $election_node->title . " (" . $election_node->nid . ")");

		$policy_node = node_load($election_node->field_sirius_trust_policy['und'][0]['target_id']);
		if (!$policy_node) { return array('success' => FALSE, 'msg' => 'No policy found in the election.'); }

		$employer_node = node_load($election_node->field_grievance_shop['und'][0]['target_id']);
		if (!$employer_node) { return array('success' => FALSE, 'msg' => "No employer found in the election."); }

		$benefit_nodes = node_load_multiple(sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id'));
		$relationship_nodes = node_load_multiple(sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id'));

		foreach ($benefit_nodes as $benefit_nid => $benefit_node) {
			sirius_minilog("Checking benefit " . $benefit_node->title);

			sirius_minilog_indent();

			// Check the subscriber. We store their eligibility results to make the dependent checks cheaper
			sirius_minilog("Checking relationship [self]");
			unset($options['subscriber_eligibility']);
			$result = sirius_trust_wb_scan_record_start($mode, $employer_node, $policy_node, $worker_node, NULL, $benefit_node, $options);
			$change_count += $result['change_count'];
			sirius_minilog($result['msg']);
			$options['subscriber_eligibility'] = $result['eligibility'];

			// Check each dependent
			foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
				sirius_minilog("Checking relationship " . $relationship_node->title);
				$result = sirius_trust_wb_scan_record_start($mode, $employer_node, $policy_node, $worker_node, $relationship_node, $benefit_node, $options);
				$change_count += $result['change_count'];
				sirius_minilog($result['msg']);
			}
			sirius_minilog_outdent();
		}
	}

	return array('success' => TRUE, 'change_count' => $change_count);
}

function sirius_trust_wb_scan_record_start($mode, $employer_node, $policy_node, $worker_node, $relationship_node, $benefit_node, $options = array()) {
	$change_data = array(
		'mode' => $mode, 
		'employer_nid' => $employer_node->nid, 
		'policy_nid' => $policy_node->nid,
		'relationship_nid' => $relationship_node->nid,
		'benefit_nid' => $benefit_node->nid,
		'scan' => 'start',
		'subscriber_worker_nid' => $worker_node->nid,
	);

	// Find the dependent.
	if ($relationship_node) {
		$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		if (!$dependent_contact_nid) { return array('success' => FALSE, 'msg' => 'The relationship does not reference a contact.'); }
		$dependent_worker_nid = sirius_worker_get_by_contact_nid($dependent_contact_nid, TRUE);
		if (!$dependent_worker_nid) { return array('success' => FALSE, 'msg' => 'The contact is not associated with a worker.'); }
		$change_data['dependent_worker_nid'] = $dependent_worker_nid;
	} else {
		$change_data['dependent_worker_nid'] = $worker_node->nid;
	}

	// Find out if the benefit is already active
	$search_params = array();
	// $search_params['active'] = 'Yes';
	$search_params['ts'] = $options['ts'];
	$search_params['benefit_nid'] = $benefit_node->nid;
	$search_params['nids_only'] = TRUE;
	if ($relationship_node) {
		$search_params['worker_nid'] = $dependent_worker_nid;
	} else {
		$search_params['worker_nid'] = $worker_node->nid;
	}
	$probe = sirius_trust_worker_benefits_search($search_params);
	if ($probe) {
		$change_data['msg'] = 'Benefit is already active';
		$change_data['action'] = 'ok';
		Sirius_Trust_Wb_Scan_Changelog::getInstance()->log($change_data);
		return array('success' => TRUE, 'msg' => 'Benefit is already active.');
	}

	// Check eligibility.
	$params = array();
	$params['policy_node'] = $policy_node;
	$params['benefit_node'] = $benefit_node;
	$params['subscriber_worker_node'] = $worker_node;
	$params['shortcut'] = TRUE;
	$params['ts'] = $options['ts'];

	// We really should scan just "benefit_start". But ... what if the settings are such that "benefit_start"
	// and "benefit_continue" are in conflict? Then we'd start a benefit and immediately terminate it.
	// So ... we scan both. 
	// $params['scan_type'] = 'benefit_start'; 

	if ($relationship_node) { $params['relationship_node'] = $relationship_node; }
	if ($options['subscriber_eligibility']) { $params['subscriber_eligibility'] = $options['subscriber_eligibility']; }

	$eligibility = sirius_trust_eligibility_plugin_check_all($params);
	if (!$eligibility['success']) {
		$change_data['msg'] = 'Not eligible: ' . $eligibility['msg'];
		$change_data['action'] = 'ok';
		Sirius_Trust_Wb_Scan_Changelog::getInstance()->log($change_data);
		return array("success" => TRUE, 'msg' => "Not eligible: " . $eligibility['msg'], 'eligibility' => $eligibility);
	}

	sirius_minilog("There is no benefit active but the worker is eligible. Starting the benefit.");

	// Did the benefit end any time between the first of last month and the end of this month? If so, just extend it.
	unset($search_params['active']);
	$search_params['end_date_range'] = array(
		'start' => date('Y-m-1 00:00:00', strtotime('-1 month', $options['ts'])),
		'end' => date('Y-m-d', $options['ts']),
	);
	$search_params['ts'] = strtotime('-1 month', strtotime(date('Y-m-15', $options['ts'])));
	$probe = sirius_trust_worker_benefits_search($search_params);
	if ($probe) {
		$change_data['msg'] = 'The worker had the benefit last month.';
		$change_data['action'] = 'extend';

		sirius_minilog("The worker had the benefit last month. Extending.");
		$wb_nid = reset($probe);
		$wb_node = node_load($wb_nid);
		unset($wb_node->field_sirius_date_end['und']);
	} else {
		$change_data['msg'] = 'The worker did not have the benefit last month.';
		$change_data['action'] = 'start';

		sirius_minilog("The worker did not have the benefit last month. Starting.");

		global $user;
		$domain_nid = sirius_domain_nid();
    $wb_node = new stdClass();
    $wb_node->type = 'sirius_trust_worker_benefit';
    $wb_node->language = LANGUAGE_NONE;
    $wb_node->uid = $user->uid;
    $wb_node->status = 1;
    $wb_node->promote = 0;
    $wb_node->comment = 0;
    if ($domain_nid) { $wb_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
    $wb_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-1 00:00:00', $options['ts']);
    $wb_node->field_sirius_trust_benefit['und'][0]['target_id'] = $benefit_node->nid;
    $wb_node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;
    if ($relationship_node) {
    	$wb_node->field_sirius_contact_relation['und'][0]['target_id'] = $relationship_node->nid;
    	$wb_node->field_sirius_trust_subscriber['und'][0]['target_id'] = $worker_node->nid;
    	$wb_node->field_sirius_worker['und'][0]['target_id'] = $dependent_worker_nid;
    } else {
			$wb_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;
    }
    $wb_node->field_sirius_trust_election['und'][0]['target_id'] = $election_node->nid;
    $wb_node->field_sirius_notes['und'][0]['value'] = 'Started by benefit scan.';
	}

	// sirius_trust_worker_benefit_node_presave($wb_node);
	$result = array();
	if ($mode == 'live') {
		$result['change_count'] = 1;
		node_save($wb_node);
	} else {
		sirius_minilog("(mode = $mode, record not saved.)");
	}

	Sirius_Trust_Wb_Scan_Changelog::getInstance()->log($change_data);

	$result['success'] = TRUE;
	$result['msg'] = 'Benefit started.';
	$result['eligibility'] = $eligibility;

	return $result;
}


