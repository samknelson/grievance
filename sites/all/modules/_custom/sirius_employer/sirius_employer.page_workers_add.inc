<?php

function sirius_employer_node_tab_workers_add_form($form, &$form_state, $employer_node) {
	$access = Sirius_Worker_Access::getInstance();
	$is_staff = $access->access('staff');

	// Ensure that we have a stub job
	$industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
	if (!$industry_tid) {
		return sirius_form_error('There is no industry specified for this employer.');
	}
	$job_nid = sirius_dispatch_job_stub_fetch($employer_node->nid, $industry_tid, TRUE);
	if (!$job_nid) {
		return sirius_form_error('Please create a stub job for this employer.');
	}
	$form_state['job_nid'] = $job_nid;


	$form['#tree'] = TRUE;
	$form['#attributes'] = array('autocomplete' => 'none');

	$form_state['employer_nid'] = $employer_node->nid;

	$form['core_fields'] = array(
		'#title' => t('Worker'),
		'#type' => 'fieldset',
	);

	$form['core_fields']['field_sirius_name']['given'] = array(
		'#type' => 'textfield',
		'#title' => t('First Name'),
		'#required' => TRUE,
	);
	$form['core_fields']['field_sirius_name']['middle'] = array(
		'#type' => 'textfield',
		'#title' => t('Middle Name'),
		'#required' => FALSE,
	);
	$form['core_fields']['field_sirius_name']['family'] = array(
		'#type' => 'textfield',
		'#title' => t('Last Name'),
		'#required' => TRUE,
	);

	$form['core_fields']['field_sirius_dob'] = array(
	  '#type' => 'date_popup',
		'#title' => t('Date of Birth'),
		'#required' => TRUE,
	  '#date_format' => 'm/d/Y',
	);

	$form['core_fields']['field_sirius_ssn'] = array(
	  '#type' => 'textfield',
		'#title' => t('SSN'),
		'#required' => TRUE,
		'#size' => 12,
	);

	$form['core_fields']['field_sirius_phone'] = array(
	  '#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#required' => TRUE,
		'#size' => 12,
	);

	$form['core_fields']['field_sirius_email'] = array(
	  '#type' => 'textfield',
		'#title' => t('Email Address'),
		'#required' => FALSE,
		'#size' => 24,
	);

	$form['core_fields']['field_sirius_address']['thoroughfare'] = array(
	  '#type' => 'textfield',
		'#title' => t('Address 1'),
		'#required' => TRUE,
	);

	$form['core_fields']['field_sirius_address']['premise'] = array(
	  '#type' => 'textfield',
		'#title' => t('Address 2'),
		'#required' => FALSE,
	);

	$form['core_fields']['field_sirius_address']['locality'] = array(
	  '#type' => 'textfield',
		'#title' => t('City'),
		'#required' => TRUE,
	);

	$form['core_fields']['field_sirius_address']['administrative_area'] = array(
	  '#type' => 'textfield',
		'#title' => t('State'),
		'#required' => TRUE,
		'#size' => 2,
	);

	$form['core_fields']['field_sirius_address']['postal_code'] = array(
	  '#type' => 'textfield',
		'#title' => t('Zip'),
		'#required' => TRUE,
		'#size' => 5,
	);

	$form['employee_fields'] = array(
		'#title' => t('Employment'),
		'#type' => 'fieldset',
	);

	$form['employee_fields']['field_sirius_id'] = array(
	  '#type' => 'textfield',
		'#title' => t('EIN'),
		'#required' => TRUE,
		'#size' => 12,
	);

	$form['employee_fields']['start_date'] = array(
	  '#type' => 'date_popup',
		'#title' => t('Start Date'),
		'#required' => TRUE,
	  '#date_format' => 'm/d/Y',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_employer_node_tab_workers_add_form_validate($form, &$form_state) {
	$employer_nid = $form_state['employer_nid'];

	if (isset($form_state['values']['core_fields']['field_sirius_ssn'])) {
		$val = $form_state['values']['core_fields']['field_sirius_ssn'];
		if ($val && (!sirius_ssn_validate($val))) {
			form_set_error('core_fields][field_sirius_ssn', 'Please enter a valid SSN.');
		}
		$val = $form_state['values']['core_fields']['field_sirius_ssn'] = sirius_ssn_format($val);
		$probe = sirius_worker_get_by_ssn($val);
		if ($probe) {
			form_set_error('core_fields][field_sirius_ssn', 'There is already a worker with that SSN.');
		}
	}

	if (isset($form_state['values']['core_fields']['field_sirius_phone'])) {
		$val = $form_state['values']['core_fields']['field_sirius_phone'];
		if ($val && (!sirius_phone_validate($val))) {
			form_set_error('core_fields][field_sirius_phone', 'Please enter a valid phone number.');
		}
		$form_state['values']['core_fields']['field_sirius_phone'] = sirius_phone_format($val);
	}

	if (isset($form_state['values']['core_fields']['field_sirius_email'])) {
		$val = $form_state['values']['core_fields']['field_sirius_email'];
		if ($val && (!sirius_email_validate($val))) {
			form_set_error('core_fields][field_sirius_email', 'Please enter a valid email address.');
		}
		$form_state['values']['core_fields']['field_sirius_email'] = sirius_email_format($val);
	}

	// Validate for uniqueness: Core fields
	foreach (array('field_sirius_id', 'field_sirius_id2', 'field_sirius_id3', 'field_sirius_email', 'field_sirius_phone', 'field_sirius_ssn') as $field_name) {
		if (!isset($form_state['values']['core_fields'][$field_name])) { continue; }

		$val = trim($form_state['values']['core_fields'][$field_name]);
		$form_state['values']['core_fields'][$field_name] = $val;
		if (!$val) { continue; }

		$probe = sirius_worker_get_by_field($field_name, $val, TRUE);
		if ($probe) {
			// form_set_error("core_fields][$field_name", "There is already a worker with the value: [" . $val . "]");
		}
	}

	// Validate for uniqueness: EIN
	if (isset($form_state['values']['employee_fields']['field_sirius_id'])) {
		$val = trim($form_state['values']['employee_fields']['field_sirius_id']);
		$form_state['values']['employee_fields']['field_sirius_id'] = $val;
		$probe = sirius_worker_ein_lookup($employer_nid, $val);
		if ($probe) {
			// form_set_error("employee_fields][field_sirius_id", "There is already a worker with the EIN: [" . $val . "]");
		}
	}

	// Validate start date
	$ts = strtotime($form_state['values']['employee_fields']['start_date']);
	if ($ts === NULL || $ts > time() || $ts < strtotime('1975-01-01')) {
		form_set_error("employee_fields][start_date", "Please enter a date in the past.");
	}
}

function sirius_employer_node_tab_workers_add_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);

	// Create the worker node
  global $user;
  $worker_node = new stdClass();
  $worker_node->type = 'sirius_worker';
  $worker_node->language = LANGUAGE_NONE;
  $worker_node->uid = $user->uid;
  $worker_node->status = 1;
  $worker_node->promote = 0;
  $worker_node->comment = 0;

  // Set the domain
  $worker_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();

  foreach (array('field_sirius_id', 'field_sirius_id2', 'field_sirius_id3', 'field_sirius_email', 'field_sirius_phone', 'field_sirius_ssn') as $field_name) {
  	if (isset($form_state['values']['core_fields'][$field_name])) {
  		$worker_node->{$field_name}['und'][0]['value'] = $form_state['values']['core_fields'][$field_name];
  	}
  }

  foreach (array('given', 'middle', 'family') as $col) {
  	if (isset($form_state['values']['core_fields']['field_sirius_name'][$col])) {
  		$worker_node->{'field_sirius_name'}['und'][0][$col] = $form_state['values']['core_fields']['field_sirius_name'][$col];
  	}
  }

	if (isset($form_state['values']['core_fields']['field_sirius_dob'])) {
		$worker_node->{'field_sirius_dob'}['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($form_state['values']['core_fields']['field_sirius_dob']));
	}

  $has_address = FALSE;
  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $col) {
    if ($form_state['values']['core_fields']['field_sirius_address'][$col]) {
      $has_address = TRUE;
      $worker_node->field_sirius_address['und'][0][$col] = $form_state['values']['core_fields']['field_sirius_address'][$col];
    }
  }
  if ($has_address) {
    $worker_node->field_sirius_address['und'][0]['country'] = 'US';
  }

	node_save($worker_node);
  drupal_set_message("Worker " . $worker_node->title . " created.");


  // Assign the EIN
  $ssn = $form_state['values']['core_fields']['field_sirius_ssn'];
  $employer_nid = $form_state['employer_nid'];
  $ein = $form_state['values']['employee_fields']['field_sirius_id'];
  if ($ein) {
  	$result = sirius_worker_ein_set($ssn, $employer_nid, $ein);
  	if (!$result['success']) {
  		drupal_set_message("Unable to assign the EIN: " . $result['msg'], 'warning');
  	} else {
  		drupal_set_message("Assigned EIN [$ein]");
  	}
  }

  // Dispatch the worker
  $emphist_record = array();
  $emphist_record['worker_nid'] = $worker_node->nid;
  $emphist_record['job_nid'] = $form_state['job_nid'];
  $emphist_record['start'] = date('Y-m-d', strtotime($form_state['values']['employee_fields']['start_date']));
	$result = sirius_dispatch_emphist_save($emphist_record);
	if (!$result['success']) {
		drupal_set_message("Unable to dispatch this worker: " . $result['msg'], 'warning');
	} else {
		drupal_set_message("Dispatched the worker as of " . $form_state['values']['employee_fields']['start_date']);
	}

	$form_state['rebuild'] = TRUE;
}
