<?php

function sirius_smf_feed_payments_account_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form['account_nid'] = array(
		'#type' => 'select',
		'#title' => 'Account',
		'#options' => array(NULL => '-- Select --') + sirius_ledger_account_list(),
		'#required' => TRUE,
		'#default_value' => $data['account_nid'],
	);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_payments_account_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$data['account_nid'] = $form_state['values']['account_nid'];

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Selection saved");
}
