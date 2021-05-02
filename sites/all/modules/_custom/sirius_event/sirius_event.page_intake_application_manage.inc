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

	$form['intro'] = array('#markup' => $html);

	$status = $application_node->field_sirius_type['und'][0]['value'];

	$actions = array(NULL => t('-- Select --'));
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
	}

	if ($actions) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => $actions,
			'#required' => TRUE,
		);

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
	$application_node = node_load($form_state['application_nid']);
	$application_json = sirius_json_get($application_node);

	$action = $form_state['values']['action'];
	if ($action == 'accept') {
		$worker_node = sirius_log_handler($application_node, 'sirius_worker');
		if ($worker_node) {
			form_set_error('action', 'There is already a worker associated with this application.');
		}

		$ssn = $application_json['wizard']['intake']['identity']['ssn'];
		if (sirius_worker_get_by_ssn($ssn)) {
			form_set_error('action', 'There is an existing worker with the same SSN as this application.');
		}
	}

}

function sirius_node_tab_event_intake_application_manage_form_submit($form, &$form_state) {
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
		$application_node->field_sirius_type['und'][0]['value'] = 'submitted';
		sirius_jsonfield_log($application_node, "status_set", "Changed status to submitted");
		node_save($application_node);
		drupal_set_message("Status set to \"submitted\".");
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
		$application_json = sirius_json_get($application_node);

		$event_node = sirius_log_handler($application_node, 'sirius_event');
		$event_json = sirius_json_get($event_node);
		$event_nid = $event_node->nid;

		// ////////////////////////////////////
		// Create the worker
		// ////////////////////////////////////

	  $worker_node = new stdClass();
	  $worker_node->type = 'sirius_worker';
	  $worker_node->language = LANGUAGE_NONE;
	  $worker_node->uid = $user->uid;
	  $worker_node->status = 1;
	  $worker_node->promote = 0;
	  $worker_node->comment = 0;
	  $domain_nid = sirius_domain_nid();
	  if ($domain_nid) { $worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	  $worker_node->field_sirius_name['und'][0]['given'] = $application_json['wizard']['intake']['identity']['name_first']; 
	  $worker_node->field_sirius_name['und'][0]['family'] = $application_json['wizard']['intake']['identity']['name_last']; 
	  $worker_node->field_sirius_phone['und'][0]['value'] = $application_json['wizard']['intake']['phone'];
	  $worker_node->field_sirius_email['und'][0]['value'] = $application_json['wizard']['intake']['email'];
	  $worker_node->field_sirius_ssn['und'][0]['value'] = $application_json['wizard']['intake']['identity']['ssn'];

	  $addr = $application_json['wizard']['intake']['addr'];
	  $addr['country'] = 'US';
	  $worker_node->field_sirius_address['und'][0] = $addr;

	  node_object_prepare($worker_node);
	  $worker_node = node_submit($worker_node);
		node_save($worker_node);
	  drupal_set_message("Created worker: " . l($worker_node->title . ' (' . $worker_node->field_sirius_id['und'][0]['value'] . ')', 'node/' . $worker_node->nid));
	  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

		// ////////////////////////////////////
		// Set the work status
		// ////////////////////////////////////

	  $ws_tid = $event_json['intake']['work_status'];
	  if ($ws_tid) {
	  	$ws_term = taxonomy_term_load($ws_tid);
	  	$result = sirius_worker_work_status_set($worker_node, $ws_tid, 'Intake', array());
	  	if ($result['success']) {
	  		drupal_set_message("Set work status to: " . $ws_term->name);
	  	} else {
	  		drupal_set_message("Unable to set work status to " . $ws_term->name . ': ' . $result['msg'], 'warning');
	  	}
	  }

		// ////////////////////////////////////
		// Set the member status
		// ////////////////////////////////////

	  $ms_tid_level = $event_json['intake']['member_status_level'];
	  list ($ms_tid, $level_code) = explode('|', $ms_tid_level);
  	$ms_term = taxonomy_term_load($ms_tid);
	  if ($ms_tid) {
	  	$result = sirius_worker_member_status_set($worker_node, $ms_term, TRUE, 'intake');
	  	if ($result['success']) {
	  		drupal_set_message("Set member status to: " . $ms_term->name);
	  	} else {
	  		drupal_set_message("Unable to set member status to " . $ms_term->name . ': ' . $result['msg'], 'warning');
	  	}
	  }

		// ////////////////////////////////////
		// Set the level
		// ////////////////////////////////////

	  if ($level_code) {
	  	$level = sirius_training_levels_fetch_record($ms_tid, $level_code);
	  	$result = sirius_training_level_graduate($worker_node, $ms_term, $level, array());
	  	if ($result['success']) {
	  		drupal_set_message("Set level to: " . $level['title']);
	  	} else {
	  		drupal_set_message("Unable to set level to " . $level['title'] . ': ' . $result['msg'], 'warning');
	  	}
	  }

		// ////////////////////////////////////
		// Create the materials
		// ////////////////////////////////////


	  $required_attachments = $event_json['intake']['attachments']['required'];
	  if (!$required_attachments) { $required_attachments = array(); }
	  $required_attachments = array_filter(array_values($required_attachments));
	  $optional_attachments = $event_json['intake']['attachments']['optional'];
	  if (!$optional_attachments) { $optional_attachments = array(); }
	  $optional_attachments = array_filter(array_values($optional_attachments));

	  $attachment_tids = array_unique($required_attachments + $optional_attachments);
	  if ($attachment_tids) {
	  	$attachment_tids = array_filter(array_values($attachment_tids));
	  	foreach ($attachment_tids as $tid) {
	  		$attachment_term = taxonomy_term_load($tid);

	  		$attachment = $application_json['wizard']['intake']['attachments'][$tid];
	  		if ($attachment) {
	  			$attachment_json = array();
	  			$attachment_json['material']['title'] = $attachment_term->name;
	  			$attachment_json['material']['document_type_tid'] = $tid;
	  			$attachment_json['material']['summary'] = 'Uploaded with the intake wizard';

	  			$file =  (array) file_load($attachment['fid']);
	  			$file['display'] = TRUE;
					$material_node = sirius_log('material', $attachment_term->name, $attachment_json['material']['summary'], $attachment_json['material']['summary'], NULL, array($worker_node->nid), array($file), $attachment_json);
					drupal_set_message("Created attachment node: " . l($attachment_term->name, 'node/' . $material_node->nid));
	  		}
	  	}
	  }

		// ////////////////////////////////////
		// Register for the event
		// ////////////////////////////////////

		$paccess = $event_json['intake']['register']['paccess'];
		if ($paccess) {
			list ($prole_tid, $pstatus_tid) = explode('|', $paccess);
			$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
			if ($result['success']) {
				$participant_node = node_load($result['nid']);
				drupal_set_message("Created participant record: " . l($participant_node->title, 'node/' . $participant_node->nid));
			} else {
				drupal_set_message("Unable to create participant record: " . $result['msg'], 'warning');
			}
		}

		// ////////////////////////////////////
		// Update the application node
		// ////////////////////////////////////
		
	  $application_node->field_sirius_log_handler['und'][] = array('target_id' => $worker_node->nid);
	  if ($participant_node) { $application_node->field_sirius_log_handler['und'][] = array('target_id' => $participant_node->nid); }
		$application_node->field_sirius_type['und'][0]['value'] = 'accepted';
		sirius_jsonfield_log($application_node, "status_set", "Changed status from $old_status to $new_status");
		node_save($application_node);
		drupal_set_message("Status changed.");

		// ////////////////////////////////////
		// Send the notification
		// ////////////////////////////////////

		if ($form_state['values']['accept']['notification']['medium']) {
			$result = sirius_event_notify(
				$application_node,
				$email,
				$phone,
				$form_state['values']['accept']['notification'], 
				array('application_node' => $application_node, 'worker_node' => $worker_node, 'participant_node' => $participant_node, 'event_node' => $event_node)
			);
		}

		// ////////////////////////////////////
		// Create a log entry
		// ////////////////////////////////////

		sirius_log('event:intake_accepted', 'accepted', 'Intake application accepted', 'Intake application accepted', NULL, array($worker_node->nid), array(), array());
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
}
