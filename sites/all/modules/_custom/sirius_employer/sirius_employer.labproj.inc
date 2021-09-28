<?php

function sirius_employer_labproj_is_detail_by_group($labproj_json) {
	if ($labproj_json['settings']['detail_by_group'] != 'Yes') { return FALSE; }
	if (!$labproj_json['settings']['job_groups']) { return FALSE; }
	if (!array_filter(array_values($labproj_json['settings']['job_groups']))) { return FALSE; }
	return TRUE; 
}

function sirius_employer_labproj_list($search_params) {
	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_log');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->fieldCondition('field_sirius_category', 'value', 'labproj');
	$query->fieldCondition('field_sirius_type', 'value', array('draft', 'active'), 'in');
	if ($search_params['employer_nid']) { $query->fieldCondition('field_sirius_log_handler', 'target_id', $search_params['employer_nid']); }
	if ($search_params['key']) { $query->fieldCondition('field_sirius_notes', 'value', $search_params['key']); }
	if ($search_params['yearmonth']) { $query->fieldCondition('field_sirius_notes', 'value', '%/' . $search_params['yearmonth'], 'like'); }
	$limit = $search_params['limit'];
	if (!$limit) { $limit = 1000; }
	$query->range(0, $limit);
	$query->propertyOrderBy('nid', 'desc');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }
	return array_keys($stmt['node']);
}

function sirius_employer_labproj_fetch($employer_nid, $year, $month, $create = FALSE) {
	$search_params = array();
	$search_params['employer_nid'] = $employer_nid;
	$search_params['key'] = sirius_employer_labproj_key($employer_nid, $year, $month);
	$labproj_nids = sirius_employer_labproj_list($search_params);
	$labproj_nid = array_shift($labproj_nids);
	$labproj_node = node_load($labproj_nid);
	if ($labproj_node) { return $labproj_node; }

	if (!$create) { return FALSE; }

	$labproj_node = sirius_log(
		'labproj', 
		'draft', 
		sirius_employer_labproj_title($employer_nid, $year, $month), 
  	'', 
  	sirius_employer_labproj_key($employer_nid, $year, $month), 
  	array($employer_nid), 
  	NULL,
  	array()
  );

  $labproj_node->title = sirius_employer_labproj_title($employer_nid, $year, $month);
  node_save($labproj_node);

  return $labproj_node;
}

function sirius_employer_labproj_title($employer_nid, $year, $month) {
	return 'Labor Projection - ' . sirius_node_title($employer_nid) . " - $year/$month"; 
}

function sirius_employer_labproj_key($employer_nid, $year, $month) { 
	return "$employer_nid/$year/$month";
}

function sirius_employer_labproj_get_yearmonth($labproj_node) {
	$key = $labproj_node->field_sirius_notes['und'][0]['value'];
	list ($employer_nid, $year, $month) = explode('/', $key);
	return array($year, $month);
}

function sirius_employer_labproj_get_status($labproj_node) {
	return $labproj_node->field_sirius_type['und'][0]['value'];
}

function sirius_employer_labproj_status_options() {
	return array('draft' => 'Draft', 'active' => 'Submitted');
}

function sirius_employer_labproj_validate($days) {
	foreach ($days as $day => $day_record) {
		if ($day_record['total'] && (
			!is_numeric($day_record['total']) || 
			($day_record['total'] < 0) || 
			(round($day_record['total']) != $day_record['total'])
		)) {
			return array('success' => FALSE, 'msg' => 'Please make sure that each total is a nonnegative integer.');
		}

		if ($day_record['groups']) {
			$group_total = 0;
			foreach ($day_record['groups'] as $group_nid => $value) {
				if ($value && (
					!is_numeric($value) || 
					($value < 0) || 
					(round($value) != $value)
					)) {
					return array('success' => FALSE, 'msg' => 'Please make sure that each entry is a nonnegative integer.');
				}
				if (is_numeric($value)) { $group_total += $value; }
			}
		}
		/*
		if ($group_total > $day_record['total']) {
			return array('success' => FALSE, 'msg' => 'Please make sure that the total is greater than the sum of the group totals.');
		}
		*/
	}
	return array('success' => TRUE);
}

function sirius_employer_labproj_edit($labproj_node, $days) {
	$result = sirius_employer_labproj_validate($days);
	if (!$result['success']) { return $result; }

 	$labproj_json = sirius_json_get($labproj_node);

	$log_node = sirius_log(
		'labproj', 
		'modified', 
		$labproj_node->title, 
  	'', 
  	$labproj_node->field_sirius_notes['und'][0]['value'], 
  	sirius_fieldvals($labproj_node, 'field_sirius_log_handler', 'target_id'), 
  	NULL,
		$labproj_json
  );

	$total = 0;
	$group_totals = array();

	$max = 0;
	$group_maxes = array();

	$is_detail_by_group = sirius_employer_labproj_is_detail_by_group($labproj_json);
	if ($is_detail_by_group) {
		foreach ($days as $day => $day_record) {
			$days[$day]['total'] = 0;
			if ($day_record['groups']) {
				foreach ($day_record['groups'] as $group_id => $value) {
					if ($value) {
						$days[$day]['total'] += $value;
					}
				}
			}
		}
	}

	foreach ($days as $day => $day_record) {
		if ($day_record['total']) { $total += $day_record['total']; }
		if ($day_record['total'] > $max) { $max = $day_record['total']; }
		if ($day_record['groups']) {
			foreach ($day_record['groups'] as $group_id => $value) {
				if ($value) { $group_totals[$group_id] += $value; }
				if ($value > $group_maxes[$group_id]) { $group_maxes[$group_id] = $value; }
			}
		}
	}
	$labproj_json['totals']['total'] = $total;
	$labproj_json['totals']['group_totals'] = $group_totals;
	$labproj_json['totals']['max'] = $max;
	$labproj_json['totals']['group_maxes'] = $group_maxes;

	$labproj_json['days'] = $days;

	sirius_json_set($labproj_node, $labproj_json);

	node_save($labproj_node);
	return array('success' => TRUE);
}

/**
 * We define a “delinquent” employer as one who has not created an employment forecast for the subsequent month 
 * (i.e. in August, an employer is delinquent if they do not have an employment foreacst for September.) 
 */

function sirius_employer_labproj_delinquent($year, $month) {
	$delinquent_year = date('Y');
	$delinquent_month = date('n');
	++$delinquent_month;
	if ($delinquent_month > 12) { $delinquent_month = 1; ++$delinquent_year; }

	if ($year < $delinquent_year || ($year == $delinquent_year && $month <= $delinquent_month)) { return TRUE; }
	return FALSE;
}