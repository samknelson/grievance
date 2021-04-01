<?php

function sirius_event_participants_widgets() {
	return array(
		//'view' => t('View Worker Participant List'),
		//'status' => t('Manage Worker Participant List'),
		//'contact' => t('Manage Contact Participant List'),
		'list' => t('Manage Participant List'),
		'reports' => t('Download reports'),
		'summary' => t('View a Summary'),
		'attendance' => t('Attendance Sheets'),
		'certifications' => t('Certifications'),
		'mightysearch_lite' => t('Advanced Search'),
		'mightysearch' => t('Advanced Search and Tag'),
	);
}

function sirius_event_term_tab_policy_participants($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_policy_participants_form', $event_type_term);
}

function sirius_event_term_tab_policy_participants_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$paccess_options = sirius_event_paccess_options($event_type_term);
	$participants_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

  $prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
  $prole_terms = taxonomy_term_load_multiple($prole_tids);
  $prole_options = array();
  foreach ($prole_terms as $prole_tid => $prole_term) {
  	$prole_options[$prole_tid] = $prole_term->name;
  }

	if ($json['participants']['rules']) {
		$max = count($json['participants']['rules']) + 2;
	} else {
		$max = 2;
	}

	$form['#tree'] = TRUE;

	for ($i=0; $i<$max; ++$i) {
		$form["participants_$i"] = array(
			'#type' => 'fieldset',
			'#title' => t('Participants Rule #@i', array('@i' => $i+1)),
		);

		//
		// Who is performing the action?
		//

		$form["participants_$i"]['name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name for this rule'),
			'#multiple' => TRUE,
			'#default_value' => $json['participants']['rules'][$i]['name'],
		);

		//
		// Who is performing the action?
		//

		$form["participants_$i"]['paccess'] = array(
			'#type' => 'select',
			'#title' => t('These users have permission to use this rule.'),
			'#options' => $paccess_options,
			'#multiple' => TRUE,
		);
		if ($json['participants']['rules'][$i]['paccess']) { $form["participants_$i"]['paccess']['#default_value'] = $json['participants']['rules'][$i]['paccess']; }

		//
		// What action?
		//

		$form["participants_$i"]["widget"] = array(
			'#title' => t('Participants list may be accessed as:'),
			'#type' => 'select',
			'#options' => sirius_event_participants_widgets(),
			'#multiple' => TRUE,
		);
		// @todo: Cleanup legacy
		foreach (array('view', 'status', 'contact') as $legacy_widget) {
			if ($json['participants']['rules'][$i]['widget'][$legacy_widget]) {
				unset($json['participants']['rules'][$i]['widget'][$legacy_widget]);
				$json['participants']['rules'][$i]['widget']['list'] = 'list';
			}
		}
		if ($json['participants']['rules'][$i]['widget']) { $form["participants_$i"]['widget']['#default_value'] = $json['participants']['rules'][$i]['widget']; }

		//
		// What sort of participant?
		//

		$form["participants_$i"]["participants"] = array(
			'#title' => t('Access is granted to participants with these roles and statuses'),
			'#type' => 'select',
			'#options' => $participants_options,
			'#multiple' => TRUE,
		);
		if ($json['participants']['rules'][$i]['participants']) { $form["participants_$i"]['participants']['#default_value'] = $json['participants']['rules'][$i]['participants']; }
	}
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_event_term_tab_policy_participants_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$i = 0;
	$result = array();
	while ($form_state['values']["participants_$i"]['paccess']) {
		$result[] = $form_state['values']["participants_$i"];
		++$i;
	}

	$json['participants']['rules'] = $result;
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

