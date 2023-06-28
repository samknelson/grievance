<?php

function sirius_dispatch_elig_cache_cleanup($mode = 'test') {
	$count = 0;

	sirius_minilog('Cleaning up expired eligiblity cache entries');
	$cutoff = time() - 24*60*60;
	$sql = "delete from sirius_dispatch_elig_cache where ts < :cutoff";
	if ($mode == 'live') {
		$stmt = sirius_sql_query($sql, array(':cutoff' => $cutoff));
	}
	return array('success' => TRUE, 'msg' => "Deleted any expired eligibility cache entries.");
}

function sirius_dispatch_elig_cache_set($job_nid, $worker_nid, $plugin, $details) {
	$ts = time();
	if (!$job_nid || !$worker_nid || !$plugin) { return; }

	$sql = "insert ignore into sirius_dispatch_elig_cache (job_nid, worker_nid, plugin, details, ts) ";
	$sql .= "values (:job_nid, :worker_nid, :plugin, :details, :ts) ";
	$sql .= "on duplicate key update ts = :ts ";
	$sql_args = array();
	$sql_args[':job_nid'] = $job_nid;
	$sql_args[':worker_nid'] = $worker_nid;
	$sql_args[':plugin'] = $plugin;
	$sql_args[':details'] = $details;
	$sql_args[':ts'] = $ts;
	$stmt = sirius_sql_query($sql, $sql_args);
}

function sirius_dispatch_elig_cache_clear_job($job_nid) {
	$sql = "delete from sirius_dispatch_elig_cache where job_nid = :job_nid";
	$stmt = sirius_sql_query($sql, array(':job_nid' => $job_nid));
}