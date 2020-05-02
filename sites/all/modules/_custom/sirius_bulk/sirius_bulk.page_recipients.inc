<?php

function sirius_bulk_node_tab_recipients($bulk_node) {
	drupal_goto('node/' . $bulk_node->nid . '/sirius_bulk_recipients/workers');
}