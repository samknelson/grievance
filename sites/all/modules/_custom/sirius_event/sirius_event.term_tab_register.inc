<?php

/*
[user with user role X] [worker with work status X] [worker with member status X] [existing contact] [new contact]
CAN BE REGISTERED FOR 
[role x]
WITH 
[participant status x]
BY
[self] [user with role X] [participant with role x]
BETWEEN
[start date] and [end date]

======================================

e.g.

[user with role "staff"] can be registered for [teacher] with participant status [enrolled] by [user with role "event admin"]

[worker with any member status] can be registered for [student] with participant status [enrolled] by [teachers]
*/

function sirius_event_register_widgets() {
	return array(
		'user_select' => t('User Select'),
		'user_byname' => t('User by Name'),
		'user_self' => t('Self-Registration'),
		'worker_byname' => t('Worker by Name'),
		'contact_bulk' => t('Contact Bulk'),
		'bullpen_signin' => t('Bullpen Signin'),
	);
}

function sirius_event_term_tab_policy_register($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_policy_register_form', $event_type_term);
}

function sirius_event_term_tab_policy_register_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$paccess_options = sirius_event_paccess_options($event_type_term);
	$register_as_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

  $prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
  $prole_terms = taxonomy_term_load_multiple($prole_tids);
  $prole_options = array();
  foreach ($prole_terms as $prole_tid => $prole_term) {
  	$prole_options[$prole_tid] = $prole_term->name;
  }

	$work_statuses = sirius_taxonomy_load('sirius_work_status', 'tid', 'name', TRUE);

	$industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

	foreach ($member_status_options as $member_status_tid => $name) {
		$member_status_term = $member_status_terms[$member_status_tid];
		$industry_tid = $member_status_term->field_sirius_industry['und'][0]['tid'];
		$industry_term = $industry_terms[$industry_tid];
		$member_status_options[$member_status_tid] = $name . ' (' . $industry_term->name . ')';
	}

	$user_roles = user_roles();

	if ($json['register']['rules']) {
		$max = count($json['register']['rules']) + 2;
	} else {
		$max = 2;
	}

	$form['#tree'] = TRUE;

	for ($i=0; $i<$max; ++$i) {
		$form["register_$i"] = array(
			'#type' => 'fieldset',
			'#title' => t('Registration Rule #@i', array('@i' => $i+1)),
		);

		$form["register_$i"]['name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name for this rule'),
			'#multiple' => TRUE,
			'#default_value' => $json['register']['rules'][$i]['name'],
		);

		//
		// Who is performing the action?
		//

		$form["register_$i"]['paccess'] = array(
			'#type' => 'select',
			'#title' => t('These users have permission to use this rule.'),
			'#options' => $paccess_options,
			'#multiple' => TRUE,
		);
		if ($json['register']['rules'][$i]['paccess']) { $form["register_$i"]['paccess']['#default_value'] = $json['register']['rules'][$i]['paccess']; }

		//
		// Who is the participant?
		//

		$form["register_$i"]["participant"] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'user' => t('User'),
				'worker' => t('Worker'),
				'contact' => t('Contact'),
			),
			'#title' => t('The participant to be registered must be of this type.'),
			'#default_value' => $json['register']['rules'][$i]['participant'],
			'#id' => "register_{$i}_participant",
		);

		$form["register_$i"]["participant_user_role"] = array(
			'#type' => 'select',
			'#title' => '',
			'#options' => $user_roles,
			'#states' => array(
				'visible' => array(
					array(":input[id=\"register_${i}_participant\"]" => array('value' => 'user')),
				)
			),
			'#multiple' => TRUE,
		);
		if ($json['register']['rules'][$i]['participant_user_role']) { $form["register_$i"]['participant_user_role']['#default_value'] = $json['register']['rules'][$i]['participant_user_role']; }

		/*
		$form["register_$i"]["register_${i}_participant_worker_member_status"] = array(
			'#type' => 'select',
			'#options' => $member_status_options,
			'#title' => t('with the member status '),
			'#states' => array(
				'visible' => array(
					array(":input[name=\"register_${i}_participant\"]" => array('value' => 'worker')),
				)
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
			'#multiple' => TRUE,
		);

		if ($work_statuses) {
			$form["register_$i"]["register_${i}_participant_worker_work_status"] = array(
				'#type' => 'select',
				'#options' => $work_statuses,
				'#title' => t('with the work status '),
				'#states' => array(
					'visible' => array(
						array(":input[name=\"register_${i}_participant\"]" => array('value' => 'worker')),
					)
				),
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
				'#multiple' => TRUE,
			);
		}
		*/

		//
		// What sort of participant?
		//

		$form["register_$i"]["register_as"] = array(
			'#title' => t('The new participant may have the role and status'),
			'#type' => 'select',
			'#options' => $register_as_options,
			'#multiple' => TRUE,
		);
		if ($json['register']['rules'][$i]['register_as']) { $form["register_$i"]['register_as']['#default_value'] = $json['register']['rules'][$i]['register_as']; }

		$form["register_$i"]["widget"] = array(
			'#title' => t('Participants may be chosen by:'),
			'#type' => 'select',
			'#options' => sirius_event_register_widgets(),
			'#multiple' => TRUE,
		);
		if ($json['register']['rules'][$i]['widget']) { $form["register_$i"]['widget']['#default_value'] = $json['register']['rules'][$i]['widget']; }

	}
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_event_term_tab_policy_register_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$i = 0;
	$result = array();
	while ($form_state['values']["register_$i"]['paccess']) {
		$result[] = $form_state['values']["register_$i"];
		++$i;
	}

	$json['register']['rules'] = $result;
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

