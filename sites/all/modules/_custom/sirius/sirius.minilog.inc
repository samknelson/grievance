<?php

/**
 * Just a utility function for storing a TEMPORARY log array in a static variable.
 *
 * This is NOT a wrapper for watchdog, just a tool to accumulate log information about a particular transaction.
 */

function sirius_minilog($msg = NULL, $type = 'info') {
	static $log = array();
	if ($type == 'reset') { $log = array(); }
	if (!$msg) { return $log; }
	$log[] = array('ts' => time(), 'msg' => $msg, 'type' => $type);
}

function sirius_minilog_render() {
	$log = sirius_minilog();
	$result = '';
	foreach ($log as $log_entry) {
		if (!$log_entry['type']) { $log_entry['type'] = 'status'; }
		$result .= date('c', $log_entry['ts']) . ': ' . $log_entry['type'] . ': ' . $log_entry['msg'] . "\n";
	}

	return $result;
}