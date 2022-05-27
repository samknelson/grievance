<?php

function sirius_node_tab_event_intake_application_manage_access($application_node) {
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_intake_application_manage($application_node) {
	return drupal_get_form('sirius_node_tab_event_intake_application_manage_form', $application_node);
}

function sirius_node_tab_event_intake_application_manage_remove_format(&$form) {
  $fields = array('xprefix', 'xsuffix', 'markup', 'description');
  if ($form['accept']['notification']['email_body']['format']) {
    $form['accept']['notification']['email_body']['format']['#access'] = FALSE;
  }
  if ($form['reject']['notification']['email_body']['format']) {
    $form['reject']['notification']['email_body']['format']['#access'] = FALSE;
  }
  return $form;
} 

function sirius_node_tab_event_intake_application_manage_form($form, &$form_state, $application_node) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
	$form_state['application_nid'] = $application_node->nid;

	$form['#tree'] = TRUE;
  $form['#after_build'][] = 'sirius_node_tab_event_intake_application_manage_remove_format';

	$worker_node = sirius_log_handler($application_node, 'sirius_worker');
	$participant_node = sirius_log_handler($application_node, 'sirius_event_participant');
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);

	$application_status_options = sirius_event_intake_application_status_options();
	$application_status = $application_node->field_sirius_type['und'][0]['value'];

	$html = '';
	$html .= sirius_fakerender_field('Status', $application_status_options[$application_status]);
	if ($worker_node) {
		$html .= sirius_fakerender_field('Worker', l($worker_node->title . ' (' . $worker_node->field_sirius_id['und'][0]['value'] . ')', 'node/' . $worker_node->nid));
	}
	if ($event_node) {
		$html .= sirius_fakerender_field('Event', l($event_node->title, 'node/' . $event_node->nid . '/sirius_event_intake/applications'));
	}
	if ($participant_node) {
		$html .= sirius_fakerender_field('Participant', l($participant_node->title, 'node/' . $participant_node->nid . '/sirius_event_participant_comments'));
	}
	$assignment = sirius_event_intake_application_assignment_get($application_node);
	if ($assignment && $assignment['name']) {
		$html .= sirius_fakerender_field('Assigned To', $assignment['name'] . ' (' . date('l, F d Y - g:ia', $assignment['ts']) . ')');
	}

	$form['intro'] = array('#markup' => $html);

	$status = $application_node->field_sirius_type['und'][0]['value'];

	$actions = array(NULL => t('-- Select --'));

	$assignment_options = sirius_event_intake_application_assignment_options($event_node);
	if ($assignment_options) { 
		$actions['assign'] = t('Assign');
	}

	if ($status == 'draft' || $status == 'rejected') {
		$actions['submit'] = t('Submit');
	}

	if ($status == 'submitted') {
		$actions['accept'] = t('Accept');
		$actions['reject'] = t('Reject');
		$actions['draft'] = t('Reset to Draft');
	}

	/*
	if ($status == 'rejected') {
		$actions['reject'] = t('Reject');
	}
	*/

	if ($status == 'accepted') {
		$actions['resend'] = t('Resend Accepted Notification');
		$actions['complete'] = t('Mark application as "Complete"');
	}

	if (user_access('sirius admin') && $_REQUEST['test']) {
		if (!$actions['accept']) {
			$actions['accept'] = t('TESTING: Rerun the "accept" action');
		}
	}

	if ($actions) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => $actions,
			'#required' => TRUE,
		);
	  $form['assign'] = array(
	  	'#type' => 'select',
	  	'#title' => t('Assign To'),
	  	'#options' => array(NULL => t('-- Select --')) + $assignment_options,
	  	'#default_value' => $assignment['name'],
			'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'assign'))),
	  );


 		$id2_info = field_info_instance('node', 'field_sirius_id2', 'sirius_worker');
 		$id2_label = $id2_info['label'];
		$form['id2'] = array(
			'#type' => 'textfield',
			'#title' => $id2_label,
			'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'accept'))),
		);

		$status_options = array(NULL => t('-- Select --'));
		$signed_out_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
		$status_options[$signed_out_tid] = 'Not Available (SIGNED OUT)';
		$signed_in_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
		$status_options[$signed_in_tid] = 'Available (SIGNED IN)';

		if ($status_options) {
			$form['dispstatus_tid'] = array(
				'#type' => 'select',
				'#title' => t('Sign in?'),
				'#options' => $status_options,
				'#default_value' => $signed_in_tid,
				'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'accept'))),
			);
		}

	  $medium_options = array(
	    NULL => t('No notification'),
	    'email' => t('Notify by Email'),
	    'sms' => t('Notify by SMS'),
	    'both' => t('Notify by both Email and SMS'),
	  );

	  $form['accept']['notification'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => t('Notification'),
			'#states' => array('visible' => array(":input[name=\"action\"]" => array(array('value' => 'accept'), array('value' => 'resend')))),
	  );

	  $form['accept']['notification']['medium'] = array(
	    '#title' => t('Send a Notification?'),
	    '#type' => 'select',
	    '#options' => $medium_options,
	    '#default_value' => 'both',
	  );

	  $form['accept']['notification']['email_subject'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Email Subject'),
	    '#default_value' => $event_json['intake']['notifications']['accept']['email_subject'],
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"accept[notification][medium]\"]" => array(
	          array('value' => 'email'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

	  $form['accept']['notification']['email_body'] = array(
	    '#type' => 'text_format',
	    '#title' => 'Email Body',
	    '#default_value' => $event_json['intake']['notifications']['accept']['email_body'],
	    '#format' => 'sirius_form_editor',
	    '#allowed_formats' => array('sirius_form_editor'),
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"accept[notification][medium]\"]" => array(
	          array('value' => 'email'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

	  $form['accept']['notification']['sms'] = array(
	    '#type' => 'textarea',
	    '#title' => t('SMS'),
	    '#default_value' => $event_json['intake']['notifications']['accept']['sms'],
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"accept[notification][medium]\"]" => array(
	          array('value' => 'sms'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

	  $form['reject']['notification'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => t('Notification'),
			'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'reject'))),
	  );

	  $form['reject']['notification']['medium'] = array(
	    '#title' => t('Send a Notification?'),
	    '#type' => 'select',
	    '#options' => $medium_options,
	    '#default_value' => 'both',
	  );

	  $form['reject']['notification']['email_subject'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Email Subject'),
	    '#default_value' => $event_json['intake']['notifications']['reject']['email_subject'],
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"reject[notification][medium]\"]" => array(
	          array('value' => 'email'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

	  $form['reject']['notification']['email_body'] = array(
	    '#type' => 'text_format',
	    '#title' => 'Email Body',
	    '#default_value' => $event_json['intake']['notifications']['reject']['email_body'],
	    '#format' => 'sirius_form_editor',
	    '#allowed_formats' => array('sirius_form_editor'),
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"reject[notification][medium]\"]" => array(
	          array('value' => 'email'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

	  $form['reject']['notification']['sms'] = array(
	    '#type' => 'textarea',
	    '#title' => t('SMS'),
	    '#default_value' => $event_json['intake']['notifications']['reject']['sms'],
	    '#states' => array(
	      'visible' => array(
	        ":input[name=\"reject[notification][medium]\"]" => array(
	          array('value' => 'sms'), 
	          array('value' => 'both')
	        )
	      )
	    ),
	  );

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_node_tab_event_intake_application_manage_form_validate($form, &$form_state) {
	if ($form_state['values']['skip_validation'] == 'Yes') { return; }

	$application_node = node_load($form_state['application_nid']);
}

function sirius_node_tab_event_intake_application_manage_form_submit($form, &$form_state) {
	// @todo
	$form_state['rebuild'] = TRUE;

	$application_node = node_load($form_state['application_nid']);
	$application_json = sirius_json_get($application_node);

	$action = $form_state['values']['action'];

	$email = $application_json['wizard']['intake']['email'];
	$phone = $application_json['wizard']['intake']['phone'];

	if ($action == 'draft') {
		$application_node->field_sirius_type['und'][0]['value'] = 'draft';
		sirius_jsonfield_log($application_node, "status_set", "Changed status to draft");
		node_save($application_node);
		drupal_set_message("Status set to \"draft\".");
	}

	if ($action == 'submit') {
		$result = sirius_event_intake_application_submit($application_node);
		if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'warning'); }
	}

	if ($action == 'reject') {
		$application_node->field_sirius_type['und'][0]['value'] = 'rejected';
		sirius_jsonfield_log($application_node, "status_set", "Changed status to rejected.");
		drupal_set_message("Status set to \"rejected\".");

		if ($form_state['values']['reject']['notification']['medium']) {
			$result = sirius_event_notify(
				$application_node,
				$email,
				$phone,
				$form_state['values']['reject']['notification'], 
				array('application_node' => $application_node, 'event_node' => $event_node)
			);
		}
	}

	if ($action == 'accept') {
		$result = sirius_event_intake_application_accept($application_node, $form_state['values']);
		if ($result['success']) {
			drupal_set_message($result['msg']); 
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}

	if ($action == 'resend') {
		$worker_node = sirius_log_handler($application_node, 'sirius_worker');
		$participant_node = sirius_log_handler($application_node, 'sirius_event_participant');
		$event_node = sirius_log_handler($application_node, 'sirius_event');
		$result = sirius_event_notify(
			$application_node,
			$email,
			$phone,
			$form_state['values']['accept']['notification'], 
			array('application_node' => $application_node, 'worker_node' => $worker_node, 'participant_node' => $participant_node, 'event_node' => $event_node)
		);
	}

	if ($action == 'assign') {
		$assignment_name = $form_state['values']['assign'];
		$result = sirius_event_intake_application_assignment_set($application_node, $assignment_name);
		if ($result['success']) {
			drupal_set_message($result['msg']); 
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}

	if ($action == 'complete') {
		$result = sirius_event_intake_application_complete($application_node);
		if ($result['success']) {
			drupal_set_message($result['msg']); 
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}	
}
