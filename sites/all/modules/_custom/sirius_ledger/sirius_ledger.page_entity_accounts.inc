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
		$ea_node = $ea->fetch($entity_node->nid, $account_nid, TRUE);
		if (!$access->access('ea_view', array('ea_node' => $ea_node))) { continue; }

		$balance = $ea->balance($ea_node);
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$row = array();
		$row[] = $account_title;
		$row[] = $currency->render_amt(-1 * $balance);

		$paths = array();
		$paths['node/' . $ea_node->nid . '/sirius_ea'] = t('View');
		$paths['node/' . $ea_node->nid . '/sirius_ea_stripe_payment'] = t('Make a Payment');
		$tools = '';
		foreach ($paths as $path => $title) {
			if (!drupal_valid_path($path)) { continue; }
			$tools .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
		}
		$row[] = $tools;

		$rows[] = $row;
	}

	if ($rows) {
		$header = array();
		$header[] = t('Account');
		$header[] = t('Balance');
		$header[] = t('Tools');

		$html .= '<h2>' . $entity_node->title . ' Accounts' . '</h2>';
		$html .= sirius_table_render($header, $rows);
	}

	$rows = array();
	foreach ($accounts as $account_nid => $account_title) {
		$ea_node = $ea->fetch($entity_node->nid, $account_nid, TRUE);
		if (!$access->access('ea_view_employer_workers', array('ea_node' => $ea_node))) { continue; }

		$row = array();
		$row[] = $account_title;

		$paths = array();
		$paths['node/' . $ea_node->nid . '/sirius_ea_employer_workers'] = t('View');
		$tools = '';
		foreach ($paths as $path => $title) {
			if (!drupal_valid_path($path)) { continue; }
			$tools .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
		}
		$row[] = $tools;

		$rows[] = $row;
	}

	if ($rows) {
		$header = array();
		$header[] = t('Account');
		$header[] = t('Tools');

		$html .= '<h2>' . $entity_node->title . ' Worker Statements' . '</h2>';
		$html .= sirius_table_render($header, $rows);
	}

	if (!$html) { $html = '<p>' . 'No accounts found.' . '</p>'; }

	return $html;
}
