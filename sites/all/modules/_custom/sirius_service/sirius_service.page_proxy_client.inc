<?php 

function sirius_service_page_proxy_client() {
	global $user;
	$request = new Sirius_Server_Request();
	$request->server_type = 'proxy';
	$request->method = 'page_fetch';
	$request->args = array();
	$request->args['account_uuid'] = $user->uuid;
	$request->args['path'] = join('/', func_get_args());

	$raw_result = sirius_service_client_request('sirius_service_server_call', serialize($request));

	if (!$raw_result['success']) {
		return sirius_page_error($raw_result['msg']);
	}

	$result = unserialize($raw_result['data']);
	if (!$result->success()) {
		return sirius_page_error($result->msg());
	}

	return $result->msg();
}