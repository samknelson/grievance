<?php

function sirius_worker_user_admin() {
  $form = array();

  $form['sirius_worker_usersync_enable'] = array(
    '#type' => 'select',
    '#title' => t('Enable Worker / User Sync?'),
    '#default_value' => variable_get('sirius_worker_usersync_enable', ''),
    '#options' => array('No' => 'No', 'Yes' => 'Yes'),
    '#description' => t('Should Sirius Workers by associated with Drupal Users?'),
  );

  $form['sirius_worker_dummy_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Dummy Domain'),
    '#default_value' => variable_get('sirius_worker_dummy_domain', ''),
    '#description' => t('Workers without an email address will be assigned an address at this domain.'),
  );

	$role_options = user_roles(TRUE);
  $form['sirius_worker_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Worker Roles'),
    '#options' => $role_options,
    '#default_value' => variable_get('sirius_worker_roles', array()),
    '#description' => t('The checked roles are used for workers. Workers are assigned to, or removed from, roles within this list, based on the settings in their member status.'),
  );

  return system_settings_form($form);
}

function sirius_worker_usersync_enabled() {
	return variable_get('sirius_worker_usersync_enable', '') == 'Yes';
}

function sirius_worker_get_uid($worker_nid) {
	$sql = "select entity_id from field_data_field_sirius_worker ";
	$sql .= "where entity_type = 'user' ";
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
	$hr = $stmt->fetchAssoc();
	return $hr['entity_id'];
}

function sirius_worker_get_user($worker_nid) {
	$uid = sirius_worker_get_uid($worker_nid);
	if (!$uid) { return NULL; }
	return user_load($uid);
}

function sirius_worker_get($uid = NULL) {
	global $user;
	if (!$uid) { $uid = $user->uid; }
	$user_full = user_load($uid);
	$worker_nid = $user_full->field_sirius_worker['und'][0]['target_id'];
	return node_load($worker_nid);
}

function sirius_worker_usersync($worker_nid) {

	if (!sirius_worker_usersync_enabled()) { return; }

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

	$sql = "select * from users ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'user' and field_data_field_sirius_worker.entity_id = users.uid ";;
	$sql .= "where name = :name or mail = :mail";
	$stmt = db_query($sql, array(':name' => $id, 'mail' => $email));
	while ($hr = $stmt->fetchAssoc()) {
		if ($hr['field_sirius_worker_target_id'] == $worker_nid) { continue; }
		return array('success' => FALSE, 'msg' => "There is already an existing user " . $hr['uid'] . " with name $id or mail $mail.");
	}


	$need_save = FALSE;
	if (!$worker_user) {
		$need_save = TRUE;
		$is_new = TRUE;
		$worker_user = new stdClass;
		$worker_user->is_new = TRUE;
		$worker_user->init = $email;
		$worker_user->status = TRUE;
		$worker_user->timezone = variable_get('date_default_timezone', '');
		$worker_user->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
	} 

	if ($worker_user->name != $id) {
		$worker_user->name = "$id";
		$need_save = TRUE;
	}

	if (!user_check_password($aat, $worker_user)) {
		$aat_hashed = user_hash_password($aat);
		$worker_user->pass = $aat_hashed;
		$need_save = TRUE;
	}

	if ($worker_user->mail != $email) {
		$worker_user->mail = $email;
		$need_save = TRUE;
	}

	if (array_diff(array_keys($worker_user->roles), array_keys($new_rids)) || 
			array_diff(array_keys($new_rids), array_keys($worker_user->roles))) {
		$worker_user->roles = $new_rids;
		$need_save = TRUE;
	}


	if (!$need_save) {
		return array('success' => TRUE, 'msg' => "User is unchanged.");
	}
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