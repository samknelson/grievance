<?php

function sirius_ledger_policy_rate_instance() {
	return Sirius_Smf_Rate::getInstance();
}

function sirius_ledger_policy_rates($context) {
	$instance = sirius_ledger_policy_rate_instance();
	return $instance->get_rates($context);
}

class Sirius_Ledger_Policy_Rate extends Sirius_Singleton {
	public function get_rates($context) {
		return array(
			'success' => FALSE,
			'msg' => 'Base get_rate function called.',
		);
	}
}

class Sirius_Smf_Rate extends Sirius_Ledger_Policy_Rate {
	public function get_rates($context) {
		return array(
			'success' => TRUE,
			'msg' => 'Whatever',
			'rate' => 100,
		);
	}
}

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

function sirius_ledger_policy_options() {
	$policy_nodes = sirius_ledger_policy_list();
	$policy_options = array();
	foreach ($policy_nodes as $policy_nid => $policy_node) {
		$policy_options[$policy_nid] = $policy_node->title;
	}
	return $policy_options;
}

function sirius_ledger_policy($entity, $options = array(), $ts = NULL) {
	$json = sirius_json_get($entity);

	if ($ts === NULL) {
		$nid = $json['ledger']['policy']['nid'];

		// Legacy support
		if (is_array($nid)) { $nid = $nid['default']; }

		if (!$nid) { return NULL; }
	} else {
		if (!$json['ledger']['policy']['ebh']) {
			$nid = $json['ledger']['policy']['nid'];
		} else {
			foreach ($json['ledger']['policy']['ebh'] as $record) {
				if ($ts < strtotime($record['date'])) { continue; }
				$nid = $record['policy'];
			}
		}

		if (!$nid) { return NULL; }
	}

	if ($options['nid_only']) { return $nid; }
	return node_load($nid);
}

function sirius_ledger_policy_lookup($worker_node, $employer_node = NULL) {
	// Get the employer
	if (!$employer_node) {
		$employer_node = node_load($worker_node->field_grievance_shop['und'][0]['target_id']);
	}
	$worker_name = $worker_node->title;
	if ($employer_node) { $employer_title = $employer_node->title; } else { $employer_title = '[no employer]'; }
	sirius_minilog("Policy lookup: Looking for policy for $worker_name at $employer_title");

	if ($employer_node) {
		$policy_node = sirius_ledger_policy($employer_node);
		if ($policy_node) { 
			sirius_minilog("Using employer policy: " . $policy_node->title);
			return $policy_node;
		}

		$contract_tplt_node = node_load($employer_node->field_grievance_contract_tplt['und'][0]['target_id']);
		if ($contract_tplt_node) {
			$policy_node = sirius_ledger_policy($employer_node);
			if ($policy_node) {
				sirius_minilog("Using contract template policy: " . $policy_node->title);
				return $policy_node;
			}
		}
	}

	$policy_node = node_load(sirius_domain_variable_get('sirius_ledger_default_policy'));
	if ($policy_node) {
		sirius_minilog("Using global policy: " . $policy_node->title);
		return $policy_node;
	}

	sirius_minilog("No policy found in employer, contract template, or global.", 'warning');
	return NULL;
}

function sirius_ledger_policy_lookup_rates($worker_node, $policy_node, $schedule_key, $ms_tid = NULL, $ts = NULL) {
	if (!$ts) { $ts = time(); }

	$policy_json = sirius_json_get($policy_node);

	$lookups = &drupal_static(__FUNCTION__);
	if (!isset($lookups)) { 
		$lookups['ms'] = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
		$lookups['level'] = sirius_training_levels_fetch_all();
	}
	$levels = sirius_training_levels_fetch_worker($worker_node);
	if ($ms_tid) { 
		$ms_tids = array($ms_tid);
	} else {
		$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	}
	$ws_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];

	if (sirius_domain_component_enabled('trust')) {
		if ($election_node) { $election_type_tid = $election_node->field_sirius_trust_election_type['und'][0]['tid']; }
	}

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
				if ($account_nid == 'pay' ||  !$election_type_tid) {
					$rate = $rate_record['rate'];
				} else {
					$rate = $rate_record['election_rates'][$election_type_tid]['rate'];
					if (!$rate) { $rate = $rate_record['rate']; }
				}
				if ($rate > $max_rates[$account_nid]) {
					$max_rates[$account_nid] = array(
						'rate' => $rate,
						'rate_skilled' => $rate_record['rate_skilled'],
						'ms_tid' => $ms_tid,
						'ms_name' => $lookups['ms'][$ms_tid],
						'level_code' => $level_code,
						'level_name' => $lookups['level'][$ms_tid][$level_code]['title'],
						'triggers' => $rate_record['triggers'],
					);
					if (!$ws_tid || !$policy_json['ledger']['policy']['settings']['ws'][$ws_tid]) {
						$max_rates[$account_nid]['rate'] = 0;
					}
				}
			}
		}
	}


	return $max_rates;
}

function sirius_ledger_policy_trigger_options($schedule_key = NULL) {
	if ($schedule_key == 'hour') {
		return array(
			'hour' => 'On Hour',
		);
	} else if (!$schedule_key) {
		return sirius_ledger_policy_trigger_options('hour') + sirius_ledger_policy_trigger_options('month');
	} else {
		return array(
			'manual' => 'Manual',
			'cron' => 'Auto',
			'payment' => 'Payment'
		);
	}
}
