<?php

function sirius_node_tab_entity_customfield_multiple_edit_access($data_node) {
	// Only log entries of category "customfield"
	if ($data_node->field_sirius_category['und'][0]['value'] != 'customfield') { return FALSE; }

	// Get the set
	$set_nid = $data_node->field_sirius_type['und'][0]['value'];
	$set_node = node_load($set_nid);

	// Get the entity
	$entity_node = sirius_log_handler($data_node, NULL);

	// Check our access
	$context = array(
		'set_node' => $set_node,
		'entity_node' => $entity_node,
		'data_node' => $data_node
	);
	return sirius_customfield_access('view', $context);
}

function sirius_node_tab_entity_customfield_multiple_edit($data_node) {
	sirius_require('sirius', 'sirius.page_entity_customfield_single.inc');
	$set_nid = $data_node->field_sirius_type['und'][0]['value'];
	$set_node = node_load($set_nid);
	$entity_node = sirius_log_handler($data_node, NULL);
	return drupal_render(drupal_get_form('sirius_node_tab_entity_customfield_form', $data_node, $set_node));
}