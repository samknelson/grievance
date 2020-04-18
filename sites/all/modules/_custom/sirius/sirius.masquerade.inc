<?php

/**
 * Implements hook_masquerade_bagan
 */

function sirius_masquerade_began($real_user, $masquerade_user) {
	sirius_log(
		'masquerade', 
		'began', 
		"Masquerade began for " . $real_user->name . ' ==> ' . $masquerade_user->name,
		'', 
		'', 
		array(),
		array(),
		array(
			'real_user_name' => $real_user->name,
			'real_user_uid' => $real_user->uid,
			'real_user_mail' => $real_user->mail,
			'masquerade_user_name' => $masquerade_user->name,
			'masquerade_user_uid' => $masquerade_user->uid,
			'masquerade_user_mail' => $masquerade_user->mail,
		)
	);
}

/**
 * Implements hook_masquerade_ended
 */

function sirius_masquerade_ended($masquerade_user, $real_user) {
	sirius_log(
		'masquerade', 
		'began', 
		"Masquerade ended for " . $masquerade_user->name . ' ==> ' . $real_user->name,
		'', 
		'', 
		array(),
		array(),
		array(
			'real_user_name' => $real_user->name,
			'real_user_uid' => $real_user->uid,
			'real_user_mail' => $real_user->mail,
			'masquerade_user_name' => $masquerade_user->name,
			'masquerade_user_uid' => $masquerade_user->uid,
			'masquerade_user_mail' => $masquerade_user->mail,
		)
	);
}

