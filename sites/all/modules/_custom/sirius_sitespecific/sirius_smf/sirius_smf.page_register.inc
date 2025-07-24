<?php

function sirius_smf_page_register() {
	return drupal_get_form('sirius_smf_page_register_form');
}

function sirius_smf_page_register_form($form, &$form_state) {
	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'Social Securtity Number',
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

	// Success
	unset($form_state['rebuild']);
	$worker_node->field_sirius_email['und'][0]['value'] = $email;
	node_save($worker_node);
	$result = sirius_worker_usersync($worker_node->nid, array('manual' => TRUE));
	if ($result['success']) {
		drupal_set_message("User registration successful.");
		drupal_goto('smf/register/thanks');
	} else {
		drupal_set_message("Registration failed: " . $result['msg'], 'error');
	}

	drupal_set_message("Registration failed: [under development]", 'warning');
}