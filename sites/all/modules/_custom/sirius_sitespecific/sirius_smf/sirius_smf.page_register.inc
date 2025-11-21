<?php

function sirius_smf_page_register() {
	return drupal_get_form('sirius_smf_page_register_form');
}

function sirius_smf_page_register_form($form, &$form_state) {
	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'Social Security Number',
		'#size' => 11,
		'#required' => TRUE,
	);

	$form['dob'] = array(
		'#type' => 'date_popup',
		'#title' => 'Date of Birth',
		'#required' => TRUE,
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'Y-m-d',
	);

	$form['last'] = array(
		'#type' => 'textfield',
		'#title' => 'Last Name',
		'#size' => 11,
		'#required' => TRUE,
	);

	$form['email'] = array(
		'#type' => 'textfield',
		'#title' => 'Email',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_page_register_form_validate($form, &$form_state) {
	$ssn = sirius_ssn_format($form_state['values']['ssn']);
	$email = sirius_email_format($form_state['values']['ssn']);

	if (!sirius_ssn_validate($ssn)) {
		form_set_error('ssn', 'Please enter a valid SSN.');
	} else {
		if (!sirius_flood_is_allowed('sirius_smf_register_ssn', $ssn)) {
			form_set_error('ssn', 'Too many registration attempts with this social security number. Please try again later.');
		}
	}

	$email = sirius_email_format($form_state['values']['email']);
	if (!sirius_email_validate($email)) {
		form_set_error('email', 'Please enter a valid email address.');
	} 

	if (!sirius_flood_is_allowed('sirius_smf_register_ip')) {
		form_set_error('ssn', 'Too many registration attempts from this address. Please try again later.');
	}
}

function sirius_smf_page_register_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	// Get our inputs
	$ssn = sirius_ssn_format($form_state['values']['ssn']);
	$dob = date('Y-m-d 00:00:00', strtotime($form_state['values']['dob']));
	$last = strtolower(trim($form_state['values']['last']));
	$email = sirius_email_format($form_state['values']['email']);

	// Register flood events
	sirius_flood_register_event('sirius_smf_register_ip');
	sirius_flood_register_event('sirius_smf_register_ssn', $ssn);

	// Check the email
	$probe = user_load_by_mail($email);
	if ($probe) {
		drupal_set_message('There is already a registered user with that email address.', 'error');
		return;
	}

	// Look up the worker
	$sql_args = array();
	$sql = "select nid from node ";
	// $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dob on field_data_field_sirius_dob.entity_type = 'node' and field_data_field_sirius_dob.entity_id = node.nid ";
	$sql .= "where field_sirius_ssn_value = :ssn ";
	$sql_args[':ssn'] = $ssn;
	$sql .= "and field_sirius_dob_value = :dob ";
	$sql_args[':dob'] = $dob;
	$sql .= "and lower(field_sirius_name_family) = :last ";
	$sql_args[':last'] = $last;
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	if (!$hr) {
		drupal_set_message("No member matches that input.", 'error');
		return;
	}
	$worker_node = node_load($hr['nid']);

	// Make sure there aren't duplicates
	$hr2 = $stmt->fetchAssoc();
	if ($hr2) {
		drupal_set_message("There are multiple members who match that input. Please contact the Trust office.", 'error');
		return;
	}

	// Make sure that the user isn't already sync'd.
	$probe = sirius_worker_get_user($worker_node->nid);
	if ($probe) {
		drupal_set_message("That member is already registered. Please sign in using the credentials on file.", 'error');
		return;
	}

	// Switch into the domain!
	sirius_domain($worker_node->field_sirius_domain['und'][0]['target_id']);

	// Make sure the worker has at least one hour entry
	$sql = "select sirius_hours_cache.*, field_data_field_sirius_attachments.entity_id as attachment_entity_id from sirius_hours_cache ";
	$sql .= "left join field_data_field_sirius_attachments on field_data_field_sirius_attachments.entity_type = 'node' and field_data_field_sirius_attachments.entity_id = sirius_hours_cache.payperiod_nid and field_data_field_sirius_attachments.delta = 0 ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
	$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid));
	$probe = $stmt->fetchAssoc();
	if (!$probe) {
		drupal_set_message("The member does not yet have any hours on file.", 'error');
		return;
	}

	unset($form_state['rebuild']);

	// Success
	$worker_node->field_sirius_email['und'][0]['value'] = $email;
	node_save($worker_node);
	$result = sirius_worker_usersync($worker_node->nid, array('manual' => TRUE));

	if ($result['success']) {
		drupal_set_message("User registration successful.");

		$result = sirius_smf_page_register_form_notify($worker_node);

		drupal_goto('smf/register/thanks');
	} else {
		drupal_set_message("Registration failed: " . $result['msg'], 'error');
	}
}

function sirius_smf_page_register_form_notify($worker_node) {
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { return array('succes' => FALSE, 'msg' => 'Contact node not found.'); }

	// Send a letter
	$sql_args = array();
	$sql = "select * from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shortname on field_data_field_grievance_shortname.entity_type = 'node' and field_data_field_grievance_shortname.entity_id = node.nid ";
	$sql .= "where 1 = 1 ";
	$sql .= "and node.type = 'grievance_letter_template' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = sirius_domain_nid();

	$sql .= "and field_grievance_shortname_value = :shortname ";
	$sql_args[':shortname'] = 'member_registration';

	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$letter_template_nid = $hr['nid'];
	if (!$letter_template_nid) {
		return array('success' => FALSE, 'msg' => 'No letter found with shortname: ' . $sql_args[':shortname']);
	}

	$letter_template_node = node_load($letter_template_nid);
	if (!$letter_template_node) {
		return array('success' => FALSE, 'msg' => 'No letter template found with nid: ' . $letter_template_nid);
	}

	$tokens = array();
	$tokens['worker_node'] = $worker_node;
	$tokens['contact_node'] = $contact_node;

	$aliases = array();
	$aliases['contact_node'] = 'node';
	$aliases['worker_node'] = 'node';

	$postal_client = sirius_postal_client_instance();
	$to = $postal_client->addr_from_node($worker_node);

	$tmp = $postal_client->mail_options_form();

	$params = array();
	$params['medium'] = 'postal';
	$params['handler_nids'] = array($worker_node->nid);
	$params['to'] = $to;

	// Replace tokens in our message
	$token_replace_options =  array();
	$token_replace_options['clear'] = TRUE;
	$token_replace_options['aliases'] = $aliases;

	$tmp = $letter_template_node->field_sirius_rawhtml['und'][0]['value'];
	if (!$tmp) { $tmp = $letter_template_node->field_grievance_notify_body['und'][0]['value']; }
	$params['body'] = token_replace($tmp, $tokens, $token_replace_options);

	$params['X-Sirius-Id'] = "UNIQUE/bulk/" . $worker_node->nid . '/' . md5(json_encode($to));
	$params['options'] = array('mail_type' => 'usps_standard');

	$result = sirius_postal_send($params);
	if ($result['success']) {
		drupal_set_message("A confirmation letter has been sent to the address on file.");
	}

	return $result;
}