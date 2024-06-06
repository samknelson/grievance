<?php

function sirius_ledger_payperiod_clear($payperiod_node, $mode) {
	sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Cleared');
	if ($mode == 'live') {
		sirius_ledger_payment_clear($payperiod_node->nid);
	} else {
		sirius_minilog("(test mode, skipping delete.)");
	}
}

function sirius_ledger_payperiod_set($payperiod_node, $mode, $options = array()) {
	if (!sirius_domain_component_enabled('ledger:worker:charge:hourly')) { return; }
	sirius_minilog("Payperiod " . l($payperiod_node->title, 'node/' . $payperiod_node->nid) . ' (' . $payperiod_node->nid . '): ' . "Calculating ledger (mode = $mode)");

	// Build the context
	$context = array();
	$context['payperiod_node'] = $payperiod_node;
	$context['employer_node'] = node_load($payperiod_node->field_grievance_shop['und'][0]['target_id']);
	$context['worker_node'] = node_load($payperiod_node->field_sirius_worker['und'][0]['target_id']);
	$context['ts'] = sirius_fieldval($payperiod_node, 'field_sirius_datetime', 'ts');
	$context['trigger'] = 'hourly';

	sirius_minilog_indent();
	$result = sirius_ledger_charge_plugin_charge_all($context, $mode);
	sirius_minilog_outdent();

	return $result;
}
