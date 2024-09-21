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

function sirius_email_send_possible($params) { return sirius_comm_send_possible('email', $params); }
function sirius_email_send($params) { return sirius_comm_send('email', $params); }

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

  $message['headers']['X-Sirius-Id'] = $params['id'];

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

function sirius_email_callback($id, $status, $json) {
  return Sirius_Comm_Email::getInstance()->webhook(array('external_id' => $id, 'status' => $status, 'json' => $json));
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

function sirius_email_render($email, $options = array()) {
  if (!$email) { return ''; }
  $email = htmlspecialchars($email);

  $email_display = $email;
  if ($options['maxlength']) {
    if (strlen($email_display) > $options['maxlength']) {
      $email_display = substr($email_display, 0, $options['maxlength'] - 3) . '...';
    }
  }

  return "<a href=\"mailto:$email\">$email_display</a>";
}

class Sirius_Comm_Email extends Sirius_Comm {
  public $type = 'email';
  public $name = 'Email';

  public function send_possible_impl($params) {
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

    return array('success' => TRUE);
  }

  public function send_impl(&$params) {
    $message = drupal_mail(
      'sirius', 
      'default', 
      $params['to'], 
      'und',
      $params
    );

    // Clear attachments, so that we don't log them.
    unset($params['attachments']);
    unset($message['params']['attachments']);

    if (!$message['send']) { 
      return array('success' => FALSE, 'msg' => "Email to $params[to] failed.", 'message' => $message);
    } else {
      return array('success' => TRUE, 'message' => $message, 'external_id' => $params['id']); 
    }
  }

  public function get_from_default() {
    return variable_get("sirius_email_default_from_address", '');
  }

  public function render_body($comm_node) {
    $body = $this->get_body($comm_node);
    $json = sirius_json_get($comm_node);
    if (!$body) { return ''; }
    $html .= '<div class="sirius_comm_section sirius_comm_body">';

    $html .= '<h2>' . t('Message') . '</h2>';
    $subject = $json['result']['message']['subject'];
    if ($subject) {
      $html .= sirius_fakerender_field('Subject:', $subject);
    }
    $html .= '<hr>';
    $html .= strip_tags($body, '<p><a><strong><b><i><br><table><img><tr><td><th><tbody>');
    //$html .= sirius_text_to_html($body);
    $html .= '</div>';
    return $html;
  }

  public function render_from($comm_node) {
    $json = sirius_json_get($comm_node);
    $html .= '<div class="sirius_comm_section sirius_comm_from">';
    $html .= '<h2>' . t('Sender') . '</h2>';
    $keys = array(
      'From',
      'Sender',
      'Reply-To'
    );
    foreach ($keys as $key) {
      $value = $json['result']['message']['headers'][$key];
      if ($value) {
        $html .= sirius_fakerender_field($key . ':', $this->render_addr($value, 'long_html'));
      }
    }
    $html .= '</div>';

    return $html;
  }

  public function render_to($comm_node) {
    $json = sirius_json_get($comm_node);
    $to = $this->get_to($comm_node);

    $html .= '<div class="sirius_comm_section sirius_comm_to">';
    $html .= '<h2>' . t('Recipient(s)') . '</h2>';
    $html .= sirius_fakerender_field('To:', $this->render_addr($to, 'long_html'));
    $keys = array(
      'Cc',
      'Bcc'
    );
    foreach ($keys as $key) {
      $value = $json['result']['message']['headers'][$key];
      if ($value) {
        $html .= sirius_fakerender_field($key . ':', $this->render_addr($value, 'long_html'));
      }
    }
    $html .= '</div>';
    return $html;
  }

  public function render_addr($addr, $format) {
    return htmlspecialchars($addr);
  }

  public function webhook_impl($params = array()) {
    // Really need different imlpementations for AWS and SendGrid. This is AWS.

    $external_id = $params['external_id'];
    if (!$external_id) { return array('success' => FALSE, 'msg' => 'No external ID sent'); }

    $status = $params['status'];

    // Convert JSON to an array (SNS sends it as a class)
    $json = json_decode(json_encode($params['json']), TRUE);

    $summary = '';
    if ($json['Message']['delivery']['recipients']) {
      foreach ($json['Message']['delivery']['recipients'] as $email) {
        if ($summary) { $summary .= ' / '; }
        $summary .= $email;
      }
    }
    if ($json['Message']['bounce']['bouncedRecipients']) {
      foreach ($json['Message']['bounce']['bouncedRecipients'] as $tmp) {
        if ($summary) { $summary .= ' / '; }
        $summary .= $tmp['emailAddress'];
        $summary .= ': ';
        $summary .= $tmp['diagnosticCode'];
      }
    }

    $event_id = $json['MessageId'];

    $details = $json;

    return array(
      'success' => TRUE, 
      'event_id' => $event_id, 
      'external_id' => $external_id, 
      'status' => $status, 
      'details' => $details,
      'msg' => $summary
    );
  }

  public function event_status_info_all() {
    return array(
      'Bounce' => array('comm_status' => 'failed'),
      'Complaint' => array('comm_status' => 'failed'),
      'Delivery' => array('comm_status' => 'sent'),
      'Send' => array(),
      'Reject' => array('comm_status' => 'failed'),
      'Open' => array('comm_status' => 'sent'),
      'Click' => array('comm_status' => 'sent'),
      'Rendering Failure' => array('comm_status' => 'failed'),
      'DeliveryDelay' => array(),
      'Subscription' => array(),
    );
  }
}
