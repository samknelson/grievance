<?php

function sirius_employer_page_me($tab = NULL) {
	$employer_node = sirius_employer_get();
	if (!$employer_node) { return t('No employer found.'); }

	$path = 'node/' . $employer_node->nid;
	if ($tab) { $path .= '/' . $tab; } 
	
	drupal_goto($path);
}
