<?php

function sirius_node_tab_dispatch_bullpen_access($job_node) {
	if ($job_node->field_sirius_dispatch_job_type['und'][0]['tid'] != sirius_get_special_tid('sirius_dispatch_job_type|BULL')) { return FALSE; }
	// @todo: Who has access to this page?
	return TRUE;
}

function sirius_node_tab_dispatch_bullpen($job_node) {
	$renderable = node_view($job_node, 'full');
	$html .= drupal_render($renderable);
	$html .= '<hr>';


	$html .= 'IF I AM A WORKER, ';
	$html .= '<ul>';
	$html .= '<li>If I\'m EBA with the employer</li>';
	$html .= '<li>If the job status is "Unfilled"</li>';
	$html .= '<li>If the report date is in the future</li>';
	$html .= '</ul>';
	$html .= "then I can ";
	$html .= '<ul>';
	$html .= '<li>Sign in to the bullpen</li>';
	$html .= '<li>Sign out of the bullpen</li>';
	$html .= '</ul>';

	$html .= '<br><br><br>';

	$html .= 'IF I AM A SHOP STEWARD, ';
	$html .= '<ul>';
	$html .= '<li>If the status is Unfilled</li>';
	$html .= '<li>If the report date is today (one day only)</li>';
	$html .= '</ul>';
	$html .= 'then I can ';
	$html .= '<ul>';
	$html .= '<li>View the list of dispatches for this job (sorted by creation date)</li>';
	$html .= '<li>Add a new dispatch from the "out of work" list and the "eba" list, selecting by name and dispatch ID</li>';
	$html .= '</ul>';

	return $html;
}