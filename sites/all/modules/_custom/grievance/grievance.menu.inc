<?php
/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "member" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function grievance_node_tabs() {
	$node_tabs = array();
	
	$node_tabs['grievance']['forward'] = array(
		'title' => 'Forward',
		'description' => '',
		'callback' => 'grievance_node_tab_forward',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance']['statuslog'] = array(
		'title' => 'Timeline',
		'description' => '',
		'callback' => 'grievance_node_tab_log',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance']['attachments'] = array(
		'title' => 'Attachments',
		'description' => '',
		'callback' => 'grievance_node_tab_attachments',
		'permissions' => array('grievance user'),
	);

	$node_tabs['grievance']['gform'] = array(
		'title' => 'Grievance Form',
		'description' => '',
		'callback' => 'grievance_node_tab_gform',
		'permissions' => array('grievance user'),
	);

	$node_tabs['grievance']['arbitrationform'] = array(
		'title' => 'Arbitration Prep Form',
		'description' => '',
		'callback' => 'grievance_node_tab_arbitrationform',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance']['notification'] = array(
		'title' => 'Notification',
		'description' => '',
		'callback' => 'grievance_node_tab_notification',
		'permissions' => array('grievance user'),
	);

	$node_tabs['grievance_shop']['contacts'] = array(
		'title' => 'Contacts',
		'description' => '',
		'callback' => 'grievance_shop_node_tab_contacts',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance_company']['contacts'] = array(
		'title' => 'Contacts',
		'description' => '',
		'callback' => 'grievance_company_node_tab_contacts',
		'permissions' => array('grievance admin'),
	);
	
	return $node_tabs;
}

/***
 * Implements hook_menu
 */
function grievance_menu() {
  $items = array();
	// Node Tabs

	$node_tabs = grievance_node_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		foreach ($node_record as $tab_name => $tab_record) {
			$items['node/%node/' . $tab_name] = array(
				'title' => $tab_record['title'],
				'description' => $tab_record['description'],
				'page callback' => 'grievance_node_tab_content',
				'page arguments' => array(1, $tab_name),
				'access callback' => 'grievance_node_tab_access',
				'access arguments' => array(1, $tab_name),
				'type' => MENU_LOCAL_TASK,
				/* 'weight' => $tab_records['weight'], */
			);
		}
	}

	$items['node/%node/loopback'] = array(
		'page callback' => 'grievance_forward_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['node/%node/loopback-gform'] = array(
		'page callback' => 'grievance_gform_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['node/%node/loopback-arbitrationform'] = array(
		'page callback' => 'grievance_arbitrationform_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance'] = array(
		'title' => t('Grievance Module'),
		'description' => t('Configuration for the Grievance suite of modules') ,
		'page callback' => 'system_admin_menu_block_page',
		'access arguments' => array('grievance admin'),
		'file' => 'system.admin.inc',
		'file path' => drupal_get_path('module', 'system'),
		'position' => 'right',
	);

	$items['admin/config/grievance/core'] = array(
		'title' => 'Grievance Module - Core',
		'description' => t('Settings for the Grievance Module core'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/gform'] = array(
		'title' => 'Grievance Module - Form',
		'description' => t('Settings for the Grievance Form'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_gform_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/arbitrationform'] = array(
		'title' => 'Grievance Module - Arbitration Prep Form',
		'description' => t('Settings for the Grievance Arbitration Prep Form'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_arbitrationform_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/bytype'] = array(
		'title' => 'Report by Type',
		'description' => '',
		'page callback' => 'grievance_report_bytype',
		'page arguments' => array(),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievances/fieldoverrides/apply'] = array(
		'title' => 'Apply Field Overrides',
		'description' => '',
		'page callback' => 'grievance_field_overrides_apply',
		'page arguments' => array(),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	// QuickSearch
	
	$items['grievance/ajax/quicksearch'] = array(
		'page callback' => 'grievance_quicksearch_autocomplete',
		'page arguments' => array(),
		'access arguments' => array('grievance admin'),
		'type' => MENU_CALLBACK,
	);

	$items['grievance/admin/quicksearch/%'] = array(
		'page callback' => 'grievance_quicksearch_page',
		'page arguments' => array(3),
		'access arguments' => array('grievance admin'),
		'type' => MENU_CALLBACK,
	);

  return $items;
}

function grievance_node_tab_content($node, $in_tab_name) {
	$node_tabs = grievance_node_tabs();
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

function grievance_node_tab_access($node, $in_tab_name) {
	$node_tabs = grievance_node_tabs();
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

function grievance_node_tab_get_addlink($content_type, $query) {
	$query['destination'] = current_path();
	
	$path = 'node/add/' . $content_type;
	$item = menu_get_item($path);
	if (!$item['access']) { return; }
	
	$result = "<div class=\"unite-addlink\">";
	$result .= '[';
	$result .= l(
		t("Add a new entry"), 
		$path,
		array('query' => $query)
	);
	$result .= ']';
	$result .= "</div>";
	
	return $result;
}
