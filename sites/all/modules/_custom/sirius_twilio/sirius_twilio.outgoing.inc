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
  $disable = sirius_domain_variable_get("sirius_sms_global_disable", '');
  if ($disable == 'Yes') { 
	  $whitelist = sirius_domain_variable_get("sirius_sms_global_disable_whitelist", '');
	  $probe = sirius_phone_format($phone);
	  if (strpos($whitelist, $probe) === FALSE) { return array('success' => FALSE, 'msg' => "Global SMS disable is on and $probe is not whitelisted."); }
	}

	$phone = sirius_phone_format_e164($phone);
  if (!$phone)                        { return array('success' => FALSE, 'msg' => "No phone number"); }
  if (!sirius_phone_validate($phone)) { return array('success' => FALSE, 'msg' => "[$phone] is not a valid phone number."); }

  $possible = sirius_phonenumber_sms_possible($phone);
  if (!$possible) 										{ return array('success' => FALSE, 'msg' => "[$phone] does not appear to be a mobile-enabled number."); }

  static $registered;
  if (sirius_domain_variable_get('sirius_twilio_sms_consolidate', FALSE)) {
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

	foreach ($queue as $phone => $msg) {
		try {
			$client = sirius_twilio_client_get();
			$client->messages->create(
		    $phone,
		    array(
		      'from' => sirius_domain_variable_get('sirius_twilio_phone', ''),
		      'body' => $msg,
		    )
			);
		} catch (Exception $e) {
			$result = array('success' => FALSE, 'msg' => "SMS message failed.", 'details' => "Failed to send message [$msg]: " . $e->getCode() . ' - ' . $e->getMessage());
			$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
			sirius_log('twilio:conversation', 'error', $msg, $msg, $phone, array($handler_nid));
		}
	}

	$queue = NULL;
	if ($result && !$result['success']) { return $result; }
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

  $disable = sirius_domain_variable_get("sirius_sms_global_disable", '');
  if ($disable == 'Yes') { 
	  $whitelist = sirius_domain_variable_get("sirius_sms_global_disable_whitelist", '');
	  $probe = sirius_phone_format($phone);
	  if (strpos($whitelist, $probe) === FALSE) { return array('success' => FALSE, 'msg' => "Global SMS disable is on and $probe is not whitelisted."); }
	}

	$context['phone'] = $phone;

	$callerid_nid = $context['callerid_nid'];
	if ($callerid_nid) { 
		$callerid_node = node_load($callerid_nid);
		$callerid_phone = $callerid_node->field_sirius_phone['und'][0]['value'];
	}
	if (!$callerid_phone) { $callerid_phone = sirius_domain_variable_get('sirius_twilio_phone', ''); }
	$callerid_phone = sirius_phone_format_e164($callerid_phone);

	if ($callerid_phone == $phone) {
		return array('success' => FALSE, 'msg' => "Caller ID is the same as the recipient ($callerid_phone)");
	}

	$msg = "Outgoing call initiated to $phone";

	try {
		$client = sirius_twilio_client_get();
		$call_params = array("url" => $base_url . '/sirius/twilio/incoming/voice');
		if ($context['amd']['MachineDetection']) { 
			$call_params['machineDetection'] = $context['amd']['MachineDetection'];
			$names = sirius_twilio_amd_setting_names();
			foreach ($names as $name) {
				$value = sirius_domain_variable_get($name);
				if ($value) { $call_params[$name] = $value; }
			}
		}

		$call = $client->calls->create($phone, $callerid_phone, $call_params);

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
	header('Content-Type: text/xml');
	print "<Response>";
	if ($gather_digits) { print "<Gather numDigits=\"$gather_digits\">"; }
	print sirius_domain_variable_get('sirius_twilio_twiml_open', '<Say><prosody>');
	print sirius_twilio_outgoing_voice_clean($msg);
	print sirius_domain_variable_get('sirius_twilio_twiml_close', '</prosody></Say>');
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

function sirius_twilio_outgoing_voice_dial($call_sid, $context, $msg, $phone, $dial_attributes = array()) {
	$phone = sirius_phone_format_e164($phone);
	$client = sirius_twilio_client_get();

	$dial_verb = '<Dial';
	foreach ($dial_attributes as $key => $value) {
		$dial_verb .= " $key=\"$value\"";
	}
	$dial_verb .= '>';

	// $response = new TwiML();
	// $response->say(sirius_twilio_outgoing_voice_clean($msg));
	// $response->dial($phone);

	header('Content-Type: text/xml');
	print "<Response>";
	if ($msg) {
		print "<Say>";
		print sirius_twilio_outgoing_voice_clean($msg);
		print "</Say>";
	}
	print $dial_verb;
	print $phone;
	print "</Dial>";
	print "</Response>";

	$msg = "Forwarded call from " . $context['phone'] . " to " . $phone;
	if (isset($context['request']['Digits'])) {
		$msg = "(User entered " . $context['request']['Digits'] . ") " . $msg;
	}
	sirius_twilio_set_context($call_sid, $context, 'dial', $msg);
	exit();
}

function sirius_twilio_outgoing_voice_clean($msg) {
	return substr(htmlspecialchars($msg), 0, 3000);
}

function sirius_twilio_outgoing_voice_play($call_sid, $context, $url) {
	header('Content-Type: text/xml');
	print "<Response><Play>" . htmlspecialchars($url, ENT_XML1 | ENT_QUOTES) . "</Play></Response>";
	sirius_twilio_set_context($call_sid, $context, 'play', $msg);
	exit();
}

/*
function sirius_twilio_outgoing_voice_stream() {
	$call_sid = $_REQUEST['call_sid'];
	if (!$call_sid) { sirius_twilio_browserphone_twiml_dial_error("Missing call_sid."); }
	$context = sirius_twilio_get_context($call_sid);
	$omg_key = $context['omg_key'];
	list($audio_nid, $delta) = explode('-', $omg_key);

	// @todo: Security is a little sketch here.
	//        We assume that if the URL has a call_sid, it's ok to stream the file associated with the context of that call_sid.
	//        That's not a univerally good assumption. We should at the very least ensure that the call is active. We should also probably authenticate callbacks from Twilio.
	//        But ... the client code is responsible for only setting the NID of an audio message uploaded as it should be, which means it's probably just a bulk message.
	//        So if somebody can guess the call sid, they can play somebody else's outgoing bulk message. Oh well.

	if (!$audio_nid > 0) { sirius_twilio_browserphone_twiml_dial_error('Invalid omg key'); }
	$audio_node = node_load($audio_nid);
	$audio_item = $audio_node->field_sirius_audio_public['und'][$delta];
	sirius_become_superuser(TRUE);
	$uri = $audio_item['uri'];
  $headers = array();
  foreach (module_implements('file_download') as $module) {
    $function = $module . '_file_download';
    $result = $function($uri);
    if ($result == -1) {
      return drupal_access_denied();
    }
    if (isset($result) && is_array($result)) {
      $headers = array_merge($headers, $result);
    }
  }
  if (count($headers)) {
    file_transfer($uri, $headers);
  }	
}
*/


function sirius_twilio_outgoing_flow($flow_sid, $phone, $callerid_nid, $flow_params = array(), $handler_nid = NULL) {
  // Are we in a global disable mode?
  $disable = sirius_domain_variable_get("sirius_sms_global_disable", '');
  if ($disable == 'Yes') { 
	  $whitelist = sirius_domain_variable_get("sirius_sms_global_disable_whitelist", '');
	  $probe = sirius_phone_format($phone);
	  if (strpos($whitelist, $probe) === FALSE) { return array('success' => FALSE, 'msg' => "Global SMS disable is on and $probe is not whitelisted."); }
	}

	// Validate the recipient phone
	$phone = sirius_phone_format_e164($phone);
  if (!$phone)                        { return array('success' => FALSE, 'msg' => "No phone number"); }
  if (!sirius_phone_validate($phone)) { return array('success' => FALSE, 'msg' => "[$phone] is not a valid phone number."); }

  $possible = sirius_phonenumber_voice_possible($phone);
  if (!$possible) 										{ return array('success' => FALSE, 'msg' => "[$phone] does not appear to be a mobile-enabled number."); }

  if (!$callerid_nid) 								{ return array('success' => FALSE, 'msg' => "No Caller ID specified."); }
	$callerid_node = node_load($callerid_nid);
	if (!$callerid_node) 								{  return array('success' => FALSE, 'msg' => "Unable to find the caller ID node."); }
	$callerid_phone = $callerid_node->field_sirius_phone['und'][0]['value'];
	if (!$callerid_phone) { $callerid_phone = sirius_domain_variable_get('sirius_twilio_phone', ''); }
	$callerid_phone = sirius_phone_format_e164($callerid_phone);

	$handlers = array();
	if ($handler_nid) { $handlers[] = $handler_nid; }

	try {
		$client = sirius_twilio_client_get();
		$client->studio->v2->flows($flow_sid)->executions->create($phone, $callerid_phone, array('parameters' => $flow_params));
	} catch (Exception $e) {
		$result = array('success' => FALSE, 'msg' => "Flow message failed.", 'details' => "Failed to send to Twilio Flow: " . $e->getCode() . ' - ' . $e->getMessage());
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:conversation', 'error', $msg, $msg, $phone, $handlers);
		return $result;
	}

	$msg = "Transferred $phone to flow $flow_sid (callerId $callerid_phone)";

	sirius_log(
		'twilio:flow', 
		'outgoing_flow', 
		$msg,
		$msg,
		$phone, 
		$handlers
	);

	return array('success' => TRUE, 'msg' => $msg);	
}