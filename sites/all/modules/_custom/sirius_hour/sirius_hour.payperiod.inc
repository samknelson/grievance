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

	// Always need at least one sourc.
	if (!$sources) { $sources['manual'] = 'Manual'; }

	// Some sites always require a "manual" source for reconciliation.
	if (!$sources['manual'] && sirius_domain_component_enabled('hour:reconcile')) {
		$sources['manual'] = 'Manual';
	}

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
	$date_db = date('Y-m-d', strtotime($date));

	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start use index (sirius_date_start) on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_payperiod' ";

	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}

	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $worker_nid;

	$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	$sql_args[':employer_nid'] = $employer_nid;

  if ($day == 1) {
		$sql .= "and field_sirius_date_start_value = :date_db ";
		$sql_args[':date_db'] = $date_db;
	} else {
		$sql .= "and field_sirius_date_start_value <= :date_db ";
		$sql .= "and field_sirius_date_end_value >= :date_db ";
		$sql_args[':date_db'] = $date_db;
	}
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) { return node_load($hr['nid']); }

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
	$start_date = "$year-$month-01 00:00:00";
  $payperiod_node->field_sirius_date_start['und'][0]['value'] = $start_date_db;
	$end_date = "$year-$month-15 23:59:00";
	$end_date_db = gmdate('Y-m-t 00:00:00', strtotime($end_date));
  $payperiod_node->field_sirius_date_end['und'][0]['value'] = $end_date_db;
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
