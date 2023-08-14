<?php

function sirius_node_tab_worker_relationships($worker_node) {
	sirius_require('sirius', 'sirius.page_contact_relationships.inc');

	$html = sirius_node_tab_contact_relationships(node_load($worker_node->field_sirius_contact['und'][0]['target_id']));

	return $html;
}
