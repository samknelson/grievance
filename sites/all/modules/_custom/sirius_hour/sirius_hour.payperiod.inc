<?php

/*
function sirius_hour_payperiod_definition() {
	$form = array();

	$form['intro'] = array('#markup' => '');

	return $form;
}
*/

function sirius_hour_payperiod_list_requested() {
	$html .= views_embed_view('sirius_payperiods', 'block_1');
	
	return $html;
}

function sirius_hour_payperiod_list_all() {
	$html .= '<a href="/node/add/sirius-payperiod?destination=sirius/hour/payperiod/all" class="tool-button">';
	$html .= t('Add a new Payperiod');
	$html .= '</a>';

	$html .= views_embed_view('sirius_payperiods', 'block');

	return $html;
}

function sirius_hour_payperiod_list_mine() {
	$worker_node = sirius_worker_get();
	if (!$worker_node) { return 'There is no worker associated with your user.'; }

	$html .= views_embed_view('sirius_payperiods', 'block_2', $worker_node->nid);

	return $html;
}

function sirius_hour_payperiod_sources($json) {
	$sources = array();
	if ($json['entries']) {
		foreach ($json['entries'] as $source_key => $entry) {
			$sources[$source_key] = $entry['source_name'];
		}
	}
	if (!$sources['manual']) { $sources['manual'] = 'Manual'; }
	return $sources;
}

function sirius_hour_payperiod_compute(&$json) {
	if (!$json['entries']) { $json['entries'] = array(); }
	$json['totals'] = array();

	// First pass: data cleanup, compute totals
	$live_count = 0;
	$live_source_key = '';
	$note = '';
	foreach ($json['entries'] as $source_key => $entry) {
		// Walk the rows
		$totals = array();
		if (!$entry['rows']) { $entry['rows'] = array(); }
		foreach ($entry['rows'] as $delta => $row) {
			// Remove empty or deleted rows
			if (!array_filter(array_values($row)) || $row['delete']) { 
				unset($json['entries'][$source_key]['rows'][$delta]);
				continue; 
			}

			$qty = $row['qty'];
			if (!is_numeric($qty)) { $qty = 0; }
			$department_tid = $row['department'];
			$hour_type_tid = $row['hour_type'];
			$day = $row['day'];
			if (!$department_tid || !$hour_type_tid || !$day) { 
				$json['entries'][$source_key]['rows'][$delta]['note'] = "Incomplete";
				continue;
			}

			$ms_level = $row['ms_level'];

			$totals['hours']['total'] += $qty;
			$totals['hours']['by_type'][$hour_type_tid] += $qty;
			$totals['hours']['by_dept'][$department_tid] += $qty;
			$totals['hours']['by_type_dept'][$hour_type_tid][$department_tid] += $qty;
			$totals['hours']['by_dept_type'][$department_tid][$hour_type_tid] += $qty;
			if ($ms_level) { $totals['hours']['by_ms_level'][$ms_level] += $qty; }

			$totals['hours']['by_day'][$day]['total'] += $qty;
			$totals['hours']['by_day'][$day]['by_type'][$hour_type_tid] += $qty;
			$totals['hours']['by_day'][$day]['by_dept'][$department_tid] += $qty;
			$totals['hours']['by_day'][$day]['by_type_dept'][$hour_type_tid][$department_tid] += $qty;
			$totals['hours']['by_day'][$day]['by_dept_type'][$department_tid][$hour_type_tid] += $qty;
		}
		$json['entries'][$source_key]['totals'] = $totals;
		if (!$entry['rows']) { continue; }

		// First live entry generates the actual totals.
		if ($entry['status'] == 'live') {
			++$live_count;
			$live_source_key = $source_key;
			$json['totals'] = $totals;
		}
	}

	// Set some warnings
	if ($live_count > 1) { $note .= "There are $live_count sets of entries marked as \"live\"."; }
	if ($live_count < 1) { $note .= "No entries are marked as \"live\"."; }

	$json['reconcile']['msg'] = $note;
	if ($note && $json['reconcile']['status'] != 'reconciled') { $json['reconcile']['status'] = 'warning'; }
}

function sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_nid, $year, $month, $day, $create_active = FALSE) {
	// Search for an existing payperiod node
	$date = "$year-$month-$day 00:00:00";
	$date_db = gmdate('Y-m-d H:i:s', strtotime($date));
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_payperiod');
  $query->fieldCondition('field_sirius_worker', 'target_id', $worker_nid);
  $query->fieldCondition('field_grievance_shop', 'target_id', $employer_nid);
  $query->fieldCondition('field_sirius_datetime', 'value', $date_db, '<=');
  $query->fieldCondition('field_sirius_datetime_completed', 'value', $date_db, '>');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if ($stmt['node']) { return node_load(array_keys($stmt['node'])[0]); }

  // Didn't find one. 
  if (!$create_active) { return NULL; }

  $payperiod_node = new stdClass();
  $payperiod_node->type = 'sirius_payperiod';
  $payperiod_node->no_recurse = TRUE;
  node_object_prepare($payperiod_node);
  $payperiod_node->language = LANGUAGE_NONE;
  $payperiod_node->uid = $user->uid;
  $payperiod_node->status = 1;
  $payperiod_node->promote = 0;
  $payperiod_node->comment = 0;
	$start_date = "$year-$month-1 00:00:00";
	$start_date_db = gmdate('Y-m-d H:i:s', strtotime($start_date));
  $payperiod_node->field_sirius_datetime['und'][0]['value'] = $start_date_db;
	$end_date = "$year-$month-15 23:59:00";
	$end_date_db = gmdate('Y-m-t 23:59:00', strtotime($end_date));
  $payperiod_node->field_sirius_datetime_completed['und'][0]['value'] = $end_date_db;
  $payperiod_node->field_sirius_active['und'][0]['value'] = $create_active;
  $payperiod_node->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
  $payperiod_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
  if ($domain_nid) { $payperiod_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
  $payperiod_node = node_submit($payperiod_node);
  node_object_prepare($payperiod_node);
  node_save($payperiod_node);

  // Create the employer payperiod as a side effect
  sirius_hour_epayperiod_load($domain_nid, $employer_nid, $year, $month, $day, $create_active);

  return $payperiod_node;
}