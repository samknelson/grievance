<?php

function sirius_worker_term_tab_tor_settings($tor_term) {
	return drupal_get_form('sirius_worker_term_tab_tor_settings_form', $tor_term);
}

function sirius_worker_term_tab_tor_settings_form($form, &$form_state, $tor_term) {
	$form_state['tor_tid'] = $tor_term->tid;
	$tor_json = sirius_json_get($tor_term);

	$form['#tree'] = TRUE;

	/*
	$form['tor']['start_date'] = array(
		'#type' => 'fieldset',
		'#title' => 'Start Date',
	);

	$form['tor']['start_date']['required'] = array(
		'#title' => t('Required?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Is the start date for this time-off entry required? Generally this should be "Yes", unless we are permitting time-off requests with an unspecified date.'),
		'#default_value' => $tor_json['tor']['start_date']['required'],
	);

	$form['tor']['start_date']['when'] = array(
		'#title' => t('When?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'future' => t('Future'),
			'past' => t('Past'),
		),
		'#description' => t('Should the start date of a newly created time-off entry be in the future or the past? For unplanned absences, this will be in the past. For planned absences, it will be in the future.'),
		'#default_value' => $tor_json['tor']['start_date']['when'],
	);

	$form['tor']['end_date'] = array(
		'#type' => 'fieldset',
		'#title' => 'End Date',
	);

	$form['tor']['end_date']['required'] = array(
		'#title' => t('Required?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Is the end date for this time-off entry required? For requested time off, generally the end date should be required. For injury or sick leave, or for other unplanned absences, the end date will often not be known in advance.'),
		'#default_value' => $tor_json['tor']['end_date']['required'],
	);
	*/
	
	$status_options = sirius_worker_schedule_tor_status_options();
	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$access_options = array();
	$access_options["employer"] = "Employer: Current Employee";
	foreach ($ms_options as $tid => $name) {
		$access_options["ms_tid:$tid"] = "Workers: Member Status $name";
	}
	$permissions = array(
		'add' => 'Add',
		'view' => 'View',
		'edit' => 'Edit',
	);
	foreach ($status_options as $status_key => $status_name) {
		$permissions["change_status_to_$status_key"] = "Change status to $status_name";
	}

	foreach ($status_options as $status_key => $status_name) {
		$form['tor']['status'][$status_key] = array(
			'#type' => 'fieldset',
			'#title' => 'Options for Status: ' . $status_name,
		);

		$form['tor']['status'][$status_key]['active'] = array(
			'#title' => t('Active?'),
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#description' => t('Is this status an option for this time-off type?'),
			'#default_value' => $tor_json['tor']['status'][$status_key]['active'],
			'#id' => "tor_status_active_$status_key",
		);

		foreach ($permissions as $permission_key => $permission_name) {
			if ($permission_key == "change_status_to_$status_key") { continue; }
			$form['tor']['status'][$status_key]['permission'][$permission_key] = array(
				'#title' => "Who Can: $permission_name",
				'#type' => 'select',
				'#options' => $access_options,
				'#multiple' => TRUE,
				'#states' => array('visible' => array(":input[id=\"tor_status_active_$status_key\"]" => array('value' => 'Yes')))
			);
			if ($tor_json['tor']['status'][$status_key]['permission'][$permission_key]) {
				$form['tor']['status'][$status_key]['permission'][$permission_key]['#default_value'] = $tor_json['tor']['status'][$status_key]['permission'][$permission_key];
			}
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

function sirius_worker_term_tab_tor_settings_form_submit($form, &$form_state) {
	$tor_term = taxonomy_term_load($form_state['tor_tid']);
	$tor_json = sirius_json_get($tor_term);
	$tor_json['tor'] = $form_state['values']['tor'];
	sirius_json_set($tor_term, $tor_json);
	taxonomy_term_save($tor_term);
	drupal_set_message("Settings saved.");
}
