<?php

function sirius_email_test_form($form, &$form_state) {
  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => 'To',
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Subject',
  );

  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Body',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_email_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $params = $form_state['values'];
  $result = sirius_email_send($params);
  $msg = $result['msg'];
  if ($result['success']) {
    drupal_set_message($msg);
  } else {
    drupal_setMessage($msg, 'error');
  }
}

function sirius_email_admin() {
	$form["sirius_email_default_from_address"] = array(
		'#type' => 'textfield',
		'#title' => t('Default FROM Email Address'),
		'#description' => t('Must be an address that has been validated as a sender on the system.'),
		'#default_value' => variable_get("sirius_email_default_from_address", ''),
	);

  $form["sirius_email_global_disable"] = array(
    '#type' => 'select',
    '#title' => t('Global Disable'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to globally disable email on this server.'),
    '#default_value' => variable_get("sirius_email_global_disable", ''),
  );

  $form["sirius_email_global_disable_whitelist"] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist'),
    '#description' => t('When global disable is on, use the whitelist to indicate which email address <strong>can</strong> receive emails. Enter patters one per line. Regular expressions are permitted, such as ".*@domain.com"'),
    '#default_value' => variable_get("sirius_email_global_disable_whitelist", ''),
    '#states' => array(
      'visible' => array(
        array(':input[name="sirius_email_global_disable"]' => array('value' => 'Yes')),
      )
    ),
  );

	$form["sirius_email_replyto_address"] = array(
		'#type' => 'textfield',
		'#title' => 'Global Reply-To Email',
		'#default_value' => variable_get("sirius_email_replyto_address", ''),
    '#description' => t('If set, ALL emails sent from the system will use this reply-to addres. If left blank, the reply-to address will be the name of the current user.')
	);

	$form["sirius_email_replyto_name"] = array(
		'#type' => 'textfield',
		'#title' => 'Global Reply-To Name',
		'#default_value' => variable_get("sirius_email_replyto_name", ''),
	);

  $form["sirius_ses_configuration_set"] = array(
    '#type' => 'textfield',
    '#title' => 'Amazon SES Configuration Set Name',
    '#default_value' => variable_get("sirius_ses_configuration_set", ''),
  );

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

/**
 * Implements hook_mail_alter
 */

function sirius_mail_alter(&$message) {
  // Set the "AWS SES Configuration Set"
  if (variable_get("sirius_ses_configuration_set", '')) {
    $message['headers']['X-SES-CONFIGURATION-SET'] = variable_get("sirius_ses_configuration_set", '');
  }

  // Are we in a global disable mode?
  $disable = variable_get("sirius_email_global_disable", '');
  if ($disable != 'Yes') { return; }

  if (stristr($message['to'], ",")) { // then we have a list
    $tos = explode(",", $message['to']);
  }
  else {
    $tos = (array) $message['to'];
  }

  $whitelist_patterns = explode("\n", variable_get("sirius_email_global_disable_whitelist", ''));

  foreach ($tos as $to) {
    $to = trim($to);

    // Is this recipient whitelisted?
    $whitelisted = FALSE;
    foreach ($whitelist_patterns as $whitelist_pattern) {
      $whitelist_pattern = trim($whitelist_pattern);
      if (!$whitelist_pattern) { continue; }
      if (!preg_match("/$whitelist_pattern/", $to)) { continue; }
      $whitelisted = TRUE;
      break;
    }

    // One failed recipient and we don't send.
    if (!$whitelisted) { break; }
  }

  // All were whitelisted.
  if ($whitelisted) { return; }

  // Oh well.
  $message['send'] = FALSE;
}


function sirius_email_send($params) {
  // Allow callers to set the X-Sirius-Id, and don't resent with the same ID. This allows callers to set unique email keys.
  $id = $params['X-Sirius-Id'];
  if ($id) {
    if (sirius_email_already_sent($id)) {
      return array('success' => FALSE, 'msg' => "Skipping sending for message with id $id, because it's already been sent.");
    }
  }

  if (!$params['to']) {
    return array('success' => FALSE, 'msg' => "No email address.");
  }

  $to = $params['to'];
  if (function_exists('mailparse_rfc822_parse_addresses')) {
    $to = mailparse_rfc822_parse_addresses($params['to']);
    if (!$to) {
      return array('success' => FALSE, 'msg' => $params['to'] . " does not look like a valid email address.");
    }
  }

	$message = drupal_mail(
		'sirius', 
		'default', 
		$params['to'], 
		'und',
		$params 
	);

  // Don't log attachments
  unset($message['params']['attachments']);
  
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

	return array('success' => TRUE, 'msg' => "Email sent to " . $params['to']);
}

/**
 * Implements hook_mail()
 */

function sirius_mail($key, &$message, $params) {
  global $user;

  // Pass params through to the message
  if (isset($params['attachments'])) { $message['params']['attachments'] = $params['attachments']; }
  
  if ($params['headers']['Cc']) { $message['headers']['Cc'] = $params['headers']['Cc']; } 
  if ($params['headers']['cc']) { $message['headers']['Cc'] = $params['headers']['cc']; } 
  if ($params['headers']['Bcc']) { $message['headers']['Bcc'] = $params['headers']['Bcc']; } 
  if ($params['headers']['bcc']) { $message['headers']['Bcc'] = $params['headers']['bcc']; } 

  // See comment from Takirimi https://api.drupal.org/api/drupal/includes%21mail.inc/function/drupal_mail/7.x
  $message['headers']['Cc'] = preg_replace('/\s*,\s*/', ',', $message['headers']['Cc']);
  $message['headers']['Bcc'] = preg_replace('/\s*,\s*/', ',', $message['headers']['Bcc']);

  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];

  // Everything's HTML
  $message['headers']['Content-Type'] = 'text/html; charset=UTF-8;';

  // Reply-to is either set, or it's the current user.
  if (variable_get("sirius_email_replyto_address", '')) {
    $message['headers']['Reply-To'] = variable_get("sirius_email_replyto_name", '') . '<' . variable_get("sirius_email_replyto_address", '') . '>';
  } else if ($params['reply-to']) {
    $message['headers']['Reply-To'] = $params['reply-to'];
  } else {
    $message['headers']['Reply-To'] = $user->mail;
  }

  // Tracking number
  if ($params['X-Sirius-Id']) {
    $id = $params['X-Sirius-Id'];
  } else {
    $id =  uuid_generate();
  }
  $message['headers']['X-Sirius-Id'] = $id;

  // "From" 
  $from_email = $params['from'];
  if (!$from_email) { $from_email = variable_get("sirius_email_default_from_address", ''); }

  // Custom header: 
  $message['from'] = 
    $message['params']['from'] = 
    $message['headers']['From'] = 
    $message['headers']['Sender'] = 
    $message['headers']['Return-Path'] = 
    $from_email;
}

function sirius_email_already_sent($id) {
  // SQL for speed
  $sql = "select * from node ";
  $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_notes_value = :id ";
  $sql .= "and field_sirius_category_value = 'email' ";
  $sql .= "and field_sirius_type_value = 'sending' ";
  $stmt = db_query($sql, array(':id' => $id));
  $hr = $stmt->fetchAssoc();
  if ($hr) { return TRUE; }
  return FALSE;
}

function sirius_email_callback($id, $type, $json) {
  if ($id) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'sirius_log');
    $query->fieldCondition('field_sirius_notes', 'value', $id);
    $query->fieldCondition('field_sirius_log_handler', 'target_id', 0, '>');
    $query->fieldCondition('field_sirius_category', 'value', 'email');
    $query->fieldCondition('field_sirius_type', 'value', 'sending');
    $query->propertyOrderBy('nid', 'desc');
    $stmt = $query->execute();
    if ($stmt['node']) {
      $previous_log_node = node_load(array_keys($stmt['node'])[0]);
      $handler_nids = sirius_fieldvals($previous_log_node, 'field_sirius_log_handler', 'target_id');
    }
  }

  $summary = '';
  if ($json->Message->delivery->recipients) {
    foreach ($json->Message->delivery->recipients as $email) {
      if ($summary) { $summary .= ' / '; }
      $summary .= $email;
    }
  }
  if ($json->Message->bounce->bouncedRecipients) {
    foreach ($json->Message->bounce->bouncedRecipients as $tmp) {
      if ($summary) { $summary .= ' / '; }
      $summary .= $tmp->emailAddress;
      $summary .= ': ';
      $summary .= $tmp->diagnosticCode;
    }
  }

  sirius_log(
    'email', 
    $type, 
    "Callback of type $type", 
    $summary,
    $id, 
    $handler_nids,
    NULL,
    $json
  );
}
