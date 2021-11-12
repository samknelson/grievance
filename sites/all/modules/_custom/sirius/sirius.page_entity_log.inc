<?php

function sirius_node_tab_entity_log_quicknav($log_bucket) {
	if (!$log_bucket['can_add']) { return ''; }
	
	$item = menu_get_item();
	$href = $item['href'];

	$links = array(
		array('path' => $href, 'title' => 'View'),
		array('path' => "$href/add", 'title' => 'Add'),
	);

	return sirius_quicknav($links);

}

function sirius_node_tab_entity_log_access($node, $log_bucket_id) {
	return TRUE;
}

function sirius_node_tab_entity_log($node, $log_bucket_id) {
	sirius_require('sirius', 'sirius.page_entity_log_add.inc');
	sirius_require('sirius', 'sirius.page_entity_log_view.inc');

	$log_bucket = sirius_log_bucket($log_bucket_id);

	$action = sirius_node_tab_arg(0);

	if (($action == 'add') && $log_bucket['can_add']) {
		return drupal_get_form('sirius_node_tab_entity_log_add_form', $node, $log_bucket_id);
	} else {
		return drupal_get_form('sirius_node_tab_entity_log_view_form', $node, $log_bucket_id);
	}
}

