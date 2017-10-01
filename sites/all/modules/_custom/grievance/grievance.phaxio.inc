<?php

function grievance_phaxio_admin() {
  $form = array();

  $form['grievance_phaxio_api_key'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_key', ''),
    '#title' => t('Phaxio API Key'),
    '#type' => 'textfield',
  );

  $form['grievance_phaxio_api_secret'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_secret', ''),
    '#title' => t('Phaxio API Secret'),
    '#type' => 'textfield',
  );

  $form['grievance_phaxio_api_host'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_host', 'https://api.phaxio.com/v1/'),
    '#title' => t('Phaxio API Host'),
    '#type' => 'textfield',
  );

  $form['grievance_phaxio_callback_token'] = array(
		'#default_value' => variable_get('grievance_phaxio_callback_token', ''),
    '#title' => t('Phaxio Callback Token'),
    '#type' => 'textfield',
  );

  return system_settings_form($form);
}

function grievance_phaxio_callback_sent() {
	watchdog('phaxio', '<pre>' . print_r($_REQUEST, 1) . '</pre>');
	$data = json_decode($_REQUEST['fax'], true);
	watchdog('phaxio', '<pre>' . print_r($data, 1) . '</pre>');

	$fax_id = $data['id'];
	if (!$fax_id) {
		watchdog('grievance_phaxio', "Callback - no ID found in json data.  Callback was not logged: <pre>" . print_r($_REQUEST, 1) . '</pre>');
		return;
	}

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_external_id', 'value', 'phaxio_id_sent_' . $fax_id);
  $stmt = $query->execute();
  if (!$stmt['node']) {
		watchdog('grievance_phaxio', "Callback - no matching log entry.  Callback was not logged: <pre>" . print_r($_REQUEST, 1) . '</pre>');
		return;
  }
  $original_log_nids = array_keys($stmt['node']);
  $original_log_nid = $original_log_nids[0];
  $original_log_node = node_load($original_log_nid);
  $grievance_nid = $original_log_node->field_grievance['und'][0]['target_id'];
  $grievance_node = node_load($grievance_nid);

  global $user;
  $log_node = new stdClass();
  if ($data['status'] == 'success') {
  	$log_node->title = 'Fax delivery succeeded.';
  } else {
  	$log_node->title = 'Fax delivery failed.';
  }
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance = $original_log_node->field_grievance;
  $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get(
  	'grievance_log_phaxio_callback_type',
  	$original_log_node->field_grievance_log_type['und'][0]['tid']
  );
  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s'); 
  $log_node->body['und'][0]['value'] = print_r($data, 1);
  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
}

function grievance_phaxio_callback_access($token) {
	if (!variable_get('grievance_phaxio_callback_token', '')) {
		watchdog('grievance_phaxio', 'A callback was received but no grievance_phaxio_callback_token has been configured.  Returning "Access Denied".');
		return FALSE;
	}

  if (variable_get('grievance_phaxio_callback_token', '') != $token) {
		watchdog('grievance_phaxio', 'A callback was received but the URL token [' . $token . '] does not match the configured grievance_phaxio_callback_token [' . variable_get('grievance_phaxio_callback_token', '') . '].  Returning "Access Denied".');
		return FALSE;
	}

	return TRUE;

	/*
	// I couldn't get this to work.
	if (!$all_headers['X-Phaxio-Signature']) {
		watchdog('grievance_phaxio', 'A callback was received without an X-Phaxio-Signature header.  Returning "Access Denied".');
		return FALSE;
	}

	if (!variable_get('grievance_phaxio_callback_token', '')) {
		watchdog('grievance_phaxio', 'A callback was received but no grievance_phaxio_callback_token has been configured.  Returning "Access Denied".');
		return FALSE;
	}

	global $base_url;
	$url = $base_url . $_SERVER['REQUEST_URI'];
  $postParameters = $_REQUEST;
  $uploadedFiles = $_FILES;
  ksort($postParameters);
  foreach ($postParameters as $key => $value) {
    $url .= $key . $value;
  }
  foreach ($uploadedFiles as $key => $value) {
    $url .= $key . sha1_file($value['tmp_name']);
  }
  watchdog('debug', 'DDD');
	watchdog('debug', 'My hash is ' . hash_hmac("sha1", $url, $all_headers['X-Phaxio-Signature'], FALSE));
	watchdog('debug', 'EEE');

  $hmac = hash_hmac("sha1", $url, $all_headers['X-Phaxio-Signature'], FALSE);

  watchdog('debug', 'Here with ' . $hmac);

  if (variable_get('grievance_phaxio_callback_token', '') != base64_encode($hmac)) {
		watchdog('grievance_phaxio', 'A callback was received but the X-Phaxio-Signature signed message [' . $all_headers['X-Phaxio-Signature'] . '] does not match the configured grievance_phaxio_callback_token [' . variable_get('grievance_phaxio_callback_token', '') . '].  Returning "Access Denied".');
		return FALSE;
	}
	*/

	return TRUE;
}