<?php

/**
 * Implements hook_sirius_log_buckets_alter()
 */

function sirius_teamsters631_sirius_log_buckets_alter(&$buckets) {
	// The "tc:contact" log entries should appear in the Training Center log, but not in the main worker log.
	$buckets['worker_tc']['categories'][] = 'tc:contact';
	$buckets['worker_tc']['categories'][] = 'tc:comment';
	$buckets['worker']['categories_exclude'][] = 'tc:contact';
	$buckets['worker']['categories_exclude'][] = 'tc:comment';
	$buckets['worker']['categories_exclude'][] = 'worker:level';
}

/**
 * Implements hook_sirius_log_my_excluded_categories()
 */
function sirius_teamsters631_sirius_log_my_excluded_categories() {
	if (!user_access('sirius training staff')) {
		return array('tc:contact', 'tc:comment', 'worker:level');
	}
}