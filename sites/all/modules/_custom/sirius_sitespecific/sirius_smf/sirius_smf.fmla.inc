<?php

class Sirius_Smf_Fmla extends Sirius_Singleton {
	public function start_date($worker_nid, $employer_nid, $year = NULL, $month = NULL) {
		if (!$worker_nid) { return NULL; }
		if (!$employer_nid) { return NULL; }

		if (!$year) { $year = date('Y'); }
		if (!$month) { $month = date('n'); }
		$month += 0;

		$sql_params = array();
		$sql_params[':worker_nid'] = $worker_nid; 
		$sql_params[':employer_nid'] = $employer_nid;
		$sql_params[':year'] = $year;
		$sql_params[':month'] = $month;

		$sql = "select * from sirius_hours_cache ";
		$sql .= "where worker_nid = :worker_nid ";
		$sql .= "and employer_nid = :employer_nid ";
		$sql .= "and (year < :year or (year = :year and month < :month)) ";
		$sql .= "order by year desc, month desc, day desc ";

		$stmt = sirius_sql_query($sql, $sql_params);
		$date = '';
		while ($hr = $stmt->fetchAssoc()) {
		  $hour_type_term = taxonomy_term_load($hr['hours_type_tid']);
		  if (!$hour_type_term) { break; }
		  if ($hour_type_term->field_sirius_id['und'][0]['value'] != 'fmla') { break; }
		  $date = $hr['month'] . '/' . $hr['year'];
		}

		return $date;
	}
}