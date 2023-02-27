<?php

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

function sirius_email_rfc822_format($name, $email) {
  $email = sirius_email_format($email);
  if (!sirius_email_validate($email)) { return NULL; }
  $name = preg_replace('/[^a-zA-Z0-9_ ]/', '', $name);
  if ($name) { $rfc822 = "$name <$email>"; } else { $rfc822 = $email; }
  return $rfc822;
}

function sirius_email_rfc822_parse($rfc822) {
  if (!function_exists('mailparse_rfc822_parse_addresses')) { return array(); }

  $error_reporting_orig = error_reporting();
  error_reporting(0);
  $records = mailparse_rfc822_parse_addresses($rfc822);
  error_reporting($error_reporting_orig);

  return $records;
}

function sirius_email_rfc822_validate($rfc822) {
  if (!function_exists('mailparse_rfc822_parse_addresses')) { return TRUE; }

  $records = sirius_email_rfc822_parse($rfc822);
  if (!$records) { return FALSE; }

  foreach ($records as $record) {
    if (!sirius_email_validate($record['address'])) {
      return FALSE;
    }
  }

  return TRUE;
}

function sirius_email_whitelist_validate($email) {
  $disable = variable_get("sirius_email_global_disable", '');
  if ($disable != 'Yes') { return TRUE; }
  $whitelist_patterns = explode("\n", variable_get("sirius_email_global_disable_whitelist", ''));

  foreach ($whitelist_patterns as $whitelist_pattern) {
    $whitelist_pattern = trim($whitelist_pattern);
    if (!$whitelist_pattern) { continue; }
    if (preg_match("/$whitelist_pattern/", $email)) { return TRUE; }
  }
  return FALSE;
}

function sirius_email_send_possible($params) {
  // Need a main recipient
  if (!$params['to']) { return array('success' => FALSE, 'msg' => 'There is no "to" email address.'); }

  // For each of the recipients....
  foreach (array('to', 'cc', 'Cc', 'bcc', 'Bcc', 'from', 'From', 'reply-to', 'Reply-To') as $key) {
    if ($key == 'to') { $rfc822 = $params[$key]; } else { $rfc822 = $params['headers'][$key]; }
    if (!$rfc822) { continue; }
    $records = sirius_email_rfc822_parse($rfc822);
    if (!$records) { continue; }

    foreach ($records as $record) {
      // Must be a valid email address
      if (!sirius_email_validate($record['address'])) { 
        return array('success' => FALSE, 'msg' => "The header for $key, $rfc822, does not look like a valid rfc822-formatted email address.");
      }

      // Must be whitelisted
      if (!sirius_email_whitelist_validate($record['address'])) { 
        return array('success' => FALSE, 'msg' => "Email is globally disabled and the header for $key, " . htmlspecialchars($rfc822) . ", does not match any of the whitelist patterns.");
      }
    }
  }

  // Allow callers to set the X-Sirius-Id, and don't resent with the same ID. This allows callers to set unique email keys.
  $id = $params['X-Sirius-Id'];
  if ($id) {
    if (sirius_email_already_sent($id)) {
      return array('success' => FALSE, 'msg' => "Skipping sending for message with id $id, because it's already been sent.");
    }
  }

  return array('success' => TRUE);
}


function sirius_email_send($params) {
  $result = sirius_email_send_possible($params);
  if (!$result['success']) { return $result; }

	$message = drupal_mail(
		'sirius', 
		'default', 
		$params['to'], 
		'und',
		$params 
	);

  // Don't log attachments or body
  unset($message['params']['attachments']);
  unset($message['body']);
  
  sirius_log(
    'email', 
    'sending', 
    $params['to'], 
    strip_tags($params['body']),
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
  if ($message['headers']['Cc']) { $message['headers']['Cc'] = preg_replace('/\s*,\s*/', ',', $message['headers']['Cc']); }
  if ($message['headers']['Bcc']) { $message['headers']['Bcc'] = preg_replace('/\s*,\s*/', ',', $message['headers']['Bcc']); }

  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];

  // Everything's HTML
  $message['headers']['Content-Type'] = 'text/html; charset=UTF-8;';

  // Reply-to is either set, or it's the current user.
  if (variable_get("sirius_email_replyto_address", '')) {
    $message['headers']['Reply-To'] = variable_get("sirius_email_replyto_name", '') . '<' . variable_get("sirius_email_replyto_address", '') . '>';
  } else if ($params['headers']['reply-to']) {
    $message['headers']['Reply-To'] = $params['headers']['reply-to'];
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

  // A hack. Because of something weird with UNITE HERE's AWS configuration
  // if (preg_match('/uhgrievance/', $from_email)) { unset($message['headers']['Reply-To']); }

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

function sirius_page_emailaddress_list() {
  return views_embed_view('sirius_emailaddresses', 'block_1');
}


function sirius_emailaddress_node_validate($node, $form, &$form_state) {
  if (!sirius_email_validate($node->field_sirius_id['und'][0]['value'])) {
   form_set_error('field_sirius_id', t('Please enter a valid phone number.'));
  }
}

function sirius_emailaddress_node_presave($node) {
  $node->field_sirius_id['und'][0]['value'] = sirius_email_format($node->field_sirius_id['und'][0]['value']);
  $node->title = $node->field_sirius_id['und'][0]['value'];
}

function sirius_emailaddress_lookup($emailaddress) {
  // Look up the node
  $domain_nid = sirius_domain_nid();
  $emailaddress = sirius_email_format($emailaddress);
  $sql_args = array(':emailaddress' => $emailaddress);
  $sql = "select field_data_field_sirius_id.entity_id from field_data_field_sirius_id ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
  $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
  $sql .= "and field_data_field_sirius_id.bundle = 'sirius_emailaddress' ";
  $sql .= "and field_data_field_sirius_id.field_sirius_id_value = :emailaddress ";

  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $nid = $hr['entity_id'];
  if ($nid) {
    $node = node_load($nid);
    sirius_minilog("Existing record found.");
  } else {
    global $user;
    $node = new stdClass();
    $node->type = 'sirius_emailaddress';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_sirius_id['und'][0]['value'] = $emailaddress;
    if ($domain_nid) { $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
    sirius_minilog("Existing record not found, creating a new one.");
  }

  return $node;
}

function sirius_email_render($email) {
  if (!$email) { return ''; }
  $email = htmlspecialchars($email);

  return "<a href=\"mailto:$email\">$email</a>";
}
