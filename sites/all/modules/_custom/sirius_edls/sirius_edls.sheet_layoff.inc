<?php

function sirius_edls_sheet_layoff($sheet_node) {
	$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
	if (!$employer_node) { return array('success' => FALSE, 'msg' => 'Employer not found.'); }

	$employer_json = sirius_json_get($employer_node);
	$url = $employer_json['edls']['layoff']['url'];
	if (!$url) { return array('success' => FALSE, 'msg' => 'No URL is set for this employer.'); }

	$assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
	if (!$assignments) { return array('success' => FALSE, 'msg' => 'There are no workers on this sheet.'); }

	$worker_ids = array();
	foreach ($assignments as $crew_uuid => $crew_assignments) {
		foreach ($crew_assignments as $assignment) {
			$worker_ids[] = $assignment['worker_id'];
		}
	}

	$html = 'To lay off the workers on this sheet, click the link below.';
	$html .= '<br /><br />';
	$html .= '<strong>';
	$html .= "[<a href=\"$url?eins=" . join(',', $worker_ids) . "\" target=\"_blank\">";
	$html .= "Layoff Link";
	$html .= '</a>]';
	$html .= '</strong>';
	$html .= '<br /><br />';
	$html .= 'The link will open in a new window and you will be asked to confirm the layoffs.';

	return array('success' => TRUE, 'msg' => $html);
}