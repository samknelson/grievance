<?php

function sirius_trust_node_tab_worker_beneficiary_access($worker_node) {
  return Sirius_Trust_Access::getInstance()->access('worker_beneficiary_edit', array('worker_node' => $worker_node));
}

function sirius_trust_node_tab_worker_beneficiary_form($form, &$form_state, $worker_node) {
	$beneficiary = Sirius_Trust_Beneficiary::getInstance();

	$form = $beneficiary->form($form, $form_state, $worker_node);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_beneficiary_form_validate($form, &$form_state) {
	$beneficiary = Sirius_Trust_Beneficiary::getInstance();
	return $beneficiary->form_validate($form, $form_state);
}

function sirius_trust_node_tab_worker_beneficiary_form_submit($form, &$form_state) {
	$beneficiary = Sirius_Trust_Beneficiary::getInstance();
	return $beneficiary->form_submit($form, $form_state);
}
