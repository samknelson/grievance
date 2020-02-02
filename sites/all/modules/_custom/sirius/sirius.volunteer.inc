<?php

function sirius_page_volunteer_admin() {
	$keys = sirius_specialtid_keys();
	$domain_nid = sirius_domain_nid();

	$form["sirius_volunteer_selfregister_enabled_$domain_nid"] = array(
		'#type' => 'select',
		'#title' => t('Volunteer Self-Registration'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes')
		),
		'#default_value' => variable_get("sirius_volunteer_selfregister_enabled_$domain_nid", NULL),
	);

	$role_options = array(NULL => t('-- Select --')) + user_roles(TRUE);

	$form["sirius_volunteer_selfregister_role_$domain_nid"] = array(
		'#type' => 'select',
		'#title' => 'Self-Register Role',
		'#options' => $role_options,
		'#default_value' => variable_get("sirius_volunteer_selfregister_role_$domain_nid", NULL),
	);

	$form["sirius_volunteer_selfregister_code_$domain_nid"] = array(
		'#type' => 'select',
		'#title' => 'Self-Register Role',
		'#options' => $role_options,
		'#default_value' => variable_get("sirius_volunteer_selfregister_role_$domain_nid", NULL),
	);

  return system_settings_form($form);
}

function sirius_page_volunteer_selfregister($domain_nid) {
	global $user;

	$domain_node = node_load($domain_nid);
	if ($domain_node->type != 'sirius_domain') { return t('Not a domain.'); }

	if (!$user || !$user->uid) {
		return t('Show register form now.');
	}

	if ($user && $user->uid) {
		// I'm a user, but I don't have the volunteer permission. This isn't really a volunteer user
		if (!user_access('sirius volunteer')) {
			return t('You are signed in to the system, but your user does not have the permission "sirius volunteer". Please ' . 
				l(t('log out'), 'user/logout', array('query' => array('destination' => current_path()))) .
				t(' and register as a volunteer.'));
		}

		// I'm a user and I don't have domain access and I have 
		if (!sirius_domain_access($domain_nid)) { 
			return t('You are already signed in, but your user does not have access to this domain. Please ');
		}
	}
		
}