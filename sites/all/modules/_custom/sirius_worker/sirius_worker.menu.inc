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

  $node_tabs['sirius_worker']['sirius_worker_industry'] = array(
    'title' => 'Industries',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_industry',
    'permissions' => array('sirius'),
  );

	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_worker_menu() {
  $items = array();

  //
	// Node Tabs
  // 

	$node_tabs = sirius_worker_node_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		foreach ($node_record as $tab_name => $tab_record) {
			$items['node/%node/' . $tab_name] = array(
				'title' => $tab_record['title'],
				'description' => $tab_record['description'],
				'page callback' => 'sirius_worker_node_tab_content',
				'page arguments' => array(1, $tab_name),
				'access callback' => 'sirius_worker_node_tab_access',
				'access arguments' => array(1, $tab_name),
				'type' => MENU_LOCAL_TASK,
			);
			if ($tab_record['weight']) {
				$items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
			}
		}
	}

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

  $items['sirius/worker/me'] = array(
    'title' => 'Sirius: Worker: Me',
    'description' => '',
    'page callback' => 'sirius_worker_page_me',
    'page arguments' => array(3),
    'access arguments' => array('sirius dispatch worker'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/config'] = array(
    'title' => 'Worker Configuration ',
    'description' => '',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_worker_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

function sirius_worker_node_tab_content($node, $in_tab_name) {
	$node_tabs = sirius_worker_node_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		if ($node->type == $node_type) {
			foreach ($node_record as $tab_name => $tab_record) {
				if ($tab_name == $in_tab_name) {
					$callback = $tab_record['callback'];
					return $callback($node);
				}
			}
		}
	}
	
	drupal_set_message("Bad tab: $node->type, $in_tab_name");
	
	return "";
}

function sirius_worker_node_tab_access($node, $in_tab_name) {
	// Check generic permissions
	$node_tabs = sirius_worker_node_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		if ($node->type == $node_type) {
			foreach ($node_record as $tab_name => $tab_record) {
				if ($tab_name == $in_tab_name) {
					foreach ($tab_record['permissions'] as $permission) {
						if (!user_access($permission)) { return false; }
					}
					
					return true;
				}
			}
		}
	}
	
	return false;
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



