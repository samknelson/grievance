<?php

function sirius_ledger_node_tab_ea_payments_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_payments', array('ea_node' => $ea_node));
}

function sirius_ledger_node_tab_ea_payments($ea_node) {
	$ea = Sirius_Ledger_EA::getInstance();
	$participant_node = $ea->participant_node($ea_node);
	$account_node = $ea->account_node($ea_node);

	$params = array();
	$params['account_nid'] = $account_node->nid;
	$params['payer_nid'] = $participant_node->nid;
	$payments = sirius_ledger_payments_search($params);

	$options = array();
	$options['hide_account'] = TRUE;
	return sirius_ledger_payments_render($payments, $options);
}
