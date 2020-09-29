<?php

function sirius_ledger_node_tab_worker_ar($worker_node) {
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { return 'There is no contact associated with this worker.'; }
	$entries = sirius_ledger_ar_fetch(array('ledger_participant' => $contact_node->nid));
	return sirius_ledger_ar_render($entries, array('hide_participant' => TRUE));
}
