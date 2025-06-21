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

  $node_tabs['grievance_shop']['employer_labproj'] = array(
    'title' => 'Labor Projections',
    'description' => '',
    'callback' => 'sirius_employer_node_tab_labproj_list',
    'permissions' => array('sirius employer labor projection manager'),
    'file' => 'sirius_employer.page_labproj_list.inc',
    'component' => 'employer:labproj',
    'skip_node_view_check' => TRUE,
  );


  $node_tabs['sirius_log']['employer_labproj_view'] = array(
    'title' => 'Labor Projection View',
    'description' => '',
    'callback' => 'sirius_employer_node_tab_labproj_view',
    'permissions' => array('sirius employer labor projection manager'),
    'file' => 'sirius_employer.page_labproj_view.inc',
    'weight' => 5,
    'component' => 'employer:labproj',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['employer_labproj_status'] = array(
    'title' => 'Labor Projection Status',
    'description' => '',
    'callback' => 'sirius_employer_node_tab_labproj_status',
    'permissions' => array('sirius employer labor projection manager'),
    'file' => 'sirius_employer.page_labproj_status.inc',
    'weight' => 6,
    'component' => 'employer:labproj',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['employer_labproj_edit'] = array(
    'title' => 'Labor Projection Edit',
    'description' => '',
    'callback' => 'sirius_employer_node_tab_labproj_edit',
    'permissions' => array('sirius employer labor projection manager'),
    'file' => 'sirius_employer.page_labproj_edit.inc',
    'weight' => 7,
    'component' => 'employer:labproj',
    'skip_node_view_check' => TRUE,
  );


  sirius_node_tab_set_module('sirius_employer', $node_tabs);

	return $node_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_employer_menu() {
  $items = array();

  $items['sirius/employer/list'] = array(
    'title' => 'Employers',
    'access callback' => 'sirius_employer_page_list_access',
    'page callback' => 'sirius_employer_page_list',
    'type' => MENU_NORMAL_ITEM,
  );

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

  $items['sirius/employer/labproj'] = array(
    'title' => 'Labor Projections',
    'page callback' => 'sirius_employer_page_labproj_all',
    'file' => 'sirius_employer.page_labproj_all.inc',
    'access arguments' => array('sirius employer admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/labproj/all'] = array(
    'title' => 'Query',
    'page callback' => 'sirius_employer_page_labproj_all',
    'file' => 'sirius_employer.page_labproj_all.inc',
    'access arguments' => array('sirius employer admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/employer/labproj/enforce'] = array(
    'title' => 'Submitted Reports',
    'page callback' => 'sirius_employer_page_labproj_enforce',
    'file' => 'sirius_employer.page_labproj_enforce.inc',
    'access arguments' => array('sirius employer admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/employer/labproj/rawdata'] = array(
    'title' => 'Raw Data Export',
    'page callback' => 'sirius_employer_page_labproj_rawdata',
    'file' => 'sirius_employer.page_labproj_rawdata.inc',
    'access arguments' => array('sirius employer admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
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



