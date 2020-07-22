<?php

function sirius_ledger_node_tab_employer_payments($employer_node) {
	return views_embed_view('sirius_payments', 'block_2', $employer_node->nid);
}