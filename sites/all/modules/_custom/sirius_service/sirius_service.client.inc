<?php

function sirius_service_client_active() {
	return (sirius_domain_variable_get('sirius_service_client_active') == 'Yes');
}

function sirius_service_client_request() {
	// Are we an active client?
	if (!sirius_service_client_active()) { return array('success' => FALSE, 'msg' => 'Client is not active in the client configuration.'); }

	// Rudimentary cache lookup
	$cache =  &drupal_static(__FUNCTION__);
	if (!isset($cache)) { $cache = array(); }
	$func = $args[0];
	$info = sirius_service_func_info($func);
	if ($info['cache']) {
		$cache_key = json_encode($args);
		if ($cache[$cache_key]) {
			sirius_minilog("Cache hit: $cache_key");
			$result = $cache[$cache_key];
			$result['cache'] = TRUE;
			return $result;
		} else {
			sirius_minilog("Cache miss: $cache_key");
		}
	}

	// Get an implementation
	$instance = sirius_service_client_instance();
	if (!$instance) { return array('success' => FALSE, 'msg' => 'Client instance not specified.'); }

	// Make the request
	$args = func_get_args();
	$json = $instance->request($args);
	if (!$json['success']) { return $json; }

  // Rudimentary cache set
  if ($info['cache']) {
  	$cache_max = 100;
  	if (count($cache) < $cache_max) {
  		$cache[$cache_key] = $json;
	  	sirius_minilog("Cache set: $cache_key");
  	} else {
	  	sirius_minilog("Cache set skipped because cache is full.");
  	}
  }

  // If the server threw anything to drupal_set_message(), it should have been passed back to the client.
  // Let's show it here.
  if ($json['drupal_messages']) {
  	foreach ($json['drupal_messages'] as $type => $messages) {
  		foreach ($messages as $message) {
  			drupal_set_message($message, $type);
  		}
  	}
  }

	return $json;
}








function sirius_service_client_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_service_client_info');
    ksort($infos);
  }
  return $infos;
}

function sirius_service_client_info($type) {
  $infos = &drupal_static(__FUNCTION__); 
  if (!isset($infos)) { $infos = sirius_service_client_info_all(); }
  return $infos[$type];
}

function sirius_service_client_instance($type = NULL) {
	if (!$type) { $type = sirius_service_client_type(); }
  $info = sirius_service_client_info($type);
  return $info['instance'];
}

function sirius_service_client_type() {
	return sirius_domain_variable_get('sirius_service_client_type', 'default');
}

function sirius_service_client_settings() {
	$settings = json_decode(sirius_domain_variable_get('sirius_service_client_settings'), TRUE);

	// Legacy support
	if (!$settings && $type == 'default') {
		$settings['host'] = sirius_domain_variable_get('sirius_service_client_host');
		$settings['account'] = sirius_domain_variable_get('sirius_service_client_account');
		$settings['access'] = sirius_domain_variable_get('sirius_service_client_access');
	}

	return $settings;
}

/** 
 * Implements hook_sirius_service_client_info
 */

function sirius_service_sirius_service_client_info() {
  $infos = array();
  Sirius_Service_Client_Default::getInstance()->setInfo($infos);
  return $infos;
}

class Sirius_Service_Client extends Sirius_Singleton {
  public function settings_form() { return array(); }
  public function request($args) { return array('success' => FALSE, 'msg' => 'Root request() called. Classes must override this method.'); }
}

class Sirius_Service_Client_Default extends Sirius_Service_Client {
	public $type = 'default';
	public $name = 'Default Sirius Client';
	public $description = 'Default Sirius client; connects to a Sirius server.';

  public function settings_form() { 
	  $form['intro'] = array(
	  	'#markup' => '<p>' . 'This server\'s IP is listed as <strong>' . $_SERVER['SERVER_ADDR'] . '</strong>. That address must be allowed on the main server. Note that because this server may connect through a proxy, the listed IP may not be the same as the IP that is used for requests.' . '</p>'
	  );

		$form['host'] = array(
			'#type' => 'textfield',
			'#title' => 'Remote Host',
		);

		$form['account'] = array(
			'#type' => 'textfield',
			'#title' => 'Account ID',
		);

		$form['access'] = array(
			'#type' => 'textfield',
			'#title' => 'Access Token',
		);

		return $form;
	}

	public function request($args) {
		$settings = sirius_service_client_settings();

		// Get our connection variables
		$host = $settings['host'];
		if (!$host) { return array('success' => FALSE, 'msg' => 'No host specified in the client configuration.'); }

		$account = $settings['account'];
		if (!$account) { return array('success' => FALSE, 'msg' => 'No account ID specified in the client configuration.'); }

		$access = $settings['access'];
		if (!$access) { return array('success' => FALSE, 'msg' => 'No access code specified in the client configuration.'); }
		$access_obscure = substr($access, 0, 3) . preg_replace('/./', '*', substr($access, 3));

		sirius_minilog("Connecting with: Host: $host / account = $account / access = $access_obscure");

		// Get our URL
		$url = 'https://' . $host . '/sirius_service/sirius/generic.json';
		sirius_minilog("Connecting to: " . $url);

		// Make an HTTP request
	  $ch = curl_init();
	  curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json'));
		curl_setopt($ch, CURLOPT_HEADER, 0);
	  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($args));
		curl_setopt($ch, CURLOPT_USERPWD, $account . ":" . $access);  

	  $result = curl_exec($ch);

	  if (!$result) {
	  	$error = curl_error($ch);
	  	sirius_minilog("Connection error: $error");
	  	return array('success' => FALSE, 'msg' => $error);
	  }
	  curl_close($ch);

	  // Extract our result
	  $json = json_decode($result, TRUE);
	  if (!$json['success']) {
	  	$msg = $json['msg'];
	  	if (!$msg) { $msg = $json[0]; }
	  	if (!$msg) { $msg = "Server did not issue a valid response."; }
	  	return array('success' => FALSE, 'msg' => $msg);
	  }

	  // Do a utility cleanup, anything that looks like a node should become an object
	  $json = _sirius_service_array_to_node($json);

	  return $json;
	}
}
