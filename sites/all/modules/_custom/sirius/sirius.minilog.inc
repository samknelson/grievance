<?php

/**
 * Just a utility function for storing a TEMPORARY log array in a static variable.
 *
 * This is NOT a wrapper for watchdog, just a tool to accumulate log information about a particular transaction.
 */

function sirius_minilog($msg = NULL, $type = 'info') {
	static $log = array();
	if ($type == 'reset') { $log = array(); }

	static $indent = 0;
	if ($type == 'indent') { ++$indent; return; }
	if ($type == 'outdent') { --$indent; return; }

	if (!$msg) { return $log; }

	for ($i=0; $i<$indent; ++$i) { $msg = "... " . $msg; }
		
	$log_entry = array('ts' => time(), 'msg' => $msg, 'type' => $type);
	sirius_minilog_echo($log_entry);
	$log[] = $log_entry;
}

function sirius_minilog_reset() {
	return sirius_minilog(NULL, 'reset');
}

function sirius_minilog_indent() {
	return sirius_minilog(NULL, 'indent'); 
}

function sirius_minilog_outdent() {
	return sirius_minilog(NULL, 'outdent'); 
}

function sirius_minilog_render() {
	$log = sirius_minilog();
	$result = '';
	foreach ($log as $log_entry) {
		if (!$log_entry['type']) { $log_entry['type'] = 'status'; }
		$result .= date('c', $log_entry['ts']) . ': ' . $log_entry['type'] . ': ';
		$result .= wordwrap($log_entry['msg'], 120, "\n                           ", TRUE);
		$result .= "\n";
	}

	return $result;
}

function sirius_minilog_render_html() {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/minilog.css');

	$log = sirius_minilog();
	if (!$log) { return ''; }
	$result = '<div class="sirius_minilog">';
	$last_entry = $log[count($log)-1];

	if ($last_entry) {
		$result .= '<div class="sirius_minilog_punchline">';
		$result .= $last_entry['msg'];
		$result .= '</div>';
	}

	$ts_now = time();
	foreach ($log as $log_entry) {
		if (!$log_entry['type']) { $log_entry['type'] = 'status'; }
		$result .= '<div class="sirius_minilog_row">';
		$result .= '<strong>';

		$result .= date('h:i:s A ', $log_entry['ts']);
		if ($log_entry['type'] != 'info') {
			$result .= $log_entry['type'] . ': ';
		}
		$result .= '</strong>';
		$result .= $log_entry['msg'];
		$result .= '</div>';
	}
	$result .= '</div>';

	return $result;
}

function sirius_minilog_watchdog($type) {
	$log = sirius_minilog();
	foreach ($log as $log_entry) { watchdog($type, $log_entry['msg']); }
}

function sirius_minilog_echo_active($set_active = NULL) {
	static $active;
	if (isset($set_active)) { $active = TRUE; }
	return $active;
}

function sirius_minilog_echo($log_entry) {
	if (!sirius_minilog_echo_active()) { return; }

	if (!$log_entry['type']) { $log_entry['type'] = 'status'; }
	print $log_entry['type'] . ': ';
	print $log_entry['msg'];
	print "\n";
}