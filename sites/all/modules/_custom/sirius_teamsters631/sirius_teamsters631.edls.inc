<?php

/**
 * Implements hook_sirius_edls_notify_pre_send_alter
 */

function sirius_teamsters631_sirius_edls_notify_pre_send_alter(&$context) {
	$employer_json = sirius_json_get($context['employer_node']);

	// Set the "From"
	$context['notify_params']['sms_from'] = $employer_json['edls']['teamsters631']['twilio']['phone'];

	// Change the client
	sirius_teamsters631_edls_twilio_client_get($context['employer_node']);
}

/**
 * Implements hook_sirius_edls_notify_post_send_alter
 */

function sirius_teamsters631_sirius_edls_notify_post_send_alter(&$context) {
	// Restore
	sirius_teamsters631_edls_twilio_client_get(NULL);
}

function sirius_teamsters631_edls_twilio_client_get($employer_node) {
	// Cache all Twilio connections by employer NID
	$cache = &drupal_static(__FUNCTION__);
	if (!isset($cache)) { $cache = array(); }

	// Remember our default client
	if (!isset($cache['DEFAULT'])) {
		$cache['DEFAULT'] = sirius_twilio_client_get();
	}

	// Which client do we want?
	if ($employer_node) {
		$cache_key = $employer_node->nid;
	} else {
		$cache_key = 'DEFAULT';
	}

	// Connect if necessary
	if (!isset($cache[$cache_key])) {
		$employer_json = sirius_json_get($employer_node);

    $account_sid = $employer_json['edls']['teamsters631']['twilio']['account_sid'];
    $account_token = $employer_json['edls']['teamsters631']['twilio']['account_token'];
    
    if ($account_sid) {
			$cache[$cache_key] = sirius_twilio_client_connect(array(
				'provider' => sirius_twilio_provider_get(), 
				'account_sid' => $account_sid,
				'account_token' => $account_token,
			));
		} else {
			$cache[$cache_key] = $cache['DEFAULT'];
		}
	}

	return sirius_twilio_client_get($cache[$cache_key]);
}