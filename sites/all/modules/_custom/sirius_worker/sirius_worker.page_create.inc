<?php

function sirius_worker_page_create($employer_nid) {
	$employer_node = node_load($employer_nid);

	$access = Sirius_Worker_Access::getInstance();

	$context = array('employer_node' => $employer_node);

	if (!$access->access('create worker', $context)) { return sirius_page_error("Access denied."); }

	return drupal_get_form('sirius_worker_page_create_form', $employer_node);
}

function sirius_worker_page_create_form($form, &$form_state, $employer_node) {
	$settings = sirius_worker_create_settings();
	if (!$settings) { return sirius_form_error("Worker creation has not been configured. Please go to " . l('the settings page', 'sirius/worker/config/create') . ' to configure this form.'); }

	$access = Sirius_Worker_Access::getInstance();
	$is_staff = $access->access('staff');

	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$form['core_fields'] = array(
		'#type' => 'fieldset',
	);

	$fields = array(
  	'field_sirius_name' => t('Name'),
  	'field_sirius_dob' => t('DOB'),
  	'field_sirius_ssn' => t('SSN'),
  	'field_sirius_id' => t('ID'),
  	'field_sirius_id2' => t('ID2'),
  	'field_sirius_id3' => t('ID3'),
  	'field_sirius_aat' => t('Password'),
  	'field_sirius_phone' => t('Phone'),
  	'field_sirius_email' => t('Email'),
  	'field_sirius_address' => t('Address'),
  );

  foreach ($fields as $field_name => $field_title) {
  	if (!$settings['core_fields'][$field_name]) { continue; }
  	if ($settings['core_fields'][$field_name] == 'none') { continue; }

  	if ($settings['fiecore_fieldslds'][$field_name] == 'admin_only' && !$is_staff) { continue; }

  	$required = ($settings['core_fields'][$field_name] == 'required');

  	if ($field_name == 'field_sirius_name') {
  		$form['core_fields'][$field_name]['given'] = array(
  			'#type' => 'textfield',
  			'#title' => t('First Name'),
  			'#required' => $required,
  		);
  		$form['core_fields'][$field_name]['middle'] = array(
  			'#type' => 'textfield',
  			'#title' => t('Middle Name'),
  			'#required' => FALSE,
  		);
  		$form['core_fields'][$field_name]['family'] = array(
  			'#type' => 'textfield',
  			'#title' => t('Last Name'),
  			'#required' => $required,
  		);
  	}
 
   	if ($field_name == 'field_sirius_dob') {
   		$form['core_fields'][$field_name]['value'] = array(
		    '#type' => 'date_popup',
  			'#title' => $field_title,
  			'#required' => $required,
		    '#attributes' => array(
		      'autocomplete' => 'off',
		    ),
		    '#date_format' => 'Y-m-d',
  		);
  	}

   	if (in_array($field_name, array('field_sirius_ssn', 'field_sirius_id', 'field_sirius_id2', 'field_sirius_id3', 'field_sirius_aat', 'field_sirius_phone', 'field_sirius_email', 'field_sirius_address'))) {
   		$form['core_fields'][$field_name]['value'] = array(
  			'#type' => 'textfield',
  			'#title' => $field_title,
  			'#required' => $required,
  		);
  	}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_page_create_form_validate($form, &$form_state) {
	if (isset($form_state['values']['core_fields']['field_sirius_ssn']['value'])) {
		$val = $form_state['values']['core_fields']['field_sirius_ssn']['value'];
		if ($val && (!sirius_ssn_validate($val))) {
			form_set_error('core_fields][field_sirius_ssn][value', 'Please enter a valid SSN.');
		} elseif ($val && sirius_worker_get_by_ssn($val)) {
			form_set_error('core_fields][field_sirius_ssn][value', 'That SSN is not unique.');
		}
		$form_state['values']['core_fields']['field_sirius_ssn']['value'] = sirius_ssn_format($val);
	}

	if (isset($form_state['values']['core_fields']['field_sirius_phone']['value'])) {
		$val = $form_state['values']['core_fields']['field_sirius_phone']['value'];
		if ($val && (!sirius_phone_validate($val))) {
			form_set_error('core_fields][field_sirius_phone][value', 'Please enter a valid phone number.');
		}
		$form_state['values']['core_fields']['field_sirius_phone']['value'] = sirius_phone_format($val);
	}

	if (isset($form_state['values']['core_fields']['field_sirius_email']['value'])) {
		if ($val && (!sirius_email_validate($val))) {
		$val = $form_state['values']['core_fields']['field_sirius_email']['value'];
			form_set_error('core_fields][field_sirius_email][value', 'Please enter a valid email address.');
		}
		$form_state['values']['core_fields']['field_sirius_email']['value'] = sirius_email_format($val);
	}
}

function sirius_worker_page_create_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$employer_node = node_load($form_state['employer_nid']);

	dpm($form_state['values']);
}