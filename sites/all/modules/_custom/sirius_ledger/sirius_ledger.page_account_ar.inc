<?php

function sirius_ledger_node_tab_account_ar($account_node) {
	$entries = sirius_ledger_ar_fetch(array('ledger_account' => $account_node->nid));
	return sirius_ledger_ar_render($entries);
}