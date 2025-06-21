<?php

function sirius_smf_page_wym_modal($wym_node) {
	if (!$wym_node->type == 'sirius_worker_month') { return sirius_page_error('Not a WYM node.'); }
	drupal_set_title($wym_node->title);
	$renderable = node_view($wym_node);
	return drupal_render($renderable);
}