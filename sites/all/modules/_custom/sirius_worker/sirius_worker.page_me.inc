<?php

function sirius_worker_page_me($tab = NULL) {
	$worker_node = sirius_worker_get();
	if (!$worker_node) { return t('No worker found.'); }

	$path = 'node/' . $worker_node->nid;
	if ($tab) { $path .= '/' . $tab; } 
	
	drupal_goto($path);
}