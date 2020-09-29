<?php

function sirius_ledger_node_tab_worker_payments($worker_node) {
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { return 'There is no contact associated with this worker.'; }

	$html = '';
	$html .= l('Add a Payment or Adjustment', 'sirius/ledger/payment/create', array(
		'query' => array('destination' => current_path(), 'payer_worker' => $worker_node->field_sirius_id['und'][0]['value']), 
		'attributes' => array('class' => array('tool-button'))
	));
	$html .= views_embed_view('sirius_payments', 'block_2', $contact_node->nid);
	return $html;
}