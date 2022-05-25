<?php

function report_cache_purge($report_type, $report_id) {
	$stmt = db_delete('report_cache');
	$stmt->condition('report_type', $report_type);
	$stmt->condition('report_id', $report_id);
	$count = $stmt->execute();
	sirius_minilog("$report_type: $report_id: Purged $count records");

	$stmt = db_delete('report_cache');
	$stmt->condition('report_type', $report_type . ' :: PARAMETERS');
	$stmt->condition('report_id', $report_id);
	$stmt->execute();
	sirius_minilog("$report_type: $report_id: Purged parameters");

	return $count;
}

function report_cache_purge_date($report_type, $start_ts = NULL, $end_ts = NULL, $suffix = NULL) {
	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
	if (!$start_ts) { $start_ts = strtotime('1970-1-1'); }
	if (!$end_ts) { $end_ts = strtotime('-30 days'); }

	sirius_minilog("$report_type: Purging old reports from $from_ts to $end_ts.");

	$sql = "select distinct report_id from report_cache where report_type = :report_type and report_domain_nid = :domain_nid";
	$stmt = db_query($sql, array(':report_type' => $report_type, ':domain_nid' => $domain_nid));
	while ($hr = $stmt->fetchAssoc()) {
		$report_id = $hr['report_id'];

		if ($suffix) {
			// if (!str_ends_with($report_id, $suffix)) { // PHP 8
			if (substr($report_id, -1 * strlen($suffix)) !== $suffix) {
				sirius_minilog("$report_type: Skipping $report_id (Doesn't end in $suffix).");
				continue;
			}
		}

		$report_id_clean = preg_replace('/ \- (.+)/', '', $report_id);

		$ts = strtotime($report_id_clean);
		if ($ts < 1) {
			sirius_minilog("$report_type: Skipping $report_id (couldn't parse the date).");
			continue;
		}

		if ($ts < $start_ts || $ts > $end_ts) {
			sirius_minilog("$report_type: Skipping $report_id (date not in range).");
			continue;
		}

		sirius_minilog("$report_type: $report_type - Purging $report_id.");
		report_cache_purge($report_type, $report_id);
	}
}
