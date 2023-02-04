<?php

function sirius_steward_page_sched_list() {
	return drupal_get_form('sirius_steward_page_sched_list_form');
}

function sirius_steward_page_sched_list_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employers,
		'#required' => TRUE,
	);

	$form['date'] = array(
		'#title' => t('Date'),
		'#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Go'),
	);

	return $form;
}

function sirius_steward_page_sched_list_form_submit($form, &$form_state) {
	$employer_nid = $form_state['values']['employer_nid'];
	$sched_node = sirius_steward_sched_fetch($employer_nid, $form_state['values']['date'], TRUE);
	drupal_goto('node/' . $sched_node->nid . '/sirius_steward_log_sched');
}