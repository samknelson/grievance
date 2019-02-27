<?php

function sirius_node_tab_worker_dispatch_status_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dispatch_status($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_dispatch_status_form', $worker_node);


}

function sirius_node_tab_worker_dispatch_status_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	// Show the current dispatch status, current employer, current dispatch
	$html = '';
	$current_status_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	$current_status_term = taxonomy_term_load($current_status_tid);
	$html .= t('Current dispatch status: ') . '<strong>' . $current_status_term->name . '</strong>';
	if ($current_status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$html .= '<br />' . t('Availability date: ') . '<strong>' . $worker_node->field_sirius_dispatch_availdate['und'][0]['value'] . '</strong>';
	}

	$employer_node = node_load($worker_node->field_grievance_shop['und'][0]['target_id']);
	if ($employer_node) {
		$html .= '<br />' . t('Current Employer: ') . '<strong>' . $employer_node->title . '</strong>';
	}

	$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
	if ($dispatch_node) {
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$html .= '<br />' . t('Current dispatch: ') . '<strong>' . $job_node->title . '</strong> (' . l('View', 'node/' . $dispatch_node->nid . '/sirius_dispatch_worker_view') . ')';
	}

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);

	// If we have permission to resign a dispatch, show a resign option
	$need_submit = FALSE;
	if ($dispatch_node && (user_access('sirius dispatch staff') || user_access('sirius dispatch resign'))) {
		$need_submit = TRUE;
		$form['resign'] = array(
			'#type' => 'select',
			'#title' => t('Resign this dispatch?'),
			'#options' => array(
				'no' => t('No, I do not wish to resign.'),
				'yes' => t('Yes, I wish to resign this dispatch.'),
			),
		);
	}

	// If we have permission to terminate a dispatch, show a termiante option
	if ($dispatch_node && (user_access('sirius dispatch staff'))) {
		$need_submit = TRUE;
		$form['terminate'] = array(
			'#type' => 'select',
			'#title' => t('Terminate this dispatch?'),
			'#options' => array(
				'no' => t('No, the employer does not wish to terminate this dispatch.'),
				'yes' => t('Yes, the employer wishes to terminate this dispatch.'),
			),
		);
	}

	// If we don't have a dispatch ...
	if (!$dispatch_node) {

		$status_default = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];

		if (user_access('sirius dispatch staff')) {
			// ... Staff can set the status to anything not-employed
			$status_options = array(
				sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_blocked') => t('Not Employed / Blocked (SIGNED OUT)'),
				sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available') => t('Not Employed / Not Available (SIGNED OUT)'),
				sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available') => t('Not Employed / Available (SIGNED IN)'),
			);
			
			$need_submit = TRUE;
			$form['status'] = array(
				'#type' => 'select',
				'#title' => t('Sign in?'),
				'#options' => $status_options,
				'#default_value' => $status_default,
			);
		} else if (
			user_access('sirius dispatch signin') && 
				(
					$status_default == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available') || 
					$status_default == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')
				)
			) {

			$status_options = array(
				sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available') => t('Not Employed / Not Available (SIGNED OUT)'),
				sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available') => t('Not Employed / Available (SIGNED IN)'),
			);
			
			$need_submit = TRUE;
			$form['status'] = array(
				'#type' => 'select',
				'#title' => t('Sign in?'),
				'#options' => $status_options,
				'#default_value' => $status_default,
			);
		}
	}

	if ($need_submit) {


		// Must confirm
		$form['confirm'] = array(
			'#type' => 'checkbox',
			'#required' => TRUE,
			'#title' => t('Yes, please perform the action selected above.'),
		);

		// Submit button
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit'
		);
	}

	$form += sirius_log_render_form($worker_node->nid, join('+', array('worker:employer', 'dispatch:status', 'worker:dispstatus', 'worker:availdate')));

	return $form;
}

function sirius_node_tab_worker_dispatch_status_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];

	if ($form_state['values']['resign'] == 'yes') {
		$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
		if ($dispatch_node) {
			drupal_set_message(sirius_dispatch_resign($dispatch_node));
		}
	}

	if ($form_state['values']['terminate'] == 'yes') {
		$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
		if ($dispatch_node) {
			drupal_set_message(sirius_dispatch_terminate($dispatch_node));
		}
	}

	if ($form_state['values']['status']) {
		drupal_set_message(sirius_dispatch_worker_dispstatus_set($worker_node, $form_state['values']['status']));
	}
}

