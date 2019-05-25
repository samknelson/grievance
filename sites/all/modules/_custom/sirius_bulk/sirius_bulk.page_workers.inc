<?php

function sirius_bulk_node_tab_workers($bulk_node) {
	return views_embed_view('sirius_bulkmsg', 'block_1', $bulk_node->nid);
}