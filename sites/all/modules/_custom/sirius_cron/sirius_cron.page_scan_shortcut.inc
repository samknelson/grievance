<?php

function sirius_cron_page_shortcut($key) {
	$scan_node = sirius_cron_scan_get($key, FALSE);
	if (!$scan_node) { return sirius_page_error("That cron job hasn't been run yet."); }
	return drupal_goto('node/' . $scan_node->nid . '/sirius_cron_scan_info');
}