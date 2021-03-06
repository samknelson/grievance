<?php

use Twilio\Rest\Client;
use Twilio\TwiML;

/**********************************************************************************
 * SMS
 **********************************************************************************/

function sirius_twilio_incoming_sms() {
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

