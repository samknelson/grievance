<?php

function sirius_hour_cache_set_worker($worker_nid) {
	sirius_hour_cache_clear_worker($worker_nid);

	$sql = 'select entity_id from field_data_field_sirius_worker ';
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_payperiod' ";
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));

	$payperiod_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$payperiod_nids[] = $hr['entity_id'];
	}
	$payperiod_nodes = node_load_multiple($payperiod_nids);
	foreach ($payperiod_nodes as $payperiod_nid => $payperiod_node) {
		sirius_hour_cache_set($payperiod_node);
	}
}

function sirius_hour_cache_clear_worker($worker_nid) {
	$sql = "delete from sirius_hours_cache where worker_nid = :worker_nid";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
}

function sirius_hour_cache_set($payperiod_node) {
	// Clear the cache
	sirius_hour_cache_clear($payperiod_node);

	// If not final, don't write the cache
	if ($payperiod_node->field_sirius_active['und'][0]['value'] != 'Yes') { return; }

	// Get JSON data
	$json = sirius_jsonfield_util_node_json_get($payperiod_node);

	$payperiod_nid = $payperiod_node->nid;
	$employer_nid = $payperiod_node->field_grievance_shop['und'][0]['target_id'];
	$worker_nid = $payperiod_node->field_sirius_worker['und'][0]['target_id'];

	// Get worker name
	$sql = "select title from node where nid = :worker_nid";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
	$hr = $stmt->fetchAssoc();
	$worker_name = $hr['title'];

	// Get employer name
	$sql = "select title from node where nid = :employer_nid";
	$stmt = db_query($sql, array(':employer_nid' => $employer_nid));
	$hr = $stmt->fetchAssoc();
	$employer_name = $hr['title'];

	// Get lookups for hours type and departments
	static $department_names;
	if (!$department_names) { $department_names = sirius_taxonomy_load('grievance_department', 'tid', 'name'); }
	static $hour_type_names;
	if (!$hour_type_names) { $hour_type_names = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name'); }

	if ($json['totals']['hours']['by_day']) {
		$payperiod_ts = sirius_fieldval($payperiod_node, 'field_sirius_date_start', 'ts');
		$month = gmdate('n', $payperiod_ts);
		$year = gmdate('Y', $payperiod_ts);
		foreach ($json['totals']['hours']['by_day'] as $day => $by_day) {
			foreach ($by_day['by_type_dept'] as $hours_type_tid => $by_dept) {
				$hours_type_name = $hour_type_names[$hours_type_tid];
				foreach ($by_dept as $department_tid => $qty) {
					// drupal_set_message("Records for: $payperiod_ts, $year-$month-$day / $worker_nid / $employer_nid / $hours_type_tid / $department_tid / $qty");
					$department_name = $department_names[$department_tid];
					$sql = 'insert into sirius_hours_cache (payperiod_nid, year, month, day, worker_nid, employer_nid, hours_type_tid, department_tid, hours, total, employer_name, worker_name, hours_type_name, department_name) ';
					$sql .= 'values (:payperiod_nid, :year, :month, :day, :worker_nid, :employer_nid, :hours_type_tid, :department_tid, :hours, :total, :employer_name, :worker_name, :hours_type_name, :department_name)';
					$stmt = db_query($sql, array(
							':payperiod_nid' => $payperiod_nid,
							':year' => $year,
							':month' => $month,
							':day' => $day,
							':worker_nid' => $worker_nid,
							':employer_nid' => $employer_nid,
							':hours_type_tid' => $hours_type_tid,
							':department_tid' => $department_tid,
							':hours' => $qty,
							':total' => 0,
							':employer_name' => $employer_name,
							':worker_name' => $worker_name,
							':hours_type_name' => $hours_type_name,
							':department_name' => $department_name,
						)
					);
				}
			}
		}
	}
}


function sirius_hour_cache_clear($payperiod_node) {
	// Delete and recreate the payperiod entries. 
	$sql = "delete from sirius_hours_cache where payperiod_nid = :payperiod_nid";
	$stmt = db_query($sql, array(':payperiod_nid' => $payperiod_node->nid));
}

/**
 * Params:
 * - start_year
 * - start_month
 * - start_day
 * - end_year
 * - end_month
 * - end_day
 * - hour_type ("all" to show breakout)
 * - employer ("all" to show breakout)
 * - department ("all" to show breakdown)
 * - 

function sirius_hours_cache_fetch($params) {

}

/**
 * @todo: Currently creating this table manually. Bad. Needs to be part of the install script.

create table sirius_hours_cache (
	payperiod_nid integer,
	year integer,
	month integer,
	day integer,
	worker_nid integer,
	employer_nid integer,
	hours_type_tid integer,
	department_tid integer,
	hours numeric(10,6),
	total numeric(10,2),
	worker_name varchar(255),
	employer_name varchar(255),
	hours_type_name varchar(255),
	department_name varchar(255)
);

create index sirius_hours_cache_idx01 on sirius_hours_cache (payperiod_nid);
create index sirius_hours_cache_idx02 on sirius_hours_cache (year, month, day);
create index sirius_hours_cache_idx03 on sirius_hours_cache (worker_nid);
create index sirius_hours_cache_idx04 on sirius_hours_cache (employer_nid);
create index sirius_hours_cache_idx05 on sirius_hours_cache (worker_nid, year, month, day);
*/