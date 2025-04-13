<?php

/**
 * Implements hook_sirius_service_func_info_all_alter
 * 
 * The right thing to do would be to implement client and server objects. But screw it, we're just going 
 * to implement functions and enable and disable them.
 */

function sirius_edls_sirius_service_func_info_all_alter(&$info) {
	// These functions are only enabled if we are a server.
	if (!sirius_domain_component_enabled('edls:service:server')) { return; }

  $settings = sirius_domain_variable_get('sirius_service_server_funcs', array());

  $info['sirius_edls_server_worker_list'] = array(
    'callback' => 'sirius_edls_server_worker_list',
    'enabled' => $settings['sirius_edls_server_worker_list']['enabled'],
  );

  $info['sirius_edls_server_employer'] = array(
    'callback' => 'sirius_edls_server_employer',
    'enabled' => $settings['sirius_edls_server_employer']['enabled'],
  );
}

function sirius_edls_server_employer($employer_uuid, $employer_access_code) {
	if (!$employer_uuid) { return array('success' => FALSE, 'msg' => 'Missing required employer_uuid.'); }
	if (!$employer_access_code) { return array('success' => FALSE, 'msg' => 'Missing required employer_access_code.'); }

	// Why is there no function node_load_by_uuid()?
	$nids = entity_get_id_by_uuid('node', array($employer_uuid), FALSE);
	if (!$nids) { return array('success' => FALSE, 'msg' => 'Employer not found with the given UUID.'); }
	$employer_nid = reset($nids);
	$employer_node = node_load($employer_nid);
	if ($employer_node->type != 'grievance_shop') { return array('success' => FALSE, 'msg' => 'Employer not found with the given UUID [ERR2].'); }

	//
	// We allow legacy support for employers in the root domain. This is not a good arrangement, but the
	// only user, Teamsters631, has all their data in the root domain, and has different servers running in different domains. So
	// this is ick.
	//

	if ($employer_node->field_sirius_domain['und'][0]['target_id']) {
		$domain_nid = sirius_domain_nid();
		if ($domain_nid > 0) {
			if ($employer_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { 
				return array('success' => FALSE, 'msg' => 'Employer found, but it is not in the correct domain.');
			}
		}
	} else {
		sirius_domain(-1);
	}

	$employer_json = sirius_json_get($employer_node);
	$check = $employer_json['edls']['server']['access_code'];
	if (!$check) {
		return array('success' => FALSE, 'msg' => 'There is no emloyer access code on the server. Connections to this employer are not allowed.');
	}

	if ($employer_access_code != $check) {
		return array('success' => FALSE, 'msg' => 'Incorrect access code.');
	}

	return array('success' => TRUE, 'employer_nid' => $employer_nid);
}

function sirius_edls_server_worker_list($employer_uuid, $employer_access_code) {
	$result = sirius_edls_server_employer($employer_uuid, $employer_access_code);
	if (!$result['success']) { return $result; }

	$employer_nid = $result['employer_nid'];

	$filters = array();
	$filters['skip_assignment'] = TRUE;
	$filters['include_ein'] = TRUE;
	$filters['include_phone'] = TRUE;
	$workers = sirius_edls_worker_list($employer_nid, date('Y-m-d'), $filters);

	return array('success' => TRUE, 'workers' => $workers);
}

/*
// @todo: Stupid class loading order. Doing this to make sure that the class is loaded first. 
//        It is time to get autoloading working.

sirius_require('sirius_service', 'sirius_service.server.inc');

/** 
 * Implements hook_sirius_service_server_info
 *

function sirius_edls_sirius_service_server_info() {
  $infos = array();
  Sirius_Edls_Service_Server::getInstance()->setInfo($infos);
  return $infos;
}

class Sirius_Edls_Service_Server extends Sirius_Service_Server {
  public $type = 'default';
  public $name = 'EDLS Server';
  public $description = 'Server for the employer daily labor scheduler (EDLS).';

  public function settings_form() { 
    return sirius_form_error("How about some settings?");
  }

  public function respond($request) { 
    return $this->respond_error('I am an EDLS server. Hear me roar.');
  }
}
*/

