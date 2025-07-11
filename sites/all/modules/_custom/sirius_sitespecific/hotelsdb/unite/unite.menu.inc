<?php
/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "member" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function unite_node_tabs() {
	$node_tabs = array();
	
	$node_tabs['local']['properties'] = array(
		'title' => 'Properties',
		'description' => '',
		'callback' => 'unite_node_tab_local_properties',
		'permissions' => array('unite hotels user'),
	);
	$node_tabs['local']['contracts'] = array(
		'title' => 'Contracts',
		'description' => '',
		'callback' => 'unite_node_tab_local_contracts',
		'permissions' => array('unite hotels user'),
	);
	/*
	$node_tabs['property']['contracts'] = array(
		'title' => 'Contracts',
		'description' => '',
		'callback' => 'unite_node_tab_property_contracts',
		'permissions' => array('unite hotels user'),
	);
	*/

	foreach (array('property', 'actor', 'property_actor') as $content_type) {
		$node_tabs[$content_type]['revisions/field_revisions'] = array(
			'title' => 'Field Revisions',
			'description' => '',
			'callback' => 'unite_node_tab_field_revisions',
			'permissions' => array('unite hotels user'),
			'file' => 'unite.page_field_revisions.inc',
			'weight' => 99,
		);
	}

	/*
	$node_tabs['property']['unite_relationships'] = array(
		'title' => 'Relationships',
		'description' => '',
		'callback' => 'unite_node_tab_property_relationships',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_relationships.inc',
	);
	*/

	$node_tabs['property']['unite_amenities'] = array(
		'title' => 'Hotel Amenities',
		'description' => '',
		'callback' => 'unite_node_tab_amenities_edit',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_amenities_edit.inc',
		'weight' => 1,
	);

	$node_tabs['property']['unite_casino'] = array(
		'title' => 'Casino',
		'description' => '',
		'callback' => 'unite_node_tab_casino_edit',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_casino_edit.inc',
		'weight' => 2,
	);

	$node_tabs['property']['unite_restaurants'] = array(
		'title' => 'Food Service',
		'description' => '',
		'callback' => 'unite_node_tab_restaurants_edit',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_restaurants_edit.inc',
		'weight' => 3,
	);

	$node_tabs['property']['unite_contract'] = array(
		'title' => 'Contract',
		'description' => '',
		'callback' => 'unite_node_tab_contract_edit',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_contract_edit.inc',
		'weight' => 4,
	);

	$node_tabs['property']['unite_fairhotel'] = array(
		'title' => 'FairHotel',
		'description' => '',
		'callback' => 'unite_node_tab_fairhotel_edit',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_fairhotel_edit.inc',
		'weight' => 5,
	);


	/*
	$node_tabs['property']['unite_colocation'] = array(
		'title' => 'Colocation',
		'description' => '',
		'callback' => 'unite_node_tab_property_colocation',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_property_colocation.inc',
		'weight' => 10,
	);
	*/

	$node_tabs['hdb_convention_center']['unite_convention_properties'] = array(
		'title' => 'Properties',
		'description' => '',
		'callback' => 'unite_node_tab_convention_properties',
		'permissions' => array('unite hotels user'),
		'file' => 'unite.page_convention_properties.inc',
	);

  sirius_node_tab_set_module('unite', $node_tabs);

	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function unite_all_access() {
	return TRUE; 
}

function unite_menu() {
  $items = array();

	$items['unite/home'] = array(
		'title' => 'UNITE HERE Properties Database',
		'page callback' => 'unite_page_home',
		'access arguments' => array('unite hotels user'),
		'type' => MENU_NORMAL_ITEM,
		'file' => 'unite.page_home.inc',
	);

	$items['unite/export/union-hotel-guide'] = array(
		'title' => 'Union Hotel Guide export',
		'page callback' => 'unite_export_union_hotel_guide',
		'access callback' => 'unite_export_union_hotel_guide_access',		
		'access arguments' => array(),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['unite/export/union-hotel-guide/%'] = array(
		'title' => 'Union Hotel Guide export',
		'page callback' => 'unite_export_union_hotel_guide',
		'page arguments' => array(3),
		'access callback' => 'unite_all_access',		
		'access arguments' => array(),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['unite/report/changed-status'] = array(
		'title' => 'Report - Changed Status',
		'page callback' => 'unite_report_changed_status',
		'page arguments' => array(),
		'access callback' => 'unite_union_hotel_guide_access',		
		'access arguments' => array(),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/unite'] = array(
		//this section is for  create a  section
		'title' => t('UNITE HERE') ,//this section will create  under admin->config
		'description' => t('Configuration for the unite suite of modules') ,
		'page callback' => 'system_admin_menu_block_page',
		'access arguments' => array('unite hotels admin'),
		'file' => 'system.admin.inc',
		'file path' => drupal_get_path('module', 'system'),
		'position' => 'right',
	);   

	$items['admin/config/unite/core'] = array(
		'title' => 'UNITE HERE Core',
		'description' => 'Settings for the unite core',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('unite_admin'),
		'access arguments' => array('unite hotels admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	// QuickSearch
	$items['unite/ajax/quicksearch'] = array(
		'page callback' => 'unite_quicksearch_autocomplete',
		'page arguments' => array(),
		'access arguments' => array('unite hotels admin'),
		'type' => MENU_CALLBACK,
	);

	$items['unite/admin/quicksearch/%'] = array(
		'page callback' => 'unite_quicksearch_page',
		'page arguments' => array(3),
		'access arguments' => array('unite hotels admin'),
		'type' => MENU_CALLBACK,
	);

	// Acquire

	$items['unite/actor/acquire'] = array(
		'title' => t('Actor Acquire'),
		'page callback' => 'unite_actor_acquire',
		'page arguments' => array(),
		'access arguments' => array('unite hotels admin'),
		'type' => MENU_CALLBACK,
	);

	$items['unite/local/acquire'] = array(
		'title' => t('Local Acquire'),
		'page callback' => 'unite_local_acquire',
		'page arguments' => array(),
		'access arguments' => array('unite hotels admin'),
		'type' => MENU_CALLBACK,
	);


  $items['sirius/modal/%node/unite-relationship-verify'] = array(
    'title'            => 'Verify',
    'page callback'    => 'unite_property_verify',
    'page arguments'   => array(2),
		'access arguments' => array('unite hotels user'),
		'file' 						 => 'unite.modal_property_verify.inc',
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/hotelsdb/changelog'] = array(
    'title' => 'PropertiesDB Changelog',
    'page callback' => 'unite_page_changelog',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'unite.page_changelog.inc',
    'weight' => 1,
  );


  return $items;
}

function unite_union_hotel_guide_access() {
	return true;
}

function unite_node_tab_get_addlink($content_type, $query) {
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

/**
 * Implments hook_modal_paths
 */

function unite_modal_paths() {
  $paths = array();

  $paths['sirius/modal/%node/unite-relationship-verify'] = array(
    'style' => 'sirius-modal-style-small',
  );

  return $paths;
}

/*
function sirius_modal_styles() {
  $styles = array();

  $styles['sirius-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 600,
      'height' => 500,
    ),
  );

  return $styles;
}
*/