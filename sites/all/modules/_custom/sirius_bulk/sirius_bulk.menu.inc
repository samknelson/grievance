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
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_body_voice'] = array(
    'title' => 'Voice Message',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_body_voice',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
    'file' => 'sirius_bulk.page_body_voice.inc',
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_body_sms'] = array(
    'title' => 'SMS Message',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_body_sms',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
    'file' => 'sirius_bulk.page_body_sms.inc',
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_body_postal'] = array(
    'title' => 'Postal Message',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_body_postal',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
    'file' => 'sirius_bulk.page_body_postal.inc',
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_body_email'] = array(
    'title' => 'Email Message',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_body_email',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
    'file' => 'sirius_bulk.page_body_email.inc',
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_test'] = array(
    'title' => 'Test',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_test',
    'permissions' => array('sirius bulk send'),
    'weight' => 3,
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_recipients'] = array(
    'title' => 'Recipients',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipients_workers',
    'permissions' => array('sirius bulk send'),
    'weight' => 4,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_recipients/contacts'] = array(
    'title' => 'Contacts',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipients_contacts',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_recipients/workers'] = array(
    'title' => 'Workers',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipients_workers',
    'permissions' => array('sirius bulk send'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_bulk']['sirius_bulk_manual'] = array(
    'title' => 'Manual Confirmation',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_manual',
    'permissions' => array('sirius bulk manual'),
    'weight' => 5,
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_send'] = array(
    'title' => 'Send',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_send',
    'permissions' => array('sirius bulk send'),
    'weight' => 6,
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_send/send'] = array(
    'title' => 'Send',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_send',
    'permissions' => array('sirius bulk send'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'component' => 'bulk',
  );

  $node_tabs['sirius_bulk']['sirius_bulk_send/throttle'] = array(
    'title' => 'Throttle',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_throttle',
    'permissions' => array('sirius bulk send'),
    'weight' => 2,
    'file' => 'sirius_bulk.page_throttle.inc',
    'component' => 'bulk',
  );

	$node_tabs['sirius_bulk']['sirius_bulk_queue'] = array(
		'title' => 'Results',
		'description' => '',
		'callback' => 'sirius_bulk_node_tab_queue',
		'permissions' => array('sirius bulk send'),
		'weight' => 7,
    'component' => 'bulk',
	);

  $node_tabs['sirius_bulk']['sirius_bulk_delete'] = array(
    'title' => 'Delete',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_delete',
    'permissions' => array('sirius bulk admin'),
    'weight' => 7,
    'file' => 'sirius_bulk.page_delete.inc',
    'component' => 'bulk',
  );

  $node_tabs['sirius_log']['sirius_bulk_recipient'] = array(
    'title' => 'Bulk Recipient',
    'description' => '',
    'callback' => 'sirius_bulk_node_tab_recipient',
    'permissions' => array('sirius bulk admin'),
    'weight' => 7,
    'file' => 'sirius_bulk.page_recipient.inc',
    'component' => 'bulk',
  );

  sirius_node_tab_set_module('sirius_bulk', $node_tabs);
	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_bulk_menu() {
  $items = array();

  $items['sirius/bulk'] = array(
    'title' => 'Bulk Messages',
    'description' => '',
    'access callback' => 'sirius_bulk_page_landing_access',
    'access arguments' => array(),
    'page callback' => 'sirius_bulk_page_landing',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
  );

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



