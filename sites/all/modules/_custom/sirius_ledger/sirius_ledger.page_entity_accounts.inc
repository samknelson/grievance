<?php

function sirius_ledger_node_tab_entity_accounts_access($entity_node) {
	$accounts = sirius_ledger_account_list();
	if (!$accounts) { return FALSE; }

	return Sirius_Ledger_Access::getInstance()->access('entity_view', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_entity_accounts($entity_node) {
	$ea = Sirius_Ledger_EA::getInstance();
	$access = Sirius_Ledger_Access::getInstance();
	$accounts = sirius_ledger_account_list();

	$context = array();
	$context['entity_node'] = $entity_node;


	$rows = array();
	foreach ($accounts as $account_nid => $account_title) {
		$context['account_node'] = node_load($account_nid);
		if (!$access->access('ea_view', $context)) { continue; }
		$ea_node = $ea->fetch($entity_node->nid, $account_nid, TRUE);

		$balance = $ea->balance($ea_node);
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$row = array();
		$row[] = $account_title;
		$row[] = $currency->render_amt(-1 * $balance);
		$row[] = l('View', 'node/' . $ea_node->nid . '/sirius_ea_ar', array('attributes' => array('class' => array('tool-button'))));
		$rows[] = $row;
	}

	if (!$rows) { return '<p>' . t('No accounts available.') . '</p>'; }

	$header = array();
	$header[] = t('Account');
	$header[] = t('Balance');
	$header[] = t('Tools');

	$html = '<h2>' . t('Please select an account.') . '</h2>';
	$html .= sirius_table_render($header, $rows);

	return $html;
}

