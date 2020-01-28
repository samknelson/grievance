<?php

function sirius_bulk_node_tab_workers($bulk_node) {
	$view = views_get_view('sirius_bulkmsg');
	if (!$view || !$view->access('block_1')) { return; }	
	$result = $view->preview('block_1');
	return $result;
}