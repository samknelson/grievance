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

  //
	// Node Tabs
  // 

	$node_tabs = sirius_bulk_node_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		foreach ($node_record as $tab_name => $tab_record) {
			$items['node/%node/' . $tab_name] = array(
				'title' => $tab_record['title'],
				'description' => $tab_record['description'],
				'page callback' => 'sirius_bulk_node_tab_content',
				'page arguments' => array(1, $tab_name),
				'access callback' => 'sirius_bulk_node_tab_access',
				'access arguments' => array(1, $tab_name),
				'type' => MENU_LOCAL_TASK,
			);
			if ($tab_record['weight']) {
				$items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
			}
		}
	}

  $items['sirius/config/bulk'] = array(
    'title' => 'Bulk Message Configuration ',
    'description' => '',
    'access arguments' => array('sirius bulk admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_bulk_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

function sirius_bulk_node_tab_content($node, $in_tab_name) {
	$node_tabs = sirius_bulk_node_tabs();
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

function sirius_bulk_node_tab_access($node, $in_tab_name) {
	// Check generic permissions
	$node_tabs = sirius_bulk_node_tabs();
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



