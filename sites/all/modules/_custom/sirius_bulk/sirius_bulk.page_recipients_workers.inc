<?php

function sirius_bulk_node_tab_recipients_workers($bulk_node) {
	// @todo: This is a terrible hack. But it's surprisingly difficult to pass the bulk node from this page into the view
	//        in such a way that a VBO action will receive it. I tried passing setting "'pass rows' => TRUE,",
	//        but that only works for individually checked boxes, not boxes on a page on the view that is not displayed.
	//        And setting attributes of the view itself was unsuccessful. And since the activity happens on the bulk processing page,
	//        the view can't look at the URL. So ... a hack, "good 'nuff for now".
	$_SESSION['my_bulk_nid'] = $bulk_node->nid;

	$view = views_get_view('sirius_bulkmsg');
	if (!$view || !$view->access('block_1')) { return; }	
	$result = $view->preview('block_1');
	return $result;
}