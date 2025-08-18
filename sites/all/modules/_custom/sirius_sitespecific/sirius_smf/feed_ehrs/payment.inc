<?php

function sirius_smf_feed_ehrs_payment_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
  $monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
  $access_context = array('wizard_node' => $wizard_node);

	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	if (!$eup_node) { return sirius_form_error("System error: eup node not found."); }
	$employer_node = $monthly->get_employer($eup_node);

	$ea = Sirius_Ledger_EA::getInstance();

	$account_nid = 2457512; // @todo
	$ea_node = $ea->fetch($employer_node->nid, $account_nid, TRUE);

	sirius_require('sirius_ledger', 'sirius_ledger.page_ea_stripe_payment.inc');
	$form = sirius_ledger_node_tab_ea_stripe_payment_form($form, $form_state, $ea_node);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_payment_form_validate($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.page_ea_stripe_payment.inc');
	return sirius_ledger_node_tab_ea_stripe_payment_form_validate($form, $form_state);
}

function sirius_smf_feed_ehrs_payment_form_submit($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.page_ea_stripe_payment.inc');
	$result = sirius_ledger_node_tab_ea_stripe_payment_form_submit($form, $form_state);

	if ($result['success']) {
		$info = $form_state['info'];
		$data = $form_state['data'];
		$wizard_node = $form_state['wizard_node'];
		$current_step = $form_state['current_step'];
		$account_nid = $form_state['account_nid'];
		sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Payment submitted", array('nonsequential' => FALSE));
	}
}
