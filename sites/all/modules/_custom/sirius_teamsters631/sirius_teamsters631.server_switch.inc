<?php

function sirius_teamsters631_server_switch_leave() {
	$which = sirius_teamsters631_banner_which();
	if (!$which) { return "Don't know which site to redirect to."; }

	global $base_url;

	if ($which == 'uh') { $url = 'https://dispatch.631train.com'; }
	if ($which == 'tc') { $url = 'https://dispatch.teamsters631.com'; }
	if (!$url) { $url = $base_url; }

	$url .= "/teamsters631/switch/arrive?which=$which";

	global $user;
	$user_full = user_load($user->uid);

	$worker_node = sirius_worker_get();

	if ($worker_node) {
		$worker_id = $worker_node->field_sirius_id['und'][0]['value'];
		$worker_json = sirius_json_get($worker_node);
		$token = uuid_generate();
		$worker_json['teamsters631']['server_switch']['token'] = $token;
		$worker_json['teamsters631']['server_switch']['ts'] = time();
		sirius_json_set($worker_node, $worker_json);
		node_save($worker_node);

		$url .= "&worker_id=$worker_id&token=$token";
	}

	drupal_goto($url);
}

function sirius_teamsters631_server_switch_arrive() {
	require_once('modules/user/user.pages.inc');
	$destination = '<front>';

	$which_old = $_REQUEST['which'];
	if ($which_old == 'tc') { $which_new = 'uh'; }
	if ($which_old == 'uh') { $which_new = 'tc'; }
	if (!$which_old || !$which_new) { drupal_goto($destination); }

	if ($which_old == 'tc') {
		$which_old_name = 'Teamsters 631 Training Center';
		$which_old_nid = 2457403;
		$which_new_name = 'Teamsters 631 Union Hall';
		$which_new_nid = 2457401;
	} else {
		$which_old_name = 'Teamsters 631 Union Hall';
		$which_old_nid = 2457401;
		$which_new_name = 'Teamsters 631 Training Center';
		$which_new_nid = 2457403;
	}
	drupal_set_message("Welcome to $which_new_name.");
	global $user;

	// TESTING ONLY: If I'm already logged in but with the wrong domain, log me out.
	/*
	$user_full = user_load($user->uid);
	$user_domain_nid = $user_full->field_sirius_domain['und'][0]['target_id'];
	if ($user && $user->uid && $user_domain_nid && ($user_domain_nid != $which_new_nid)) {
	  module_invoke_all('user_logout', $user);
	  $user = NULL;
	}

	// TESTING ONLY: Switch into the new domain
	sirius_domain($which_new_nid);
	*/

	// If I'm already logged in to the new domain, just continue
	if ($user && $user->uid) { return sirius_teamsters631_switch_redirect('Already logged in.'); }

	// Check the token
	$token = $_REQUEST['token'];
	$worker_id = $_REQUEST['worker_id'];
	if (!$token || !$worker_id) { return sirius_teamsters631_switch_redirect('No token or worker_id.'); }

	// Load the new worker
	$new_worker = sirius_worker_get_by_id($worker_id);
	if (!$new_worker) { return sirius_teamsters631_switch_redirect("Worker $worker_id not found."); }
	$new_uid = sirius_worker_get_uid($new_worker->nid);
	if (!$new_uid) { return sirius_teamsters631_switch_redirect("No user found for worker $worker_id / " . $new_worker->nid); }

	// Check the token
	$result = sirius_service_client_request('sirius_teamsters631_switch_authenticate', $worker_id, $token);
	if (!$result['success']) { return sirius_teamsters631_switch_redirect($result['msg']); }
	if (!$result['data']) { return sirius_teamsters631_switch_redirect('Login failed.'); }

	// Login as the new worker
  drupal_session_regenerate();
  global $user;
	$new_user = user_load($new_uid);
  $user = $new_user;
  $edit = array(); // Passed by reference.
  user_module_invoke('login', $edit, $user);

  // Log the login
	sirius_log(
		'server_switch',
		'arrival',
		"Authenticated incoming worker " . $new_worker->title . ' as user ' . $new_user->name,
		'',
		'',
		NULL
	);


	return sirius_teamsters631_switch_redirect("Logged in as " . $new_worker->title);
}

function sirius_teamsters631_switch_authenticate($worker_id, $token) {
	// Must be a matching worker
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return FALSE; }
	$worker_json = sirius_json_get($worker_node);
	if (!$worker_json) { return FALSE; }

	// Worker must have a matching token
	if (!$token) { return FALSE; }
	if ($token != $worker_json['teamsters631']['server_switch']['token']) { return FALSE; }

	// Token must be less than 60 seconds old
	if (time() - 60 > $worker_json['teamsters631']['server_switch']['ts']) { return FALSE; }

	// Looks good!
	return TRUE;
}

function sirius_teamsters631_switch_redirect($msg) {
	drupal_set_message($msg);
	drupal_goto('<front>');
}

function sirius_teamsters631_is_remote($value = NULL) {
	$is_remote = &drupal_static(__FUNCTION__);
	if (isset($value)) { $is_remote = $value; }
	return $is_remote;
}

function sirius_teamsters631_sirius_node_tab_worker_contact_form_submit_alter($form, &$form_state) {
	if (sirius_teamsters631_is_remote()) { return; }

	$which = sirius_teamsters631_banner_which();
	if (!$which) { return; }
	$result = sirius_service_client_request('sirius_teamsters631_sirius_node_tab_worker_contact_form_submit_remote', $form, $form_state);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_sirius_node_tab_worker_contact_form_submit_remote($form, &$form_state) {
	sirius_teamsters631_is_remote(TRUE);

	$worker_id = $form_state['worker_node']->field_sirius_id['und'][0]['value'];
	$form_state['worker_node'] = sirius_worker_get_by_id($worker_id);
	return sirius_node_tab_worker_contact_form_submit($form, $form_state);
}






	
function sirius_teamsters631_sirius_worker_skill_set_alter(&$args) {
	if (sirius_teamsters631_is_remote()) { return; }

	$which = sirius_teamsters631_banner_which();
	if ($which != 'tc') { return; }

	list($worker_node, $tids, $expires, $action, $reason, $files, $certification_nid, $participant_nid) = $args;
	// On the remote server, TIDs are meaningless. So convert them into names.
	$term_names = array();
	foreach ($tids as $tid) {
		$term = taxonomy_term_load($tid);
		$term_names[] = $term->name;
	}

	$result = sirius_service_client_request('sirius_teamsters631_sirius_worker_skill_set_remote', $worker_node, $term_names, $expires, $action, $reason);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	else if (!$result['data']['success']) { drupal_set_message("Remote update failed: " . $result['data']['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_sirius_worker_skill_set_remote($worker_node_remote, $term_names, $expires, $action, $reason) {
	sirius_teamsters631_is_remote(TRUE);
	$reason .= ' (Imported from Training Center.)';

	$worker_id = $worker_node_remote->field_sirius_id['und'][0]['value'];
	if (!$worker_id) { return array('success' => FALSE, 'msg' => 'No ID for this worker.'); }
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "No worker found with id $worker_id"); }

	$term_name_lookup = sirius_taxonomy_load('sirius_skill', 'tid', 'name');
	$term_name_lookup = array_flip($term_name_lookup);

	$tids = array();
	foreach ($term_names as $term_name) {
		$tid = $term_name_lookup[$term_name];
		if (!$tid) { return array('success' => FALSE, 'msg' => "Skill not found: $term_name"); }
		$tids[] = $tid;
	}

	return sirius_worker_skill_set($worker_node, $tids, $expires, $action, $reason, array());
}







function sirius_teamsters631_sirius_worker_member_status_set_alter(&$args) {
	if (sirius_teamsters631_is_remote()) { return; }

	$result = sirius_service_client_request('sirius_teamsters631_sirius_worker_member_status_set_remote', $args);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	else if (!$result['data']['success']) { drupal_set_message("Remote update failed: " . $result['data']['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_sirius_worker_member_status_set_remote($args) {
	sirius_teamsters631_is_remote(TRUE);

	list($worker_node_remote, $term_remote, $tf, $reason, $attachments, $ts) = $args;

	$reason .= ' (Imported from remote server.)';

	$worker_id = $worker_node_remote->field_sirius_id['und'][0]['value'];
	if (!$worker_id) { return array('success' => FALSE, 'msg' => 'No ID for this worker.'); }
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "No worker found with id $worker_id"); }

	$term_name_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$term_name_lookup = array_flip($term_name_lookup);

	$term_name = $term_remote->name;
	$term_tid = $term_name_lookup[$term_name];
	$local_term = taxonomy_term_load($term_tid);
	if (!$local_term) { return array('success' => FALSE, 'msg' => "Member status $term_name not found."); }

	return sirius_worker_member_status_set($worker_node, $local_term, $tf, $reason);
}








function sirius_teamsters631_sirius_worker_work_status_set_alter(&$args) {
	if (sirius_teamsters631_is_remote()) { return; }
	if (!sirius_teamsters631_banner_which()) { return; }

	$result = sirius_service_client_request('sirius_teamsters631_sirius_worker_work_status_set_remote', $args);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	else if (!$result['data']['success']) { drupal_set_message("Remote update failed: " . $result['data']['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_sirius_worker_work_status_set_remote($args) {
	sirius_teamsters631_is_remote(TRUE);

	list($worker_node_remote, $term_remote, $reason, $attachments, $json, $ts) = $args;

	$reason .= ' (Imported from remote server.)';

	$worker_id = $worker_node_remote->field_sirius_id['und'][0]['value'];
	if (!$worker_id) { return array('success' => FALSE, 'msg' => 'No ID for this worker.'); }
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "No worker found with id $worker_id"); }

	$term_name_lookup = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$term_name_lookup = array_flip($term_name_lookup);

	$term_name = $term_remote->name;
	$term_tid = $term_name_lookup[$term_name];
	$local_term = taxonomy_term_load($term_tid);
	if (!$local_term) { return array('success' => FALSE, 'msg' => "Work status $term_name not found."); }

	return sirius_worker_work_status_set($worker_node, $local_term->tid, $reason, array(), $json);
}

function sirius_teamsters631_node_insert_worker_remote($rnode) {
	sirius_teamsters631_is_remote(TRUE);
	if ($rnode->type != 'sirius_worker') { return array('success' => FALSE, 'msg' => 'Not a worker node.'); }

	$rnode = (array) $rnode;

	global $user;

	$node = new stdClass();
	$node->type = 'sirius_worker';
	$node->uid = $user->uid;
	foreach ($rnode as $key => $value) {
		if (preg_match('/^field_/', $key)) {
			$node->{$key} = $value;
		}
	}
	unset($node->field_sirius_contact);

	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
	}
	$node->title = $rnode['title'];
 	node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;

  node_save($node);
	return array('success' => TRUE);
}


function sirius_teamsters631_taxonomy_term_updateinsert($term) {
	// Don't loop if we're already running remotely
	if (sirius_teamsters631_is_remote()) { return; }

	// Only run if we're TC or UH
	$banner_which = sirius_teamsters631_banner_which();
	if (!$banner_which) { return; }

	// Only interested in certain vocabularies
  $vid = $term->vid;
  $vocabulary = taxonomy_vocabulary_load($vid);
  $vocabulary_name = $vocabulary->machine_name;
  if (!in_array($vocabulary_name, array('sirius_skill'))) { return; }

  // Only sync one way
  if ( ($vocabulary_name == 'sirius_skill') && ($banner_which != 'tc') ) { return; }

  // Load the remote taxonomies, 
  $result = sirius_service_client_request('sirius_taxonomy_load', 'sirius_skill', 'tid', 'name');
  if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); return; }
  $remote_skill_lookup = array_flip($result['data']);

  $result = sirius_service_client_request('sirius_taxonomy_load', 'sirius_industry', 'tid', 'name');
  if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); return; }
  $remote_industry_lookup = array_flip($result['data']);

  $result = sirius_service_client_request('taxonomy_get_vocabularies');
  if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); return; }
  $remote_vocabulary_lookup = array();
  foreach ($result['data'] as $vid => $record) { $remote_vocabulary_lookup[$record['machine_name']] = $vid; }

  // Create our remote term
  $remote_term = clone $term;
  unset($remote_term->parent);
  unset($remote_term->original);
  unset($remote_term->uuid);
  unset($remote_term->field_sirius_domain);

  // Try to lookup the remote TID based on the new and original names. If we don't succeed, we're going to create a new term
  if ($term->original) { $original_name = $term->original->name; } else { $original_name = ''; }
  if ($original_name) { $remote_tid = $remote_skill_lookup[$original_name]; }
  if (!$remote_tid) { $remote_tid = $remote_skill_lookup[$term->name]; }
  $remote_term->tid = $remote_tid;

  // Set the remote vid
  $remote_vid = $remote_vocabulary_lookup[$vocabulary_name];
  if (!$remote_vid) { drupal_set_message("Vocabulary $vocabulary_name doesn't exist on the remote server.", 'warning'); return; }
	$remote_term->vid = $remote_vid;

	// Set the remote industries
	if ($term->field_sirius_industry['und']) {
		$remote_term->field_sirius_industry['und'] = array();
		foreach ($term->field_sirius_industry['und'] as $delta => $record) {
			$local_industry_term = taxonomy_term_load($record['tid']);
			$remote_industry_tid = $remote_industry_lookup[$local_industry_term->name];
			if ($remote_industry_tid) {
				$remote_term->field_sirius_industry['und'][] = array('tid' => $remote_industry_tid);
			}
		}
	}

	$result = sirius_service_client_request('sirius_teamsters631_taxonomy_term_updateinsert_remote', $remote_term);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	else if (!$result['data']['success']) { drupal_set_message("Remote update failed: " . $result['data']['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_taxonomy_term_updateinsert_remote($remote_term) {
	$remote_term = (object) $remote_term;
	if (sirius_domain_nid()) {
		$remote_term->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
	}
	sirius_teamsters631_is_remote(TRUE);
	taxonomy_term_save($remote_term);
	return array('success' => TRUE);	
}

function sirius_teamsters631_sirius_training_level_graduate_alter(&$args) {
	if (sirius_teamsters631_is_remote()) { return; }
	if (sirius_teamsters631_banner_which() != 'tc') { return; }
	$result = sirius_service_client_request('sirius_teamsters631_sirius_training_level_graduate_remote', $args);
	if (!$result['success']) { drupal_set_message("Remote connection failed: " . $result['msg'], 'warning'); }
	else if (!$result['data']['success']) { drupal_set_message("Remote update failed: " . $result['data']['msg'], 'warning'); }
	return $result;
}

function sirius_teamsters631_sirius_training_level_graduate_remote($args) {
	sirius_teamsters631_is_remote(TRUE);

	list($worker_node_remote, $term_remote, $level, $params) = $args;

	$worker_id = $worker_node_remote->field_sirius_id['und'][0]['value'];
	if (!$worker_id) { return array('success' => FALSE, 'msg' => 'No ID for this worker.'); }
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "No worker found with id $worker_id"); }

	$term_name_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$term_name_lookup = array_flip($term_name_lookup);

	$term_name = $term_remote->name;
	$term_tid = $term_name_lookup[$term_name];
	$local_term = taxonomy_term_load($term_tid);
	if (!$local_term) { return array('success' => FALSE, 'msg' => "Member status $term_name not found."); }

	return sirius_training_level_graduate($worker_node, $local_term, $level, $params);
}
