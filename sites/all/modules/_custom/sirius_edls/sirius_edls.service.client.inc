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

		if (!$result['workers']) {
			sirius_minilog("... No workers found.");
			continue;
		}

		// Set the worker ID for the client server. This is:
		// - The EIN where avialable
		// - DISP+dispatch ID otherwise

		foreach ($result['workers'] as $delta => $worker) {
			$id = $worker['worker_ein'];
			if (!$id) { $id = 'DISP' . $worker['worker_id']; } 
			$id = 'O' . $id;
			$result['workers'][$delta]['worker_id'] = $id;
		}

		// Create lists by member status
		$workers_by_ms = array();
		foreach ($result['workers'] as $worker) {
			$ms = $worker['worker_ms'];
			$workers_by_ms[$ms][] = $worker;
		}

	  $ms_list = sirius_edls_outsource_ms_list($employer_node, 'name');
		foreach ($workers_by_ms as $ms => $workers) {
			if (!$ms_list[$ms]) {
				sirius_minilog("No member status configured for id $ms.", 'warning');
				continue;
			}

			$count = count($workers);
			sirius_minilog("... Assigning $count workers to member status: $ms: $ms_list[$ms]");
			sirius_edls_outsource_set_all_by_ms($employer_node, $workers, $ms);
		}
	}

	return array('success' => TRUE);
}

function sirius_edls_client_event_fetch($mode = 'test', $force = FALSE) {
	$result = sirius_edls_client_request('sirius_dispatch_group_search', array('domain_root' => 1, 'ts' => time(), 'limit' => 500));
	if (!$result['success']) { return $result; }
	$domain_nid = sirius_domain_nid();

	$count = $count_old = $count_new = 0;
	foreach ($result['data'] as $delta => $remote_node) {
		$uuid = $remote_node->uuid;
		if (!$uuid) {
			return array('success' => FALSE, 'msg' => "At least one remote node doesn't have a UUID. Something is wrong. Bailing out now.");
		}
		$external_id = $domain_nid . ':' . $uuid;

		++$count;

		$sql_args = array();
		$sql = "select nid from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "where type = 'sirius_dispatch_job_group' ";

		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;

		$sql .= "and json_denorm_external_id = :external_id ";
		$sql_args[':external_id'] = $external_id;

		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$local_nid = $hr['nid'];
		$local_node = node_load($local_nid);

		if ($local_node) {
			++$count_old;
		} else {
			++$count_new;

			global $user;
			$local_node = new stdClass();
			$local_node->type = $remote_node->type;
			node_object_prepare($local_node);
			$local_node->language = LANGUAGE_NONE;
			$local_node->uid = $user->uid;
			$local_node->status = 1;
			$local_node->promote = 0;
			$local_node->comment = 0;
		}

		$local_node->title = $remote_node->title;
		$local_node->field_sirius_datetime_completed = $remote_node->field_sirius_datetime_completed;
		$local_node->field_sirius_datetime = $remote_node->field_sirius_datetime;
		$local_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;

		$json = sirius_json_get($remote_node);
		$json['external_id'] = $external_id;
		sirius_json_set($local_node, $json);

		sirius_minilog("Writing record: " . $local_node->title);
		if ($mode == 'live') {
			node_save($local_node);
		} else {
			sirius_minilog("... (Skipping save because mode = $mode)");
		}
	}

	sirius_minilog("Found $count records. Updated $count_old, created $count_new.");

	return array('success' => TRUE);
}


class Sirius_Edls_Service_Client extends Sirius_Service_Client_Default {
	public $type = 'edls';
	public $name = 'EDLS Client';
  public $description = 'Client for the employer daily labor scheduler (EDLS).';
}
