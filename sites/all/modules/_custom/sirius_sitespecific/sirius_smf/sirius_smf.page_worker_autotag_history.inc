<?php

function sirius_smf_node_tab_worker_autotag_history($worker_node) {
	// Some lookups
	sirius_require('sirius_smf', 'sirius_smf.eligibility_buildup.inc');
	$worker_nid = $worker_node->nid;
	$employers = sirius_employer_list();
	$medical_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'MEDICAL');
	$dental_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'DENTAL');
	if (!$medical_type_tid || !$dental_type_tid) {
		return sirius_page_error("Make sure that there is a benefit type with the name or ID 'DENTAL' and 'MEDICAL'.");
	}

	// Find the start date. This is the date of first hours.
	$sql = "select * ";
	$sql .= "from sirius_hours_cache ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year, month, day limit 1 ";
	$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid));
	$hr = $stmt->fetchAssoc();
	if ($hr) {
		$y = $hr['year'];
		$m = $hr['month'];
		if (strlen($m) < 2) { $m = "0$m"; }
		$ym_first = "$y-$m";
	} else {
		$ym_first = '2022-01';
	}
	if ($ym_first < '2010-01') { $ym_first = '2010-01'; }

	// Get a list of months
	$y = date('Y');
	$m = date('m');
	$ym = "$y-$m";
	$count = 0;
	$months = array();
	while (TRUE) {
		$month = array();
		$month['y'] = $y;
		$month['m'] = $m;
		$month['ts'] = strtotime($ym . '-15');
		$month['hours']['by_employer'] = array();
		$months[$ym] = $month;

		++$count;
		$m--;
		if ($m < 1) { $m = 12; $y--; }
		if (strlen($m) < 2) { $m = "0$m"; }
		$ym = "$y-$m";
		if ($count > 100) { break; }
		if ($ym < $ym_first) { break; }
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
		$months[$ym]['hours']['by_employer'][$employer_nid] += $hr['hours'];
		$months[$ym]['hours']['total'] += $hr['hours'];
	}

	// Sort hours by employer
	foreach ($months as $ym => $ignore) {
		uksort($months[$ym]['hours']['by_employer'], function($a, $b) use ($ym, $months) { 
			return $months[$ym]['hours']['by_employer'][$b] <=> $months[$ym]['hours']['by_employer'][$a];
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

		// Get the coverage
		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['benefit_types'] = array($medical_type_tid);
		$search_params['ts'] = $col1_ts;
		$wb_nodes = sirius_trust_worker_benefits_search($search_params);

		// Get the elected benefits
		$benefit_names = array();
		if ($election_node) {
			$benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
			foreach ($benefit_nids as $benefit_nid) {
				if (!$benefit_lookup[$benefit_nid]) { $benefit_lookup[$benefit_nid] = node_load($benefit_nid); }
				$benefit_type_tid = $benefit_lookup[$benefit_nid]->field_sirius_trust_benefit_type['und'][0]['tid'];
				if (!in_array($benefit_type_tid, array($medical_type_tid, $dental_type_tid))) { continue; }
				$benefit_names[] = $benefit_lookup[$benefit_nid]->title;
			}
		}

		// Get the threshold
		$threshold = NULL;
		if ($election_node) {
			$policy_nid = $election_node->field_sirius_policy['und'][0]['target_id'];
			if (!$policy_lookup[$policy_nid]) { $policy_lookup[$policy_nid] = node_load($policy_nid); }
			$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->threshold_from_worker_and_policy($worker_node, $policy_lookup[$policy_nid]);
		}

		$col1_parts = $col2_parts = array();
		$col1_parts[] = '<strong>' . date('F Y', $col1_ts) . '</strong>';
		if ($wb_nodes) {
			$col1_parts[] = 'Coverage: ' . '<strong>Yes</strong>';
		} else {
			$col1_parts[] = 'Coverage: ' . '<strong>No</strong>';
		}

		if ($threshold) {
			if ($total >= $threshold) {
				$col1_parts[] = 'Eligible: ' . '<strong>Yes</strong>';
			} else {
				$col1_parts[] = 'Eligible: ' . '<strong>No</strong>';
			}
		}

		if ($benefit_names) {
			$col1_parts[] = 'Election: ' . '<strong>' . join(', ', $benefit_names) . '</strong>';
		}

		$col2_parts[] = '<strong>' . date('F Y', $col2_ts) . '</strong>';
		if (!$col2_record['hours']['by_employer']) { $col2_record['hours']['by_employer'] = array(); }
		foreach ($col2_record['hours']['by_employer'] as $employer_nid => $amt) {
			$col2_parts[] = $employers[$employer_nid] . ': ' . number_format($amt, 2);
		}
		$total = $col2_record['hours']['total'];
		if (!$total) { $total = 0; }
		$col2_parts[] = 'Total: ' . '<strong>' . number_format($total, 2) . '</strong>';

		if ($threshold) {
			$col2_parts[] = 'Required: ' . '<strong>' . number_format($threshold) . '</strong>';
		}

		if ($wb_nodes) {
			$color = 'green';
		} else {
			$color = 'red';
		}

		$row = array();
		$row[] = array('data' => join('<br>', $col2_parts), 'class' => array($color));
		$row[] = array('data' => join('<br>', $col1_parts), 'class' => array($color));
		$rows[] = $row;
	}
	$header = array();
	return sirius_table_render($header, $rows);
}
