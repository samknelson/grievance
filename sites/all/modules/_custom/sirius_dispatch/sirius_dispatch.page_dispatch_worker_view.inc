<?php

function sirius_node_tab_dispatch_worker_view_access_details($dispatch_node) {
	// Access token gets us in
	if (sirius_aat_check($dispatch_node)) { return TRUE; }

	// Admins can get in
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// This user must be a worker
	global $user;
	if (!$user) { return FALSE; }
	if (!$user->uid) { return FALSE; }
	$user_full = user_load($user->uid);
	$worker_nid = $user_full->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return FALSE; }

	// The worker must be associated with this dispatch.
	if ($worker_nid == $dispatch_node->field_sirius_worker['und'][0]['target_id']) { return TRUE; }

	return FALSE;
}

function sirius_node_tab_dispatch_worker_view($dispatch_node) {
	// All users can "access" this page. But only certain users can access the details.

	if (sirius_node_tab_dispatch_worker_view_access_details($dispatch_node)) {
		return drupal_get_form('sirius_node_tab_dispatch_worker_view_form', $dispatch_node);
	} else {
		return sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node);
	}
}

function sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node) {
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);

	drupal_set_title("Dispatch Status: $status");

	$html = "This is a valid dispatch.<br /><br />";
	$html .= "Dispatch ID: <strong>" . $dispatch_node->nid . "</strong><br />";
	$html .= "Dispatch Status: <strong>$status</strong><br /><br />";
	/*
	$html .= '<br />';
	$html .= "Dispatch ID: <strong>" . $dispatch_node->nid . '</strong><br />';
	$html .= "Job Code: <strong>" . $job_node->field_sirius_id['und'][0]['value'] . '</strong><br />';
	$html .= "Worker ID: <strong>" . $worker_node->field_sirius_id['und'][0]['value'] . '</strong><br />';
	$html .= "Worker Name: <strong>" . $worker_node->title . '</strong><br />';
	$html .= '<br />';
	*/
	$html .= "To see the dispatch details, please " . l('sign in.', 'user/login', array('query' => array('destination' => current_path())));
	return $html;
}

function sirius_node_tab_dispatch_worker_view_form($form, &$form_state, $dispatch_node) {
	drupal_set_title("Dispatch #" . $dispatch_node->nid);
	$form_state['dispatch_node'] = $dispatch_node;
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$facility_node = node_load($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);

	$form['qr'] = array(
		'#type' => 'markup',
		'#markup' => sirius_qr_url(),
	);


	// Get our permission type. We might have multiple. We're a worker if we either are logged in, or if we have an access token.
	if (user_access('sirius dispatch employer')) { $is_employer = TRUE; }
	if (user_access('sirius dispatch staff')) { $is_staff = TRUE; }
	if (user_access('sirius dispatch worker')) { $is_worker = TRUE; }
	if (sirius_aat_check($dispatch_node)) { $is_worker = TRUE; }

	$actions = array(NULL => t('-- Select --'));
	if ($is_staff || $is_worker) {
		// @todo: Can only accept or decline if you are not UEB!
		if (sirius_dispatch_accept_possible($dispatch_node)['success']) { $actions['accept'] = t('Accept this dispatch'); }
		if (sirius_dispatch_decline_possible($dispatch_node)['success']) { $actions['decline'] = t('Decline this dispatch'); }
	}

	if ($is_staff || $is_employer) {
		if (sirius_dispatch_terminate_possible($dispatch_node)['success']) { $actions['terminate'] = t('Terminate this dispatch'); }
	}

	if ($is_staff || $is_worker) {
		if (sirius_dispatch_resign_possible($dispatch_node)['success']) { $actions['resign'] = t('Resign this dispatch'); }
	}

	if (count($actions) > 1) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => $actions,
			'#prefix' => '<div class="sirius_noprint">',
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
			'#suffix' => '</div>',
		);
	}

	$fields = array(
		'dispatch:field_sirius_dispatch_status',
		'worker:field_sirius_name',
		'worker:field_sirius_id',
		'worker:field_sirius_id2',
		'worker:field_sirius_ssn',
		'dispatch:field_sirius_dispatch_cbn',
		'dispatch:field_sirius_payrate',
		'job:field_grievance_shop',
		'job:field_sirius_id',
		'job:field_sirius_dispatch_job_type',
		'job:field_sirius_datetime',
		'job:fiel_sirius_dispatch_facility',
		'job:field_sirius_dispatch_hall',
		'job:field_sirius_address_notes',
		'job:field_sirius_notes',
		'job:field_sirius_address_parking',
	);

	$titles['worker:field_sirius_name'] = t('Worker Name');
	$titles['worker:field_sirius_id'] = t('Worker ID');
	$titles['dispatch:field_sirius_dispatch_status'] = t('Dispatch Status');
	$titles['job:field_sirius_dispatch_job_type'] = t('Job Type');
	$titles['worker:field_sirius_name'] = t('Worker Name');
	$titles['worker:field_sirius_name'] = t('Worker Name');

	$modes['worker:field_sirius_ssn'] = 'sirius_worker';

	$nodes = array(
		'job' => $job_node,
		'worker' => $worker_node,
		'facility' => $facility_node,
		'employer' => $employer_node,
		'dispatch' => $dispatch_node,
	);

	foreach ($fields as $field_id) {
		list ($which_node, $field_name) = explode(':', $field_id);
		$node = $nodes[$which_node];
		$mode = $modes[$field_id];
		if (!$mode) { $mode = 'full'; }
		$renderable = field_view_field('node', $node, $field_name, $mode);
		if ($titles[$field_id]) {
			$renderable['#title'] = $titles[$field_id];
		}
		$html .= render($renderable);
	}

	$form['body'] = array(
		'#type' => 'markup',
		'#markup' => $html,
	);

	return $form;
}

function sirius_node_tab_dispatch_worker_view_form_submit($form, &$form_state) {
	$dispatch_node = $form_state['dispatch_node'];
	$action = $form_state['values']['action'];
	if ($action == 'accept') { $result = sirius_dispatch_accept($dispatch_node); }
	if ($action == 'decline') { $result = sirius_dispatch_decline($dispatch_node); }
	if ($action == 'terminate') { $result = sirius_dispatch_terminate($dispatch_node); }
	if ($action == 'resign') { $result = sirius_dispatch_resign($dispatch_node); }

	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message(t('Error: ') . $result['msg'], 'error');
	}
}

