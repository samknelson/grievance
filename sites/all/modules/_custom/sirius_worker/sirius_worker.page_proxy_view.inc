<?php

function sirius_worker_page_proxy_view_fetch_with_access_check($rnid) {
	// Proxy must be active
	if (!sirius_proxy_active()) { return NULL; }

	// Fetch the rnode
	$result = sirius_proxy_call_wrap('sirius_proxy_node_load', $rnid);
	if (!$result['success']) { return NULL; }
	$rnode = $result['data'];
	if (!$rnode) { return NULL; }

	// Make sure we fetched a worker
	$type = $rnode->type;
	if ($type != 'sirius_worker') { return NULL; }

	// Admins can see
	if (user_access('sirius worker view')) { return $rnode; }

	// @todo: Employers can see their own workers, maybe?

	// Workers can see their own record
	if (user_access('sirius')) {
		$result = sirius_proxy_call_wrap('sirius_worker_get');
		if (!$result['success']) { return FALSE; }
		$worker_me = $result['data'];
		if ($worker_me->nid == $rnid) { return $rnode; }
	}

	// Oh well
	return NULL;
}

function sirius_worker_page_proxy_view_access($rnid) {
	$worker_rnode = sirius_worker_page_proxy_view_fetch_with_access_check($rnid);
	if (!$worker_rnode) { return FALSE; }
	return TRUE; 
}

function sirius_worker_page_proxy_view($rnid) {
	$worker_rnode = sirius_worker_page_proxy_view_fetch_with_access_check($rnid);
	drupal_set_title($worker_rnode->title);
	return '<em>' . t('[@todo: decide what renders on the proxy server when viewing a worker]') . '</em>';
}