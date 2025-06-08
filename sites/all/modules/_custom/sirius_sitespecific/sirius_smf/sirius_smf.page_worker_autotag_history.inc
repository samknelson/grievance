<?php

function sirius_smf_node_tab_worker_autotag_history($worker_node) {
	$worker_nid = $worker_node->nid;

	$tids = array();
	if ($form_state['values']['tags']) {
		$tids = array_filter(array_values($form_state['values']['tags']));
	}

	// Load the employers (lookup)
	$employers = sirius_employer_list();

	// Load the tags (lookup)
	$tags_by_id = sirius_taxonomy_load('sirius_contact_tags', 'field_sirius_id', 'full');

	// Load the WYM nodes
	$tmp = sirius_smf_wym_fetch_all($worker_node->nid);
	$wym_nodes = array();
	foreach ($tmp as $wym_nid => $wym_node) {
		$ym = date('Y-m', strtotime($wym_node->field_sirius_date_start['und'][0]['value']));
		$wym_nodes[$ym] = $wym_node;
	}
	krsort($wym_nodes);
	$ym_first = end(array_keys($wym_nodes));
	if (!$ym_first) { $ym_first = '2022-01'; }
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
		if (!$months[$ym]) { continue; }

		$employer_nid = $hr['employer_nid'];
		$months[$ym]['hours']['by_employer'][$employer_nid] += $hr['hours'];
		$months[$ym]['hours']['total'] += $hr['hours'];
	}
	foreach ($months as $ym => $ignore) {
		uksort($months[$ym]['hours']['by_employer'], function($a, $b) use ($ym, $months) { 
			return $months[$ym]['hours']['by_employer'][$b] <=> $months[$ym]['hours']['by_employer'][$a];
		}); 
	}

	// Load the hours thresholds. This requires the payperiod node for each month. There might be multiple thresholds. The default is 80.
	$domain_nid = sirius_domain_nid();
	foreach ($months as $ym => $ingore) {
		$y = $months[$ym]['y'];
		$m = $months[$ym]['m'];
		foreach ($months[$ym]['hours']['by_employer'] as $employer_nid => $ignore2) {
			// Find the payperiod
			$payperiod_node = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_nid, $y, $m, 1, FALSE);
			if (!$payperiod_node) { continue; }
			$payperiod_json = sirius_json_get($payperiod_node);
			$elig_hours = $payperiod_json['smf']['elig_hrs'];
			if (!$elig_hours) { $elig_hours = 80; }
			$months[$ym]['hours']['elig'][] = $elig_hours;
		}
	}
	foreach ($months as $ym => $ignore) {
		$tmp = $months[$ym]['hours']['elig'];
		if (!$tmp) { $tmp = array(80); }
		$tmp = array_unique($tmp);
		sort($tmp);
		$months[$ym]['hours']['elig'] = $tmp;
	}

	// Pull the notes from each benefit node. Clean and uniquify them and break them out by month
	$all_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct', array('benefit_types' => $benefit_types));
	foreach ($months as $ym => $month) {

		$notes = array();
		foreach ($all_benefits_received as $benefit_nid => $benefit_node) {
			$start_ts = strtotime($benefit_node->field_sirius_date_start['und'][0]['value']);
			if (!$start_ts) { continue; }
			if ($start_ts > $month['ts']) { continue; }

			if ($benefit_node->field_sirius_date_end['und'][0]['value']) {
				$end_ts = strtotime($benefit_node->field_sirius_date_end['und'][0]['value']);
				if ($end_ts < $month['ts']) { continue; }
			}

			$note = $benefit_node->field_sirius_notes['und'][0]['value'];
			$note = preg_replace('/\s*coverage coming from\s*/i', '', $note);
			$note = trim($note);
			$note = preg_replace('/;$/', '', $note);
			$notes[] = $note;
		}
		$notes = array_filter(array_unique($notes));
		$months[$ym]['benefit_notes'] = $notes;
	}

	// Render the list of months
	$rows = array();
	$count = 0;
	foreach ($months as $ym => $month) {
		$y = $month['y'];
		$m = $month['m'];

		// Get autotags for the month
		$my_tids = sirius_fieldvals($wym_nodes[$ym], 'field_sirius_contact_tags', 'tid');

		$row = array();

		// Column #1: Year/$month and FMLA/Disability status
		$tag_ids = array(
			'smf_autotag_status_disability' => 'Disability', 
			'smf_autotag_status_fmla' => 'FMLA'
		);
		$html = '<strong>' . $ym . '</strong>';
		if ($wym_nodes[$ym]) {
			$parts = array();
			foreach ($tag_ids as $tag_id => $tag_name) {
				$tid = $tags_by_id[$tag_id]->tid;
				if (in_array($tid, $my_tids)) {
					$parts[] = $tag_name;
				}
			}
			if ($parts) { $html .= '<br />' . '<strong>' . join(', ', $parts) . '</strong>'; }
		}
		$row[] = $html;

		// Column #2: Employers and hours, highest hours first
		$html = '';
		$month_hours = $months[$ym]['hours']['by_employer'];
		$total_hours = $months[$ym]['hours']['total'];
		if (!$total_hours) { $total_hours = 0; }

		$parts = array();
		if (!$month_hours) {
			$parts[]  = "[No hours found]";
		} else {
			foreach ($month_hours as $employer_nid => $amt) {
				$parts[$employer_nid] = $employers[$employer_nid] . ': ' . number_format(round($amt)) . ' hrs';
			}
		}
		$parts[] = '<strong>Total: ' . number_format(round($total_hours)) . ' hrs';
		//  . '(req. ' . join(',', $months[$ym]['hours']['elig']) . ')</strong>';
		$html = join('<br />', $parts);
		$row[] = $html;

		// Column #3: Plans and Elections
		$tag_ids = array(
			'smf_autotag_policy_8868661' => 'Legacy Plan', 
			'smf_autotag_policy_8868663' => 'Event Center', 
			'smf_autotag_policy_8868660' => 'Participation Agreement', 
			'smf_autotag_policy_8868662' => 'Hotel Plan', 

			'smf_autotag_election_type_1523' => 'Family',
			'smf_autotag_election_type_1521' => 'Single+1',
			'smf_autotag_election_type_1522' => 'Single',
			'smf_autotag_election_type_no_election' => 'No Election',
			'smf_autotag_election_type_no_election_type' => 'No Election Type',
			'smf_autotag_election_type_1556' => 'Election Waived',
		);
		$html = $ym;
		if (!$wym_nodes[$ym]) {
			$html = '<div class="sirius_missing">' . "[Autotags not set]" . '</div>';
		} else {
			$parts = array();
			foreach ($tag_ids as $tag_id => $tag_name) {
				$tid = $tags_by_id[$tag_id]->tid;
				if (in_array($tid, $my_tids)) {
					$parts[] = $tag_name;
				}
			}
			if (!$parts) {
				$html = '[No benefits]';
			} else {
				$html = join('<br />', $parts);
			}
		}
		$row[] = $html;

		// Column #4: Benefits
		$tag_ids = array(
			'smf_autotag_benefit_2457502' => 'Kaiser', 
			'smf_autotag_benefit_2457510' => 'Health Net', 
			'smf_autotag_benefit_8871542' => 'Kaiser E', 
			'smf_autotag_benefit_2457506' => 'MLK', 
			'smf_autotag_benefit_2457516' => 'EHS', 

			'smf_autotag_benefit_2457515' => 'Delta', 
			'smf_autotag_benefit_3113314' => 'Liberty', 
			'smf_autotag_benefit_2457514' => 'United Concordia', 
			'smf_autotag_benefit_7173079' => 'Placeholder Historical Dental Plan', 

			'smf_autotag_benefit_2457519' => 'VSP', 

			'smf_autotag_benefit_2457517' => 'Life Insurance / AD&D', 
		);
		$html = $ym;
		if (!$wym_nodes[$ym]) {
			$html = '<div class="sirius_missing">' . "[Autotags not set]" . '</div>';
		} else {
			$parts = array();
			foreach ($tag_ids as $tag_id => $tag_name) {
				$tid = $tags_by_id[$tag_id]->tid;
				if (in_array($tid, $my_tids)) {
					$parts[] = $tag_name;
				}
			}
			if (!$parts) {
				$parts[] = '[No benefits]';
			} 
		}

		$y3 = $y;
		$m3 = $m-3;
		if ($m3 < 1) { $m3 += 12; $y3--; }
		if (strlen($m3) < 2) { $m3 = "0$m3"; }
		$ym3 = "$y3-$m3";
		$total3 = $months[$ym3]['hours']['total'];
		if (!$total3) { $total3 = 0; }
		$parts[] = "Coverage From " . $ym3 . ": " . number_format(round($total3)) . ' hrs';
		if ($months[$ym]['benefit_notes']) {
			$parts[] = join('<br />', $months[$ym]['benefit_notes']);
		}
		$html = join('<br />', $parts);
		$row[] = $html;

		// Column #5: Tools
		$html = '';
		if ($wym_nodes[$ym]) {
			$html = l('&neArr;&nbsp;Details', 'sirius/smf/modal/wym/' . $wym_nodes[$ym]->nid, array('html' => TRUE, 'attributes' => array('class' => array('tool-button'))));
		}
		$row[] = $html;

		$rows[] = $row;

		++$count;
		$m--;
		if ($m < 1) { $m = 12; $y--; }
		if (strlen($m) < 2) { $m = "0$m"; }
		$ym = "$y-$m";
		if ($count > 100) { break; }
		if ($ym < $ym_first) { break; }
	}
	$header = array();

	$html = '<hr>';
	$html .= sirius_table_render($header, $rows);

	return $html;
}
