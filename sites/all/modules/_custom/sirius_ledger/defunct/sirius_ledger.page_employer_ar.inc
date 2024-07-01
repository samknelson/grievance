<?php

function sirius_ledger_node_tab_employer_ar_access($entity_node) {
	$access = sirius_access_instance('ledger');
	return $access->access('entity_page_view', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_employer_ar($employer_node) {
	$entries = sirius_ledger_ar_fetch(array('ledger_participant' => $employer_node->nid));
	return sirius_ledger_ar_render($entries);
}
