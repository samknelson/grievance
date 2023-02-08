<?php

function sirius_steward_node_tab_worker_sched_access($worker_node) {
	// Must have an employer
	$employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);
	if (!$employer_node) { return FALSE; }

	return TRUE;
}

function sirius_steward_node_tab_worker_sched($worker_node) {
	$employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);
	sirius_require('sirius_steward', 'sirius_steward.page_employer_sched.inc');
	return drupal_get_form('sirius_steward_node_tab_employer_sched_form', $employer_node);
}