<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;

function sirius_twilio_client_get($account_sid = NULL, $account_token = NULL) {
	static $client;
	if (!$client) {
		require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_twilio/twilio-php/Twilio/autoload.php';
		// Autoload error with respect to TwiML, I think it has to do with capitalization
		// require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_twilio/twilio-php/Twilio/Twiml.php';
		if (!$account_sid) { $account_sid = sirius_domain_variable_get('sirius_twilio_account_sid', ''); }
		if (!$account_token) { $account_token = sirius_domain_variable_get('sirius_twilio_account_token', ''); }
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

/**
 * @todo: This function should move into a different file
 * 
 * @todo: There doesn't seem to be any way to fetch a count from Twilio except by fetching all the calls and then counting them.
 *        Irritating and expensive but true. See
 *        https://www.twilio.com/blog/replacing-absolute-paging-and-related-properties
 * 
 * @todo: This function cannot possibly return a count larger than 1000, because that's the maximum page size and we don't page.
 * 
 * @todo: This function can only count a single status at a time, because that's a limitation from Twilio. But if, for example, 
 *        we want to count all calls that are "ringing", "queued", and "in-progress", we have to iterate through the statuses.
 *        this runs the risk of counting a call twice or not at all, depending on the order of the queries.
 * 
 * @todo: This is a terribly expensive function, since it fetches up to 1000 full records from Twilio and then returns just a count.
 */

function sirius_twilio_call_count_yucky($params, $limit = 100) {
	try {
		$client = sirius_twilio_client_get();
		$calls = $client->calls->read($params, 1000);
		if (!$calls) { $calls = array(); }
		return array('success' => TRUE, 'count' => count($calls));
	} catch (Exception $e) {
		return array('success' => FALSE, 'msg' => "Twilio query failed: " . $e->getCode() . ' - ' . $e->getMessage());
	}

	return array('success' => TRUE, 'count' => 50);
}