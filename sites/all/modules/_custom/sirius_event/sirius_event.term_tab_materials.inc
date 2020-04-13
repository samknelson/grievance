<?php

function sirius_event_term_tab_policy_materials($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_policy_materials_form', $event_type_term);
}

function sirius_event_term_tab_policy_materials_form($form, &$form_state, $event_type_term) {
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

	if ($json['materials']['rules']) {
		$max = count($json['materials']['rules']) + 2;
	} else {
		$max = 2;
	}

	$actions = drupal_map_assoc(array('list', 'view', 'add', 'edit', 'delete'));

	$form['#tree'] = TRUE;

	for ($i=0; $i<$max; ++$i) {
		$form["materials_$i"] = array(
			'#type' => 'fieldset',
			'#title' => t('Participants Rule #@i', array('@i' => $i+1)),
		);

		//
		// Who is performing the action?
		//

		$form["materials_$i"]['name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name for this rule'),
			'#multiple' => TRUE,
			'#default_value' => $json['materials']['rules'][$i]['name'],
		);

		//
		// Who is performing the action?
		//

		$form["materials_$i"]['paccess'] = array(
			'#type' => 'select',
			'#title' => t('These users have permission to use this rule.'),
			'#options' => $paccess_options,
			'#multiple' => TRUE,
		);
		if ($json['materials']['rules'][$i]['paccess']) { $form["materials_$i"]['paccess']['#default_value'] = $json['materials']['rules'][$i]['paccess']; }

		//
		// What action?
		//

		$form["materials_$i"]["actions"] = array(
			'#title' => t('The selected users can perform these actions on materials'),
			'#type' => 'select',
			'#options' => $actions,
			'#multiple' => TRUE,
		);
		if ($json['materials']['rules'][$i]['actions']) { $form["materials_$i"]['actions']['#default_value'] = $json['materials']['rules'][$i]['actions']; }
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_event_term_tab_policy_materials_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$i = 0;
	$result = array();
	while ($form_state['values']["materials_$i"]['paccess']) {
		$result[] = $form_state['values']["materials_$i"];
		++$i;
	}

	$json['materials']['rules'] = $result;
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

