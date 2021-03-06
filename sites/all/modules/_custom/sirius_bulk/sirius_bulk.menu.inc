<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_bulk_node_tabs() {
	$node_tabs = array();

  $node_tabs['sirius_bulk']['sirius_bulk_medium'] = array(
    'title' => 'Medium',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_medium',
    'permissions' => array('sirius bulk send'),
    'weight' => 1,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_body'] = array(
    'title' => 'Body',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_body',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_test'] = array(
    'title' => 'Test',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_test',
    'permissions' => array('sirius bulk send'),
    'weight' => 3,
  );

	$node_tabs['sirius_bulk']['sirius_bulk_recipients'] = array(
		'title' => 'Recipients',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_recipients_contacts',
		'permissions' => array('sirius bulk send'),
		'weight' => 4,
	);

  $node_tabs['sirius_bulk']['sirius_bulk_recipients/contacts'] = array(
    'title' => 'Contacts',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipients_contacts',
    'permissions' => array('sirius bulk send'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_recipients/workers'] = array(
    'title' => 'Workers',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipients_workers',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_manual'] = array(
    'title' => 'Manual Confirmation',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_manual',
    'permissions' => array('sirius bulk manual'),
    'weight' => 5,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_send'] = array(
    'title' => 'Send',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_send',
    'permissions' => array('sirius bulk send'),
    'weight' => 6,
  );

	$node_tabs['sirius_bulk']['sirius_bulk_queue'] = array(
		'title' => 'Results',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_queue',
		'permissions' => array('sirius bulk send'),
		'weight' => 7,
	);

	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_bulk_menu() {
  $items = array();

  $items['sirius/bulk/config'] = array(
    'title' => 'Bulk Message Configuration',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_bulk_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/bulk/config/config'] = array(
    'title' => 'Config',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_bulk_config'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/bulk/config/poll'] = array(
    'title' => 'Poll',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'sirius_bulk_page_poll',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
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

  $items['sirius/bulk/create'] = array(
    'title' => 'Create a Bulk Message',
    'description' => '',
    'access arguments' => array('sirius bulk send'),
    'page callback' => 'sirius_bulk_page_create',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/bulk/manual'] = array(
    'title' => 'Manually Deliver Bulk Messages',
    'description' => '',
    'access arguments' => array('sirius bulk manual'),
    'page callback' => 'sirius_bulk_page_manual',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
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



