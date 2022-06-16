<?php

function sirius_node_tab_dispatch_manage($dispatch_node) {
	return drupal_get_form('sirius_node_tab_dispatch_manage_form', $dispatch_node);
}

function sirius_node_tab_dispatch_manage_form($form, &$form_state, $dispatch_node) {
	$form_state['dispatch_node'] = $dispatch_node;

	$renderable = node_view($dispatch_node, 'full');
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => drupal_render($renderable)
	);

	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);

	$actions = array(NULL => t('-- Select --'));

		/*
	$form['impossible'] = array(
		'#type' => 'fieldset',
		'#title' => 'Action Info',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
		*/

	$possible = sirius_dispatch_notify_possible($dispatch_node);
	if ($possible['success']) {
		$actions['notify'] 				= t('Send a notification');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['notify_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Notify not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_accept_possible($dispatch_node);
	if ($possible['success']) {
		$actions['accept'] 				= t('Accept this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['accept_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Accept not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_decline_possible($dispatch_node);
	if ($possible['success']) {
		$actions['decline'] 			= t('Decline this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['decline_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Decline not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_undecline_possible($dispatch_node);
	if ($possible['success']) {
		$actions['undecline'] 			= t('Un-Decline this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['undecline_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Un-Decline not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_terminate_possible($dispatch_node);
	if ($possible['success']) {
		$actions['terminate'] 		= t('Layoff this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['terminate_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Layoff not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_rollover_possible($dispatch_node);
	if ($possible['success']) {
		$actions['rollover'] 		= t('Rollover this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['rollover_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Rollover not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_resign_possible($dispatch_node);
	if ($possible['success']) {
		$actions['resign'] 			= t('Voluntary layoff this dispatch');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['resign_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'Voluntary layoff not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_vlri_possible($dispatch_node);
	if ($possible['success']) {
		$actions['vlri'] 			= t('Voluntary layoff this dispatch with a request to convert to involuntary');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['vlri_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'VLRI not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$possible = sirius_dispatch_vlri_resolve_possible($dispatch_node);
	if ($possible['success']) {
		$actions['vlri_resolve'] 			= t('Resolve layoff request');
	} else {
		if ($_REQUEST['debug']) {
			$form['impossible']['vlri_resolve_impossible'] = array('#markup' => '<div class="sirius_form_intro">' . 'VLRI Resolve not possible: ' . $possible['msg'] . '</div>');
		}
	}

	$actions['send'] 				  = t('Send a copy');

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => $actions,
		'#required' => TRUE,
	);

	$prefix = '<div class="sirius_field_intro">';
	$prefix .= "<p>Workers who are laid off within 24 hours automatically keep their senioriy. Select \"Yes\" to preserve seniority even if the layoff is more than 24 hours later than the report time.</p>";
	$availdate = $worker_node->field_sirius_dispatch_availdate['und'][0]['value'];
	if ($availdate) {
		$availdate = date('m/d/Y h:i a', strtotime($availdate . ' GMT'));
		$prefix .= '<p>' . t('The worker\'s <em>previous</em> seniority date was <strong>@date</strong>. ', array('@date' => $availdate)) . '</p>';
	}
	$prefix .= '</div>';

	$form['keep_seniority'] = array(
		'#title' => t('Keep Seniority?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => 'No',
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'terminate')),
				array(':input[name="action"]' => array('value' => 'resign')),
				array(':input[name="action"]' => array('value' => 'vlri')),
			)
		),
		'#field_prefix' => $prefix,
	);

	$medium_options = array(NULL => t('Use the worker\'s notification preference'));
	$medium_options += sirius_dispatch_notify_medium_options();
	$form['medium'] = array(
		'#title' => t('Notify by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][0]['value'],
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'notify')),
			)
		),
	);

	$send_medium_options = array(NULL => t('Use the worker\'s notification preference'));
	$send_medium_options += sirius_dispatch_send_medium_options($worker_node);
	$form['send_medium'] = array(
		'#title' => t('Send by:'),
		'#type' => 'select',
		'#options' => $send_medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][2]['value'],
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'send')),
			)
		),
	);

	$form['recipient'] = array(
		'#title' => t('Recipient:'),
		'#type' => 'textfield',
		'#description' => t('Leave blank to send to the worker.'),
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'send')),
			)
		),
	);

	$rollover_options = array(NULL => t('-- Select --'));
	$rollover_options += sirius_employer_list(TRUE, $employer_node->field_sirius_industry['und'][0]['tid']);
	$form['rollover_employer_nid'] = array(
		'#title' => t('Rollover To:'),
		'#type' => 'select',
		'#options' => $rollover_options,
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'rollover')),
			)
		),
	);

  $form['rollover_type'] = array(
    '#type' => 'select',
    '#title' => 'Rollover Type:',
    '#options' => array('permanent' => 'Permanent', 'temporary' => 'Temporary'),
    '#description' => t('Permanent rollovers will result in a layoff, whether or not the worker accepts the rollover. Temporary rollovers will not affect the worker\' current dispatch.'),
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'rollover')),
			)
		),
  );

  $form['rollover_end_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date:',
    '#states' => array(
      'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'rollover'),
					':input[name="rollover_type"]' => array('value' => 'temporary')
				),
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

	$form['vlri_resolve'] = array(
		'#title' => t('Resolve:'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --'), 'accept' => t('Accept'), 'reject' => t('Reject')),
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'vlri_resolve')),
			)
		),
	);


	$form['notes'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
		'#description' => t('Notes will be appended to the log, which may be visible to the worker. If you choose the action "send a copy", the notes will be included in the email or text message sent to the worker.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Go',
	);

  return $form;
}

function sirius_node_tab_dispatch_manage_form_submit($form, &$form_state) {
	$dispatch_node = $form_state['dispatch_node'];
	$action = $form_state['values']['action'];
	$notes = $form_state['values']['notes'];

	if ($form_state['values']['keep_seniority'] == 'Yes') {
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$json = sirius_json_get($worker_node);
		$json['dispatch']['keep_seniority'] = 'Yes';
		sirius_json_set($worker_node, $json);
		node_save($worker_node);
	}

	if ($action == 'notify') 				{ $result = sirius_dispatch_notify($dispatch_node, $form_state['values']['medium'], $notes); }
	if ($action == 'send') 					{ $result = sirius_dispatch_send($dispatch_node, $form_state['values']['send_medium'], $form_state['values']['recipient'], $notes); }
	if ($action == 'accept')				{ $result = sirius_dispatch_accept($dispatch_node, $notes); }
	if ($action == 'decline')				{ $result = sirius_dispatch_decline($dispatch_node, $notes); }
	if ($action == 'undecline')			{ $result = sirius_dispatch_undecline($dispatch_node, $notes); }
	if ($action == 'terminate')			{ $result = sirius_dispatch_terminate($dispatch_node, $notes); }
	if ($action == 'rollover')			{ $result = sirius_dispatch_rollover($dispatch_node, $form_state['values']['rollover_employer_nid'], $form_state['values']['rollover_type'], $form_state['values']['rollover_end_date'], $notes); }
	if ($action == 'resign')				{ $result = sirius_dispatch_resign($dispatch_node, $notes); }
	if ($action == 'vlri')					{ $result = sirius_dispatch_vlri($dispatch_node, $notes); }
	if ($action == 'vlri_resolve')	{ $result = sirius_dispatch_vlri_resolve($dispatch_node, $notes, $form_state['values']['vlri_resolve']); }

	if ($result['success']) {
		drupal_set_message("Success - " . $result['msg']);
	} else {
		drupal_set_message("Error - " . $result['msg'], 'error');
	}
}
