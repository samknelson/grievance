<?php

function sirius_service_user_tab_proxy_server($account_uid) {
	$account = user_load($account_uid);

	$forbidden_permissions = array(
		'sirius admin',
		'sirius service admin',
	);
	foreach ($forbidden_permissions as $forbidden_permission) {
		if (user_access($forbidden_permission, $account)) {
			return sirius_page_error("Users with the permission <strong>$forbidden_permission</strong> cannot be associated with remote users.");
		}
	}

	return drupal_get_form('sirius_service_user_tab_proxy_server_form', $account);
}

function sirius_service_user_tab_proxy_server_form($form, &$form_state, $account) {
	$form_state['account_uid'] = $account->uid;

	$form['#tree'] = TRUE;

	$account_json = sirius_json_get($account);

	$form['proxy']['server']['client_uuid'] = array(
		'#type' => 'textfield',
		'#title' => t('Client UUID'),
		'#default_value' => $account_json['proxy']['server']['client_uuid'],
		'#description' => 'Enter the UUID of the <strong>proxy client user</strong>. That user will have permission to run on the local server as if they were <strong>' . $account->name . '</strong>.',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;	
}

function sirius_service_user_tab_proxy_server_form_submit($form, &$form_state) {
	sirius_require('uuid', 'uuid.inc');

	$account = user_load($form_state['account_uid']);
	$account_json = sirius_json_get($account);
	$account_json['proxy']['server'] = $form_state['values']['proxy']['server'];

	$client_uuid = $form_state['values']['proxy']['server']['client_uuid'];
	if ($client_uuid) {
		if (!uuid_is_valid($client_uuid)) {
	    drupal_set_message("That doesn't look like a valid UUID.", 'error');
	    return;
	  }

	  $instance = sirius_service_server_instance('proxy');
  	$response = $instance->user_lookup(array('account_uuid' => $client_uuid));
  	if (!$response->success()) {
  		drupal_set_message("Lookup failed: " . $response->msg(), 'error');
  		return;
  	}

  	if ($response->account && ($response->account['uid'] != $account->uid)) {
  		drupal_set_message("That UUID is already associated with an account: " . $response->account['name'], 'error');
  		return;
  	}
  }

	sirius_json_set($account, $account_json);
	user_save($account);
	drupal_set_message("Settings saved.");
}