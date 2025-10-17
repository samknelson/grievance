<?php


function sirius_trust_wb_scan_worker_stop($mode, $worker_node, $options = array()) {
	$change_count = 0;
	$search_params['subscriber_nid'] = $worker_node->nid;
	$search_params['active'] = 'Yes';
	$wb_nodes = sirius_trust_worker_benefits_search($search_params);

	foreach ($wb_nodes as $wb_nid => $wb_node) {
		sirius_minilog("Scanning worker benefit " . $wb_node->title . ' (' . $wb_node->nid . ')');
		sirius_minilog_indent();
		$result = sirius_trust_wb_scan_record_stop($mode, $wb_node, $options);
		$change_count += $result['change_count'];
		sirius_minilog_outdent();
	}

	return array('success' => TRUE, 'change_count' => $change_count);
}

function sirius_trust_wb_scan_record_stop($mode, $wb_node, $options = array()) {
	$change_data = array(
		'mode' => $mode,
		'wb_nid' => $wb_node->nid,
		'scan' => 'stop',
	);

	$wb_json = sirius_json_get($wb_node);
	$wb_json_orig = $wb_json;
	$need_save = FALSE;
	unset($wb_json['scan']['warnings']);

	$subscriber_worker_node  = node_load($wb_node->field_sirius_trust_subscriber['und'][0]['target_id']);
	if (!$subscriber_worker_node) { return array('success' => FALSE, 'msg' => 'No subscriber worker record.'); }
	$change_data['subscriber_worker_nid'] = $subscriber_worker_node->nid;

	$employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);

	$election_node = sirius_trust_worker_election($subscriber_worker_node, $options['ts']);

	if ($election_node) { $policy_node = node_load($election_node->field_sirius_trust_policy['und'][0]['target_id']); }
	if (!$policy_node) { $policy_node = sirius_ledger_policy_lookup($subscriber_worker_node, $employer_node); }
	if (!$policy_node) { return array('success' => FALSE, 'msg' => 'No policy record.'); }

	$relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);
	if ($relationship_node) {
		$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$dependent_worker_node = sirius_worker_get_by_contact_nid($dependent_contact_nid);
		$change_data['relationship_nid'] = $relationship_node->nid;
	} else {
		$dependent_worker_node = $subscriber_worker_node;
	}

	$change_data['dependent_worker_nid'] = $dependent_worker_node->nid;

	if (!$dependent_worker_node) { return array('success' => FALSE, 'msg' => 'No dependent worker record.'); }

	$benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
	if (!$benefit_node) { return array('success' => FALSE, 'msg' => 'No benefit record.'); }

	$change_data['benefit_nid'] = $benefit_node->nid;

	$params = array();
	$params['policy_node'] = $policy_node;
	$params['employer_node'] = $employer_node;
	$params['benefit_node'] = $benefit_node;
	$params['subscriber_worker_node'] = $subscriber_worker_node;
	$params['relationship_node'] = $relationship_node;
	$params['shortcut'] = TRUE;
	$params['scan_type'] = 'benefit_continue';
	$params['ts'] = $options['ts'];
	$result = sirius_trust_eligibility_plugin_check_all($params);
	if (!$result['success']) {
		sirius_minilog("No longer eligible: " . $result['msg']);
		sirius_minilog("The benefit will be expired.");

		$change_data['msg'] = "No longer eligible: " . $result['msg'];
		$change_data['action'] = 'terminated';

		$date = gmdate('Y-m-1', $options['ts']);
		$wb_node->field_sirius_date_end['und'][0]['value'] = $date;
		$wb_json['scan']['msg'] = $result['msg'];
		$wb_json['scan']['ts'] = time();
		$need_save = TRUE;
		if ($mode == 'live') {
			$log_node = sirius_log(
				'trust:wb:scan', 
				'terminated',
				'Automated scan terminated benefit: ' . $result['msg'], 
				'Automated scan terminated benefit: ' . $result['msg'], 
				NULL,
				array($wb_node->nid)
			);
		}
	} else if ($result['warning']) {
		sirius_minilog("Worker is eligible for the benefit, but with a warning: " . $result['msg']);
		$wb_json['scan']['warnings'] = $result['msg'];

		$change_data['msg'] = "Warning: " . $result['msg'];
		$change_data['action'] = 'warning';
	} else {
		sirius_minilog("Worker is still eligible for the benefit.");
		$wb_json['scan']['msg'] = "Ok";

		$change_data['action'] = 'ok';
	}

	if (sirius_revision_changed_impl($wb_json, $wb_json_orig, 10) || sirius_revision_changed_impl($wb_json_orig, $wb_json, 10)) {
		$need_save = TRUE;
	}

	$result = array();
	if ($need_save) {
		sirius_json_set($wb_node, $wb_json);
		if ($mode == 'live') {
			node_save($wb_node);
			sirius_minilog("Worker benefit updated.");
			$result['change_count'] = 1;
		} else {
			sirius_minilog("(Test mode, worker benefit not saved.)");
		}
	}

	Sirius_Trust_Wb_Scan_Changelog::getInstance()->log($change_data);

	$result['success'] = TRUE;
	$result['msg'] = 'Benefit stopped.';

	return $result;
}

function sirius_trust_benefits_list_in_order($benefit_nids) {
	if (!$benefit_nids) { return array(); }

	$benefit_types = &drupal_static(__FUNCTION__);
	if (!isset($benefit_types)) {
		$benefit_types = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'full');
	}

	$benefit_nodes = node_load_multiple($benefit_nids);

 	uksort($benefit_nodes, function($benefit_nid_a, $benefit_nid_b) use ($benefit_nodes, $benefit_types) {
 		$type_tid_a = $benefit_nodes[$benefit_nid_a]->field_sirius_trust_benefit_type['und'][0]['tid'];
 		$type_tid_b = $benefit_nodes[$benefit_nid_b]->field_sirius_trust_benefit_type['und'][0]['tid'];

 		$weight_a = $benefit_types[$type_tid_a]->weight;
 		$weight_b = $benefit_types[$type_tid_b]->weight;

 		return $weight_a <=> $weight_b;
 	});

	$benefit_names = array();
	foreach ($benefit_nodes as $benefit_nid => $benefit_node) {
		$benefit_names[$benefit_nid] = $benefit_node->title;
	}

	return $benefit_names;
}
