<?php

//////////////////////////
// TID
//////////////////////////

function sirius_get_special_tid($which) {
	list ($vocabulary_machine_name, $key) = explode('|', $which);
	$is_multidomain = sirius_domain_taxonomy_is_multidomain($vocabulary_machine_name);

	$variable_name = "sirius_specialtid_$which";

	if ($is_multidomain) { return sirius_domain_variable_get($variable_name); } else { return variable_get($variable_name); }
}

function sirius_specialtid_keys() {

	return array(
		'sirius_worker_dispatch_status' => array(
			'employed' => array('title' => 'Employed'),
			'not_employed_not_available' => array('title' => 'Not Employed, Not Available'),
			'not_employed_available' => array('title' => 'Not Employed, Available'),
		),
		'sirius_dispatch_job_type' => array(
			'CBN' => array('title' => 'Call by Name'),
			'FAN' => array('title' => 'Call by Name, then Fill as Needed'),
			'OPEN' => array('title' => 'Open'),
			'BULL' => array('title' => 'Bullpen'),
		),
		'sirius_dispatch_sib' => array(
			'not_blocked' => array('title' => 'Not Blocked'),
			'blocked' => array('title' => 'Blocked'),
			'threestrikes' => array('title' => 'Three-Strikes'),
		),
		'sirius_gender' => array(
			'M' => array('title' => 'Male'),
			'F' => array('title' => 'Female'),
		),
		'grievance_contact_types' => array(
			'steward' => array('title' => 'Shop Steward'),
		),
		'grievance_log_types' => array(
			'modal_default' 								=> array('title' => 'Default Quick Log Type'),
			'incident' 											=> array('title' => 'Grievance Log Incident Type', 'description' => 'Grievance log type to be used for incidents -- i.e. an entry of this type is created the date that the incident occurred.'),
			'insert' 												=> array('title' => 'Grievance Log Insert Type', 'description' => 'Grievance log type to be used when a grievance record is first saved into the database.'),
			'update_with_status_change' 		=> array('title' => 'Grievance Log Update Type With Status Change', 'description' => 'Grievance log type to be used for updates where the status changes -- i.e. when the record is saved and the status is different.'),
			'update_without_status_change' 	=> array('title' => 'Grievance Log Update Type Without Status Change', 'description' => 'Grievance log type to be used for updates where the status does not change -- i.e. an entry is created each time the grievance is saved.'),
			'notify' 												=> array('title' => 'Grievance Log Company Notification Type', 'description' => 'Grievance log type to be used for notification to the company.'),
			'member_forward' 								=> array('title' => 'Grievance Log Member Notification Type', 'description' => 'Grievance log type to be used for notification to the member.'),
			'assignment' 										=> array('title' => 'Grievance Log Assignment Type', 'description' => 'Grievance log type to be used for assignment to a staff member.'),
			'first_call' 										=> array('title' => 'Grievance Log First Call Type', 'description' => 'Grievance log type to be used for initial call to grievant.  Used for reporting.'),
			'phaxio_callback' 							=> array('title' => 'Phaxio Callback Type', 'description' => 'Grievance log type to be used for Phaxio callback -- i.e. fax confirmation receipts.'),
			'spi'	 													=> array('title' => 'SPI Type'),
			'term' 													=> array('title' => 'Term Type'),
			'written_warning' 							=> array('title' => 'Written Warning Type'),
			'final_warning'	 								=> array('title' => 'Final Warning Type',),
			'suspension' 										=> array('title' => 'Suspension Type'),
			'merge' 												=> array('title' => 'Merge Type', 'description' => 'Log type to be used when a grievance is merged into another grievance.'),
			'timelines_extended' 						=> array('title' => 'Timelines Extended Type', 'description' => 'Log type to be used when a when timelines are extended by mutual agreement.'),
			'intake_1' 											=> array('title' => 'Intake #1', 'description' => 'Log type for extra intake date #1'),
			'intake_2' 											=> array('title' => 'Intake #2', 'description' => 'Log type for extra intake date #2'),
			'intake_3' 											=> array('title' => 'Intake #3', 'description' => 'Log type for extra intake date #3'),
			'intake_4' 											=> array('title' => 'Intake #4', 'description' => 'Log type for extra intake date #4'),
			'intake_5' 											=> array('title' => 'Intake #5', 'description' => 'Log type for extra intake date #5'),
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

		foreach ($subkeys as $subkey => $info) {
			$idx = "sirius_specialtid_$key|$subkey";
			if ($is_multidomain) { $idx = sirius_domain_variable_name($idx); }
			$form["sirius_specialtid_$key"][$idx] = array(
				'#type' => 'select',
				'#title' => $info['title'],
				'#description' => $info['description'],
				'#options' => sirius_taxonomy_load($key, 'tid', 'name', TRUE),
				'#default_value' => variable_get($idx),
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
