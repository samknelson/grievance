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
	$handler_nid = sirius_twilio_handler_get_sms($phone, $msg);

	sirius_log(
		'twilio:conversation', 
		'incoming_sms', 
		$msg, 
		'<pre>' . print_r($_REQUEST, 1) . '</pre>', 
		$phone, 
		array($handler_nid)
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
  if (preg_match('/outbound/', $context['direction'])) {
  	$context['phone'] = sirius_phone_format_e164($context['to']);
  } else {
  	$context['phone'] = sirius_phone_format_e164($context['from']);
  }


	if ($is_new) { 
		$log_type = "initiated_incoming";
		$log_msg = "Incoming call from " . $context['phone'];
	} else {
		$log_type = "heard";
		$log_msg = "User entered: " . $context['digits'];
	}

	sirius_twilio_set_context($call_sid, $context, $log_type, $log_msg, '<pre>' . print_r($_REQUEST, 1) . '</pre>');
  sirius_dispatch_handle_voice($call_sid, $context);
	sirius_become_superuser(FALSE);
}

