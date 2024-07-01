<?php

function sirius_ledger_node_tab_worker_ar_access($worker_node) {
	$accounts = sirius_ledger_account_list();
	if (!$accounts) { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_worker_ar($worker_node) {
	$ea = Sirius_Ledger_EA::getInstance();
	$accounts = sirius_ledger_account_list();

	$html = '<h2>' . t('Please select an account.') . '</h2>';

	$html .= '<ul>';
	foreach ($accounts as $account_nid => $account_title) {
		$ea_node = $ea->fetch($worker_node->nid, $account_nid, TRUE);
		$html .= '<li>';
		$html .= l($account_title, 'node/' . $ea_node->nid);
		$html .= '</li>';
	}
	$html .= '</ul>';
	return $html;
}
