<?php

function sirius_edls_page_summary() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	return drupal_get_form('sirius_edls_page_summary_form', $employer_node);
}

function sirius_edls_page_summary_form($form, &$form_state, $employer_node) {
	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => date('Y-m-d'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$date = $form_state['values']['date'];
		$markup = sirius_edls_summary($employer_node, $date, $options);
		$form['results'] = array('#markup' => '<hr>' . $markup);
	}

	return $form;
}

function sirius_edls_page_summary_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}