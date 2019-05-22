<?php

use Twilio\Rest\Client;
use Twilio\TwiML;

/**********************************************************************************
 * SMS
 **********************************************************************************/

function sirius_twilio_outgoing_sms($phone, $msg, $handler_nid = NULL) {

	if (strlen($msg) > 1595) {
		return array('success' => FALSE, 'msg' => 'SMS sending failed: the outoing message length exceeded 1600 characters.');
	}

  // Are we in a global disable mode?
  $disable = variable_get("sirius_sms_global_disable", '');
  if ($disable == 'Yes') { 
	  $whitelist = variable_get("sirius_sms_global_disable_whitelist", '');
	  $probe = sirius_phone_format($phone);
	  if (strpos($whitelist, $probe) === FALSE) { return array('success' => FALSE, 'msg' => "Global SMS disable is on and $probe is not whitelisted."); }
	}

	$phone = sirius_phone_format_e164($phone);
  if (!$phone)                        { return array('success' => FALSE, 'msg' => "No phone number"); }
  if (!sirius_phone_validate($phone)) { return array('success' => FALSE, 'msg' => "[$phone] is not a valid phone number."); }

  static $registered;
  if (variable_get('sirius_twilio_sms_consolidate', TRUE)) {
		$result = sirius_twilio_outgoing_sms_queue(FALSE, $phone, $msg);
		if (!$registered) {
			$registered = TRUE;
			drupal_register_shutdown_function('sirius_twilio_outgoing_sms_queue', TRUE);
		}
  } else {
		$result = sirius_twilio_outgoing_sms_queue(TRUE, $phone, $msg);
	}

	if ($result['success']) {
		sirius_log(
			'twilio:conversation', 
			'outgoing_sms', 
			"Sending to $phone: $msg",
			"Sending to $phone: $msg",
			$phone, 
			array($handler_nid)
		);
		$result['msg'] = "Sending to $phone: $msg";
	}

	return $result;
}

function sirius_twilio_outgoing_sms_queue($deliver_now, $phone = NULL, $msg = NULL) {
	static $queue;

	if ($phone) { 
		if ($queue[$phone]) { $queue[$phone] .= "\n\n"; }
		$queue[$phone] .= $msg;
	}

	if (!$deliver_now) { return array('success' => TRUE, 'msg' => "Queued for delivery: $phone, $msg"); }

	drupal_set_message("Here with $deliver_now.");

	foreach ($queue as $phone => $msg) {
		try {
			$client = sirius_twilio_client_get();
			$client->messages->create(
		    $phone,
		    array(
		      'from' => variable_get('sirius_twilio_phone', ''),
		      'body' => $msg,
		    )
			);
		} catch (Exception $e) {
			$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
			sirius_log('twilio:conversation', 'error', $msg, $msg, $phone, array($handler_nid));
			$result = array('success' => FALSE, 'msg' => "SMS message failed.");
		}
	}

	$queue = NULL;
	return array('success' => TRUE, 'msg' => 'Queue delivered.');
}

/**********************************************************************************
 * Voice
 **********************************************************************************/

function sirius_twilio_call_sid($call) {
	$call_arr = $call->toArray();
	return $call_arr['sid'];
}

function sirius_twilio_outgoing_voice($phone, $context) {
	global $base_url;
	$phone = sirius_phone_format_e164($phone);

  $disable = variable_get("sirius_sms_global_disable", '');
  if ($disable == 'Yes') { 
	  $whitelist = variable_get("sirius_sms_global_disable_whitelist", '');
	  $probe = sirius_phone_format($phone);
	  if (strpos($whitelist, $probe) === FALSE) { return array('success' => FALSE, 'msg' => "Global SMS disable is on and $probe is not whitelisted."); }
	}

	$context['phone'] = $phone;
	$msg = "Outgoing call initiated to $phone";
	try {
		$client = sirius_twilio_client_get();
		$call = $client->calls->create($phone, variable_get('sirius_twilio_phone', ''), array("url" => $base_url . '/sirius/twilio/incoming/voice'));
		$call_sid = sirius_twilio_call_sid($call);
		$result = array('success' => TRUE, 'msg' => 'Initiated call to ' . $phone);
	} catch (Exception $e) {
		sirius_log('twilio:outgoing', 'error', $msg, $msg, $phone, array($handler_nid));
		sirius_log('twilio:outgoing', 'error', $e->getCode() . ' - ' . $e->getMessage(), '', $phone, array($handler_nid));
		$result = array('success' => FALSE, 'msg' => "Call failed.");
	}

	sirius_twilio_set_context($call_sid, $context, 'initiated_outgoing', "Called: $phone");
	return $result;
}

function sirius_twilio_outgoing_voice_repeat($call_sid, $context, $msg = NULL) {
	header('Content-Type: text/xml');

	$msg = $msg . $context['last_msg'];
	$gather_digits = $context['last_gather_digits'];

	sirius_twilio_outgoing_voice_say($call_sid, $context, $msg, $gather_digits);
}

function sirius_twilio_outgoing_voice_say($call_sid, $context, $msg, $gather_digits) {
	$client = sirius_twilio_client_get();
	$response = new TwiML();

	if ($gather_digits) {
		$gather = $response->gather(array('numDigits' => $gather_digits));
		$gather->say($msg);
	} else {
		$response->say($msg);
	}

	header('Content-Type: text/xml');
	print "<Response>";
	if ($gather_digits) { print "<Gather numDigits=\"$gather_digits\">"; }
	print variable_get('sirius_twilio_twiml_open', '<Say><prosody>');
	print $msg;
	print variable_get('sirius_twilio_twiml_close', '</prosody></Say>');
	if ($gather_digits) { print "</Gather>"; }
	print "</Response>";

	$context['last_msg'] = $msg;
	$context['last_gather_digits'] = $gather_digits;
	if (isset($context['request']['Digits'])) {
		$msg = "(User entered " . $context['request']['Digits'] . ") " . $msg;
	}
	sirius_twilio_set_context($call_sid, $context, 'say', $msg);
	exit();
}

function sirius_twilio_outgoing_voice_dial($call_sid, $context, $msg, $phone) {
	$phone = sirius_phone_format_e164($phone);
	$client = sirius_twilio_client_get();
	$response = new TwiML();
	$response->say($msg);
	$response->dial($phone);

	header('Content-Type: text/xml');
	echo $response;

	$msg = "Forwarded call from " . $context['phone'] . " to " . $phone;
	if (isset($context['request']['Digits'])) {
		$msg = "(User entered " . $context['request']['Digits'] . ") " . $msg;
	}
	sirius_twilio_set_context($call_sid, $context, 'dial', $msg);
	exit();
}
