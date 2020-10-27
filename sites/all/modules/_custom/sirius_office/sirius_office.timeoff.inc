<?php

function sirius_office_node_access_timeoff_request($node, $op, $account) {
	// Timeoff admin can do anything
	if (user_access('sirius admin') || user_access('sirius office timeoff admin')) { return NODE_ACCESS_ALLOW; }

	// Timeoff request can create.
	if (user_access('sirius office timeoff request')) {
		if ($op == 'create') { return NODE_ACCESS_ALLOW; }
	}

	// Nobody else can do anything
	return NODE_ACCESS_DENY;;
}


function sirius_office_form_alter_timeoff_request_node_form(&$form, &$form_state, $form_id) {
	// Doesn't apply to admins
	if (user_access('sirius admin') || user_access('sirius office timeoff admin')) { return; }

	// Must have a worker
	$worker_node = sirius_worker_get();
	if (!$worker_node) { $form = array('#markup' => t('There is no worker associated with your user.')); return; }
	$form['field_sirius_worker']["und"][0]['target_id']['#default_value'] = $worker_node->title . ' (' . $worker_node->nid . ')';
	$form['field_sirius_worker']['#access'] = FALSE;

	// Force unapproved.
	$form['field_sirius_active']['und']['#default_value'] = 'No';
	$form['field_sirius_active']['#access'] = FALSE;
}

function sirius_office_page_timeoff_request() {
	if (user_access('sirius office timeoff admin')) {
		drupal_goto(current_path() . '/pending');
	} else {
		drupal_goto(current_path() . '/mine');
	}
}

function sirius_office_page_timeoff_request_list_pending() {
	$html .= '<a href="/node/add/sirius-office-timeoff-request?destination=' . current_path() . '" class="tool-button">';
	$html .= t('Add a new Timeoff Request');
	$html .= '</a>';

	$html .= views_embed_view('sirius_office_timeoff_requests', 'block_1');
	
	return $html;
}

function sirius_office_page_timeoff_request_list() {
	$html .= '<a href="/node/add/sirius-office-timeoff-request?destination=' . current_path() . '" class="tool-button">';
	$html .= t('Add a new Timeoff Request');
	$html .= '</a>';

	$html .= views_embed_view('sirius_office_timeoff_requests', 'block');

	return $html;
}

function sirius_office_page_timeoff_request_list_mine() {
	$worker_node = sirius_worker_get();
	if (!$worker_node) { return 'There is no worker associated with your user.'; }

	$html .= '<a href="/node/add/sirius-office-timeoff-request?destination=' . current_path() . '" class="tool-button">';
	$html .= t('Add a new Timeoff Request');
	$html .= '</a>';

	$html .= views_embed_view('sirius_office_timeoff_requests', 'block_2', $worker_node->nid);

	return $html;
}