<?php

/**
 * I am an API-only module.  To make good use of me:
 *
 * 1. Make sure that your content type has a field_aat in it.
 *
 * 2. At a suitable time, call sirius_aat_token_set($node).  You might consider doing so during hook_node_presave().
 *
 * 3. Pass around to your users any link to any page on the side that ends in ?aat=[access_token]
 *
 * 4. When you want to know if somebody has the access token, call sirius_aat_check($node).  You might consider doing so during hook_node_access().
 */

/**
 * Implements hook_page_build
 */

function sirius_page_build(&$page) {
	$aat = $_REQUEST['aat'];
	if (!$aat) { return; }
	$_SESSION['aat'][$aat] = TRUE;
}

function sirius_aat_set($node, $len = 6) {
	if (!$node->field_sirius_aat['und'][0]['value']) {
	  // $characters = '23456789ABCDEFGHJKLMNPQRSTUVWXYZ'; // Leave off 0/O, 1/I.
	  // Use digits so this is phone-friendly.
	  $characters_first = '123456789';
	  $characters = '0123456789';
  	$aat = '';
  	for ($i = 0; $i < $len; $i++) {
  		if ($i == 0) {
    		$aat .= $characters_first[rand(0, strlen($characters))];
    	} else {
    		$aat .= $characters[rand(0, strlen($characters))];
    	}
  	}
  	$node->field_sirius_aat['und'][0]['value'] = $aat;
	}
}

function sirius_aat_check($node) {
	$aat = $node->field_sirius_aat['und'][0]['value'];
	if (!$aat) { return FALSE; }
	if ($_REQUEST['aat'] == $aat) { return TRUE; }
	return $_SESSION['aat'][$aat];
}
