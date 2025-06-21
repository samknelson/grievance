<?php

function sirius_ledger_node_tab_contact_payments($contact_node) {
	return views_embed_view('sirius_payments', 'block_2', $contact_node->nid);

}