<?php

function sirius_ledger_node_tab_employer_payments($entity_node) {
	$params = array();
	$params['payer_nid'] = $entity_node->nid;
	$payment_nodes = sirius_ledger_payments_search($params);
	return sirius_ledger_payments_render($payment_nodes);
}