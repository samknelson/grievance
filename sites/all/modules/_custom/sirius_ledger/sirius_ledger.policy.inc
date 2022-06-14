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

function sirius_ledger_policy_options() {
	$policy_nodes = sirius_ledger_policy_list();
	$policy_options = array();
	foreach ($policy_nodes as $policy_nid => $policy_node) {
		$policy_options[$policy_nid] = $policy_node->title;
	}
	return $policy_options;
}

function sirius_ledger_policy($entity) {
	$json = sirius_json_get($entity);
	$nid = $json['ledger']['policy']['nid'];

	// Legacy support
	if (is_array($nid)) { $nid = $nid['default']; }

	if (!$nid) { return NULL; }
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
			sirius_minilog("-- Using employer policy: " . $policy_node->title);
			return $policy_node;
		}

		$contract_tplt_node = node_load($employer_node->field_grievance_contract_tplt['und'][0]['target_id']);
		if ($contract_tplt_node) {
			$policy_node = sirius_ledger_policy($employer_node);
			if ($policy_node) {
				sirius_minilog("-- Using contract template policy: " . $policy_node->title);
				return $policy_node;
			}
		}
	}

	$policy_node = node_load(sirius_domain_variable_get('sirius_ledger_default_policy'));
	if ($policy_node) {
		sirius_minilog("-- Using global policy: " . $policy_node->title);
		return $policy_node;
	}

	sirius_minilog("-- No policy found in employer, contract template, or global.", 'warning');
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








function sirius_ledger_policy_rate_title($item, $lookups) {
	if ($item['title']) { return $item['title']; }

	$parts = array();
	foreach (array('account', 'schedule', 'ms_level', 'election_type', 'benefit', 'skill') as $key) {
		$value = $item[$key];

		if (!$value) { continue; }
		if ($value == 'all') { continue; }
		if (is_array($value)) {
			$values = array_filter(array_values($value));
			if (!count($values)) { continue; }

			if (count($values) > 2) {
				$first = reset($values);
				$parts[] = $lookups["${key}_options"][$first] . ' (+' . (count($values)-1) . ' more)';
			} else {
				$a = array();
				foreach ($values as $v) {
					$a[] = $lookups["${key}_options"][$v];
				}
				$parts[] = join(', ', $a);
			}
		} else {
			$parts[] = $lookups["${key}_options"][$value];
		}
	}

	$rate = $item['rate'];
	if (!$rate) { $rate = 0; }
	$title = '$' . number_format($rate, 2);
	if ($parts) {
		$title .= ': ';
		$title .= join(' / ', $parts);
	}
	return $title;
}














function sirius_ledger_policy_lookup_rates_new($worker_node, $policy_node, $schedule_key, $options = array()) {
	$ts = $options['ts'];
	if (!$ts) { $ts = time(); }
	$date = date('m/d/Y', $ts);
	$policy_json = sirius_json_get($policy_node);
	$items = $policy_json['ledger']['policy']['rates_new'];
	if (!$items) {
		sirius_minilog("There are no rate rules associated with this policy.", 'warning');
		return array();
	}
	uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$b]['weight']; });

	sirius_minilog("$schedule_key: Rates: Searching for matching rate rules.");

	// Fetch the worker member statuses and levels. @todo: this is not working historically.
	$worker_levels = sirius_training_levels_fetch_worker($worker_node);
	if ($options['ms_tid']) {
		$worker_ms_tids = array($options['ms_tid']);
	} else { 
		$worker_ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	}
	$worker_ms_levels = array();
	foreach ($worker_ms_tids as $ms_tid) {
		$worker_ms_levels[] = $ms_tid;
		if ($worker_levels[$ms_tid]) { $worker_ms_levels[] = $ms_tid . '|' . $worker_levels[$ms_tid]['code']; }
	}

	// Fetch the worker work status as of the date. If the policy doesn't apply to this work status, no rates apply
	$worker_ws_tid = sirius_worker_history_asof($worker_node, 'worker:work_status', $ts);

	$rates = array();
	foreach ($items as $item) {
		sirius_minilog("$schedule_key: Checking rate rule <em>$item[title]</em>");
		$account_nid = $item['account'];

		// Only one rate for each account
		if ($rates[$account_nid]) {
			sirius_minilog("$schedule_key: Already found a matching rule for this account. Skipping.");
			continue;
		}

		// Must have the same schedule (monthly, semimonthly, hourly)
		if ($item['schedule'] != $schedule_key) {
			sirius_minilog("$schedule_key: Not a $schedule_key rule. Skipping.");
			continue;
		}

		// Must have one of the correct work statuses
		$ok_ws_tids = array();
		if ($item['ws']) { $ok_ws_tids = array_filter(array_values($item['ws'])); }
		if ($ok_ws_tids) {
			if (!$worker_ws_tid) {
				sirius_minilog("$schedule_key: Worker did not have a work status as of $date.");
				continue;
			}
			if (!in_array($worker_ws_tid, $ok_ws_tids)) {
				sirius_minilog("$schedule_key: Worker did not have one of the work statuses for this policy as of $date.");
				continue;
			}
		}


		// Must have one of the correct member status / levels
		$ok_ms_levels = array();
		if ($item['ms_level']) { $ok_ms_levels = array_filter(array_values($item['ms_level'])); }
		if ($ok_ms_levels) {
			if (!array_intersect($ok_ms_levels, $worker_ms_levels)) {
				sirius_minilog("$schedule_key: Worker does not have one of the required member statuses or levels. Skipping.");
				continue;
			}
		}

		// Must have one of the correct election types.
		$election_types = array();
		if ($item['election_type']) { $election_types = array_filter(array_values($item['election_type'])); }
		if ($election_types) {
			$election_node = sirius_trust_worker_election($worker_node, $ts);
			if (!$election_node) {
				sirius_minilog("$schedule_key: Worker did not have an election. Skipping.");
				continue;
			}
			if (!in_array($election_node->field_sirius_trust_election_type['und'][0]['tid'], $election_types)) {
				sirius_minilog("$schedule_key: Worker did not have the correct election type. Skipping.");
				continue;
			}
		}

		// Worker must have one of the matching benefits
		$ok_benefits = array();
		if ($item['benefit']) { $ok_benefits = array_filter(array_values($item['benefit'])); }
		if ($ok_benefits) {
			$wb_nodes = sirius_trust_worker_benefits($worker_node, $ts);
			$my_benefits = array();
			foreach ($wb_nodes as $wb_nid => $wb_node) {
				$my_benefits[] = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
			}
			if (!array_intersect($ok_benefits, $my_benefits)) {
				sirius_minilog("$schedule_key: Worker was not receiving at least one of the required benefits. Skipping.");
				continue;
			}
		}

		sirius_minilog("$schedule_key: Looks good");
		$rates[$account_nid] = $item;
	}
	return $rates;
}
