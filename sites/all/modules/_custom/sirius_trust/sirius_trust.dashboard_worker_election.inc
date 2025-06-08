<?php

function sirius_trust_dashboard_worker_election_data($prefs, $options = array()) {
	$rows = array();

	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(); }

	$election_node = sirius_trust_worker_election($worker_node);
	if (!$election_node) { return array(); }

	$relationship_nids = sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id');
	$relationship_nodes = node_load_multiple($relationship_nids);
	if (!$relationship_nodes) { return array(); }

	$html = '';
	foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
		$related_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$html .= '<li>';
		$html .= sirius_node_title($related_contact_nid);
		$html .= '</li>';
	}
	if (!$html) { return array(); }
	$html = "<ul>$html</ul>";


	$tools = l(
		'View Details',
		'node/' . $worker_node->nid . '/sirius_worker_relationships',
		array('attributes' => array('class' => array('tool-button')))
	);

	$record = array(
		'tools' => $tools,
		'details' => $html,
	);

	return array($record);
}
