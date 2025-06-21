<?php

function sirius_ledger_page_ar_view($ledger_id) {
	$entry = sirius_ledger_ar_fetch_entry($ledger_id);
	if (!$entry) { return "<em>Ledger entry <strong>$ledger_id</strong> not found."; }

	$html .= sirius_fakerender_field("ID", $entry['ledger_id']);
	$html .= sirius_fakerender_field("Status", $entry['ledger_status']);
	$html .= sirius_fakerender_field("Account", sirius_node_title($entry['ledger_account']) . ' (' . $entry['ledger_account'] . ')');
	$html .= sirius_fakerender_field("Participant", sirius_node_title($entry['ledger_participant']) . ' (' . $entry['ledger_participant'] . ')');
	$html .= sirius_fakerender_field("Reference", sirius_node_title($entry['ledger_reference']) . ' (' . $entry['ledger_reference'] . ')');
	$html .= sirius_fakerender_field("Date", date('c', $entry['ledger_ts']));
	$html .= sirius_fakerender_field("Memo", $entry['ledger_memo']);
	$html .= sirius_fakerender_field("Key", $entry['ledger_key']);
	$html .= sirius_fakerender_field("Amount", '$' . number_format($entry['ledger_amount']));

	// We'd like to use our Sirius json viewer. But that doesn't play nicely with modals.

	$html .= '<h2>' . t('Metadata') . '</h2>';

	$json = json_decode($entry['ledger_json'], TRUE);
	$html .= '<pre>';
	$html .= json_encode($json, JSON_PRETTY_PRINT);
	$html .= '</pre>';

	return $html;
}