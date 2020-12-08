<?php

function sirius_ledger_node_tab_account_ar($account_node) {
	$entries = sirius_ledger_ar_fetch(array('ledger_account' => $account_node->nid, 'limit' => 1000));
	return sirius_ledger_ar_render($entries);
}