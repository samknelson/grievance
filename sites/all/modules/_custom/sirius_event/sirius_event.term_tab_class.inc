<?php

function sirius_event_term_tab_class($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_class_form', $event_type_term);
}

function sirius_event_term_tab_class_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$paccess_options = sirius_event_paccess_options($event_type_term);
	$register_as_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$class_json = $json['class'];
	$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name'); 

	$form['#tree'] = TRUE;

	$form['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Class Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Is this event type a class?'),
		'#default_value' => $class_json['enabled'],
	);

	$form['skills'] = array(
		'#type' => 'fieldset',
		'#title' => t('Skills'),
	);

	$form['skills']['paccess'] = array(
		'#type' => 'select',
		'#title' => t('When a user achieves this participant role and status ...'),
		'#options' => $paccess_options,
		'#multiple' => TRUE,
	);
	if ($class_json['skills']['paccess']) { $form['skills']['paccess']['#default_value'] = $class_json['skills']['paccess']; }

	$form['skills']['skills'] = array(
		'#type' => 'select',
		'#title' => t('... grant or renew these skills'),
		'#options' => $skill_options,
		'#multiple' => TRUE,
		'#size' => 10,
	);
	if ($class_json['skills']['skills']) { $form['skills']['skills']['#default_value'] = $class_json['skills']['skills']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_class_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['class'] = $form_state['values'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

