<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_worker_node_tabs() {
	$node_tabs = array();

	$node_tabs['sirius_worker']['worker_user'] = array(
		'title' => 'User',
		'description' => '',
		'callback' => 'sirius_node_tab_worker_user',
		'permissions' => array('sirius worker admin'),
	);

  $node_tabs['sirius_worker']['sirius_worker_work_status'] = array(
    'title' => 'Work Status',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_work_status',
    'permissions' => array('sirius'),
  );

  $node_tabs['sirius_worker']['sirius_worker_skill'] = array(
    'title' => 'Skills',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill',
    'permissions' => array('sirius'),
  );

  $node_tabs['sirius_worker']['sirius_worker_contact'] = array(
    'title' => 'Contact',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact',
    'permissions' => array('sirius'),
  );

  $node_tabs['sirius_worker']['sirius_worker_industry'] = array(
    'title' => 'Industries',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_industry',
    'permissions' => array('sirius'),
  );

  $node_tabs['sirius_worker']['sirius_worker_merge'] = array(
    'title' => 'Merge',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_merge',
    'permissions' => array('sirius worker admin'),
  );

	return $node_tabs;
}


function sirius_worker_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_member_status']['sirius_member_status_optin'] = array(
    'title' => 'Opt-In',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_member_status_optin',
    'permissions' => array('sirius worker admin'),
    'weight' => 4,
  );

  return $term_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_worker_menu() {
  $items = array();

  $items['sirius/employer/worker/list'] = array(
    'title' => 'Sirius: Employer: Worker: List',
    'description' => '',
    'page callback' => 'sirius_worker_page_employer_worker_list',
    'page arguments' => array(),
    'access callback' => array('sirius_worker_page_employer_worker_list_access'),
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/worker/list/%'] = array(
    'title' => 'Sirius: Employer: Worker: List',
    'description' => '',
    'page callback' => 'sirius_worker_page_employer_worker_list',
    'page arguments' => array(4),
    'access callback' => array('sirius_worker_page_employer_worker_list_access'),
    'access arguments' => array(4),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/worker/list-ineligible'] = array(
    'title' => 'Sirius: Employer: Worker: Ineligible List',
    'description' => '',
    'page callback' => 'sirius_worker_page_employer_worker_list_ineligible',
    'page arguments' => array(),
    'access callback' => array('sirius_worker_page_employer_worker_list_ineligible_access'),
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/me'] = array(
    'title' => 'Sirius: Worker: Me',
    'description' => '',
    'page callback' => 'sirius_worker_page_me',
    'page arguments' => array(3),
    'access arguments' => array('sirius dispatch worker'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/skill/expiring'] = array(
    'title' => 'Skill Expiration Report',
    'description' => '',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_skill_expiring',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/skill/expiring/%'] = array(
    'title' => 'Skill Expiration Report',
    'description' => '',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_skill_expiring',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/me'] = array(
    'title' => 'Sirius: Worker: Me',
    'description' => '',
    'page callback' => 'sirius_worker_page_me',
    'page arguments' => array(3),
    'access arguments' => array('sirius dispatch worker'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/skill-delete'] = array(
    'title' => 'Delete a Skill',
    'description' => '',
    'page callback' => 'sirius_worker_skill_term_delete',
    'page arguments' => array(3),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );


  $items['sirius/worker/config/user'] = array(
    'title' => 'Worker / User Configuration',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_worker_user_admin'),
    'type' => MENU_NORMAL_ITEM,
  );


  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_worker_modal_paths() {
  $paths = array();

  /*
  $paths['grievance/modal/%node/log-add'] = array(
    'style' => 'grievance-modal-style',
  );
  */

  return $paths;
}

function sirius_worker_modal_styles() {
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



