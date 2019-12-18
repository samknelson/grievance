<?php

function sirius_dispatch_term_tab_industry($industry_term) {
	return drupal_get_form('sirius_dispatch_term_tab_industry_form', $industry_term);
}

function sirius_dispatch_term_tab_industry_form($form, &$form_state, $industry_term) {
	$form_state['industry_tid'] = $industry_term->tid;
	$json = json_decode($industry_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['notes'] = array(
		'#title' => t('Default Dispatch Notes'),
		'#type' => 'textarea',
		'#default_value' => $json['dispatch']['default']['notes'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_dispatch_term_tab_industry_form_submit($form, &$form_state) {
	$industry_tid = $form_state['industry_tid'];
	$industry_term = taxonomy_term_load($industry_tid);
	$json = json_decode($industry_term->field_sirius_json['und'][0]['value'], TRUE);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['dispatch']['default']['notes'] = $form_state['values']['notes'];
	$industry_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($industry_term);
	drupal_set_message(t('Settings saved.'));
}

