<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_emphist_cleanup.inc'); sirius_smf_oneoff_emphist_cleanup();" ; sleep 10; done

function sirius_smf_oneoff_emphist_cleanup() {
	sirius_domain(2457501);
	// variable_set('sirius_smf_oneoff_emphist_last_worker_nid', 0);

	$last_worker_nid = variable_get('sirius_smf_oneoff_emphist_last_worker_nid', 0);
	$sql = "select distinct worker_nid from sirius_hours_cache ";
	$sql .= "where worker_nid > :last_worker_nid ";
	$sql .= "and hours > 0 ";
	$sql .= "order by worker_nid ";

	$stmt = db_query($sql, array(':last_worker_nid' => $last_worker_nid));

	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['worker_nid'];
	}

	$limit = 100;
	$count = 0;
	$total = count($worker_nids);
	$ts_start = $ts_now = time();

	foreach ($worker_nids as $worker_nid) {
		if ($count) {
			$ts_elapsed = $ts_now - $ts_start;
			$ts_per_record = round($ts_elapsed / $count, 2);
			$ts_estimate = $ts_per_record * ($total - $count);
			$interval = format_interval($ts_estimate);
			$elapsed = format_interval($ts_elapsed);
		} else {
			$interval = "undefined";
		}

		print "Cleaning up: $worker_nid, #$count / $limit of $total. At $ts_per_record seconds per record, estimate $interval\n";
		++$count;
		if ($count > $limit) { break; }

		sirius_smf_emphist_from_hours($worker_nid);
		variable_set('sirius_smf_oneoff_emphist_last_worker_nid', $worker_nid);
		$ts_now = time();
	}
}