<?php

function sirius_training_node_tab_worker_evaluations_request_access($worker_node) {
	// Evaluations must be enabled for this worker's member status
	if (!sirius_training_evaluations_enabled_by_worker($worker_node)) { return FALSE; }

	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}


function sirius_training_node_tab_worker_evaluations_request($worker_node) {
	return drupal_get_form('sirius_training_node_tab_worker_evaluations_request_form', $worker_node);
}

function sirius_training_node_tab_worker_evaluations_request_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name'),
		'#description' => t('Enter the name of the person who will be submitting your evaluation'),
		'#required' => TRUE,
	);

	$form['address'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone or Email'),
		'#description' => t('Enter the email address or the cell phone number of the person who will be submitting your evaluation. The recipient must have a system login and have permission to submit an evaluation on your behalf.'),
		'#required' => TRUE,
	);

	$form['comments'] = array(
		'#type' => 'textarea',
		'#title' => t('Comments'),
		'#description' => t('Enter any additional comments that you wish to include with your evaluation request.'),
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_node_tab_worker_evaluations_request_form_validate($form, &$form_state) {
	$address = $form_state['values']['address'];
	if (!sirius_email_validate($address) && !sirius_phone_validate($address)) {
		form_set_error('address', 'Please enter a valid email address or phone number.');
	}
}

function sirius_training_node_tab_worker_evaluations_request_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	// Add the worker node as a tokenized object
	$tokens = array();
	$token_aliases = array();
	$tokens['worker_node'] = $worker_node;
	$tokens['sirius']['request'] = $form_state['values'];
	$tokens['sirius']['response']['email'] = sirius_domain_variable_get('sirius_training_evaluations_email', '');

	global $base_url;
	$tokens['sirius']['response']['evaluate_url'] = $base_url . '/sirius/training/evaluation/' . $worker_node->field_sirius_id['und'][0]['value'];

	$token_aliases['worker_node'] = 'node';

	$email_subject = sirius_domain_variable_get('sirius_training_evaluations_request_email_subject', array());
	$email_body = sirius_domain_variable_get('sirius_training_evaluations_request_email_body', array())['value'];
	$sms = sirius_domain_variable_get('sirius_training_evaluations_request_sms', array());

	$email_subject = token_replace($email_subject, $tokens, array('clear' => FALSE, 'aliases' => $token_aliases));
	$email_body = token_replace($email_body, $tokens, array('clear' => FALSE, 'aliases' => $token_aliases));
	$sms = token_replace($sms, $tokens, array('clear' => FALSE, 'aliases' => $token_aliases));

	$name = $form_state['values']['name'];

	$params = array();
	$address = $form_state['values']['address'];
	if (sirius_email_validate($address)) {
		$email = sirius_email_format($address);
    $params['subject'] = $email_subject;
    $params['body'] = $email_body;
    $params['to'] = $email;
		$result = sirius_email_send($params);
		if ($result['success']) {
			sirius_log(
				'worker:training', 
				'evaluation_request', 
				"Evaluation request sent by email to $name ($email)",
				drupal_html_to_text("$email_subject / $email_body"),
				'',
				array($worker_node->nid)
			);
		}
	} else  if (sirius_phone_validate($address)) {
		$phone = sirius_phone_format($address);
    $params['sms'] = $sms;
		$params['to'] = $phone;
		$result = sirius_sms_send($params);
		if ($result['success']) {
				sirius_log(
				'worker:training', 
				'evaluation_request', 
				"Evaluation request sent by SMS to $name ($phone)",
				$sms,
				'',
				array($worker_node->nid)
			);
		}
	}

	if ($result['success']) {
		drupal_set_message("Your evaluation request has been sent. You may submit another request below.");

	} else {
		drupal_set_message("Message failed: " . $result['msg'], 'error');
		$form_state['rebuild'] = TRUE;
	}
}