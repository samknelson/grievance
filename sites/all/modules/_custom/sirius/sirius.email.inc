<?php

function sirius_email_admin() {
	$form["sirius_email_default_from_address"] = array(
		'#type' => 'textfield',
		'#title' => 'Default FROM Email Address',
		'#description' => t('Must be an address that has been validated as a sender on the system.'),
		'#default_value' => variable_get("sirius_email_default_from_address", ''),
	);

	/*
	$form["sirius_email_default_replyto_address"] = array(
		'#type' => 'textfield',
		'#title' => 'Default Reply-To Email Address',
		'#default_value' => variable_get("sirius_email_default_replyto_address", ''),
	);

	$form["sirius_email_default_replyto_name"] = array(
		'#type' => 'textfield',
		'#title' => 'Default Reply-To Email Address',
		'#default_value' => variable_get("sirius_email_default_replyto_name", ''),
	);
	*/

  return system_settings_form($form);
}

function sirius_email_format($email) {
	$email = trim($email);
	$email = strtolower($email);
	return $email;
}

function sirius_email_validate($email) {
	$email = sirius_email_format($email);
	if (filter_var($email, FILTER_VALIDATE_EMAIL)) { return TRUE; }
	return FALSE;
}

function sirius_email_send($params) {
	$message = drupal_mail(
		'sirius', 
		'default', 
		$params['to'], 
		'und',
		$params 
	);

  sirius_log(
    'email', 
    'sending', 
    $params['to'], 
    $params['body'],
    $message['headers']['X-Sirius-Id'], 
    $params['handler_nids'],
    NULL,
    json_encode($message)
  );

	return array('success' => TRUE, 'msg' => 'Email sent.');
}

/**
 * Implements hook_mail()
 */

function sirius_mail($key, &$message, $params) {
  global $user;

  // Pass params through to the message
  if (isset($params['attachments'])) { $message['params']['attachments'] = $params['attachments']; }
  if ($params['headers']['Cc']) { $message['headers']['Cc'] = $params['headers']['cc']; } 
  if ($params['headers']['Bcc']) { $message['headers']['Bcc'] = $params['headers']['bcc']; } 
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];

  // Everything's HTML
  $message['headers']['Content-Type'] = 'text/html; charset=UTF-8;';

  // Reply-to is either set, or it's the current user.
  if ($params['reply-to']) {
    $message['headers']['Reply-To'] = $params['reply-to'];
  } else {
    $message['headers']['Reply-To'] = $user->mail;
  }

  // Tracking number
  $id =  uuid_generate();
  $message['headers']['X-Sirius-Id'] = $id;

  // "From" 
  $from_email = $params['from'];
  if (!$from_email) {
	  // @TODO Put this into a configuration variable!!!!
	  $from_email = variable_get("sirius_email_default_from_address", '');
  }

  // Custom header: 

  $message['from'] = 
    $message['params']['from'] = 
    $message['headers']['From'] = 
    $message['headers']['Sender'] = 
    $message['headers']['Return-Path'] = 
    $from_email;
}


