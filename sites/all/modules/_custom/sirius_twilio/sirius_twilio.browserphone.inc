<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;
use Twilio\TwiML;

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
			print_r($parameters);
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
		$url = "$base_url/sirius/twilio/browserphone/twiml/omg/play?token=$token&omg_key=$omg_key";
		$call->update(array('Url' => $url));
	} catch (Exception $e) {
		print "Error: " . $e->getCode() . ": " . $e->getMessage();
		exit();
	}

	print "Ok.";
	exit();
}

function sirius_twilio_browserphone_twiml_omg_play() {
	$token = $_REQUEST['token'];
	$omg_key = $_REQUEST['omg_key'];
	if (!$token || !$omg_key) { sirius_twilio_browserphone_twiml_dial_error("Missing token or message key."); }
	$parameters = sirius_twilio_browserphone_parameters_get($token);
	if (!$parameters) { sirius_twilio_browserphone_twiml_dial_error("Unable to load the parameters for this browser phone."); }
	if (!$parameters['omg_options'][$omg_key]) { sirius_twilio_browserphone_twiml_dial_error("Access to that outgoing message is denied."); }
	global $base_url;
	$url = "$base_url/sirius/twilio/browserphone/twiml/omg/stream?token=$token&omg_key=$omg_key";
	header('Content-Type: text/xml');
	print "<Response><Play>" . htmlspecialchars($url, ENT_XML1 | ENT_QUOTES) . "</Play></Response>";
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


function sirius_twilio_browserphone_twiml_dial_error($msg) {
	header('Content-Type: text/xml');
	print '<Response>';
	print '<Say>';
	print $msg;
	print '</Say>';
	print '</Response>';
	exit();
}

/*
 * @todo This function doesn't ever get called, as far as I can tell.
 */

function sirius_twilio_browserphone_twiml_dial() {
	return 'Broken.';
	// Get our input
	$token = $_REQUEST['token'];
	$phone = $_REQUEST['to'];
	$callerid_nid = $_REQUEST['callerid_nid'];

	// Get our parameters
	$parameters = sirius_twilio_browserphone_parameters_get($token);
	if (!$parameters) {
		sirius_twilio_browserphone_twiml_dial_error('Unable to load the parameters for this browser phone.');
	}

	// Get our phone number
	if (!sirius_phone_validate($phone)) {
		sirius_twilio_browserphone_twiml_dial_error('That does not appear to be a valid phone number.');
	}
	$phone = sirius_phone_format_e164($phone);

	// Get our Caller ID number
	if (!$callerid_nid) {
		sirius_twilio_browserphone_twiml_dial_error('Please select a Caller ID.');
	}
	if (!$parameters['callerid_options'][$callerid_nid]) {
		sirius_twilio_browserphone_twiml_dial_error('You do not have permission to use that Caller ID.');
	}
	$callerid_node = node_load($callerid_nid);
	$callerid_phone = sirius_phone_format_e164(sirius_fieldval($callerid_node, 'field_sirius_phone'));

	$parameters['phone'] = $phone;
	sirius_twilio_browserphone_parameters_set($token, $parameters);

	header('Content-Type: text/xml');
	print "<Response>";
	print "<Dial callerId=\"$callerid_phone\">$phone</Dial>";
	print "</Response>";
	exit();
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
	if ($omg_node && $omg_node->field_sirius_audio['und'][0]) {
		$omg_options = array();
		foreach ($omg_node->field_sirius_audio['und'] as $delta => $item) {
			$omg_options["$omg_nid-$delta"] = $item['description'];
		}
		$parameters['omg_options'] = $omg_options;
	}

	global $user;

  drupal_add_library('system', 'ui.tabs');
	drupal_add_js('https://media.twiliocdn.com/sdk/js/client/v1.6/twilio.min.js', 'external');
	drupal_add_js('sites/all/modules/_custom/sirius_twilio/js/sirius_twilio_browserphone.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_twilio') . '/css/sirius_twilio_browserphone.css');
	
	$client = sirius_twilio_client_get();
	$token = sirius_twilio_capability_token_get();
	$parameters['token'] = $token;
	drupal_add_js(array('sirius_twilio_browserphone' => $parameters), array('type' => 'setting'));
	sirius_twilio_browserphone_parameters_set($token, $parameters);


	$html = '<div id="sirius_browserphone">';
  $html .= '<div id="sirius_browserphone_tabs">';

  $html .= '<ul>';
  $html .= '<li><a href="#sirius_browserphone_tab_phone">Phone</a></li>';
  $html .= '<li><a href="#sirius_browserphone_tab_settings">Settings</a></li>';
  $html .= '<li><a href="#sirius_browserphone_tab_log">Log</a></li>';
  $html .= '</ul>';

  $html .= '<div id="sirius_browserphone_tab_phone">';
	$html .= '  Phone Number:<br />';
	$html .= '  <input id="sirius_browserphone_number" type="text" placeholder="Enter a phone #" value="" />';
	$html .= '  <br />';
	$html .= '  <button id="sirius_browserphone_button_call">Call</button>';
	$html .= '  <button id="sirius_browserphone_button_hangup">Hangup</button>';
	if ($omg_options) {
		$html .= '  <br />';
		$html .= '  <select id="sirius_browserphone_omg"></select>';
		$html .= '  <button id="sirius_browserphone_button_omg">Play OMG</button>';
	}
	$html .= '  <div id="sirius_browserphone_volume">';
	$html .= '    <label>Mic Volume</label>';
	$html .= '    <div id="sirius_browserphone_volume_input"></div><br/><br/>';
	$html .= '    <label>Speaker Volume</label>';
	$html .= '    <div id="sirius_browserphone_volume_output"></div>';
	$html .= '  </div>';
	$html .= '  <br />';
	$html .= '</div>';

  $html .= '<div id="sirius_browserphone_tab_settings">';
	$html .= ' <label>Devices</label>';
	$html .= ' <select id="sirius_browserphone_devices"></select>';
	$html .= ' <label>Caller ID</label>';
	$html .= ' <select id="sirius_browserphone_callerid"></select>';
  $html .= '</div>';

  $html .= '<div id="sirius_browserphone_tab_log">';
	$html .= '  <div id="sirius_browserphone_log"></div>';
  $html .= '</div>';

  $html .= '</div>'; // Tabs
  $html .= '</div>'; // Wrapper

	return $html;
}


function sirius_twilio_browserphone_parameters_get($capability_token) {
  if (!$capability_token) { return array(); }
  
  // We look for the most recent outgoing log entry of type "twilio:conversation"
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:browserphone');
  $query->fieldCondition('field_sirius_notes', 'value', $capability_token);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $log_node = node_load(array_keys($stmt['node'])[0]);
  return json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
}

function sirius_twilio_browserphone_parameters_set($capability_token, $parameters, $type = 'parameters_set', $msg = NULL, $notes = NULL) {
  $parameters_json = json_encode($parameters);

  sirius_log(
    'twilio:browserphone', 
    $type, 
    $msg, 
    $notes,
    $capability_token, 
    $parameters['handler_nid'],
    NULL,
    $parameters_json
  );
}
