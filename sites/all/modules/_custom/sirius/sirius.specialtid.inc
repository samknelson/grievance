<?php

//////////////////////////
// TID
//////////////////////////

function sirius_get_special_tid($which) {
	return variable_get('sirius_specialtid_' . $which, NULL);
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
		'sirius_dispatch_job_tags' => array(
			'ASAP' => 'As Soon As Possible (ASAP)',
		),
	);
}

function sirius_specialtid_admin() {
	$keys = sirius_specialtid_keys();

	foreach ($keys as $key => $subkeys) {
		$vocabulary = taxonomy_vocabulary_machine_name_load($key);

		$form["sirius_specialtid_$key"] = array(
			'#type' => 'fieldset',
			'#title' => $vocabulary->name,
		);

		foreach ($subkeys as $subkey => $title) {
			$form["sirius_specialtid_$key"]["sirius_specialtid_$key|$subkey"] = array(
				'#type' => 'select',
				'#title' => $title,
				'#options' => sirius_taxonomy_load($key, 'tid', 'name', TRUE),
				'#default_value' => variable_get("sirius_specialtid_$key|$subkey", NULL),
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
