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

function sirius_ledger_policy_lookup($schedule_key, $worker_node, $employer_node = NULL, $account_nid = NULL) {
	// Get the employer
	if (!$employer_node) {
		$employer_node = node_load($worker_node->field_grievance_shop['und'][0]['target_id']);
	}
	$employer_name = '[no employer]';
	$contract_tplt_name = '[no contract]';

	$ws_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];
	$ws_term = taxonomy_term_load($ws_tid);
	$ws_name = $ws_term->name;
	if (!$ws_tid) {
		sirius_minilog("Policy lookup for $schedule_key: No work status has been set for this worker. No policy will match.");
		return NULL;
	}

	if ($employer_node) {
		$employer_name = $employer_node->title;

		$employer_json = sirius_jsonfield_util_node_json_get($employer_node);
		$policy_node = node_load($employer_json['ledger']['policy']['nid']['default']);
		$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
		if ($policy_node && 
			$policy_json['ledger']['policy']['settings']['schedule'][$schedule_key] &&
			$policy_json['ledger']['policy']['settings']['ws'][$ws_tid] &&
			(!$account_nid || $policy_json['ledger']['policy']['settings']['accounts'][$account_nid])) {

			sirius_minilog("Policy lookup for $schedule_key / $ws_name: Found the policy " . $policy_node->title . " for " . $worker_node->title . ' at employer ' . $employer_node->title);
			return $policy_node;
		}

		$contract_tplt_node = node_load($employer_node->field_grievance_contract_tplt['und'][0]['target_id']);
		if ($contract_tplt_node) {
			$contract_tplt_name = $contract_tplt_node->title;
			$contract_tplt_json = sirius_jsonfield_util_node_json_get($contract_tplt_node);
			$policy_node = node_load($contract_tplt_json['ledger']['policy']['nid']['default']);
			$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
			if ($policy_node && 
				$policy_json['ledger']['policy']['settings']['schedule'][$schedule_key] &&
				$policy_json['ledger']['policy']['settings']['ws'][$ws_tid] &&
				(!$account_nid || $policy_json['ledger']['policy']['settings']['accounts'][$account_nid])) {
				sirius_minilog("Policy lookup for $schedule_key / $ws_name: Found the policy for " . $worker_node->title . ' at employer ' . $employer_node->title . ' in contract template ' . $contract_tplt_node->title);
				return $policy_node;
			}
		}
	}

	$policy_node = node_load(sirius_domain_variable_get('sirius_ledger_default_policy'));
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	if ($policy_node && 
		$policy_json['ledger']['policy']['settings']['schedule'][$schedule_key] &&
		$policy_json['ledger']['policy']['settings']['ws'][$ws_tid] &&
			(!$account_nid || $policy_json['ledger']['policy']['settings']['accounts'][$account_nid])) {
		sirius_minilog("Policy lookup for $schedule_key / $ws_name: No policy found for " . $worker_node->title . ' at employer ' . $employer_name . ' in contract template ' . $contract_tplt_name . '. Using the global policy.');
		return $policy_node;
	}

	sirius_minilog("Policy lookup for $schedule_key / $ws_name: No policy found for " . $worker_node->title . ' at employer ' . $employer_name . ' in contract template ' . $contract_tplt_name . ' and there is no matching global policy set.', 'warning');
	return NULL;
}

function sirius_ledger_policy_lookup_rates($worker_node, $policy_node, $schedule_key) {
	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) { 
		$lookups['ms'] = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
		$lookups['level'] = sirius_training_levels_fetch_all();
	}
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
			foreach ($rates as $account_nid => $rate_record) {
				$rate = $rate_record['rate'];
				if ($rate > $max_rates[$account_nid]) {
					$max_rates[$account_nid] = array(
						'rate' => $rate,
						'ms_tid' => $ms_tid,
						'ms_name' => $lookups['ms'][$ms_tid],
						'level_code' => $level_code,
						'level_name' => $lookups['level'][$ms_tid][$level_code]['title'],
						'triggers' => $rate_record['triggers'],
					);
				}
			}
		}
	}

	return $max_rates;
}
