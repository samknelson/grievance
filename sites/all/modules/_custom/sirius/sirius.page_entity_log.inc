<?php

function sirius_node_tab_entity_log_quicknav($log_bucket) {
	$item = menu_get_item();
	$href = $item['href'];

	$links = array();

	if ($log_bucket['views']) {
		foreach ($log_bucket['views'] as $view_id => $view_record) {
			$links[] = array('path' => "$href/view/$view_id", 'title' => $view_record['title']); 
		}
	} else {
		$links[] = array('path' => $href, 'title' => 'View');
	}

	if ($log_bucket['can_add']) { $links[] = array('path' => "$href/add", 'title' => 'Add'); }

	if (count($links) < 2) { return ''; }

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
		$view_id = sirius_node_tab_arg(1);
		if (!$view_id && $log_bucket['views']) {
			$default_view_id = reset(array_keys($log_bucket['views']));
			drupal_goto(current_path() . "/view/$default_view_id");
		}
		
		return drupal_get_form('sirius_node_tab_entity_log_view_form', $node, $log_bucket_id, $view_id);
	}
}

