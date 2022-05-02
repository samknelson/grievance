<?php

function sirius_service_client_active() {
	return (sirius_domain_variable_get('sirius_service_client_active') == 'Yes');
}

function sirius_service_client_request() {
	if (!sirius_service_client_active()) { return array('success' => FALSE, 'msg' => 'Client is not active in the client configuration.'); }

	$host = sirius_domain_variable_get('sirius_service_client_host');
	if (!$host) { return array('success' => FALSE, 'msg' => 'No host specified in the client configuration.'); }

	$account = sirius_domain_variable_get('sirius_service_client_account');
	if (!$account) { return array('success' => FALSE, 'msg' => 'No account ID specified in the client configuration.'); }

	$access = sirius_domain_variable_get('sirius_service_client_access');
	if (!$access) { return array('success' => FALSE, 'msg' => 'No access code specified in the client configuration.'); }
	$access_obscure = substr($access, 0, 3) . preg_replace('/./', '*', substr($access, 3));

	sirius_minilog("Connecting with: Host: $host / account = $account / access = $access_obscure");

	$url = 'https://' . $host . '/sirius_service/sirius/generic.json';
	sirius_minilog("Connecting to: " . $url);

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json'));
	curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode(func_get_args()));
	curl_setopt($ch, CURLOPT_USERPWD, $account . ":" . $access);  

  $result = curl_exec($ch);

  if (!$result) {
  	$error = curl_error($ch);
  	sirius_minilog("Connection error: $error");
  	return array('success' => FALSE, 'msg' => $error);
  }
  curl_close($ch);

  $json = json_decode($result, TRUE);
  if (!$json['success']) {
  	$msg = $json['msg'];
  	if (!$msg) { $msg = $json[0]; }
  	if (!$msg) { $msg = "Server did not issue a valid response."; }
  	return array('success' => FALSE, 'msg' => $msg);
  }

  $json = _sirius_service_array_to_node($json);

	return $json;
}
