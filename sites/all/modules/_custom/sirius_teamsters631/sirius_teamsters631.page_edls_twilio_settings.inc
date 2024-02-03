<?php

function sirius_edls_page_631twilio() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_631twilio_form', $employer_node));
	return $html;
}

function sirius_edls_page_631twilio_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}