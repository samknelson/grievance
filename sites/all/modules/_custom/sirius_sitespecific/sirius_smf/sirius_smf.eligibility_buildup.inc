<?php

class Sirius_Smf_Eligibility_Plugin_Buildup extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'smf-buildup';
	public $name = 'SMF: Buildup';
	public $description = 'Hours buildup / nobreak eligibility -- standard';

	public function check($settings, $params) {
		$worker_node = $params['subscriber_worker_node'];
		$worker_nid = $worker_node->nid;

		if ($params['employer_node']) {
			$threshold = $this->threshold_from_worker_and_employer($worker_node, $params['employer_node']);
		} else if ($params['policy_node']) {
			$threshold = $this->threshold_from_worker_and_policy($worker_node, $params['policy_node']);
		} else {
			return array('success' => FALSE, 'msg' => 'No policy or employer specified, so we don\'t know the hours threshold.');
		}

		// "as of" means a two-month lag
		$asof_ts = $params['ts'];
		$asof_year = date('Y', $asof_ts);
		$asof_month = date('n', $asof_ts);
		$asof_ym = "$asof_year-$asof_month";

		// Benefit month is three months before current
		$threemonthsprev_month = $asof_month;
		$threemonthsprev_year = $asof_year;
		$threemonthsprev_month -= 3;
		if ($threemonthsprev_month < 1) { $threemonthsprev_year--; $threemonthsprev_month += 12; }
		$threemonthsprev_ym = "$threemonthsprev_year-$threemonthsprev_month";

		// Tools for our results
		$paths = array(
			"node/$worker_nid/sirius_worker_hours" => 'Hours Entries',
		);
		$tools = array();
		foreach ($paths as $path => $label) {
			if (drupal_valid_path($path)) {
				$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}

		// Get the first hours date
	  $sql = 'select year, month '; 
	  $sql .= 'from sirius_hours_cache ';
	  $sql .= "where worker_nid = :worker_nid ";
	  $sql .= "and ( ";
	  $sql .= "  (year < :year) ";
	  $sql .= "  or (year = :year and month <= :month) ";
	  $sql .= ") ";
	  $sql .= "order by year, month ";
	  
	  $sql_args = array(
	    ':worker_nid' => $worker_nid,
	    ':year' => $threemonthsprev_year,
	    ':month' => $threemonthsprev_month,
	  );

	  $stmt = sirius_sql_query($sql, $sql_args);
	  $hr = $stmt->fetchAssoc();
	  $first_year = $hr['year'];
	  $first_month = $hr['month'];
	  if (!$first_year || !$first_month) {
			return array(
				'success' => FALSE,
				'msg' => "No hours entries found prior to $year-$month (threshold=$threshold).",
				'tools' => $tools,
			);
	  }
	  $first_ym = $first_year . '-' . $first_month;

	  // Generate a list of ym's in descending order
    $avoid_infinite_loops = 0;
    $ym = $threemonthsprev_ym;
    $data = array();
    while (TRUE) {
      ++$avoid_infinite_loops; if ($avoid_infinite_loops > 10000) { break; }
	    list ($y, $m) = explode('-', $ym);
	    if ($y < $first_year || ($y == $first_year && $m < $first_month)) { break; }

    	$data[$ym] = array();

      --$m; 
      if ($m < 1) { $m = 12; --$y; }
      $ym = "$y-$m";
    }

		$break_count = 0;
		$buildup_count = 0;
		$incomplete_data = FALSE;
		$threemonthsprev_nonzero = FALSE;
		$threemonthsprev_elig = FALSE;

		$current_break_count = 0;
		$current_break_over = FALSE;

		$current_buildup_count = 0;
		$current_buildup_over = FALSE;

		$result = array();

		foreach ($data as $ym => $record) {
			list ($y, $m) = explode('-', $ym);

			$elig_hrs = $threshold;

			// Find the hours
			$sql = 'select sum(hours) as c '; 
			$sql .= 'from sirius_hours_cache ';
			$sql .= "where worker_nid = :worker_nid ";
			$sql .= "and year = :y ";
			$sql .= "and month = :m ";
			$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':y' => $y, ':m' => $m));
			$hr = $stmt->fetchAssoc();
			$hrs = $hr['c'];

			// Eligibility month
			if ($m == $threemonthsprev_month && $y == $threemonthsprev_year) {
				if ($hrs > 0) {
					$threemonthsprev_nonzero = TRUE;
				}
				if ($hrs >= $elig_hrs) { $threemonthsprev_elig = TRUE; }
			}

			// "Current Break"
			if ($hrs >= $elig_hrs) {
				$current_break_over = TRUE;
			} else if (!$current_break_over) {
				++$current_break_count;
			}

			// "Current Buildup"
			if ($hrs >= $elig_hrs) {
				if (!$current_buildup_over) { ++$current_buildup_count; }
			} else {
				$current_buildup_over = TRUE;
			}

			// Buildup is at least 150 hours in consecutive months
			if ($hrs >= $elig_hrs) {
				++$buildup_count;
			} else {
				$buildup_count = 0;
			}

			// Break is less than 150 hours in consecutive months
			if ($hrs < $elig_hrs) {
				++$break_count;
			} else {
				$break_count = 0;
			}

			// Buildup complete
			if ($buildup_count > 2) {
				$msg = "The most recent buildup (threshold=$threshold) was complete after starting in $ym.";
				if ($current_break_count) {
					$msg .= " (There have been $current_break_count subsequent months of low hours.)";
				} else {
					$msg .= " (There are no subsequent break months.)"; 
				}
				$result = array(
					'success' => TRUE,
					'msg' => $msg,
				);
				break;
			}

			// Break complete
			if ($break_count >= 12) {
				$result = array(
					'success' => FALSE,
					'msg' => "There was a break starting in $ym (hours $hrs < elig_hours $elig_hrs).",
				);
				break;
			}
		}

		if (!$result) {
			$result = array(
				'success' => FALSE,
				'msg' => "No successful buildup (threshold=$threshold) found prior to $threemonthsprev_ym.",
			);
		}

		$result['buildup_count'] = $buildup_count;
		$result['break_count'] = $break_count;
		$result['incomplete_data'] = $incomplete_data;
		$result['threemonthsprev_elig'] = $threemonthsprev_elig;
		$result['threemonthsprev_nonzero'] = $threemonthsprev_nonzero;
		$result['current_break_over'] = $current_break_over;
		$result['current_break_count'] = $current_break_count;
		$result['current_buildup_over'] = $current_buildup_over;
		$result['current_buildup_count'] = $current_buildup_count;
		$result['asof_month'] = $asof_month;
		$result['asof_year'] = $asof_year;
		$result['threemonthsprev_month'] = $threemonthsprev_month;
		$result['threemonthsprev_year'] = $threemonthsprev_year;
		$result['tools'] = $tools;

		// $warning_break_count = $settings['warning_break_count'];
		$warning_break_count = 10;
		if ($warning_break_count && ($current_break_count >= $warning_break_count)) {
			$result['warning'] = TRUE;
		}

		return $result;
	}

	public function industry_from_ms_tid($ms_tid) {
		$cache = &drupal_static(__FUNCTION__);
		if (!isset($cache)) { $cache = array(); }

		if (!isset($cache[$ms_tid])) {
			$ms_term = taxonomy_term_load($ms_tid);
			$cache[$ms_tid] = $ms_term->field_sirius_industry['und'][0]['tid'];
		}

		return $cache[$ms_tid];
	}

	public function threshold_from_ms_tid($ms_tid) {
		$cache = &drupal_static(__FUNCTION__);
		if (!isset($cache)) { $cache = array(); }

		if (!isset($cache[$ms_tid])) {
			$ms_term = taxonomy_term_load($ms_tid);
			$ms_json = sirius_json_get($ms_term);
			$val = $ms_json['smf']['threshold'];
			if (!($val > 0)) { $val = 100; }
			$cache[$ms_tid] = $val;
		}

		return $cache[$ms_tid];
	}

	public function threshold_from_worker_and_industry($worker_node, $industry_tid) {
		if (!$worker_node->field_sirius_member_status['und'][0]['tid']) { return 100; }

		foreach ($worker_node->field_sirius_member_status['und'] as $tmp) {
			$ms_tid = $tmp['tid'];
			if ($industry_tid != $this->industry_from_ms_tid($ms_tid)) { continue; }
			return $this->threshold_from_ms_tid($ms_tid);
		}

		return 100;
	}

	public function threshold_from_worker_and_employer($worker_node, $employer_node) {
		$industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
		return $this->threshold_from_worker_and_industry($worker_node, $industry_tid);
	}

	public function threshold_from_worker_and_policy($worker_node, $policy_node) {
		$policy_json = sirius_json_get($policy_node);
		$industry_tid = $policy_json['smf']['industry'];
		return $this->threshold_from_worker_and_industry($worker_node, $industry_tid);
	}
}
