<?php

function sirius_worker_get_user($worker_nid) {
	$sql = "select entity_id from field_data_field_sirius_worker ";
	$sql .= "where entity_type = 'user' ";
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
	$hr = $stmt->fetchAssoc();
	$worker_uid = $hr['entity_id'];
	if (!$worker_uid) { return NULL; }
	return user_load($worker_uid);
}

function sirius_worker_get($uid = NULL) {
	global $user;
	if (!$uid) { $uid = $user->uid; }
	$user_full = user_load($uid);
	$worker_nid = $user_full->field_sirius_worker['und'][0]['target_id'];
	return node_load($worker_nid);
}

function sirius_worker_usersync($worker_nid) {
	$worker_node = node_load($worker_nid);
	if (!$worker_node) {
		return array('success' => FALSE, 'msg' => "Worker #$worker_nid not found.");
	}
	if ($worker_node->type != 'sirius_worker') {
		return array('success' => FALSE, 'msg' => "Node #$worker_nid is not of type 'sirius_worker'.");
	}

	// Get an ID
	$id = $worker_node->field_sirius_id['und'][0]['value'];
	if (!$id) {
		return array('success' => FALSE, 'msg' => "This worker does not have an ID. Sync is not possible.");
	}

	// Get an aat (aka "password")
	require_once('includes/password.inc');
	$aat = $worker_node->field_sirius_aat['und'][0]['value'];
	if (!$aat) {
		node_save($worker_node);
		$aat = $worker_node->field_sirius_aat['und'][0]['value'];
	}
	if (!$aat) {
		return array('success' => FALSE, 'msg' => "No AAT found or generated. Sync is not possible.");
	}

	// Get an email. If we can't find one, fake it.
	$email = $worker_node->field_sirius_email['und'][0]['value'];
	if (!$email) { 
		$email = "worker_" . $id . "@" . variable_get('sirius_worker_dummy_domain', 'nodomain.com');
	}

	// Get the user
	$worker_user = sirius_worker_get_user($worker_nid);

	// Let's figure out the roles. First get all the available worker roles.
	foreach (array_values(variable_get('sirius_worker_roles', array())) as $rid) {
		$all_rids[$rid] = TRUE;
	}
	

	// Next, get all the user's member statuses.
	$member_status_tids = array();
	if ($worker_node->field_sirius_member_status['und']) {
		foreach ($worker_node->field_sirius_member_status['und'] as $tmp) {
			$member_status_tids[] = $tmp['tid'];
		}
	}

	// Next, get all the RIDs which are checked as associated with one of the member statuses
	$yes_rids = array(DRUPAL_AUTHENTICATED_RID => TRUE);
	foreach ($member_status_tids as $member_status_tid) {
		$member_status_term = taxonomy_term_load($member_status_tid);
		if ($member_status_term->field_sirius_roles['und']) {
			foreach ($member_status_term->field_sirius_roles['und'] as $tmp) {
				$yes_rids[$tmp['value']] = TRUE;
			}
		}
	}
	$no_rids = array_diff_assoc($all_rids, $yes_rids);

	// Next, get the current rids. We're going to add the YES's, and remove the NO's.  
	// (This is a little more cumbersome than expected because some RIDs, like admins, might need to be ignored.)
	$current_rids = array();
	if ($worker_user && $worker_user->roles) {
		$current_rids = $worker_user->roles;
	}

	// Create the new RIDs. This needs to be an associated array
	$new_rids = array();
	foreach ($current_rids+$yes_rids as $rid => $ignore) {
		if (!$rid) { continue; }
		if ($no_rids[$rid]) { continue; }
		$new_rids[$rid] = TRUE;
	}

	if (!$worker_user) {
		$is_new = TRUE;
		$worker_user = new stdClass;
		$worker_user->is_new = TRUE;
		$worker_user->init = $email;
		$worker_user->status = TRUE;
		$worker_user->timezone = variable_get('date_default_timezone', '');
		$worker_user->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
	} 

	$worker_user->name = "$id";
	$worker_user->pass = user_hash_password($aat);
	$worker_user->mail = $email;
	$worker_user->roles = $new_rids;

	drupal_save_session(FALSE);
	user_save($worker_user);
	drupal_save_session(TRUE);

	if ($is_new) {
		$msg = "Created a new user with name $id, email $email, and password $aat.";
	} else {
		$msg = "Updated existing user; set name = $id, $email = $email, and password = $aat.";
	}
	return array('success' => TRUE, 'msg' => $msg);
}