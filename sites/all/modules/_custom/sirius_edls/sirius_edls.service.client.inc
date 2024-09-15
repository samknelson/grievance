<?php

/** 
 * Implements hook_sirius_service_client_info
 */

function sirius_edls_sirius_service_client_info() {
  $infos = array();
  Sirius_Edls_Service_Client::getInstance()->setInfo($infos);
  return $infos;
}

// @todo: Stupid class loading order. Doing this to make sure that the class is loaded first. 
//        It is time to get autoloading working.

sirius_require('sirius_service', 'sirius_service.client.inc');

// Utility wrapper function, mostly to 'unwrap' a response. We might have e.g.
// [success=FALSE, msg=Bad Host]
// But we might have
// [success=TRUE, data=[success=FALSE, msg=Bad Employer UUID]]
// So this unwraps the data if it looks like a result.

function sirius_edls_client_request() {
	$args = func_get_args();

	$result = sirius_service_client_request($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]);

	if (!$result['success']) { return $result; }
	if (isset($result['data']['success'])) { return $result['data']; }
	return $result;
}

function sirius_edls_client_employer($employer_uuid, $employer_access_code) {
	return sirius_edls_client_request('sirius_edls_server_employer', $employer_uuid, $employer_access_code);
}

function sirius_edls_client_worker_list($employer_uuid, $employer_access_code) {
	return sirius_edls_client_request('sirius_edls_server_worker_list', $employer_uuid, $employer_access_code);
}

function sirius_edls_client_worker_fetch($mode = 'test', $force = FALSE) {
	$employers = sirius_edls_employer_list();
	foreach ($employers as $employer_nid => $employer_name) {
		sirius_minilog("Scanning $employer_name [$employer_nid]");
		$employer_node = node_load($employer_nid);
		$employer_json = sirius_json_get($employer_node);
		if ($employer_json['edls']['client']['active'] != 'Yes') {
			sirius_minilog("... Employer is not active as a client.", 'warning');
			continue;
		}

		sirius_minilog("... Fetching workers");
		$employer_uuid = $employer_json['edls']['client']['uuid'];
		$employer_access_code = $employer_json['edls']['client']['access_code'];
		$result = sirius_edls_client_worker_list($employer_uuid, $employer_access_code);
		if (!$result['success']) {
			sirius_minilog("... Fetch failed: " . $result['msg']);
			continue;
		}
	}

	return array('success' => TRUE);
}

class Sirius_Edls_Service_Client extends Sirius_Service_Client_Default {
	public $type = 'edls';
	public $name = 'EDLS Client';
  public $description = 'Client for the employer daily labor scheduler (EDLS).';
}
