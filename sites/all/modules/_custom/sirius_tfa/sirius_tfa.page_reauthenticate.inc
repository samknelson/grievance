<?php

function sirius_tfa_page_reauthenticate() {
	return drupal_get_form('sirius_tfa_page_reauthenticate_form');
}

function sirius_tfa_page_reauthenticate_form($form, &$form_state) {
	$form['password'] = array(
		'#type' => 'password',
		'#title' => t('Current Password'),
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_tfa_page_reauthenticate_form_submit($form, &$form_state) {
	require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

	$password = trim($form_state['values']['password']);
	if (strlen($password) == 0) {
		drupal_set_message("Please enter a password.", 'error');
		return;
	}

	global $user;
	if (!user_check_password($form_state['values']['password'], $user)) {
		drupal_set_message("Your current password is incorrect.", 'error');
		return;
	}

  if (!$user->uid) {
    drupal_set_message("User is not logged in.", 'error');
    drupal_goto('<front>');
  }

  sirius_tfa_reauthenticate_last(time());

  if ($_REQUEST['destination']) {
  	drupal_goto($_REQUEST['destination']); 
  } else {
  	drupal_set_message("Password correct. You have been re-authenticated for ten minutes.");
  }
}