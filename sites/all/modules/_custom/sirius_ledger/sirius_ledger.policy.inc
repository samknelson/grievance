<?php

function sirius_ledger_policy_list() {
	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_json_definition');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->fieldCondition('field_sirius_type', 'value', 'sirius_ledger:policy');
	$query->propertyOrderBy('title');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	return node_load_multiple(array_keys($stmt['node']));
}

function sirius_ledger_policy_lookup($schedule_key, $worker_node, $employer_node = NULL) {
	// Get the employer
	if (!$employer_node) {
		$employer_node = node_load($worker_node->field_grievance_shop['und'][0]['target_id']);
	}
	if (!$employer_node) {
		sirius_minilog('Policy lookup: No employer found for ' . $worker_node->title, 'warning');
		return NULL;
	}

	$employer_json = sirius_jsonfield_util_node_json_get($employer_node);
	$policy_node = node_load($employer_json['ledger']['policy']['nid']['default']);
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	if ($policy_node && $policy_json['ledger']['policy']['settings']['schedule'][$schedule_key]) {
		sirius_minilog("Policy lookup: Found the policy " . $policy_node->title . " for " . $worker_node->title . ' at employer ' . $employer_node->title);
		return $policy_node;
	}

	$contract_tplt_node = node_load($employer_node->field_grievance_contract_tplt['und'][0]['target_id']);
	$contract_tplt_json = sirius_jsonfield_util_node_json_get($contract_tplt_node);
	$policy_node = node_load($contract_tplt_json['ledger']['policy']['nid']['default']);
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	if ($policy_node && $policy_json['ledger']['policy']['settings']['schedule'][$schedule_key]) {
		sirius_minilog("Policy lookup: Found the policy for " . $worker_node->title . ' at employer ' . $employer_node->title . ' in contract template ' . $contract_tplt_node->title);
		return $policy_node;
	}

	$policy_node = node_load(sirius_domain_variable_get('sirius_ledger_default_policy'));
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	if ($policy_node && $policy_json['ledger']['policy']['settings']['schedule'][$schedule_key]) {
		sirius_minilog("Policy lookup: No policy found for " . $worker_node->title . ' at employer ' . $employer_node->title . ' in contract template ' . $contract_tplt_node->title . '. Using the global policy.');
		return $policy_node;
	}

	sirius_minilog("Policy lookup: No policy found for " . $worker_node->title . ' at employer ' . $employer_node->title . ' in contract template ' . $contract_tplt_node->title . ' and there is no global policy set.', 'warning');
	return NULL;
}

function sirius_ledger_policy_lookup_rates($worker_node, $policy_node, $schedule_key) {
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	$levels = sirius_training_levels_fetch_worker($worker_node);
	$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');

	// Find the highest matching rate
	$max_rates = array();
	foreach ($ms_tids as $ms_tid) {
		$rates = array();

		$level_code = $levels[$ms_tid]['code'];
		if ($level_code) {
			$rate_key = "$ms_tid|$level_code";
			$rates = $policy_json['ledger']['policy']['rates'][$schedule_key][$rate_key];
		}

		if (!$rates) {
			$rate_key = $ms_tid;
			$rates = $policy_json['ledger']['policy']['rates'][$schedule_key][$rate_key];
		}

		if ($rates) {
			foreach ($rates as $account_nid => $rate) {
				if ($rate > $max_rates[$account_nid]) {
					$max_rates[$account_nid] = $rate;
				}
			}
		}
	}

	return $max_rates;
}