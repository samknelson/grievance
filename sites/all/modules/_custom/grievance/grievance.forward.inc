<?php

/**
 * @todo: This is legacy code, and should probably be replaced with calls to sirius_loopback_access.
 */

function grievance_forward_loopback($node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title);
	return node_view($node);
}

function grievance_forward_loopback_access($node) {
	return sirius_loopback_access();

	if ($_SERVER['SERVER_ADDR'] == $_SERVER['REMOTE_ADDR'] ||
		  $_SERVER['SERVER_ADDR'] == $_SERVER['HTTP_X_FORWARDED_FOR'] ||
		  variable_get('grievance_loopback_ip', '') == $_SERVER['REMOTE_ADDR'] || 
		  variable_get('grievance_loopback_ip', '') == $_SERVER['HTTP_X_FORWARDED_FOR'] ||
		  '52.43.112.81' == $_SERVER['REMOTE_ADDR'] ||
		  '35.161.76.142' == $_SERVER['REMOTE_ADDR'] ||
		  '35.161.76.142' == $_SERVER['HTTP_X_FORWARDED_FOR']) {
		return TRUE; 
	}

	if (user_access('sirius admin')) { return TRUE; }

	return FALSE;
}
