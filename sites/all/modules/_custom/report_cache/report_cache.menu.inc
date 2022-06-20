<?php

/***
 * Implements hook_menu
 */
function report_cache_menu() {
  $items = array();

	$items['sirius/reports'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_landing',
		'page arguments' => array(3),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/reports/list'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_landing',
		'page arguments' => array(3),
		'access arguments' => array('grievance report'),
		'weight' => 1,
		'type' => MENU_DEFAULT_LOCAL_TASK
	);

	$items['sirius/reports/%'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_report',
		'page arguments' => array(2),
   	'access callback' => 'report_cache_access',
    'access arguments' => array(2),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/reports/%/list'] = array(
		'title' => 'List',
		'description' => '',
		'page callback' => 'report_cache_page_report',
		'page arguments' => array(2),
   	'access callback' => 'report_cache_access',
    'access arguments' => array(2),
		'type' => MENU_DEFAULT_LOCAL_TASK,
		'weight' => 1,
	);

	$items['sirius/reports/%/form'] = array(
		'title' => 'Generate',
		'description' => '',
		'page callback' => 'report_cache_page_report_generate',
		'page arguments' => array(2),
		'access callback' => 'report_cache_access',
		'access arguments' => array(2),
		'type' => MENU_LOCAL_TASK,
		'weight' => 2,
	);

	$items['sirius/reports/%/auto'] = array(
		'title' => 'Auto',
		'description' => '',
		'page callback' => 'report_cache_page_report_auto',
		'page arguments' => array(2),
		'access callback' => 'report_cache_access',
		'access arguments' => array(2),
		'type' => MENU_LOCAL_TASK,
		'weight' => 3,
		'file' => 'report_cache.page_report_auto.inc',
	);

  return $items;
}
