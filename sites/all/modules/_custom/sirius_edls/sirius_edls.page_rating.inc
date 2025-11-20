<?php

function sirius_edls_page_rating() {
	return drupal_get_form('sirius_edls_page_rating_form');
}

function sirius_edls_page_rating_form($form, &$form_state) {
	if (!sirius_domain_component_enabled('worker:rating')) { return sirius_form_error("The component worker:rating is not enabled."); }

	$rating = Sirius_Worker_Rating::getInstance();
	$rating_types = $rating->types(array('indent' => TRUE));
	if (!$rating_types) { return sirius_form_error("No rating types are defined."); }

	$form['rating_type_tid'] = array(
		'#type' => 'select',
		'#title' => 'Rating Type',
		'#options' => array(NULL => t('-- Select --')) + $rating_types,
		'#default_value' => $rating->user_default_type(),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_page_rating_form_submit($form, &$form_state) {
	$rating = Sirius_Worker_Rating::getInstance();
	$rating->user_default_type_set($form_state['values']['rating_type_tid']);
	drupal_set_message("Default rating type set.");
}