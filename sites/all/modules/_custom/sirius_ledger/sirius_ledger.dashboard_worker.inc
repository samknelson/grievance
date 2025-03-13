<?php

function sirius_ledger_dashboard_worker_data($prefs, $options = array()) {
	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(); }

	$ea = Sirius_Ledger_EA::getInstance();
	$access = Sirius_Ledger_Access::getInstance();
	$accounts = sirius_ledger_account_list();

	$context = array();
	$context['entity_node'] = $worker_node;

	$rows = array();
	$html = '';
	foreach ($accounts as $account_nid => $account_title) {
		$context['account_node'] = node_load($account_nid);
		$ea_node = $ea->fetch($worker_node->nid, $account_nid, TRUE);
		if (!$access->access('ea_view', array('ea_node' => $ea_node))) { continue; }

		$balance = $ea->balance($ea_node);
		if (!$balance) { continue; }
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$html .= '<div>';
		$html .= $account_title;
		$html .= '<br />';
		$html .= 'Outstanding Balance: ';
		$html .= '<strong>';
		$html .= $currency->render_amt(-1 * $balance);
		$html .= '</strong>';
		$html .= '</div>';
	}

	if (!$html) { return array(); }

	$tools = l(
		'Account Details',
		'node/' . $worker_node->nid . '/sirius_worker_ledger',
		array('attributes' => array('class' => array('tool-button')))
	);

	$record = array(
		'tools' => $tools,
		'details' => $html,
	);

	return array($record);
}
