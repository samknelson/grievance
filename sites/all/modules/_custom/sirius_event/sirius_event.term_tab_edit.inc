<?php

function sirius_event_term_tab_policy_edit($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_policy_edit_form', $event_type_term);
}

function sirius_event_term_tab_policy_edit_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$paccess_options = sirius_event_paccess_options($event_type_term);

	if ($json['edit']['rules']) {
		$max = count($json['edit']['rules']) + 2;
	} else {
		$max = 2;
	}

	$form['#tree'] = TRUE;

	for ($i=0; $i<$max; ++$i) {
		$form["edit_$i"] = array(
			'#type' => 'fieldset',
			'#title' => t('Access rule #@i', array('@i' => $i+1)),
		);
		$form["edit_$i"]['paccess'] = array(
			'#type' => 'select',
			'#title' => t('Events may be edited by these users.'),
			'#options' => $paccess_options,
			'#default_value' => $json['edit']['rules'][$i]['paccess'],
			'#multiple' => TRUE,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_policy_edit_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$i = 0;
	$result = array();
	while ($form_state['values']["edit_$i"]['paccess']) {
		$result[] = $form_state['values']["edit_$i"];
		++$i;
	}

	$json['edit']['rules'] = $result;
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

