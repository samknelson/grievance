<?php

function sirius_hour_payperiod_definition() {
	$form = array();

	$form['intro'] = array('#markup' => '');

	return $form;
}

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

function sirius_hour_payperiod_compute(&$json) {
	$totals_payperiod = array();
	$totals_monthly = array();
	$totals_daily = array();

	$horus_payperiod = array();
	$hours_monthly = array();
	$hours_daily = array();

	if ($json['entries']) { 

		usort($json['entries'], function($a, $b) {
			return $a['start_ts'] <=> $b['start_ts'];
		});
		
		foreach ($json['entries'] as $entry) {
			$day = date('Y-m-d', $entry['start_ts']);
			$month = date('Y-m', $entry['start_ts']);
			$duration = round( ($entry['end_ts'] - $entry['start_ts']) / (60*60), 2);
			$rate = $entry['rate'];
			$hour_type_tid = $entry['hour_type_tid'];
			$department_tid = $entry['department_tid'];

			$hours_payperiod[$hour_type_tid][$department_tid] += $duration;
			$hours_monthly[$month][$hour_type_tid][$department_tid] += $duration;
			$hours_daily[$day][$hour_type_tid][$department_tid] += $duration;

			$totals_payperiod[$hour_type_tid][$department_tid] += $rate * $duration;
			$totals_monthly[$month][$hour_type_tid][$department_tid] += $rate * $duration;
			$totals_daily[$day][$hour_type_tid][$department_tid] += $rate * $duration;
		}
	}

	$json['hours']['payperiod'] = $hours_payperiod;
	$json['hours']['monthly'] = $hours_monthly;
	$json['hours']['daily'] = $hours_daily;

	$json['totals']['payperiod'] = $totals_payperiod;
	$json['totals']['monthly'] = $totals_monthly;
	$json['totals']['daily'] = $totals_daily;
}

function sirius_hour_payperiod_load($worker_nid, $employer_nid, $year, $month, $day, $create_active = FALSE) {
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
  $payperiod_node = node_submit($payperiod_node);
  node_object_prepare($payperiod_node);
  node_save($payperiod_node);
  return $payperiod_node;
}