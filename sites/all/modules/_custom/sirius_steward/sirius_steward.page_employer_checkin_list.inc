<?php

function sirius_steward_node_tab_employer_checkin_list_access($employer_node) {
	// At the moment, anybody can see the checkin list.
	return TRUE;
}

function sirius_steward_node_tab_employer_checkin_list($employer_node) {

	// Get the checkin records
	$checkin_nids = sirius_steward_checkin_search(
		array(
			'employer_nid' => $employer_node->nid,
			'status' => 'in',
		)
	);

	// Get the steward records
	$stewards_out = sirius_steward_search(
		array(
			'employer_nid' => $employer_node->nid,
			'checkin_status' => 'out',
		)
	);

	if (!$checkin_nids && !$stewards_out) {
		return '<p><em>' . t('There are no stewards at this facility.') . '</em></p>';
	}

	if ($checkin_nids) {
		$html = '<h2>' . t('Checked In') . '</h2>';
		$params = array();
		$params['hide_status'] = TRUE;
		$params['hide_in'] = TRUE;
		$params['hide_out'] = TRUE;
		$params['hide_employer'] = TRUE;
		if (!user_access('sirius steward admin')) { $params['hide_tools'] = TRUE; }
		$html .= sirius_steward_checkin_render_list($checkin_nids, $params);
	}

	if ($stewards_out) {
		$html .= '<h2>' . t('Not Checked In') . '</h2>';
		$html .= '<ul>';
		foreach ($stewards_out as $worker_nid => $worker_node) {
			$worker_node = node_load($worker_nid);
			$html .= '<li>';
			$html .= $worker_node->title;
			$html .= '</li>';
		}
		$html .= '</ul>';
	}

	return $html;
}
