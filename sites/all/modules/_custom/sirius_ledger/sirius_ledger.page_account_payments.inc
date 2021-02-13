<?php

function sirius_ledger_node_tab_account_payments($account_node) {
	return views_embed_view('sirius_payments', 'block_1', $account_node->nid);

}