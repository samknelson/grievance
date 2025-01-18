<?php

function sirius_edls_page_sheet_create_reserved() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	return drupal_get_form('sirius_edls_page_sheet_create_reserved_form');
}

function sirius_edls_page_sheet_create_reserved_form($form, &$form_state) {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) { return sirius_form_error('Please select an employer.'); }

	$form['#tree'] = TRUE;

	$form['employer_nid'] = array('#type' => 'value', '#value' => $employer_node->nid);

  $form['sheet']['date_start'] = array(
	  '#type' => 'date_popup',
	  '#title' => t('Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => date('Y-m-d'),
		'#required' => TRUE,
  );

  $form['sheet']['type'] = array(
  	'#title' => 'Type',
  	'#type' => 'select',
  	'#required' => TRUE,
  	'#options' => array(NULL => t('-- Select --')) + sirius_edls_sheet_reserved_options(),
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_edls_page_sheet_create_reserved_form_submit($form, &$form_state) {
	$date = $form_state['values']['sheet']['date_start'];
	$type = $form_state['values']['sheet']['type'];
	$employer_nid = $form_state['values']['employer_nid'];

	$result = sirius_edls_sheet_reserved_fetch($employer_nid, $date, $type, TRUE);
	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
		$form_state['rebuild'] = TRUE;
		return;
	}

	drupal_set_message($result['msg']);
	drupal_goto('node/' . $result['sheet_node']->nid . '/sirius_edls_sheet_view');
}