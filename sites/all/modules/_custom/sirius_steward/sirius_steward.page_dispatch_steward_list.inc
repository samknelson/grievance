<?php

function sirius_dispatch_steward_list_access($dispatch_node) {
	// Must be able to view the details of the dispatch
	sirius_require('sirius_dispatch', 'sirius_dispatch.page_view.inc');
	if (!sirius_node_tab_dispatch_worker_view_access_details($dispatch_node)) { return FALSE; }

	// Must be an active dispatch
	$dispatch_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if (!in_array($dispatch_status, array('accepted', 'accepted_secondary'))) { return FALSE; }

	return TRUE;
}

function sirius_dispatch_steward_list($dispatch_node) {
	// Get the checkin records
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$checkin_nids = sirius_steward_facility_checkin_search(
		array(
			'employer_nid' => $employer_nid,
			'status' => 'in',
		)
	);
	if (!$checkin_nids) { 
		return '<p><em>' . t('There are no checked-in stewards at this time.') . '</em></p>';
	}

	return sirius_steward_facility_checkin_render_list(
		$checkin_nids, 
		array(
			'hide_employer' => TRUE,
			'hide_status' => TRUE, 
			'hide_in' => TRUE, 
			'hide_out' => TRUE
		)
	);
}
