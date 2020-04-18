<?php

function sirius_node_tab_worker_dispatch_status_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dispatch_status($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_dispatch_status_form', $worker_node);
}

function sirius_node_tab_worker_dispatch_status_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	$html = '';

	// List current dispatches
	$html .= '<h2>' . t('Current Dispatches') . '</h2>';
	$html .= views_embed_view('sirius_worker_dispatches', 'block_1', $worker_node->nid);


	$html .= '<h2>Dispatch Status</h2>';
	$current_status_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	$current_status_term = taxonomy_term_load($current_status_tid);
	$html .= t('Current dispatch status: ') . '<strong>' . $current_status_term->name . '</strong>';
	$availdate = date('Y-m-d H:i:s', sirius_date_ts($worker_node, 'field_sirius_dispatch_availdate'));

	if ($current_status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$html .= '<br />' . t('Availability date: ') . "<strong>$availdate</strong>";
	}

	$employer_node = node_load($worker_node->field_grievance_shop['und'][0]['target_id']);
	if ($employer_node) {
		$html .= '<br />' . t('Current Employer: ') . '<strong>' . $employer_node->title . '</strong>';
	}

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);

	$need_submit = FALSE;
	$status_options = array();

	$tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
	$possible = sirius_dispatch_worker_dispstatus_set_possible($worker_node, $tid);
	if ($possible['success']) { $status_options[$tid] = 'Not Available (SIGNED OUT)'; } 

	$tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
	$possible = sirius_dispatch_worker_dispstatus_set_possible($worker_node, $tid);
	if ($possible['success']) { $status_options[$tid] = 'Available (SIGNED IN)'; }

	if ($status_options) {
		$status_default = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
		$need_submit = TRUE;
		$form['status'] = array(
			'#type' => 'select',
			'#title' => t('Sign in?'),
			'#options' => $status_options,
			'#default_value' => $status_default,
		);
	}

	if (user_access('sirius dispatch staff')) {
		$need_submit = TRUE;
		$prefix = '';
		if ($worker_node->field_sirius_dispatch_availdate['und'][0]['value']) {
			$prefix = '<div class="sirius_field_intro">';
			$prefix .= t('The worker\'s <em>previous</em> seniority date was @date. ', array('@date' => $availdate));
			if ($current_status_tid != sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
				$json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
				if ($json['dispatch']['keep_seniority'] == 'Yes') {
					$prefix .= t(' Because the "keep seniority" flag is set, if the sign in, they will keep their previous seniority.');
				} else {
					$prefix .= t(' By default, if they sign in now, their availability date will be reset to today\'s date and they will lose their seniority.');
				}
			}
			$prefix .= '</div>';
		}

		$form['set_availdate'] = array(
			'#type' => 'checkbox',
			'#title' => t('Manually set the worker\'s availability date'),
			'#prefix' => $prefix,
		);

		$form['availdate'] = array(
			'#type' => 'date_popup',
			'#title' => t('Available As Of'),
			'#states' => array(
				'visible' => array(
					array(':input[name="set_availdate"]' => array('checked' => TRUE)),
				)
			),
			'#timepicker' => 'timepicker',
			'#timepicker_options' => array(
				'rows' => 6,
				'minutes' => array(
				  'starts' => 0,
				  'ends' => 56,
				  'interval' => 5,
				),
			),
			'#attributes' => array(
				'autocomplete' => 'off',
			),
		);
	} 

	if (user_access('sirius dispatch staff')) {
		$options = sirius_taxonomy_load('sirius_dispatch_sib', 'tid', 'name', TRUE);
		$form['sib'] = array(
			'#type' => 'select',
			'#title' => t('Sign-In Blocked?'),
			'#options' => $options,
			'#default_value' => $worker_node->field_sirius_dispatch_sib_status['und'][0]['tid'],
		);

		$form['reason'] = array(
			'#type' => 'textarea',
			'#title' => t('Notes'),
		);
	} else {
		$sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term && $sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') {
			$form['sib'] = array('#markup' => '<div class="sirius_form_intro"><strong>Sign-in is not permitted - the administrator has placed a sign-in block. Please contact the office for more information.</strong></div>');
		}
	}

	if ($need_submit) {
		// Submit button
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit'
		);
	} 

	$form += sirius_log_render_form($worker_node->nid, join('+', array('worker:employer', 'dispatch:status', 'worker:dispstatus', 'worker:availdate', 'worker:dispstatus_block')));

	return $form;
}

function sirius_node_tab_worker_dispatch_status_form_validate($form, &$form_state) {
	$worker_node = $form_state['worker_node'];

	if (user_access('sirius dispatch staff')) {
		$reason = $form_state['values']['reason'];
		$block_tid = $form_state['values']['sib'];
		if (!$reason && ($block_tid != $worker_node->field_sirius_dispatch_sib_status['und'][0]['tid'])) {
			form_set_error('reason', 'If you are changing the sign-in block status, please enter a reason.');
		}
	}
}

function sirius_node_tab_worker_dispatch_status_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];
	$reason = $form_state['values']['reason'];

	if ($form_state['values']['status']) {
		$result = sirius_dispatch_worker_dispstatus_set($worker_node, $form_state['values']['status'], NULL, $reason);
		if ($result['success']) {
			drupal_set_message($result['msg']);
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}

	if ($form_state['values']['set_availdate'] && $form_state['values']['availdate']) {
		$newdate = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['availdate']));
		$worker_node->field_sirius_dispatch_availdate['und'][0]['value'] = $newdate;
		node_save($worker_node);
		sirius_log('worker:availdate', 'set', "Set worker's availability date to $newdate", $reason, '', array($worker_node->nid));
	}

	$block_tid = $form_state['values']['sib'];
	if ($block_tid != $worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']) {
		if ($block_tid) {
			$block_term = taxonomy_term_load($block_tid);
			sirius_dispatch_worker_dispstatus_block_set($worker_node, $block_tid, $reason);
			drupal_set_message("Sign-in block has been set to " . $block_term->name);
		} else {
			sirius_dispatch_worker_dispstatus_block_set($worker_node, NULL, $reason);
			drupal_set_message("Sign-in block has been cleared.");
		}
	}
}

