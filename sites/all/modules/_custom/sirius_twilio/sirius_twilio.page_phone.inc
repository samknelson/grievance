<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;

function sirius_twilio_page_phone() {

	drupal_add_js('https://media.twiliocdn.com/sdk/js/client/v1.6/twilio.min.js', 'external');
	drupal_add_js('sites/all/modules/_custom/sirius_twilio/js/sirius_twilio.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_twilio') . '/css/sirius_twilio.css');

	try {
		$client = sirius_twilio_client_get();
		$token = sirius_twilio_capability_token_get();
		drupal_add_js(array('sirius_twilio' => array('token' => $token)), array('type' => 'setting'));

		$capability = new ClientToken($accountSid, $authToken);
		$capability->allowClientOutgoing($appSid);
		$capability->allowClientIncoming('joey');
		$token = $capability->generateToken();

	} catch (Exception $e) {
		drupal_set_message("Error: " . $e->getCode() . ": " . $e->getMessage(), 'error');
	}

	$html .= <<<EHTML
  <div id="controls">
    <div id="info">
      <div id="output-selection">
        <label>Ringtone Devices</label>
        <select id="ringtone-devices" multiple></select>
        <label>Speaker Devices</label>
        <select id="speaker-devices" multiple></select><br/>
        <a id="get-devices">Seeing unknown devices?</a>
      </div>
    </div>
    <div id="call-controls">
      <p class="instructions">Make a Call:</p>
      <input id="phone-number" type="text" placeholder="Enter a phone # or client name" />
      <button id="button-call">Call</button>
      <button id="button-hangup">Hangup</button>
      <div id="volume-indicators">
        <label>Mic Volume</label>
        <div id="input-volume"></div><br/><br/>
        <label>Speaker Volume</label>
        <div id="output-volume"></div>
      </div>
    </div>
    <div id="log"></div>
  </div>
EHTML;

	return $html;
}