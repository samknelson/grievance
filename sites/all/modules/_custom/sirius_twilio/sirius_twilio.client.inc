<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;

function sirius_twilio_client_get() {
	static $client;
	if (!$client) {
		require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_twilio/twilio-php/Twilio/autoload.php';
		// Autoload error with respect to TwiML, I think it has to do with capitalization
		// require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_twilio/twilio-php/Twilio/Twiml.php';
		
		$account_sid = sirius_domain_variable_get('sirius_twilio_account_sid', '');
		$account_token = sirius_domain_variable_get('sirius_twilio_account_token', '');
		$client = new Client($account_sid, $account_token);
	}

	return $client;
}

function sirius_twilio_capability_token_get() {
	global $user;

	// put your Twilio API credentials here
	$account_sid = sirius_domain_variable_get('sirius_twilio_account_sid', '');
	$account_token = sirius_domain_variable_get('sirius_twilio_account_token', '');
	$app_sid = sirius_domain_variable_get('sirius_twilio_app_sid', '');

	$capability = new ClientToken($account_sid, $account_token);
	$capability->allowClientOutgoing($app_sid);
	$capability->allowClientIncoming("dispatch");
	$token = $capability->generateToken();

	return $token;
}