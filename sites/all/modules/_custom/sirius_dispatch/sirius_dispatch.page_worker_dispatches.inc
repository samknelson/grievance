<?php

function sirius_node_tab_worker_dispatches_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dispatches($worker_node) {
	return views_embed_view('sirius_worker_dispatches', 'block', $worker_node->nid);
}
