<?php
/***
 * Implements hook_menu
 */
function mightysearch_menu() {
  $items = array();

	$items['mightysearch'] = array(
    'title' => 'Properties Search',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('mightysearch_search_form'),
		'access arguments' => array('unite hotels user'),
		'type' => MENU_CALLBACK,
	);

  $items['mightysearch/search'] = array(
    'title' => 'Search',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mightysearch_search_form'),
    'access arguments' => array('unite hotels user'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['mightysearch/saved'] = array(
    'title' => 'Saved Searches',
    'page callback' => 'mightysearch_page_saved_mine',
    'page arguments' => array(),
    'access arguments' => array('unite hotels user'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'mightysearch.page_saved_mine.inc',
  );

  $items['mightysearch/saved/mine'] = array(
    'title' => 'Mine',
    'page callback' => 'mightysearch_page_saved_mine',
    'page arguments' => array(),
    'access arguments' => array('unite hotels user'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'mightysearch.page_saved_mine.inc',
  );

  $items['mightysearch/saved/all'] = array(
    'title' => 'All',
    'page callback' => 'mightysearch_page_saved_all',
    'page arguments' => array(),
    'access arguments' => array('unite hotels user'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'mightysearch.page_saved_all.inc',
  );

  return $items;
}
