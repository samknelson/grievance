<?php

function sirius_proxy_active() {
	// The "Services" module must be enabled
	if (!module_exists('sirius_service')) { return FALSE; }

	// The Sirius component must be enabled
	if (!sirius_domain_component_enabled('service:client')) { return FALSE; }

	// The client must be active
	if (!sirius_service_client_active()) { return FALSE; }

	// Proxy mus be enabled
	if (sirius_domain_variable_get('sirius_proxy_active') != 'Yes') { return FALSE; }

	return TRUE;
}

function sirius_proxy_get_user_employer_nid() {
	global $user;
	$user_full = user_load($user->uid);

	if (!sirius_proxy_active()) {
		return $user_full->field_grievance_shops['und'][0]['target_id'];
	}

	$user_json = sirius_json_get($user_full);
	return $user_json['proxy']['employer_rnid'];
}

/**
 * A smart wrapper. For example, if you call with sirius_user_get(), it does a remote lookup by stored SSN
 */

function sirius_proxy_call_wrap() {
  $args = func_get_args();

  // If we're not in proxy mode, we just call locally
  if (!sirius_proxy_active()) {
    return sirius_proxy_call($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]);
  }

  $function = $args[0];

  if ($function == 'sirius_worker_get') {
    $uid = $args[1];
    global $user;
    if (!$uid) { $uid = $user->uid; }
    $user_full = user_load($uid);
    $json = sirius_json_get($user_full);
    $ssn = $json['proxy']['worker_ssn'];
    if (!$ssn) { return array('success' => TRUE, 'msg' => "No worker SSN has been set for this user.", 'data' => NULL); }
    return sirius_proxy_call('sirius_worker_get_by_ssn', $ssn);
  }

  return sirius_proxy_call($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]);
}

/**
 * Call a Sirius service. If this domain is configured as a client, then it will call the main server remotely. Otherwise, it will call locally.
 * 
 * Call with a function name, e.g. sirius_proxy_call('sirius_worker_get_by_ssn', '123-45-6789');
 * 
 * On success returns array(
 *   'success' => TRUE,
 *   'data' => ... the results of the function call ...
 * )
 * 
 * On failure returns array(
 *   'success' => FALSE,
 *   'msg' => 'Could not connect',
 * )
 */

function sirius_proxy_call() {
  $args = func_get_args();

  if (sirius_proxy_active()) {
    $result = sirius_service_client_request($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]);
    $result['is_remote'] = TRUE;
  } else {
    $callback = array_shift($args);
    $result = array(
      'success' => TRUE, 
      'data' => $callback($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]),
      'ts' => time(),
      'is_remote' => FALSE,
    );
  }

  $result = sirius_proxy_unwrap($result);

  return $result;
}

/**
 * Clean up results from a remote call
 * 
 * Later, we may want to unwrap the results of a successful client request which results in a failed function call. Convert e.g.
 * 
 * [
 *    'success' => TRUE,
 *    'data' => [
 *       'sucess' => FALSE,
 *       'msg' => 'Worker not found'
 *    ]
 * ]
 * 
 * Into
 * 
 * [
 *    'sucess' => FALSE,
 *    'msg' => 'Worker not found',
 * ]
 * 
 * At the moment we're only doing this:
 */

function sirius_proxy_unwrap($result) {
  if (!$result['success']) { return $result; }

  if (is_array($result['data']) && isset($result['data']['success'])) {
    foreach ($result['data'] as $key => $value) {
      $result[$key] = $value;
    }
  }

  return $result;
}

function sirius_proxy_node_load($nid) {
  $node = node_load($nid);
  if (!$node) {
    return array('success' => FALSE, 'msg' => "Node $nid not found.");
  }

  if ($node->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) {
    return array('success' => FALSE, 'msg' => "Node $nid is not in the server's domain.");
  }

  $ok_proxy_types = array(
    'sirius_json_definition', 
    'sirius_worker', 
    'sirius_contact', 
    'sirius_contact_relationship',
    'sirius_trust_benefit'
  );

  $type = $node->type;
  if (!in_array($type, $ok_proxy_types)) { 
    return array('success' => FALSE, 'msg' => "Node $nid is not one of the approved content types.");
  }

  if ($type == 'sirius_json_definition') {
    $subtype = $node->field_sirius_type['und'][0]['value'];
    if (!in_array($subtype, array('sirius_ledger:policy', 'sirius:disclaimer'))) {
      return array('success' => FALSE, 'msg' => "Node $nid is not one of the approved sirius_json_defintion types ($subtype).");
    }
  }

  return $node;
}