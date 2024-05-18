<?php

function sirius_worker_page_config_create() {
	return drupal_get_form('sirius_worker_page_config_create_form');
}

function sirius_worker_page_config_create_form($form, &$form_state) {
	$form['#tree'] = TRUE;
  $create_settings = sirius_worker_create_settings();

  $form['settings']['core_fields'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Core Fields',
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
	  $form['settings']['core_fields'][$field_name] = array(
	    '#type' => 'select',
	    '#title' => $field_title,
	    '#default_value' => $create_settings['core_fields'][$field_name],
	    '#options' => array(
	    	NULL => t('-- Select --'),
	    	'none' => t('Not Shown'),
	    	'optional' => t('Optional'),
	    	'required' => t('Required'),
	    	'admin_only' => t('Admins Only'),
	    ),
	  );
	}

  $form['settings']['employee_fields'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Employee Fields',
  	'#description' => t('Fields are shown when creating a worker associated with a specific employer.'),
  );

  $fields = array(
  	'field_sirius_date_start' => t('Employment Start Date'),
  	'field_sirius_id' => t('Employer ID'),
  );

  foreach ($fields as $field_name => $field_title) {
	  $form['settings']['employee_fields'][$field_name] = array(
	    '#type' => 'select',
	    '#title' => $field_title,
	    '#default_value' => $create_settings['employee_fields'][$field_name],
	    '#options' => array(
	    	NULL => t('-- Select --'),
	    	'none' => t('Not Shown'),
	    	'optional' => t('Optional'),
	    	'required' => t('Required'),
	    	'admin_only' => t('Admins Only'),
	    ),
	  );
	}

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_worker_page_config_create_form_submit($form, &$form_state) {
	sirius_worker_create_settings($form_state['values']['settings']);
}
