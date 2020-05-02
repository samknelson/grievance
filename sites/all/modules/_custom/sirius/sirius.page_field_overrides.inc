<?php

function sirius_page_field_overrides() { 
	return drupal_get_form('sirius_page_field_overrides_form');
}

function sirius_page_field_overrides_form($form, &$form_state) {
	$json = json_decode(sirius_domain_variable_get('sirius_field_overrides'), TRUE);

	if (!$json) { $json = array(); }
	$count = count($json) + 2;

	$form = array();
	$form['#tree'] = TRUE;
	for ($delta = 0; $delta < $count; ++$delta) {
		$form[$delta] = array(
			'#type' => 'fieldset',
			'#title' => t('Override #@i', array('@i' => ($delta+1))),
		);

		$form[$delta]['entity_type'] = array(
			'#title' => t('Entity Type'),
			'#type' => 'textfield',
			'#description' => t('Entity type, probably "node".'),
			'#default_value' => $json[$delta]['entity_type'],
		);

		$form[$delta]['bundle'] = array(
			'#title' => 'Bundle',
			'#type' => 'textfield',
			'#description' => t('Bundle, probably something like "grievance" or "sirius_worker".'),
			'#default_value' => $json[$delta]['bundle'],
		);

		$form[$delta]['field_name'] = array(
			'#title' => t('Field Name'),
			'#type' => 'textfield',
			'#description' => t('Field name to override, e.g. "field_grievance_organizer".'),
			'#default_value' => $json[$delta]['field_name'],
		);

		$form[$delta]['label'] = array(
			'#title' => t('Label'),
			'#type' => 'textfield',
			'#description' => t('New field label.'),
			'#default_value' => $json[$delta]['label'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);

	return $form;
}

function sirius_page_field_overrides_form_submit($form, &$form_state) {
	$json = array();
	$delta = 0;
	while ($form_state['values'][$delta]) {
		if (!$form_state['values'][$delta]['entity_type']) { ++$delta; continue; }
		$json[] = $form_state['values'][$delta];
		++$delta;
	}
	sirius_domain_variable_set('sirius_field_overrides', json_encode($json));	
}