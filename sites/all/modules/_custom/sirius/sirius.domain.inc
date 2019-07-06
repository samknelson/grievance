<?php

function sirius_domain_get_nid() {
	$domain_nid = $_SESSION['sirius_domain'];
	if (!$domain_nid) {
		global $user;
		$user_full = user_load($user->uid);
		$domain_nid = $user_full->field_sirius_domains['und'][0]['target_id'];
	}

	return $domain_nid;
}

