<?php

function sirius_service_user_tab_proxy_client($account_uid) {
	$account = user_load($account_uid);
	$account_uuid = $account->uuid;
	if (!$account_uuid) { return sirius_page_error("Account UUID not found."); }

	$request = new Sirius_Server_Request();
	$request->server_type = 'proxy';
	$request->method = 'user_lookup';
	$request->args = array('account_uuid' => $account_uuid);
	$raw_result = sirius_service_client_request('sirius_service_server_call', serialize($request));
	if (!$raw_result['success']) { return sirius_page_error($raw_result['msg']); }
	$result = unserialize($raw_result['data']);
	if (!$result->success()) { return sirius_page_error($result->msg()); }


	$html .= '<h2>' . t('Client UUID') . '</h2>';
	$html .= "<p>";
	$html .= "The UUID for this account is";
	$html .= "<br /><br />";
	$html .= "<strong>$account_uuid</strong>";
	$html .= "<br /><br />";
	$html .= "This must be entered into the account settings for a valid user on the server.";
	$html .= "</p>";
	$html .= '<h2>' . t('Lookup') . '</h2>';
	$html .= $result->msg();

	return $html;
}