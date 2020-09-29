<?php

function sirius_ledger_node_tab_employer_ar($employer_node) {
	$entries = sirius_ledger_ar_fetch(array('ledger_participant' => $employer_node->nid));
	return sirius_ledger_ar_render($entries);
}
