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
	sirius_denorm_realtime(FALSE);
	$employers = sirius_edls_employer_list();
	foreach ($employers as $employer_nid => $employer_name) {
		sirius_minilog("Scanning: $employer_name");
		sirius_minilog_indent();
		sirius_edls_client_worker_fetch_employer($employer_nid, $mode, $force);
		sirius_minilog_outdent();
	}
}

function sirius_edls_client_worker_fetch_employer($employer_nid, $mode = 'test', $force = FALSE) {
	// Get the industry
	$industry_tid = sirius_taxonomy_lookup_by_anything('sirius_industry', 'CONVENTION');
	if (!$industry_tid) {
		sirius_minilog("Please create an industry for CONVENTION.", 'error');
		return;
	}

	// Get the member statuses
  $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $ms_list = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ms_map = array();
  foreach ($ms_terms as $ms_tid => $ms_term) {
  	$ms_map[$ms_term->field_sirius_id['und'][0]['value']] = $ms_tid;
  }

  // Get the employer
	$employer_node = node_load($employer_nid);
	$employer_json = sirius_json_get($employer_node);
	if ($employer_json['edls']['client']['active'] != 'Yes') {
		sirius_minilog("Employer is not active as a client.");
		return;
	}

  // Get the stub job
	$stub_nid = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid, TRUE);		
	if (!$stub_nid) {
		sirius_minilog("There is no stub job for this employer.", 'warning');
		return;
	}

	// Get the list of member statuses active for EDLS
	$active_ms_list = $employer_json['edls']['client']['active_ms_list'];
	if (!$active_ms_list) {
		sirius_minilog("No member statuses are configured for fetching.", 'warning');
		return;
	}
	$active_ms_list = array_filter(array_values($active_ms_list));

	// Fetch the workers
	sirius_minilog("Fetching workers");
	$employer_uuid = $employer_json['edls']['client']['uuid'];
	$employer_access_code = $employer_json['edls']['client']['access_code'];
	$result = sirius_edls_client_worker_list($employer_uuid, $employer_access_code);
	if (!$result['success']) {
		sirius_minilog("Fetch failed: " . $result['msg']);
		return;
	}
	if (!$result['workers']) {
		sirius_minilog("No workers found.");
		return;
	}

	// Create lists by member status
	$workers_by_ms = array();
	$dont_layoff_by_ein = array();
	foreach ($active_ms_list as $ms_tid) {
		$workers_by_ms[$ms_tid] = array();
	}			
	foreach ($result['workers'] as $worker) {
		$ms_tid = $ms_map[$worker['worker_ms']];
		if (!$ms_tid) { continue; }
		if (!isset($workers_by_ms[$ms_tid])) { continue; }

		$dispatch_id = $worker['worker_id'];
		$dispatch_id = trim(preg_replace('/^[A-Z]+/', '', $dispatch_id));
		if (!$dispatch_id) { continue; }
		$worker['dispatch_id'] = $dispatch_id;

		$ein = $worker['worker_ein'];
		if (!$ein) { $ein = $worker['worker_id']; }
		$worker['worker_ein'] = $ein;
		$dont_layoff_by_ein[$ein] = TRUE;

		$workers_by_ms[$ms_tid][] = $worker;
	}

	sirius_minilog_indent();
	foreach ($workers_by_ms as $ms_tid => $workers) {
		sirius_minilog_outdent();
		sirius_minilog("Scanning member status: $ms_tid: $ms_list[$ms_tid]");
		sirius_minilog_indent();

		if (!$ms_list[$ms_tid]) {
			sirius_minilog("No member status configured for ms_tid $ms_tid", 'warning');
			continue;
		}

		$count = count($workers);
		sirius_minilog("Assigning $count workers.");

		// Create worker records
		foreach ($workers as $worker) {
			$need_save = FALSE;

			$worker_node =  sirius_worker_get_by_field('field_sirius_id2', $worker['dispatch_id']);

			if (!$worker_node) {
		    global $user;
		    $domain_nid = sirius_domain_nid();
		    $worker_node = new stdClass();
		    $worker_node->type = 'sirius_worker';
		    $worker_node->language = LANGUAGE_NONE;
		    $worker_node->uid = $user->uid;
		    $worker_node->status = 1;
		    $worker_node->promote = 0;
		    $worker_node->comment = 0;
		    $worker_node->title = $worker['worker_name'];
		    $worker_node->field_sirius_id['und'][0]['value'] = $worker['worker_ein'];
		    if ($domain_nid) {
					$worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
		    }
		    $worker_node->field_sirius_id2['und'][0]['value'] = $worker['dispatch_id'];
		    $need_save = TRUE;
			}

			list ($family, $given) = explode(',', $worker['worker_name']);
			$family = sirius_worker_name_canonicalize_part($family);
			$given = sirius_worker_name_canonicalize_part($given);

			if ($family != $worker_node->field_sirius_name['und'][0]['family']) {
				$worker_node->field_sirius_name['und'][0]['family'] = $family;
				$need_save = TRUE;
			}

			if ($given != $worker_node->field_sirius_name['und'][0]['given']) {
				$worker_node->field_sirius_name['und'][0]['given'] = $given;
				$need_save = TRUE;
			}

			$phone = $worker['worker_phone'];
			if ($phone != $worker_node->field_sirius_phone['und'][0]['value']) {
				$worker_node->field_sirius_phone['und'][0]['value'] = $phone;
				$need_save = TRUE;
			}

			if ($worker_node->field_sirius_id['und'][0]['value'] != $worker['worker_ein']) {
				$worker_node->field_sirius_id['und'][0]['value'] = $worker['worker_ein'];
				$need_save = TRUE;
			}

			if ($worker_node->field_sirius_member_status['und'][0]['tid'] != $ms_tid) {
				$worker_node->field_sirius_member_status['und'][0]['tid'] = $ms_tid;
				$need_save = TRUE;
			}

			if ($need_save) {
				node_save($worker_node);
				sirius_minilog("Saved worker record: " . $worker_node->title);
				++$xyz; if ($xyz > 100) { break; }
			}

			// Create or update a dispatch. Do we have an existing accepted?
			$params = array();
			$params['worker_nid'] = $worker_node->nid;
			$params['employer_nid'] = $employer_nid;
			$params['status'] = 'accepted';
			$params['ts'] = time();
			$params['count_only'] = TRUE;
			$count = sirius_dispatch_search($params);
			if ($count) { continue; }

			// If not, do we have any dispatch at all? We're just going to update the most recrent to re-accept.
			$params = array();
			$params['worker_nid'] = $worker_node->nid;
			$params['employer_nid'] = $employer_nid;
			$dispatch_nodes = sirius_dispatch_search($params);
			$dispatch_node = reset($dispatch_nodes);
			if ($dispatch_node) {
				sirius_minilog("Rehired worker record: " . $worker_node->title);
				$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'accepted';
				unset($dispatch_node->field_sirius_date_end['und']);
				node_save($dispatch_node);
				continue;
			}

			// No dispatch found. 
			sirius_minilog("Creating employment record: " . $worker_node->title);
			$emphist_record = array();
			$emphist_record['job_nid'] = $stub_nid;
			$emphist_record['type'] = 'Primary';
			$emphist_record['start'] = '2000-01-01';
			$emphist_record['worker_nid'] = $worker_node->nid;
			$result = sirius_dispatch_emphist_save($emphist_record, $dispatch_node->nid);
			if (!$result['success']) {
				sirius_minilog("Failed to save employment history for " . $worker_node->title . ': ' . $result['msg'], 'warning');
			}
		}
	}
	sirius_minilog_outdent();

	// Fetch the current workers so that we can find some to lay off
	sirius_minilog("Laying off workers.");
	$layoff_candidates = sirius_edls_dispatch_list($employer_nid, date('Y-m-d'), array('skip_assignment' => TRUE));
	sirius_minilog_indent();
	$layoff_count = 0;
	foreach ($layoff_candidates as $delta => $worker) {
		$ms_tid = $worker['worker_ms'];

		if (!$ms_tid) { continue; }
		if (!isset($workers_by_ms[$ms_tid])) { continue; }

		$ein = preg_replace('/^I/', '', $worker['worker_id']);
		if (!$ein) { continue; }
		if ($dont_layoff_by_ein[$ein]) { continue; }

		$worker_nid = sirius_worker_get_by_field('field_sirius_id', $ein, TRUE);
		if (!$worker_nid) { continue; }

		$params = array();
		$params['worker_nid'] = $worker_nid;
		$params['employer_nid'] = $employer_nid;
		$params['status'] = 'accepted';
		$params['ts'] = time();
		$dispatch_nodes = sirius_dispatch_search($params);
		$dispatch_node = reset($dispatch_nodes);
		if (!$dispatch_node) { continue; }

		sirius_minilog("Laying off $worker[worker_name] ($ein)");
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'terminated';
		$dispatch_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d', strtotime('-1 days'));
		node_save($dispatch_node);
		++$layoff_count;
	}
	sirius_minilog("Laid off $layoff_count workers.");
	sirius_minilog_outdent();

	return array('success' => TRUE);
}

//
// UGLY CODE WARNING.
//
// EVENTS
// - In EDLS, it's called an "Event". Everywhere else, it's called a "Dispatch Job Group".
// - When I fetch it, I pull the entire node, and map the UUID into a JSON "External ID" field.
// 
// FACILITIES
// - When I fetch it, I pull just the NID and title, and map the NID into the "field_sirius_id" field.
// 
// WHAT THE HECK?
// Facilities don't have a JSON field. They should. But I can't push through a structure change this morning.
// 
// And moreover, I have an existing function to fetch a "facility dropdown" which pulls just the NID and the title.
// But I don't have a "search" function to pull all the facility nodes.
// And even if I did, I'd be worried about fetching hundreds or thousands of full facility nodes all at once.
//
// So ... rather than messing with the structure on the live dispatch site, we're living with an ugly structure.
// 


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

function sirius_edls_client_facility_fetch($mode = 'test', $force = FALSE) {
	$result = sirius_edls_client_request('sirius_dispatch_facility_dropdown', array('domain_root' => 1, 'limit' => 500));
	if (!$result['success']) { return $result; }

	unset($result['data'][NULL]);
	if (!$result['data']) { return array('success' => TRUE, 'msg' => 'No facilities found.'); }

	$domain_nid = sirius_domain_nid();

	$count = $count_old = $count_new = 0;
	foreach ($result['data'] as $external_nid => $title) {
		$external_id = $domain_nid . ':' . $external_nid;

		++$count;

		$sql_args = array();
		$sql = "select nid from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
		$sql .= "where type = 'sirius_dispatch_facility' ";

		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;

		$sql .= "and field_sirius_id_value = :external_id ";
		$sql_args[':external_id'] = $external_id;

		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$local_nid = $hr['nid'];
		$local_node = node_load($local_nid);

		// Shortcut for efficiency.
		if ($local_node->title == $title) { 
			sirius_minilog("Unchanged: $title");
			continue;
		}


		if ($local_node) {
			++$count_old;
		} else {
			++$count_new;

			global $user;
			$local_node = new stdClass();
			$local_node->type = 'sirius_dispatch_facility';
			node_object_prepare($local_node);
			$local_node->language = LANGUAGE_NONE;
			$local_node->uid = $user->uid;
			$local_node->status = 1;
			$local_node->promote = 0;
			$local_node->comment = 0;
		}

		$local_node->title = $title;
		$local_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
		$local_node->field_sirius_id['und'][0]['value'] = $external_id;

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
