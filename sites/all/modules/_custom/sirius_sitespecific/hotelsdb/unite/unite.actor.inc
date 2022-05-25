<?php

function unite_actor_node_presave($node) {
	$node->field_title_canon['und'][0]['value'] = unite_actor_canonicalize_title($node->title);
}

/**
 * All uppercase
 * +,& turn into AND
 * Remove all other punctuation
 * Delete leading “The”
 * Delete ending “Co.” or “Co”
 * Delete ending “Company”
 * Delete ending “Inc.” or “Inc”
 * Delete ending “Limited” or “Ltd” or “Ltd.”
 * Delete ending LLC
 * Delete ending Corp OR corporation
 * Remove all spaces
 */

function unite_actor_canonicalize_title($title) {
	$title = strtoupper($title);
	$title = preg_replace('/[\+\&]/', 'AND', $title);
	$title = preg_replace('/[^A-Z0-9 ]/', ' ', $title);
	$title = trim($title);
	$title = preg_replace('/^THE /', '', $title);
	$title = preg_replace('/ CO$/', '', $title);
	$title = preg_replace('/ COMPANY$/', '', $title);
	$title = preg_replace('/ INC$/', '', $title);
	$title = preg_replace('/ LTD$/', '', $title);
	$title = preg_replace('/ LIMITED$/', '', $title);
	$title = preg_replace('/ LLC$/', '', $title);
	$title = preg_replace('/ CORP$/', '', $title);
	$title = preg_replace('/ CORPORATION$/', '', $title);
	$title = preg_replace('/ /', '', $title);
	return $title;
}
