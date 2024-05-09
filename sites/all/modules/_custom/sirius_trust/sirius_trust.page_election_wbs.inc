<?php

function sirius_trust_node_tab_election_wbs($election_node) {
	$wb_nodes = sirius_trust_worker_benefits_search(array('election_nid' => $election_node->nid));
	$worker_node = node_load($election_node->field_sirius_worker['und'][0]['target_id']);

	return sirius_trust_render_worker_benefits($worker_node, 'from', $wb_nodes);
}