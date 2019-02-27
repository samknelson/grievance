<?php

use Twilio\Rest\Client;
use Twilio\TwiML;

/**********************************************************************************
 * SMS
 **********************************************************************************/

function sirius_twilio_outgoing_sms($phone, $msg, $handler_nid = NULL) {
	$phone = sirius_phone_format_e164($phone);
	try {
		$client = sirius_twilio_client_get();
		$client->messages->create(
	    $phone,
	    array(
	      'from' => variable_get('sirius_twilio_phone', ''),
	      'body' => $msg,
	    )
		);
		$result = array('success' => TRUE, 'msg' => 'Sent message ' . $msg . ' by SMS to ' . $phone);
	} catch (Exception $e) {
		sirius_log('twilio:conversation', 'error', $msg, $msg, $phone, array($handler_nid));
		sirius_log('twilio:conversation', 'error', $e->getCode() . ' - ' . $e->getMessage(), '', '', array($handler_nid));
		$result = array('success' => FALSE, 'msg' => "SMS message failed.");
	}

	sirius_log('twilio:conversation', 'outgoing_sms', $msg, $msg, $phone, array($handler_nid));
	return $result;
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
	echo $response;

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

	$context = sirius_twilio_get_context($call_sid);
	sirius_twilio_set_context($call_sid, $context, 'dial', "Forwarded call from " . $context['phone'] . " to " . $phone);

	exit();
}
