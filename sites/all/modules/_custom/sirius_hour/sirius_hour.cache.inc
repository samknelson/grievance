<?php

function sirius_hour_cache_set($payperiod_node) {
	// Clear the cache
	sirius_hour_cache_clear($payperiod_node);

	// If not final, don't write the cache
	if ($payperiod_node->field_sirius_active['und'][0]['value'] != 'Yes') { return; }

	// Get JSON data
	$data = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	if (!$data || !$data['hours']['daily']) { return; }

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

	// Get looks for hours type and departments
	$department_names = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	$hour_type_names = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');

	foreach ($data['hours']['daily'] as $date => $hours_by_type) {
		list ($year, $month, $day) = explode('-', $date);
		foreach ($hours_by_type as $hours_type_tid => $hours_by_department) {
			$hours_type_name = $hour_type_names[$hours_type_tid];
			foreach ($hours_by_department as $department_tid => $hours) {
				$department_name = $department_names[$department_tid];
				$total = $data['totals']['daily'][$date][$hours_type_tid][$department_tid];
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
						':hours' => $hours,
						':total' => $total,
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