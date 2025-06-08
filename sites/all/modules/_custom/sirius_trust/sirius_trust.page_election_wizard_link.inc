<?php
function sirius_trust_node_tab_election_wizard_link_access($election_node) {
	$records = sirius_trust_election_wizard_search(array('election_nid' => $election_node->nid, 'limit' => 1));
	if (!$records) { return FALSE; }
	return TRUE;
}

function sirius_trust_node_tab_election_wizard_link($election_node) {
	$records = sirius_trust_election_wizard_search(array('election_nid' => $election_node->nid, 'limit' => 1));
	if (!$records) { return 'No election wizard found.'; }
	$wizard_nid = reset(array_keys($records));
	$wizard_node = node_load($wizard_nid);

	$html = '<p>';
	$html .= t('This election was created by a wizard.');
	$html .= '</p>';
	$html .= l('View Wizard', 'node/' . $wizard_node->nid . '/sirius_trust_election_wizard', array('attributes' => array('class' => array('tool-button'))));
	return $html;
}
