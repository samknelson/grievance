<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;
use Twilio\TwiML;

/**
 * Dial a phone number or a conference
 */

function sirius_twilio_browserphone_twiml_dial() {
	// Get our input
	$token = $_REQUEST['token'];
	$phone = $_REQUEST['to'];
	if (!$phone) { $phone = $_REQUEST['To']; }
	$callerid_nid = $_REQUEST['callerid_nid'];
	$conference_name = $_REQUEST['conference_name'];

	global $user;

	// Get our parameters
	$parameters = sirius_twilio_browserphone_parameters_get($token);
	if (!$parameters) { sirius_twilio_browserphone_twiml_dial_error('Unable to load the parameters for this browser phone.'); }

	// Get our phone number or conference name
	if (!$phone && !$conference_name) { sirius_twilio_browserphone_twiml_dial_error('Please enter a phone number of conference name.'); }
	if ($phone) {
		if (!sirius_phone_validate($phone)) { sirius_twilio_browserphone_twiml_dial_error('That does not appear to be a valid phone number.'); }
		$phone = sirius_phone_format_e164($phone);
	}

	// Get our Caller ID number
	if (!$callerid_nid) { sirius_twilio_browserphone_twiml_dial_error('Please select a Caller ID.'); }
	if (!$parameters['callerid_options'][$callerid_nid]) { sirius_twilio_browserphone_twiml_dial_error('You do not have permission to use that Caller ID.'); }
	$callerid_node = node_load($callerid_nid);
	$callerid_phone = sirius_phone_format_e164(sirius_fieldval($callerid_node, 'field_sirius_phone'));
	$parameters['callerid'] = $callerid_phone; 

	if ($phone) { $parameters['phone'] = $phone; }
	if ($conference_name) { $parameters['conference_name'] = $conference_name; }
	sirius_twilio_browserphone_parameters_set($token, $parameters); 

	$phone = '+' . sirius_phone_format_e164($phone);
	
	header('Content-Type: text/xml');
	print "<Response>";
	print "<Dial callerId=\"$callerid_phone\">";
	if ($phone) { 
		print $phone;
	} else { 
		print "<Conference endConferenceOnExit=\"true\">$conference_name</Conference>";
	}
	print "</Dial>";
	print "<Pause length=\"30\"/>";
	print "</Response>";
	exit();
}

// @todo: When call ends, if we're supposed to move into a conference, go for it.
function sirius_twilio_browserphone_twiml_callended() {
	header('Content-Type: text/xml');
	print "<Response>";
	print "<Say>Call ended</Say>";
	print "</Response>";
	exit();
}

function sirius_twilio_browserphone_twiml_dial_error($msg) {
	header('Content-Type: text/xml');
	print '<Response>';
	print '<Say>';
	print $msg;
	print '</Say>';
	print '</Response>';
	exit();
}

/**
 * Echo any TWIML to thw phone that we're connected to
 */

function sirius_twilio_browserphone_twiml_echo() {
	try {
		$client = sirius_twilio_client_get();

		// Get our input
		$token = $_REQUEST['token'];
		if (!$token) { print "Missing token."; exit(); }

		// Get our parameters
		$parameters = sirius_twilio_browserphone_parameters_get($token);
		if (!$parameters) { print "Unable to load the parameters for this browser phone."; exit(); }

		$phone = $_REQUEST['phone'];
		if (!$phone) { $phone = $parameters['phone']; }
		$phone = sirius_phone_format_e164($phone);
		if (!$phone) { print "There is no phone number associated with this browser phone."; exit(); }

		$calls = $client->calls->read(array("status" => "in-progress", "to" => $phone), 20);
		foreach ($calls as $call) { break; }
		if (!$call) { print "Call not found."; exit(); }

		if ($_REQUEST['parent']) {
			$call = $client->calls($call->parentCallSid)->fetch();
			if (!$call) { print "Call not found."; exit(); }			
		}

		$twiml = $_REQUEST['twiml'];
		if (!$twiml) { print "No twiml."; exit(); }

		$call->update(["Twiml" => $twiml]);
		print "Ok.";
		exit();
	} catch (Exception $e) {
		print "Error: " . $e->getCode() . ": " . $e->getMessage();
		exit();
	}
}

/**
 * Add a participant to a conference
 */

function sirius_twilio_browserphone_twiml_conference_add() {
	try {
		$client = sirius_twilio_client_get();

		// Get our input
		$token = $_REQUEST['token'];
		if (!$token) { print "Missing token."; exit(); }

		// Get our parameters
		$parameters = sirius_twilio_browserphone_parameters_get($token);
		if (!$parameters) { print "Unable to load the parameters for this browser phone."; exit(); }

		$conference_name = $_REQUEST['conference_name'];
		
		$conference_phone = $parameters['conference_number'];
		if (!$conference_phone) { print "Don't know which phone number to add."; exit(); }

		$callerid_phone = $parameters['callerid'];
		if (!$callerid_phone) { print "Don't know which caller ID to use."; exit(); }

		foreach ($client->conferences->read(["friendlyName" => $conference_name], 20) as $conference) { break; }
		if (!$conference) { print "Conference not found."; exit(); }
		$conference->participants->create($callerid_phone, $conference_phone);
		print "Ok.";
		exit();
	} catch (Exception $e) {
		print "Error: " . $e->getCode() . ": " . $e->getMessage();
		exit();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Outgoing Message functionality
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// The sequence is a little horrid, and security is a little complex. 
// 
// 1. The browserphone calls sirius_twilio_browserphone_twiml_omg, passing in a "token" and an "omg_key" ("$nid-$delta")
// 2. The server looks up the current in-progress call, and redirects it to the URL corresponding to sirius_twilio_browserphone_twiml_omg_play
// 3. Twilio makes a call to sirius_twilio_browserphone_twiml_omg_play().
// 4. The server responds with a <Play>url</Play>, redirecting to sirius_twilio_browserphone_twiml_omg_stream().
// 5. Twilio calls up sirius_twilio_browserphone_twiml_omg_stream(), expecting an audio file.
// 6. And the server answers with the appropriate audio file.
// 
// At each step, the request is "signed" with a twilio capability token, which we use to look up to be sure that e.g. that browserphone has access to that audio file.
// 
// @todo: DANGER: If you know a browserphone token, you can play any OMG that that browserphone has access to, forever.
//        The security is inadquate. So is the spelling.
//
// @todo: We should be able to eliminate both 1. and 2., replacing them with a simple call to twiml_echo(). This wasn't possible with the previous
//        version of the Twilio library.
//

function sirius_twilio_browserphone_twiml_omg() {
	try {
		$client = sirius_twilio_client_get();

		// Get our input
		$token = $_REQUEST['token'];
		$omg_key = $_REQUEST['omg_key'];
		if (!$token || !$omg_key) {
			print "Missing token or message key.";
			exit();
		}

		// Get our parameters
		$parameters = sirius_twilio_browserphone_parameters_get($token);
		if (!$parameters) {
			print "Unable to load the parameters for this browser phone.";
			exit();
		}

		// $phone = $parameters['phone'];
		$phone = $_REQUEST['phone'];
		if (!$phone) {
			print "Phone number not found in the request.";
			exit();
		}
		$phone = sirius_phone_format_e164($phone);

		$calls = $client->calls->read(array("status" => "in-progress", "to" => $phone), 20);
		$call_sid = NULL;
		foreach ($calls as $call) {
			$call_sid = $call->sid;
			break;
		}

		if (!$parameters['omg_options'][$omg_key]) {
			print "Access to that outgoing message is denied.";
			exit();
		}

		if (!$call) { 
			print 'Call not in progress.'; 
			exit();
		}

		// For some reason this doesn't work.
		//$twiml = '<Response>';
		//$twiml .= '<Say>Hello.</Say>';
		//$twiml .= '</Response>';
		//$call->update(array("twiml" => $twiml));

		global $base_url;
		$url = "$base_url/sirius/twilio/browserphone/twiml/omg/play?token=" . rawurlencode($token) . "&omg_key=$omg_key";
		$call->update(array('Url' => $url));
		print "Ok.";
		exit();
	} catch (Exception $e) {
		print "Error: " . $e->getCode() . ": " . $e->getMessage();
		exit();
	}
}

function sirius_twilio_browserphone_twiml_omg_play() {
	$token = $_REQUEST['token'];
	$omg_key = $_REQUEST['omg_key'];
	if (!$token || !$omg_key) { sirius_twilio_browserphone_twiml_dial_error("Missing token or message key."); }
	$parameters = sirius_twilio_browserphone_parameters_get($token);
	if (!$parameters) { sirius_twilio_browserphone_twiml_dial_error("Unable to load the parameters for this browser phone."); }
	if (!$parameters['omg_options'][$omg_key]) { sirius_twilio_browserphone_twiml_dial_error("Access to that outgoing message is denied."); }

	$omg_record = sirius_event_omg_fetch_from_key($omg_key);

	if ($omg_record['sms'] == 'Yes') {
		$sms_params = array();
		$sms_params['sms'] = $omg_record['sms_body'];
		$sms_params['to'] = $parameters['phone'];
		$result = sirius_sms_send($sms_params);
	}

	if ($omg_record['audio'] == 'Yes') {
		global $base_url;
		$url = "$base_url/sirius/twilio/browserphone/twiml/omg/stream?token=" . rawurlencode($token) . "&omg_key=$omg_key";
		header('Content-Type: text/xml');
		print "<Response><Play>" . htmlspecialchars($url, ENT_XML1 | ENT_QUOTES) . "</Play></Response>";
	} else {
		header('Content-Type: text/xml');
		print "<Response><Hangup/></Response>";
	}
	exit();
}

function sirius_twilio_browserphone_twiml_omg_stream() {
	$token = $_REQUEST['token'];
	$omg_key = $_REQUEST['omg_key'];
	if (!$token || !$omg_key) { sirius_twilio_browserphone_twiml_dial_error("Missing token or message key."); }
	$parameters = sirius_twilio_browserphone_parameters_get($token);
	if (!$parameters) { sirius_twilio_browserphone_twiml_dial_error("Unable to load the parameters for this browser phone."); }
	if (!$parameters['omg_options'][$omg_key]) { sirius_twilio_browserphone_twiml_dial_error("Access to that outgoing message is denied."); }
	list($audio_nid, $delta) = explode('-', $omg_key);
	$audio_node = node_load($audio_nid);
	$audio_item = $audio_node->field_sirius_audio['und'][$delta];

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

function sirius_twilio_browserphone_render($parameters = array()) {
	// Set up caller IDs
	$callerid_nids = $parameters['callerid_nids'];
	if (!$callerid_nids) {
		return sirius_twilio_browserphone_render_error('Please provide at least one caller ID.');
	}

	$callerid_nodes = node_load_multiple($callerid_nids);
	foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
		$parameters['callerid_options'][$callerid_nid] = $callerid_node->title;
	}

	$omg_nid = $parameters['omg_nid'];
	$omg_node = node_load($omg_nid);
	$omg_records = sirius_event_omg_list($omg_node);
	if ($omg_node && $omg_records) {
		$omg_options = array();
		foreach ($omg_records as $delta => $item) {
			$omg_options["$omg_nid-$delta"] = $item['title'];
		}
		$parameters['omg_options'] = $omg_options;
	}

	$conference_number = sirius_phone_format($parameters['conference_number']);

	global $user;

  drupal_add_library('system', 'ui.tabs');
	// drupal_add_js('https://media.twiliocdn.com/sdk/js/client/v1.6/twilio.min.js', 'external');
	drupal_add_js('sites/all/modules/_custom/sirius_twilio/js/twilio-voice.js/dist/twilio.js', 'file');
	drupal_add_js('sites/all/modules/_custom/sirius_twilio/js/sirius_twilio_browserphone_2.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_twilio') . '/css/sirius_twilio_browserphone.css');
	
	if (!$parameters['token']) {
		$client = sirius_twilio_client_get();
		$token = sirius_twilio_capability_token_get();

		$token .= '|' . $user->uid . '|' . mt_rand();
		$parameters['token'] = $token;
	}
	if (!$parameters['token']) {
		return '<div id="sirius_browserphone">' . t('Unable to obtain a capability token. Please check the Twilio settings.') . '</div>';
	}
	if (!$parameters['user']) { 
		$parameters['user'] = array('uid' => $user->uid, 'name' => $user->name);
	}

	drupal_add_js(array('sirius_twilio_browserphone' => $parameters), array('type' => 'setting'));
	sirius_twilio_browserphone_parameters_set($token, $parameters);


	$html = '<div id="sirius_browserphone">';
  $html .= '<div id="sirius_browserphone_tabs">';

  $html .= '<ul>';
  $html .= '<li><a href="#sirius_browserphone_tab_phone">Phone</a></li>';
  if ($conference_number) { $html .= '<li><a href="#sirius_browserphone_tab_conference">Conf</a></li>'; }
  $html .= '<li><a href="#sirius_browserphone_tab_keypad">Keypad</a></li>';
  $html .= '<li><a href="#sirius_browserphone_tab_settings">Settings</a></li>';
  $html .= '</ul>';

  $html .= '<div id="sirius_browserphone_tab_phone">';
	$html .= '  Phone:';
	$html .= '  <input id="sirius_browserphone_number" type="text" placeholder="Enter a phone #" value="" />';
	$html .= '  <br />';
	$html .= '  <button id="sirius_browserphone_button_call">Call</button>';
	$html .= '  <button id="sirius_browserphone_button_hangup">Hangup</button>';
	if ($omg_options) {
		$html .= '  <br />';
		$html .= '  <select id="sirius_browserphone_omg"></select>';
		$html .= '  <button id="sirius_browserphone_button_omg">Play Message</button>';
	}
	$html .= '</div>';

	if ($conference_number) {
	  $html .= '<div id="sirius_browserphone_tab_conference">';
	  $html .= '  Conference with: ' . $conference_number . '<br />';
		$html .= '  <button id="sirius_browserphone_button_conference">Start Conference</button>';
		$html .= '  <button id="sirius_browserphone_button_conference_add">Add Participant</button>';
		$html .= '  <button id="sirius_browserphone_button_conference_remove">Remove Participant</button>';
		$html .= '</div>';
	}

  $html .= '<div id="sirius_browserphone_tab_keypad">';
  $html .= '<table>';
  $html .= '<tr>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_1">1</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_2">2</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_3">3</button></td>';
  $html .= '</tr>';
  $html .= '<tr>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_4">4</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_5">5</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_6">6</button></td>';
  $html .= '</tr>';
  $html .= '<tr>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_7">7</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_8">8</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_9">9</button></td>';
  $html .= '</tr>';
  $html .= '<tr>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_s">*</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_0">0</button></td>';
  $html .= '<td><button id="sirius_browserphone_button_keypad_n">#</button></td>';
  $html .= '</tr>';
  $html .= '</table>';
	$html .= '  <div id="sirius_browserphone_volume">';
	$html .= '    <label>Mic</label>';
	$html .= '    <div id="sirius_browserphone_volume_input"></div><br/>';
	$html .= '    <label>Speaker</label>';
	$html .= '    <div id="sirius_browserphone_volume_output"></div>';
	$html .= '  </div>';
  $html .= '</div>';

  $html .= '<div id="sirius_browserphone_tab_settings">';
	$html .= ' <label>Devices</label>';
	$html .= ' <select id="sirius_browserphone_devices"></select>';
	$html .= ' <label>Caller ID</label>';
	$html .= ' <select id="sirius_browserphone_callerid"></select>';
	$html .= ' <div id="sirius_browserphone_log"></div>';
  $html .= '</div>';

  $html .= '</div>'; // Tabs
  $html .= '</div>'; // Wrapper

	return $html;
}

function sirius_twilio_browserphone_render_error($msg) {
	$html = '<div style="border: 1px solid #333333; padding: 20px; margin: 20px;">';
	$html .= t('Browser phone cannot load: ');
	$html .= '<br />';
	$html .= '<strong>';
	$html .= $msg;
	$html .= '</strong>';
	$html .= '</div>';

	return $html;
}

/**
 * Get and set parameters, indexed by token, during the life of a call.
 * @todo: It's kind of stupid that these functions aren't simply calls to get_context and set_context. What was I thinking?
 */

function sirius_twilio_browserphone_parameters_get_node($capability_token) {
  if (!$capability_token) { return NULL; }
  
  // We look for the most recent outgoing log entry of type "twilio:browserphone"
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:browserphone');
  $query->fieldCondition('field_sirius_notes', 'value', $capability_token);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  return node_load(array_keys($stmt['node'])[0]);
}

function sirius_twilio_browserphone_parameters_get($capability_token) {
	$log_node = sirius_twilio_browserphone_parameters_get_node($capability_token);
	return sirius_jsonfield_util_node_json_get($log_node); 
}

function sirius_twilio_browserphone_parameters_set($capability_token, $parameters) {
	$log_node = sirius_twilio_browserphone_parameters_get_node($capability_token);
	if ($log_node) {
		sirius_jsonfield_util_node_json_set($log_node, $parameters);
		node_save($log_node);
	} else {
	  sirius_log(
	    'twilio:browserphone', 
	    'parameters_set', 
	    $msg, 
	    $notes,
	    $capability_token, 
	    $parameters['handler_nid'],
	    NULL,
	    $parameters
	  );
	}
}
