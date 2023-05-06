<?php

use Twilio\Rest\Client;
use Twilio\TwiML;

/**********************************************************************************
 * SMS
 **********************************************************************************
 * @todo: This really should only handle the plumbing, and should call back 
 *        to a non-Twilio function for the details.
 */

function sirius_twilio_incoming_sms() {
	// Acquire superpowers
	sirius_become_superuser(TRUE);

	// Get the phone number, message, and account SID
	$phone = sirius_phone_format($_POST['From']);
	$to = sirius_phone_format($_POST['To']);
	$msg = $_POST['Body'];
	$account_sid = $_POST['AccountSid'];
	$message_sid = $_POST['MessageSid'];
	if (!$message_sid) { $message_sid = uniqid(); }


	// Get all the domains associated with the account. Default to the root domain only.
	if ($account_sid) { $domain_nids = sirius_twilio_account_domains($account_sid); }
	if (!$domain_nids) { $domain_nids = array(-1); }
	global $base_url;

	// For each of our domains 
	foreach ($domain_nids as $domain_nid) {
		// Switch into the relevant domain
		sirius_domain($domain_nid);

		// Look up the phone number. If we don't have a record, create it.
		$phonenumber_node = sirius_phonenumber_lookup($phone);
		if (!$phonenumber_node->nid) { node_save($phonenumber_node); }

		// Log the incoming mesasge
		sirius_log(
			'sms', 
			'incoming', 
			$msg, 
			'<pre>' . print_r($_REQUEST, 1) . '</pre>', 
			$phone, 
			array($phonenumber_node->nid)
		);

		// Store the message as "last incoming"
		$phonenumber_json = sirius_json_get($phonenumber_node);
		$phonenumber_json['last_sms_incoming'] = array(
			'ts' => time(),
			'msg' => $msg,
			'details' => $_POST,
		);

		$msg_trim = preg_replace('/[^A-Z]/', '', strtoupper(trim($msg)));
		if (in_array($msg_trim, array('STOP', 'STOPALL', 'UNSUBSCRIBE', 'CANCEL', 'END', 'QUIT', 'SIRIUSSTOP'))) {
			// Handle STOP
			$phonenumber_json['user_block'][$to] = array(
				'ts' => time(),
				'comments' => 'Received message: ' . $msg,
				'sms' => 'Yes',
			);
		} else if (in_array($msg_trim, array('START', 'YES', 'SIRIUSSTART'))) {
			// Handle START
			$phonenumber_json['user_block'][$to] = array(
				'ts' => time(),
				'comments' => 'Received message: ' . $msg,
				'sms' => 'No',
			);
			unset($phonenumber_json['last_sms']);
		} else {
			// Autorespond. We want to avoid autoresponse loops. So let's set a unique ID that includes the date, so we only do this once a day.
			$unique = $phone . '/response/' . date('Y-m-d');
			$params = array();
			$params['sms'] = "This system cannot respond to incoming messages. Please sign in at the link below.\n$base_url";
			$params['to'] = $phone;
			$params['X-Sirius-Id'] = $unique;
			$result = sirius_sms_send($params);
		}

		sirius_json_set($phonenumber_node, $phonenumber_json);
		node_save($phonenumber_node);
	}

	// Abandon our superpowers
	sirius_become_superuser(FALSE);
}

function sirius_twilio_incoming_sms_orig2() {
	sirius_become_superuser(TRUE);
	$phone = $_POST['From'];
	$msg = $_POST['Body'];
	$phone = sirius_phone_format_e164($phone);

	// Guess the domain from the log. It should be the domain of the last log entry that we sent to.
  // We look for the most recent outgoing log entry of type "twilio:conversation"
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:conversation');
  $query->fieldCondition('field_sirius_type', 'value', 'outgoing_sms');
  $query->fieldCondition('field_sirius_notes', 'value', $phone);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $log_node = node_load(array_keys($stmt['node'])[0]);
  if ($log_node) {
	  $domain_nid = $log_node->field_sirius_domain['und'][0]['target_id'];
  	if ($domain_nid) { sirius_domain($domain_nid); }
  }

	sirius_log(
		'twilio:conversation', 
		'incoming_sms', 
		$msg, 
		'<pre>' . print_r($_REQUEST, 1) . '</pre>', 
		$phone, 
		array()
	);
	sirius_twilio_handle_sms($msg, $phone);
	sirius_become_superuser(FALSE);
}

function sirius_twilio_incoming_sms_orig() {
	sirius_become_superuser(TRUE);
	$phone = $_POST['From'];
	$msg = $_POST['Body'];

	$phone = sirius_phone_format_e164($phone);
	$handler_nid = sirius_twilio_handler_get_sms($phone, $msg);
	$handler_nids = array();
	if (is_numeric($handler_nid)) {
		$handler_nids[] = $handler_nid;
	}

	sirius_log(
		'twilio:conversation', 
		'incoming_sms', 
		$msg, 
		'<pre>' . print_r($_REQUEST, 1) . '</pre>', 
		$phone, 
		$handler_nids
	);

  $handler_node = node_load($handler_nid);
  if ($handler_node && $handler_node->type == 'sirius_dispatch') {
    sirius_dispatch_handle_sms($handler_node, $msg, $phone);
  } else {
  	sirius_twilio_handle_sms($msg, $phone);
  }
	sirius_become_superuser(FALSE);
}

/**********************************************************************************
 * Voice
 **********************************************************************************/

function sirius_twilio_incoming_voice() {
	sirius_become_superuser(TRUE);

  $call_sid = $_REQUEST['CallSid'];
	$context = sirius_twilio_get_context($call_sid);
	if (!$context) { 
		$is_new = TRUE;
	}
	$handler_nid = $context['handler_nid'];
	$context['from'] = $_REQUEST['From'];
	$context['to'] = $_REQUEST['To'];
	$context['digits'] = $_REQUEST['Digits'];
  $context['direction'] = $_REQUEST['Direction'];
  $context['msg'] = $_REQUEST['msg'];
  $context['request'] = $_REQUEST;
  $context['request_digits'] = $_REQUEST['Digits'];
  if (preg_match('/outbound/', $context['direction'])) {
  	$context['phone'] = sirius_phone_format_e164($context['to']);
  } else {
  	$context['phone'] = sirius_phone_format_e164($context['from']);
  }

  // watchdog('debug', '<pre>' . print_r($_REQUEST, 1) . '</pre>');


	if ($is_new) { 
		$log_type = "initiated_incoming";
		$log_msg = "Incoming call from " . $context['phone'];
	} else {
		$log_type = "heard";
		$log_msg = "User entered: " . $context['digits'];
	}

	// sirius_twilio_set_context($call_sid, $context, $log_type, $log_msg, '<pre>' . print_r($_REQUEST, 1) . '</pre>');
  sirius_phonetree($call_sid, $context);
	sirius_become_superuser(FALSE);
}

