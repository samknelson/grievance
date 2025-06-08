<?php

class Sirius_Smf_Eligibility_Plugin_Hours extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'smf-hours';
	public $name = 'SMF: Hours Threshold';
	public $description = 'Hours threshold achieved';

	public function check($settings, $params) {
		$worker_node = $params['subscriber_worker_node'];

		if ($params['employer_node']) {
			$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->threshold_from_worker_and_employer($worker_node, $params['employer_node']);
		} else if ($params['policy_node']) {
			$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->threshold_from_worker_and_policy($worker_node, $params['policy_node']);
		} else {
			return array('success' => FALSE, 'msg' => 'No policy or employer specified, so we don\'t know the hours threshold.');
		}

		$ts = $params['ts'];
		$date = gmdate('Y-m-d', $ts);
		$month_ts = strtotime("-4 months", $ts);

		$year = date('Y', $month_ts);
		$month = date('m', $month_ts);
		$start_day = 1;
		$end_day = date('t', $month_ts);

		$sql = 'select sum(hours) as c, hours_type_tid '; 
		$sql .= 'from sirius_hours_cache ';
		$sql .= "where worker_nid = :worker_nid ";
		$sql .= "and year = :year ";
		$sql .= "and month = :month ";
		$sql .= "and day >= :start_day ";
		$sql .= "and day <= :end_day ";
		$sql .= "group by hours_type_tid ";
		$sql_args = array(
			':worker_nid' => $worker_node->nid,
			':year' => $year,
			':month' => $month,
			':start_day' => $start_day,
			':end_day' => $end_day,
		);
		$stmt = sirius_sql_query($sql, $sql_args);

		while ($hr = $stmt->fetchAssoc()) {
			if ( ($hr['c'] > 0) && ($this->hours_type_tid_is_fmla($hr['hours_type_tid'])) ) {
				return array('success' => TRUE, 'msg' => "Received $hr[c] FMLA hours (ignoring the the threshold $threshold)");
			}
			$hours += $hr['c'];
		}

		if ($hours >= $threshold) {
			return array('success' => TRUE, 'msg' => "Hours worked in $year/$month was $hours, which is greater than the threshold $threshold");
		} else {
			return array('success' => FALSE, 'msg' => "Hours worked in $year/$month was $hours, which is less than the threshold $threshold");
		}
	}

	public function hours_type_tid_is_fmla($hours_type_tid) {
		$cache = &drupal_static(__FUNCTION__);
		if (!isset($cache)) { $cache = array(); }
		if (!isset($cache[$hours_type_tid])) {
			$hours_type_term = taxonomy_term_load($hours_type_tid);
			$hours_type_id = $hours_type_term->field_sirius_id['und'][0]['value'];
			$cache[$hours_type_tid] = ($hours_type_id == 'fmla');
		}

		return $cache[$hours_type_tid];
	}
}
