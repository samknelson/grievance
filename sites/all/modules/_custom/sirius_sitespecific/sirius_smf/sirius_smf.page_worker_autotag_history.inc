<?php

function sirius_smf_node_tab_worker_autotag_history($worker_node) {
	// Some lookups
	sirius_require('sirius_smf', 'sirius_smf.eligibility_buildup.inc');
	$worker_nid = $worker_node->nid;
	$employers = sirius_employer_list();
	$medical_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'MEDICAL');
	$dental_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'DENTAL');
	$add_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'LIFE');

	if (!$medical_type_tid || !$dental_type_tid || !$add_type_tid) {
		return sirius_page_error("Make sure that there is a benefit type with the name or ID 'DENTAL' and 'MEDICAL' and 'LIFE'.");
	}

	$ee_account_nid = sirius_find_nid_by_id('EE', 'sirius_ledger_account');
	if (!$ee_account_nid) {
		return sirius_page_error('Make sure that there is an account with the ID "EE".');
	}
	$ea = Sirius_Ledger_EA::getInstance();
	$ea_node = $ea->fetch($worker_node->nid, $ee_account_nid, FALSE);
	if ($ea_node) {
		$invoice = Sirius_Ledger_Invoices::getInstance();
		$currency = sirius_ledger_currency_from_account_nid($ee_account_nid);
	}
	$y = date('Y');
	$m = date('m');

	$months = array();
	for ($i=0; $i<36; ++$i) {
		$ym = "$y-$m";
		$month = array();
		$month['y'] = $y;
		$month['m'] = $m;
		$month['ts'] = strtotime($ym . '-15');
		$month['hours']['by_employer'] = array();

		$months[$ym] = $month;

		--$m;
		if ($m < 1) {
			$m += 12;
			--$y;
		}
		if (strlen($m) < 2) { $m = "0$m"; }
	}

	// Load the hours
	$sql = "select * ";
	$sql .= "from sirius_hours_cache ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
	$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid));
	while ($hr = $stmt->fetchAssoc()) {
		$y = $hr['year'];
		$m = $hr['month'];
		if (strlen($m) < 2) { $m = "0$m"; }
		$ym = "$y-$m";

		$employer_nid = $hr['employer_nid'];
		$months[$ym]['hours']['by_employer'][$employer_nid]['amt'] += $hr['hours'];
		$months[$ym]['hours']['by_employer'][$employer_nid]['type'] = $hr['hours_type_tid'];
		$months[$ym]['hours']['total'] += $hr['hours'];
	}

	// Sort hours by employer
	foreach ($months as $ym => $ignore) {
		uksort($months[$ym]['hours']['by_employer'], function($a, $b) use ($ym, $months) { 
			return $months[$ym]['hours']['by_employer']['amt'][$b] <=> $months[$ym]['hours']['by_employer']['amt'][$a];
		}); 
	}

	// Generate our table
	$rows = array();
	$policy_lookup = $employer_lookup = $benefit_lookup = array();
	foreach ($months as $col1_ym => $col1_record) {
		$col1_ts = strtotime("$col1_ym-15");
		$col2_ts = strtotime('-3 months', $col1_ts);
		$col2_ym = date('Y-m', $col2_ts);
		$col2_record = $months[$col2_ym];

		// Get the election
		$election_node = sirius_trust_worker_election($worker_node, $col1_ts);

		$col1_parts = $col2_parts = array();
		$col1_parts[] = '<strong>' . date('F Y', $col1_ts) . '</strong>';
		$col2_parts[] = '<strong>' . date('F Y', $col2_ts) . '</strong>';

		// Get the elected benefits
		$elected_medical_benefit_nid = NULL;
		$elected_dental_benefit_nid = NULL;

		if ($election_node) {
			$elected_benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');

			foreach ($elected_benefit_nids as $benefit_nid) {
				if (!$benefit_lookup[$benefit_nid]) { $benefit_lookup[$benefit_nid] = node_load($benefit_nid); }

				$benefit_type_tid = $benefit_lookup[$benefit_nid]->field_sirius_trust_benefit_type['und'][0]['tid'];
				if ($benefit_type_tid == $dental_type_tid) {
					$elected_dental_benefit_nid = $benefit_nid;
				}

				if ($benefit_type_tid == $medical_type_tid) {
					$elected_medical_benefit_nid = $benefit_nid;
				}
			}
		}

		$elected_medical_benefit_name = $benefit_lookup[$elected_medical_benefit_nid]->title;
		$elected_dental_benefit_name = $benefit_lookup[$elected_dental_benefit_nid]->title;

		// Get the received benefits
		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['benefit_types'] = array($medical_type_tid);
		$search_params['ts'] = $col1_ts;
		$medical_wb_nodes = sirius_trust_worker_benefits_search($search_params);

		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['benefit_types'] = array($dental_type_tid);
		$search_params['ts'] = $col1_ts;
		$dental_wb_nodes = sirius_trust_worker_benefits_search($search_params);

		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['benefit_types'] = array($life_type_tid);
		$search_params['ts'] = $col1_ts;
		$add_wb_nodes = sirius_trust_worker_benefits_search($search_params);

		if ($medical_wb_nodes || $dental_wb_nodes) {
			// Receiving medical or dental
			$status = 'active';
		} else if ($elected_medical_benefit_nid && !$medical_wb_nodes) {
			// Elected medical or dental but not receiving it
			$status = 'ineligible';
			$ineligible_benefit_node = node_load($elected_medical_benefit_nid);
		} else if ($elected_dental_benefit_nid && !$dental_wb_nodes) {
			// Elected medical or dental but not receiving it
			$status = 'ineligible';
			$ineligible_benefit_node = node_load($elected_dental_benefit_nid);
		} else if ($add_wb_nodes) {
			// Receiving ADD
			$status = 'waived';
		} else {
			// Unknown
			$status = 'none';
		}

		// Get the policy node
		$policy_node = sirius_ledger_policy_lookup($worker_node, NULL, $col1_ts);
		$col1_parts[] = "Pol: " . $policy_node->title;
		$col1_parts[] = "Med: " . $elected_medical_benefit_name;
		$col1_parts[] = "Den: " . $elected_dental_benefit_name;

		// If ineligible, explain why
		if ($status == 'ineligible') {
			$params = array();
			$params['policy_node'] = $policy_node;
			$params['benefit_node'] = $ineligible_benefit_node;
			$params['subscriber_worker_node'] = $worker_node;
			$params['ts'] = $col1_ts;
			$params['shortcut'] = TRUE;
			$result = sirius_trust_eligibility_plugin_check_all($params);
			$col1_parts[] = "Eli:" . $result['msg'];
		}

		// Get the threshold
		$threshold = NULL;
		if ($election_node) {
			$policy_nid = $election_node->field_sirius_policy['und'][0]['target_id'];
			if (!$policy_lookup[$policy_nid]) { $policy_lookup[$policy_nid] = node_load($policy_nid); }
			$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->threshold_from_worker_and_policy($worker_node, $policy_lookup[$policy_nid]);
		}

		if (!$col2_record['hours']['by_employer']) { $col2_record['hours']['by_employer'] = array(); }
		foreach ($col2_record['hours']['by_employer'] as $employer_nid => $record) {
			$amt = $record['amt'];
			$col2_parts[] = $employers[$employer_nid] . ': ' . sirius_term_title($record['type']) . ':' . number_format($amt, 2);
		}
		$total = $col2_record['hours']['total'];
		if (!$total) { $total = 0; }
		$col2_parts[] = 'Tot: ' . '<strong>' . number_format($total, 2) . '</strong>';

		if ($threshold) {
			$col2_parts[] = 'Req: ' . '<strong>' . number_format($threshold) . '</strong>';
		}

		if ($invoice) {
			$participant_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
			$interval = $invoice->interval_from_ts($col1_ts);
			$balances = $invoice->invoice_load_balances($participant_nid, $ee_account_nid, $interval);
			$col1_parts[] = 'Charges: ' . $currency->render_amt($balances['charges']);
			$col1_parts[] = 'Payments: ' . $currency->render_amt($balances['payments']);
			$col1_parts[] = 'Balance: ' . $currency->render_amt($balances['balance_out']);
		}

		if ($status == 'active') {
			$color = 'green';
		} else if ($status == 'ineligible') {
			$color = 'red';
		} else if ($status == 'wavied') {
			$color = 'yellow';
		} else {
			$color = 'white';
		}

		$row = array();
		$row[] = array('data' => join('<br>', array_filter($col1_parts)), 'class' => array($color));
		$row[] = array('data' => join('<br>', array_filter($col2_parts)), 'class' => array($color));
		$rows[] = $row;
	}

	$header = array();
	$header[] = array('data' => 'Coverage Month', 'style' => 'width: 50%;');
	$header[] = array('data' => 'Work Month', 'style' => 'width: 50%;');
	return sirius_table_render($header, $rows);
}
