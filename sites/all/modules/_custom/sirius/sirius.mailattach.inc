<?php

function sirius_mailattach_admin($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_mailattach_mode');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#default_value' => variable_get($name, ''),
    '#options' => array(
      NULL => t('-- Select --'),
      'disabled' => t('Disabled'),
      'test' => t('Test'),
      'live' => t('Live')
    ),
    '#description' => t('In "Disabled" mode, no connection will be attempted. In "Test" mode, the system will poll the target mailbox, but will not make any changes. Attachments won\'t be downloaded, and confirmation emails won\'t be sent.'),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_user');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('IMAP User'),
    '#default_value' => variable_get($name, ''),
    '#description' => t("Enter the IMAP user to be used for mail attachment fetching."),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_pass');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('IMAP Password'),
    '#default_value' => variable_get($name, ''),
    '#description' => t("Enter the IMAP password to be used for mail attachment fetching."),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_imapstr');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('IMAP Connection String'),
    '#default_value' => variable_get($name, ''),
    '#description' => t("Enter the IMAP connection string to be used.  Here's an example: <strong>{mail.myorg.org:993/imap/ssl}INBOX</strong>"),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_bundles');
  $options = array(
    'grievance' => t('Grievance'),
    'sirius_worker' => t('Worker'),
    'grievance_shop' => t('Employer')
  );
  $form[$name] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Available Record Types'),
    '#default_value' => variable_get($name, array()),
    '#description' => t('Mailattach can attach messages to these content types.'),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_bundle_default');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + $options,
    '#title' => t('Default Record Type'),
    '#default_value' => variable_get($name, array()),
    '#description' => t('If the record type is not specified in the subject, assume this type'),
  );

  $name = sirius_domain_variable_name('sirius_mailattach_grievance_type');
  $options = array(NULL => t('-- Select --')) + sirius_taxonomy_load('grievance_document_types', 'tid', 'name');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Grievance Attachment Type'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('When an emailed attachment is attached to a grievance record, it should have this attachment type.'),
  );
 
  return system_settings_form($form);
}

function sirius_mailattach_page_test() {
  return drupal_get_form('sirius_mailattach_page_test_form');
}

function sirius_mailattach_page_test_form($form, &$form_state) {
  $addr = sirius_domain_variable_get('sirius_mailattach_user', '');
  $intro = "Mailattach address: <a href=\"mailto:$addr\">$addr</a><br /><br />";
  $form['intro'] = array('#markup' => $intro);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_mailattach_page_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $result = sirius_mailattach_fetch();
  if ($result['success']) {
    drupal_set_message($result['msg']); 
  } else {
    drupal_set_message($result['msg'], 'error');
  }
}

function sirius_mailattach_page_test_all() {
  return drupal_get_form('sirius_mailattach_page_test_all_form');
}

function sirius_mailattach_page_test_all_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_mailattach_page_test_all_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $result = sirius_mailattach_fetch_all();
  if ($result['success']) {
    drupal_set_message($result['msg']); 
  } else {
    drupal_set_message($result['msg'], 'error');
  }
}

function sirius_mailattach_fetch_all() {
  sirius_become_superuser(TRUE);
  $sql = "select nid, title from node ";
  $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_domain' ";
  $sql .= "and (field_sirius_active_value is null or field_sirius_active_value != 'No') ";
  $sql .= "order by node.title ";
  $stmt = sirius_sql_query($sql, array());
  $domains = array(-1 => 'ROOT DOMAIN');
  while ($hr = $stmt->fetchAssoc()) {
    $domains[$hr['nid']] = $hr['title'];
  }

  $my_domain_nid = sirius_domain_nid();
  $count = $count_success = $count_failure = 0;
  foreach ($domains as $domain_nid => $domain_name) {
    sirius_minilog("Domain $domain_name ($domain_nid): Starting");
    sirius_domain($domain_nid);
    $result = sirius_mailattach_fetch();
    ++$count;
    if ($result['success']) {
      sirius_minilog("Domain $domain_name ($domain_nid): SUCCESS [" . $result['msg'] . ']');
      ++$count_success;
    } else {
      sirius_minilog("Domain $domain_name ($domain_nid): FAILURE [" . $result['msg'] . ']');
      ++$count_failure;
    }
  }
  $msg = "Mailattach complete. Ran mailattach for $count domains with $count_success successes and $count_failure failures.";
  sirius_minilog($msg);
  sirius_domain($my_domain_nid);
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_mailattach_fetch() {
  if (!function_exists('mailparse_rfc822_parse_addresses')) {
    $msg = "The system is misconfigured. Please enstall the PECL mailparse extension.";
    sirius_minilog($msg, 'error');
    return array('success' => FALSE, 'msg' => $msg);
  }

  $mode = sirius_domain_variable_get('sirius_mailattach_mode');
  if ($mode != 'test' && $mode != 'live') {
    $msg = "Mailattach is disabled for this domain. Please set the \"mode\" to \"test\" or \"live\" in the mailattach settings.";
    sirius_minilog($msg, 'error');
    return array('success' => FALSE, 'msg' => $msg);
  }

  // Get our connection information.  If we're not configured, quit silently.
  $sirius_mailattach_user = sirius_domain_variable_get('sirius_mailattach_user', '');
  $sirius_mailattach_pass = sirius_domain_variable_get('sirius_mailattach_pass', '');
  $sirius_mailattach_imapstr = sirius_domain_variable_get('sirius_mailattach_imapstr', '');
  if (!$sirius_mailattach_user || !$sirius_mailattach_pass || !$sirius_mailattach_imapstr) {
    return;
  }
  sirius_minilog("Attempting connection to $sirius_mailattach_imapstr as $sirius_mailattach_user / ****");

  // Connect
  $mbox = imap_open($sirius_mailattach_imapstr, $sirius_mailattach_user, $sirius_mailattach_pass);
  if (!$mbox) { 
    $msg = "Unable to connect to $sirius_mailattach_imapstr as $sirius_mailattach_user: " . imap_last_error();
    sirius_minilog($msg, 'error');
    return array('success' => FALSE, 'msg' => $msg);
  }
  sirius_minilog("Connection successful to $sirius_mailattach_imapstr as $sirius_mailattach_user / ****");

  // Get the number of messages
  $MC = imap_check($mbox);
  if (!$MC->Nmsgs) {
    $msg = "There are no messages in the mailbox.";
    sirius_minilog($msg);
    return array('success' => TRUE, 'msg' => $msg);
  }

  // Get the list of messages
  $messages = imap_fetch_overview($mbox,"1:{$MC->Nmsgs}",0);
  if (!$messages) {
    $msg = "No messages found.";
    sirius_minilog($msg);
    return array('success' => TRUE, 'msg' => $msg);
  }

  // For each message
  $count = $count_accept = $count_reject = 0;
  foreach ($messages as $message) {
    ++$count;
    $subject = imap_utf8($message->subject);
    sirius_minilog('Processing message - From: ' . htmlspecialchars($message->from) . ' / Subject: ' . htmlspecialchars($subject));

    if ($mode != 'live') {
      sirius_minilog("(operating in test mode, so moving to the next message without processing.)");
      continue;
    }

    //
    // Hunt for an autoreply header. There doesn't seem to be a better way to do this.
    // See https://www.arp242.net/autoreply.html
    // 

    $header_string = imap_fetchheader($mbox, $message->msgno);
    preg_match_all('/([^: ]+): (.+?(?:\r\n\s(?:.+?))*)\r\n/m', $header_string, $matches);
    $headers = array_combine($matches[1], $matches[2]);
    $is_autoreply = FALSE; 
    if ($headers['Auto-submitted'] && ($headers['Auto-submitted'] != 'no')) { $is_autoreply = TRUE; }
    if ($headers['X-Auto-Response-Suppress'] == 'DR' || $headers['X-Auto-Response-Suppress'] == 'AutoReply' || $headers['AutoReply'] == 'All') { $is_autoreply = TRUE; }
    if ($headers['List-Id']) { $is_autoreply = TRUE; }
    if ($headers['List-Unsubscribe']) { $is_autoreply = TRUE; }
    if ($headers['Feedback-ID']) { $is_autoreply = TRUE; }
    if ($is_autoreply) {
      sirius_minilog("This message appears to be an autoreply. Silently skipping.", 'warning');
      continue; 
    }

    // 
    // Get the user
    //

    $from = imap_utf8($message->from);
    if (!$from) {
      sirius_minilog("No FROM address in message; skipping.", 'warning');
      ++$count_reject;
      continue;
    }
    $parts = mailparse_rfc822_parse_addresses($from);
    if (!$parts) {
      sirius_minilog("Unable to parse the FROM address ($from); skipping.", 'warning');
      ++$count_reject;
      continue;
    }
    $from_mail = $parts[0]['address'];
    if (!$from_mail) {
      sirius_minilog("Unable to parse the FROM address ($from); skipping.", 'warning');
      ++$count_reject;
      continue;
    }
    $mail_user = user_load_by_mail($from_mail);
    if (!$mail_user) {
      $msg = "The sender's email, $from_mail, does not correspond to a valid Sirius user. This message was NOT attached.";
      sirius_minilog($msg, 'warning');
      sirius_mailattach_fetch_reject($from_mail, "$msg  - Subject: " . $subject);
      ++$count_reject;
      continue;
    }

    $can_send = FALSE;
    if (user_access('sirius admin', $mail_user)) { $can_send = TRUE; }
    if (
      ($mail_user->field_sirius_domain['und'][0]['target_id'] == $domain_nid) && 
      (user_access('sirius domain admin', $mail_user) || user_access('grievance user', $mail_user))
      )
    {
      $can_send = TRUE;
    }
    if ($domain_nid <= 0 && 
    	(user_access('sirius domain admin', $mail_user) || user_access('grievance user', $mail_user))) {
    	$can_send = TRUE; 
    }
    if (!$can_send) {
      $msg = "The sender's email, $from_mail, is in the wrong domain or does not have permission to attach messages. This message was NOT attached.";
      sirius_minilog($msg, 'warning');
      sirius_mailattach_fetch_reject($from_mail, "$msg  - Subject: " . $subject);
      ++$count_reject;
      continue;
    }

    //
    // Parse the subject
    //

    if (!$subject) {
      $msg = "The subject of the email was not found.";
      sirius_minilog($msg, 'warning');
      sirius_mailattach_fetch_reject($from_mail, $msg);
      ++$count_reject;
      continue;
    }

    $parsed = sirius_mailattach_parse_subject($subject);
    if (!$parsed['success']) {
      $msg = "Subject parse failed: " . $parsed['msg'];
      sirius_minilog($msg, 'warning');
      sirius_mailattach_fetch_reject($from_mail, $msg);
      ++$count_reject;
      continue;
    }
    $entity_node = node_load($parsed['nid']);
    $bundle = $parsed['bundle'];
    sirius_minilog("The message will be added as an attachment to a record of type $parsed[bundle_alias]: " . l($entity_node->title, 'node/' . $entity_node->nid) . " (" . $entity_node->nid . ")");

    //
    // The message was accepted. We'll now pull the structure of the message and create attachments for all the bits.
    // 

    ++$count_accept;
    $msgno = $message->msgno;
    $structure = imap_fetchstructure($mbox, $msgno);

    // 
    // Fetch all attachments and move into the Drupal file system
    // 

    $attachments = array();
    sirius_imap_fetch_recursive($mbox, $msgno, $structure, $attachments);

    //
    // Create the attachment node
    //

    $json = array();
    $json['header'] = $attachments['header'];
    $json['structure'] = $structure;
    $attachment_node = new stdClass();
    $attachment_node->title = "Mailattach " . date('Y-m-d') . ': ' . $parsed['user_subject'];
    if ($bundle == 'grievance') {
      $attachment_node->type = 'grievance_attachment';
    } else {
      $attachment_node->type = 'sirius_log';
    }
    $attachment_node->language = LANGUAGE_NONE;
    $attachment_node->uid = $mail_user->uid; 
    $attachment_node->status = 1;
    $attachment_node->promote = 0;
    $attachment_node->comment = 0;
    if ($bundle == 'grievance') {
      $attachment_node->field_grievance['und'][0]['target_id'] = $entity_node->nid;
      if (sirius_domain_variable_get('sirius_mailattach_grievance_type', '')) {
        $attachment_node->field_grievance_document_type['und'][0]['tid'] = sirius_domain_variable_get('sirius_mailattach_grievance_type', '');
      }
      $attachment_node->field_grievance_label['und'][0]['value'] = substr($parsed['user_subject'], 0, 255);
    } else {
      $attachment_node->field_sirius_category['und'][0]['value'] = 'mailattach';
      $attachment_node->field_sirius_type['und'][0]['value'] = 'fetched';
      $attachment_node->field_sirius_message['und'][0]['value'] = substr($parsed['user_subject'], 0, 255);
      $attachment_node->field_sirius_summary['und'][0]['value'] = $attachments['body'];
      $attachment_node->field_sirius_log_handler['und'][0]['target_id'] = $entity_node->nid;
    }
    $idx = 0;
    foreach ($attachments['files'] as $idx => $file) {
      $file = (array) $file;
      if ($bundle == 'grievance') {
        $attachment_node->field_grievance_attachments["und"][] = $file;
      } else {
        $attachment_node->field_sirius_attachments["und"][] = $file;
      }
    }
    sirius_jsonfield_util_node_json_set($attachment_node, $json);
    node_object_prepare($attachment_node);
    $attachment_node = node_submit($attachment_node);
    node_save($attachment_node);
    sirius_minilog("Attachment node saved: "  . l($attachment_node->title, 'node/' . $attachment_node->nid) . " (" . $attachment_node->nid . ")");

    global $base_url;
    $url = $base_url . '/node/' . $entity_node->nid;
    $html = "Your email has been added as an attachment to <strong>" . $entity_node->title . '</strong><br />';
    $html .= "Link: <a href=\"$url\">$url</a>";
    sirius_mailattach_fetch_accept($from_mail, $html, $parsed['user_subject']);
  }

  // Delete all the messages
  $count_delete = 0;
  if ($mode == 'live') {
    foreach ($messages as $message) {
      $msgno = $message->msgno;
      ++$count_delete;
      imap_delete($mbox, $msgno);
    }
  }

  // Expunge any deleted messages
  imap_expunge($mbox);

  // Done with imap
  imap_close($mbox);

  // All done.
  $msg = "Mailattach complete. Processed $count messages. Accepted $count_accept, rejected $count_reject, and expunged $count_delete from the inbox.";
  sirius_minilog($msg);
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_mailattach_fetch_reject($from_mail, $msg, $subject = NULL) {
  $params = array();
  $params['subject'] = "Attachment rejected: $subject";
  $params['body'] =  "Your email to $sirius_mailattach_user was rejected: $msg";
  $params['to'] = $from_mail;
  $result = sirius_email_send($params);
  if ($result['success']) { 
    sirius_minilog("Sent rejection email to $from_mail.");
  } else {
    sirius_minilog("Failed to send rejection email to $from_mail: " . $result['msg']);
  }
}

function sirius_mailattach_fetch_accept($from_mail, $msg, $subject = NULL) {
  $params = array();
  $params['subject'] = "Attachment accepted: $subject";
  $params['body'] =  "Your email was accepted: $msg";
  $params['to'] = $from_mail;
  $result = sirius_email_send($params);
  if ($result['success']) { 
    sirius_minilog("Sent acceptance email to $from_mail.");
  } else {
    sirius_minilog("Failed to send acceptance email to $from_mail: " . $result['msg']);
  }
}

function sirius_imap_tmp_uri($msgno, $msgpath, $filename) {
  $directory = 'temporary://';
  $directory_realpath = drupal_realpath($directory);
  if (!file_exists($directory_realpath)) {
    mkdir($directory_realpath);
  }
  $domain_nid = sirius_domain_nid();
  $filename = trim(strtolower($filename));
  $filename = preg_replace('/[^a-z0-9_\.]/', '_', $filename);
  $uri = $directory . 'mailattach_' . $domain_nid . '.' . $msgno . '.' . $msgpath . '.' . $filename;
  return $uri;
}

function sirius_imap_uri($msgno, $msgpath, $filename) {
  $directory = 'private://sirius_attachments';
  $domain_nid = sirius_domain_nid();
  $filename = trim(strtolower($filename));
  $filename = preg_replace('/[^a-z0-9_\.]/', '_', $filename);
  $uri = $directory . '/' . $domain_nid . '.' . $msgno . '.' . $msgpath . '.' . $filename;
  return $uri;
}

function sirius_imap_fetch_recursive($mbox, $msgno, $structure, &$attachments, $path = NULL) {
  if (!$path) {
    $header = imap_fetchheader($mbox, $msgno);
    $attachments['header'] = $header;

    /*
    $filename = 'header.txt';
    $tmp_uri = sirius_imap_tmp_uri($msgno, '1', $filename);
    $uri = sirius_imap_uri($msgno, '1', $filename);
    $file = file_save_data($header, $tmp_uri, FILE_EXISTS_REPLACE);
    $file->display = 0;
    $file = file_move($file, $uri, FILE_EXISTS_REPLACE);
    $attachments['files'][] = $file;
    */
  }

  if (isset($structure->parts) && count($structure->parts)) {
    $i = 0;
    foreach ($structure->parts as $part) {
      ++$i;
      $sub_path = $path;
      if ($sub_path) { $sub_path .= '.'; }
      $sub_path .= $i;
      sirius_imap_fetch_recursive($mbox, $msgno, $part, $attachments, $sub_path);
    }
  } else {
    $filename = '';
    if (!$path) { $path = '1'; }

    if($structure->dparameters) {
      foreach($structure->dparameters as $object) {
        if(strtolower($object->attribute) == 'filename') {
          $filename = $object->value;
        }
      }
    }
    if($structure->ifparameters) {
      foreach($structure->parameters as $object) {
        if(strtolower($object->attribute) == 'name') {
          $filename = $object->value;
        }
      }
    }
    if (!$filename) {
      if ($structure->subtype == 'HTML') {
        $filename = 'body.html';
      }
    }
    if (!$filename) {
      $filename = 'body.txt';
    }

    $tmp_uri = sirius_imap_tmp_uri($msgno, $path, $filename);
    $uri = sirius_imap_uri($msgno, $path, $filename);
    $realpath = drupal_realpath($tmp_uri);
    $whandle = fopen($realpath, 'w');

    $coding = $structure->encoding;
    if ($coding == 0) {
      //$body = imap_7bit($body);
    } elseif ($coding == 1) {
      //$body= imap_8bit($body);
    } elseif ($coding == 2) {
      //$body = imap_binary($body);
    } elseif ($coding == 3) {
      //$body = imap_base64($body);
      stream_filter_append($whandle, 'convert.base64-decode', STREAM_FILTER_WRITE);
    } elseif ($coding == 4) {
      //$body = imap_qprint($body);
      stream_filter_append($whandle, 'convert.quoted-printable-decode', STREAM_FILTER_WRITE);
    } elseif ($coding == 5) {
      //$body = $body;
    }    

    try {
      imap_savebody($mbox, $whandle, $msgno, $path);
      $file = (object) array(
        'filename' => $filename,
        'uri' => $tmp_uri,
      );
      $file->filemime = file_get_mimetype($tmp_uri);
      $file->display = 0;
      $file->status = 1;
      $file = file_move($file, $uri, FILE_EXISTS_REPLACE);
      $attachments['files'][] = $file;
    } catch (exception $e) {
      sirius_minilog("Failed to write to $realpath: " . $e->getMessage(), 'error');
    }

    if ($filename == 'body.txt') {
      $body = imap_fetchbody($mbox, $msgno, $path);
      if($coding == 3) { $body = base64_decode($body); }
      elseif($structure->encoding == 4) { $body = quoted_printable_decode($body); } 
      $attachments['body'] = $body;   
    }
  }
}

function sirius_mailattach_parse_subject($subject) {
  // Get rid of any "re:" or "fwd:"
  $subject = trim($subject);
  while (preg_match('/^Re:/', $subject)) {
    $subject = preg_replace('/^Re:/', '', $subject);
    $subject = trim($subject);
  }
  while (preg_match('/^Fwd:/', $subject)) {
    $subject = preg_replace('/^Fwd:/', '', $subject);
    $subject = trim($subject);
  }

  // Before the colon is our filing code. After the colon is the "real" subject
  list ($code, $user_subject) = explode(':', $subject, 2);
  $code = trim($code);
  $user_subject = trim($user_subject);
  $result['user_subject'] = $user_subject;
  $result['code'] = $code;
  if (!$code) {
    $result['success'] = FALSE;
    $result['msg'] = 'No filing code found.';
    return $result;
  }

  // Find which bundles are available.
  $bundle_aliases = array(
    'grievance' => 'Grievance',
    'sirius_worker' => 'Worker',
    'grievance_shop' => 'Employer',
  );
  $all_bundles = sirius_domain_variable_get('sirius_mailattach_bundles', NULL);
  if (!$all_bundles) { 
    $all_bundles = array('grievance');
  } else {
    $all_bundles = array_filter(array_values($all_bundles));
  }
  if (!$all_bundles) {
    $result['msg'] = 'No record types are enabled. Please go to the mailattach configuration and enable at least one record type.';
    $result['success'] = FALSE;
    return $result;
  }

  // Pick a bundle. It's either at the beginning of the subject, or we use the default.
  foreach ($all_bundles as $probe) {
    $alias = $bundle_aliases[$probe];
    if (preg_match("/^$alias\s+/i", $code)) {
      $bundle = $probe;
      $code = preg_replace("/^$alias\s+/i", '', $code); 
      break;
    }
  }
  if (!$bundle) { 
    $bundle = sirius_domain_variable_get('sirius_mailattach_bundle_default', '');
  } 
  if (!$bundle) { 
    $result['msg'] = 'Don\'t know which record type to use. Either set a default record type in the mailattach configuration, or make sure that the email subject begins with a record type (e.g. "grievance 123: My subject here")';
    $result['success'] = FALSE;
    return $result;
  }
  $result['bundle'] = $bundle;
  $result['bundle_alias'] = $bundle_aliases[$bundle];
  $code = trim($code);
  if (!$code) { 
    $result['msg'] = 'Don\'t know which record to search for. Make sure that you include the record ID in the subject (e.g. "grievance 123: My subject here")';
    $result['success'] = FALSE;
    return $result;
  }

  // At this point, $code contains the code we're looking for. We'll try in various ways to find the matching node.

  // If it's an SSN, search that field.
  $ssn = sirius_ssn_format($code);
  if (!$nid && sirius_ssn_validate($ssn) && ($bundle == 'sirius_worker')) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', $bundle);
    $query->propertyCondition('status', 1);
    $query->propertyOrderBy('created', 'desc');
    $query->fieldCondition('field_sirius_ssn', 'value', $ssn);
    if ($domain_nid > 0) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
    $query->range(0, 1);
    $stmt = $query->execute();
    if (isset($stmt['node'])) { $nid = array_keys($stmt['node'])[0]; }
  }

    // Search by the worker's phone
  $phone = sirius_phone_format($code);
  if (!$nid && sirius_phone_validate($phone) && ($bundle == 'sirius_worker')) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', $bundle);
    $query->propertyCondition('status', 1);
    $query->propertyOrderBy('created', 'desc');
    $query->fieldCondition('field_sirius_phone', 'value', $phone);
    if ($domain_nid > 0) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
    $query->range(0, 1);
    $stmt = $query->execute();
    if (isset($stmt['node'])) { $nid = array_keys($stmt['node'])[0]; }
  }

  // Search by the worker's phone
  $email = sirius_email_format($email);
  if (!$nid && sirius_email_validate($email) && ($bundle == 'sirius_worker')) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', $bundle);
    $query->propertyCondition('status', 1);
    $query->propertyOrderBy('created', 'desc');
    $query->fieldCondition('field_sirius_email', 'value', $email);
    if ($domain_nid > 0) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
    $query->range(0, 1);
    $stmt = $query->execute();
    if (isset($stmt['node'])) { $nid = array_keys($stmt['node'])[0]; }
  }

  // Search by NID, NID2, NID3
  foreach (array('sirius_id', 'sirius_id2', 'sirius_id3', 'grievance_id', 'grievance_external_id') as $field) {
    if (!$nid) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('bundle', $bundle);
      $query->propertyCondition('status', 1);
      $query->propertyOrderBy('created', 'desc');
      $query->fieldCondition('field_' . $field, 'value', $code);
      if ($domain_nid > 0) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
      $query->range(0, 1);
      $stmt = $query->execute();
      if (isset($stmt['node'])) { $nid = array_keys($stmt['node'])[0]; }
    }
  }

  // If we didn't find the NID, we lose.
  if (!$nid) {
    $result['msg'] = "No matching record of type $bundle_aliases[$bundle] was found for the string [$code].";
    $result['success'] = FALSE;
    return $result;
  }

  $result['nid'] = $nid;
  $result['success'] = TRUE;
  return $result;
}

