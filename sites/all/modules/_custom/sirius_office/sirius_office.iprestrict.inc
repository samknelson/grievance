<?php

function sirius_office_iprestrict_enabled($domain_nid) {
	$field_name = "sirius_office_iprestrict_enable/$domain_nid";
	return variable_get($field_name, '') == 'Yes';
}

function sirius_office_iprestrict_admin() {
	$domain_node = sirius_domain();
	if ($domain_node) { $domain_nid = $domain_node->nid; }
	$intro_html = '<div class="sirius_form_intro">';
	$intro_html .= t('The IP Restriction function prevents users from signing in unless they are coming from specific whitelisted IP addresses. ');
	if ($domain_node) {
		$intro_html .= t('The restrictions here will apply to all users in the domain @title.', array('@title' => $domain_node->title));
	}
	$enabled = sirius_office_iprestrict_enabled($domain_nid);
	$intro_html .= '<br /><br />';
	if ($enabled) { $intro_html .= t('IP Restrictions are currently <strong>ENABLED</strong> for this domain. Users with the specified roles may only sign in from a whitelisted IP address'); }
	else 					{ $intro_html .= t('IP Restrictions are currently <strong>DISABLED</strong> for this domain. Users may sign in remotely without restriction.'); }

	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => $intro_html);

	$field_name = "sirius_office_iprestrict_enable/$domain_nid";
	$form[$field_name] = array(
		'#default_value' => variable_get($field_name, ''),
		'#type' => 'select',
		'#title' => t('IP Restriction Enabled?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$field_name = "sirius_office_iprestrict_ips/$domain_nid";
	$form[$field_name] = array(
		'#default_value' => variable_get($field_name, ''),
		'#type' => 'textarea',
		'#title' => t('Whitelisted IPs'),
		'#description' => t('Enter the IP addresses, one per line.'),
	);

	$field_name = "sirius_office_iprestrict_roles/$domain_nid";
	$form[$field_name] = array(
		'#default_value' => variable_get($field_name, array()),
		'#type' => 'checkboxes',
		'#title' => t('Roles'),
		'#options' => user_roles(TRUE),
		'#description' => t('Enter the roles which are required to sign in from one of the whitelisted IPs.'),
	);

  return system_settings_form($form);
}

function sirius_office_iprestrict_user_login($edit, $account) {
	// Get our domain
	$domain_nid = $account->field_sirius_domain['und'][0]['target_id'];

	// Is IP restriction enabled?
	if (!sirius_office_iprestrict_enabled($domain_nid)) { return; }

	// Do we have a restricted role?
	$restrict_roles = array_values(variable_get("sirius_office_iprestrict_roles/$domain_nid", array()));
	$my_roles = array_keys($account->roles);
	if (!array_intersect($restrict_roles, $my_roles)) { return; }

  // Does the user have the "sirius office iprestrict remote" permission?
	if (user_access('sirius office iprestrict remote', $account)) { return; }

	// Search for our current IP in the list
  $ip_address = ip_address();
  $ip_text = variable_get("sirius_office_iprestrict_ips/$domain_nid", '');
  $ip_regexs = preg_split('/\s+/', $ip_text);
  foreach ($ip_regexs as $ip_regex) {
  	if (!$ip_regex) { continue; }
    if (preg_match("/$ip_regex/", $ip_address)) { return; }
  }

  // Not found. No login allowed.
  global $user;
  module_invoke_all('user_logout', $user);
  session_destroy();
  $user = drupal_anonymous_user();
  drupal_set_message("Login prohibited; you can only sign in from a whitelisted IP address.", 'warning');
}
