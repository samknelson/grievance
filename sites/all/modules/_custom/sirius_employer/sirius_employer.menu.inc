<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "employer" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_employer_node_tabs() {
	$node_tabs = array();

	$node_tabs['grievance_shop']['employer_users'] = array(
		'title' => 'Users',
		'description' => '',
		'callback' => 'sirius_employer_node_tab_employer_users',
		'permissions' => array('sirius employer user manager'),
	);

  $node_tabs['grievance_shop']['employer_taxonomy_mappings'] = array(
    'title' => 'Drop-Down Mappings',
    'description' => '',
    'callback' => 'sirius_employer_node_tab_taxonomy_mappings',
    'permissions' => array('sirius employer user manager'),
  );


	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_employer_menu() {
  $items = array();

  $items['sirius/employer/me'] = array(
    'title' => 'Sirius: Employer: Me',
    'description' => '',
    'page callback' => 'sirius_employer_page_me',
    'page arguments' => array(3),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
	);

  $items['sirius/employer/config/usermanage'] = array(
    'title' => 'Employer User Manager Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_employer_usermanage_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

	return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_employer_modal_paths() {
  $paths = array();

  /*
  $paths['grievance/modal/%node/log-add'] = array(
    'style' => 'grievance-modal-style',
  );
  */

  return $paths;
}

function sirius_employer_modal_styles() {
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



