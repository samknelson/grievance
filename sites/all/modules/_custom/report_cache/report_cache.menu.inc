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
		'page arguments' => array(),
		'access arguments' => array('sirius'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/reports/by-module/%'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_landing',
		'page arguments' => array(3),
		'access callback' => 'report_cache_page_landing_access',
		// 'access arguments' => array('sirius'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/reports/list'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_landing',
		'page arguments' => array(3),
		'access arguments' => array('sirius'),
		'weight' => 1,
		'type' => MENU_DEFAULT_LOCAL_TASK
	);

	// @todo: Some day, it would be nice if all of these were
	//        tabs on the settings node. There's one json definition 
	//        node for each report type (and domain); we should probably 
	//        change the navigation so that the menu items below are tabs
	//        on those nodes.
	
	$items['sirius/reports/%'] = array(
		'title' => 'Reports',
		'description' => '',
		'page callback' => 'report_cache_page_report',
		'page arguments' => array(2),
   	'access callback' => 'report_cache_access',
    'access arguments' => array(2),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/reports/%/modal'] = array(
		'title' => 'Modal',
		'description' => '',
		'page callback' => 'report_cache_page_report',
		'page arguments' => array(2),
   	'access callback' => 'report_cache_access',
    'access arguments' => array(2),
		'type' => MENU_NORMAL_ITEM,
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
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
		'access callback' => 'report_cache_page_report_auto_access',
		'access arguments' => array(2),
		'type' => MENU_LOCAL_TASK,
		'weight' => 3,
		'file' => 'report_cache.page_report_auto.inc',
	);

	$items['sirius/reports/%/instances'] = array(
		'title' => 'Instances',
		'description' => '',
		'page callback' => 'report_cache_page_report_instances',
		'page arguments' => array(2),
		'access callback' => 'report_cache_page_report_instances_access',
		'access arguments' => array(2),
		'type' => MENU_LOCAL_TASK,
		'weight' => 3,
		'file' => 'report_cache.page_report_instances.inc',
	);

  return $items;
}

function report_cache_modal_paths() {
  $paths = array();

  $paths['sirius/reports/%/modal'] = array(
    'style' => 'report-cache-style',
  );

  return $paths;
}

function report_cache_modal_styles() {
  $styles = array();

  $styles['report-cache-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 800,
      'height' => 800,
    ),
  );

  return $styles;
}

