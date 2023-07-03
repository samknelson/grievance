<?php

function sirius_node_tab_entity_customfield_multiple_quicknav($entity_node, $set_node) {
	$context = array('set_node' => $set_node, 'entity_node' => $entity_node);
	if (!sirius_customfield_access('add', $context)) { return ''; }

	$item = menu_get_item();
	$href = $item['href'];
	$href .= '/' . $set_node->nid;
	$links = array(
		array('path' => $href, 'title' => 'List'),
		array('path' => "$href/add", 'title' => 'Add'),
	);

	return sirius_quicknav($links);
}

function sirius_node_tab_entity_customfield_multiple($entity_node, $set_node) {
	$action = sirius_node_tab_arg(1);

	$context = array('set_node' => $set_node, 'entity_node' => $entity_node);
	if ($action == 'add' && sirius_customfield_access('add', $context)) {
		sirius_require('sirius', 'sirius.page_entity_customfield_multiple_add.inc');
		return drupal_get_form('sirius_node_tab_entity_customfield_multiple_add_form', $entity_node, $set_node);
	} else {
		sirius_require('sirius', 'sirius.page_entity_customfield_multiple_list.inc');
		return drupal_get_form('sirius_node_tab_entity_customfield_multiple_list_form', $entity_node, $set_node);
	}
}

