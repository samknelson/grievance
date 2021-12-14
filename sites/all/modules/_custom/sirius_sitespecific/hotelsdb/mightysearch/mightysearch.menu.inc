<?php
/***
 * Implements hook_menu
 */
function mightysearch_menu() {
  $items = array();
	$items['mightysearch'] = array(
		'page callback' => 'drupal_get_form',
		'page arguments' => array('mightysearch_search_form'),
		'access arguments' => array('unite admin'),
		'type' => MENU_CALLBACK,
	);

  return $items;
}


function mightysearch_permission() {
	/*
  return array(
    'unite hotels admin' => array(
      'title' => t('UNITE Hotels Admin'), 
      'description' => t('Use the hotels database'),
    ),
    'unite hotels acquire' => array(
      'title' => t('UNITE Hotels Acquire'), 
      'description' => t('Cause one actor to acquire another'),
    ),
  );	
	*/
}
