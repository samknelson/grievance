<?php

//////////////////////////
// TID
//////////////////////////

function sirius_get_special_tid($which) {
	list ($vocabulary_machine_name, $key) = explode('|', $which);
	$is_multidomain = sirius_domain_taxonomy_is_multidomain($vocabulary_machine_name);

	$variable_name = $which;
	if ($is_multidomain) {
		$domain_nid = sirius_domain_nid();
		if ($domain_nid) {
			$variable_name = "$domain_nid/$which";
		}
	}
	$variable_name = "sirius_specialtid_$variable_name";
	return variable_get($variable_name, NULL);
}

function sirius_specialtid_keys() {
	return array(
		'sirius_worker_dispatch_status' => array(
			'employed' => 'Employed',
			'not_employed_not_available' => 'Not Employed, Not Available',
			'not_employed_available' => 'Not Employed, Available',
		),
		'sirius_dispatch_job_type' => array(
			'CBN' => 'Call by Name',
			'FAN' => 'Call by Name, then Fill as Needed',
			'OPEN' => 'Open',
			'BULL' => 'Bullpen',
		),
		'sirius_dispatch_sib' => array(
			'not_blocked' => 'Not Blocked',
			'blocked' => 'Blocked',
			'threestrikes' => 'Three-Strikes',
		),
		'sirius_dispatch_job_tags' => array(
			'ASAP' => 'As Soon As Possible (ASAP)',
		),
		'sirius_gender' => array(
			'M' => 'Male',
			'F' => 'Female',
		),
	);
}

function sirius_specialtid_admin() {
	$keys = sirius_specialtid_keys();
	$domain_node = sirius_domain();
	if ($domain_node) {
		drupal_set_title(drupal_get_title() . ': ' . $domain_node->title);
		$domain_nid = $domain_node->nid;
	}

	foreach ($keys as $key => $subkeys) {
		$vocabulary = taxonomy_vocabulary_machine_name_load($key);
		$is_multidomain = sirius_domain_taxonomy_is_multidomain($key);

		$form["sirius_specialtid_$key"] = array(
			'#type' => 'fieldset',
			'#title' => $vocabulary->name,
		);

		foreach ($subkeys as $subkey => $title) {
			$idx = "$key|$subkey";
			if ($is_multidomain && $domain_nid) {
				$idx = "$domain_nid/$idx";
			}
			$idx = "sirius_specialtid_$idx";
			$form["sirius_specialtid_$key"][$idx] = array(
				'#type' => 'select',
				'#title' => $title,
				'#options' => sirius_taxonomy_load($key, 'tid', 'name', TRUE),
				'#default_value' => variable_get($idx, NULL),
			);

		}
	}

  return system_settings_form($form);
}

//////////////////////////
// RID
//////////////////////////

function sirius_get_special_rid($which) {
	return variable_get('sirius_specialrid_' . $which, NULL);
}

function sirius_specialrid_keys() {
	return array(
		'worker' => 'Worker',
	);
}

function sirius_specialrid_admin() {
	$keys = sirius_specialrid_keys();

	$role_options = array(NULL => t('-- Select --')) + user_roles(TRUE);

	foreach ($keys as $key => $title) {
		$form["sirius_specialrid_$key"] = array(
			'#type' => 'select',
			'#title' => $title,
			'#options' => $role_options,
			'#default_value' => variable_get("sirius_specialrid_$key", NULL),
		);
	}

  return system_settings_form($form);
}
