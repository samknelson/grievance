<?php

function sirius_bac1_page_iu_classification_rates() {
	return drupal_get_form('sirius_bac1_page_iu_classification_rates_form');
}

function sirius_bac1_page_iu_classification_rates_form($form, &$form_state) {
	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	$field_name = 'bac1_iu_classification';
	if (!$contact_definition_form[$field_name]) {
		drupal_set_message("Please define a custom contact definition field with the machine name $field_name.", 'error');
		return array();
	}
	$options = $contact_definition_form[$field_name]['#options'];
	if ($options) { unset($options[NULL]); }
	if (!$options) { 
		drupal_set_message("The custom contact definition field $field_name must be a select list with at least one option.", 'error');
		return array();
	}

	$rates = sirius_domain_variable_get('bac1_iu_classification_rates');

	$form['#tree'] = TRUE;
	foreach ($options as $key => $value) {
		$form['rates'][$key] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#default_value' => $rates[$key],
			'#title' => "Rate for Classification: $key",
		);
	}	

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_bac1_page_iu_classification_rates_form_submit($form, &$form_state) {
	sirius_domain_variable_set('bac1_iu_classification_rates', $form_state['values']['rates']);
	drupal_set_message('Rates saved.');
}