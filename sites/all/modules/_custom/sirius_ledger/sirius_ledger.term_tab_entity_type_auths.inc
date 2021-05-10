<?php

function sirius_ledger_term_tab_entity_type_auths($entity_type_term) {
	return drupal_get_form('sirius_ledger_term_tab_entity_type_auths_form', $entity_type_term);
}

function sirius_ledger_term_tab_entity_type_auths_form($form, &$form_state, $entity_type_term) {
	$form_state['entity_type_tid'] = $entity_type_term->tid;

	$entity_type_json = sirius_jsonfield_util_term_json_get($entity_type_term);

	$form['#tree'] = TRUE;

	$form['auths']['settings']['enabled'] = array(
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $entity_type_json['auths']['settings']['enabled'],
		'#description' => t('Are authorizations enabled for this entity type?'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_term_tab_entity_type_auths_form_submit($form, &$form_state) {
	$entity_type_term = taxonomy_term_load($form_state['entity_type_tid']);
	$entity_type_json = sirius_jsonfield_util_term_json_get($entity_type_term);
	$entity_type_json['auths']['settings'] = $form_state['values']['auths']['settings'];
	sirius_jsonfield_util_term_json_set($entity_type_term, $entity_type_json);
	taxonomy_term_save($entity_type_term);
	drupal_set_message("Settings saved.");
}
