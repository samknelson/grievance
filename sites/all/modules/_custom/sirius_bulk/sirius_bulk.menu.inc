<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_bulk_node_tabs() {
	$node_tabs = array();

	$node_tabs['sirius_bulk']['sirius_bulk_workers'] = array(
		'title' => 'Recipients',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_workers',
		'permissions' => array('sirius bulk send'),
		'weight' => 1,
	);

	$node_tabs['sirius_bulk']['sirius_bulk_test'] = array(
		'title' => 'Test',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_test',
		'permissions' => array('sirius bulk send'),
		'weight' => 2,
	);


	$node_tabs['sirius_bulk']['sirius_bulk_queue'] = array(
		'title' => 'Queue',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_queue',
		'permissions' => array('sirius bulk send'),
		'weight' => 3,
	);

	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_bulk_menu() {
  $items = array();

  $items['sirius/config/bulk'] = array(
    'title' => 'Bulk Message Configuration',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_bulk_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/bulk/oneclick'] = array(
    'title' => 'One-Click Messages',
    'description' => '',
    'access arguments' => array('sirius bulk send'),
    'page callback' => 'sirius_bulk_page_oneclick_list',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/bulk/oneclick/%'] = array(
    'title' => 'One-Click Messages',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'sirius_bulk_page_oneclick',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_bulk_modal_paths() {
  $paths = array();

  /*
  $paths['grievance/modal/%node/log-add'] = array(
    'style' => 'grievance-modal-style',
  );
  */

  return $paths;
}

function sirius_bulk_modal_styles() {
  $styles = array();

  /*
  $styles['grievance-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 500,
      'height' => 350,
    ),
  );
  */

  return $styles;
}



