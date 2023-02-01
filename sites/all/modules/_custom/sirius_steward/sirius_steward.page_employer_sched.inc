<?php

function sirius_steward_node_tab_employer_sched_access($employer_node) {
	return sirius_steward_sched_access('view', $employer_node->nid);
}

function sirius_steward_node_tab_employer_sched($employer_node) {
	$stewards = sirius_steward_search(array('employer_nid' => $employer_node->nid));
	dpm($stewards);

	return 'Boo';
}
