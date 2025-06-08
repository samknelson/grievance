<?php

function sirius_trust_node_tab_worker_cobra_form($form, &$form_state, $worker_node) {

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check Cobra Eligibility'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_cobra_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$cobra = Sirius_Trust_Cobra::getInstance();
	drupal_set_message("Checking COBRA eligibility ...");
	$result =  SiriusResult::create($cobra->worker_can_elect($worker_node));
	$result->drupalSetMessage();
}
