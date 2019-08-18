<?php

/**
 * HFE = "Hold for Employer"
 */

function sirius_dispatch_hfe_get($worker_node) {
	$hfe = $worker_node->field_sirius_dispatch_hfe['und'][0]['target_id'];
	if (!$hfe) { return NULL; }
	$hfe_until = $worker_node->field_sirius_dispatch_hfe_until['und'][0]['value'];
	if (!$hfe_until) { return NULL; }
	$hfe_ts = strtotime($hfe_until);
	if ($hfe_ts < time()) { return NULL; }
	return array($hfe, $hfe_ts);
}

function sirius_dispatch_hfe_clear($worker_node) {
	unset($worker_node->field_sirius_dispatch_hfe['und']);
	unset($worker_node->field_sirius_dispatch_hfe_until['und']);
	node_save($worker_node);
	sirius_log('worker:hfe', 'clear', "Cleared HFE", '', '', array($worker_node->nid));
}

function sirius_dispatch_hfe_set($worker_node, $employer_node, $hfe_ts) {
	$worker_node->field_sirius_dispatch_hfe['und'][0]['target_id'] = $employer_node->nid;
	$worker_node->field_sirius_dispatch_hfe_until['und'][0]['value'] = date('Y-m-d H:i:s', $hfe_ts);
	node_save($worker_node);

	sirius_log('worker:hfe', 'set', "Set HFE for " . $employer_node->title . " until " . date('Y-m-d H:i', $hfe_ts), '', '', array($worker_node->nid));
}
