<?php

function sirius_employer_get($uid = NULL) {
	global $user;
	if (!$uid) { $uid = $user->uid; }
	$user_full = user_load($uid);
	$employer_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
	return node_load($employer_nid);
}

