<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_worker_node_tabs() {
	$node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_status'] = array(
    'title' => 'Industries',
    'callback' => 'sirius_node_tab_worker_status',
    'permissions' => array('sirius'),
    'file' => 'sirius_worker.page_status.inc',
    'weight' => 0,
  );

  $node_tabs['sirius_worker']['sirius_worker_status/view'] = array(
    'title' => 'View',
    'callback' => 'sirius_node_tab_worker_status',
    'permissions' => array('sirius'),
    'file' => 'sirius_worker.page_status.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );


  $node_tabs['sirius_worker']['sirius_worker_status/ws'] = array(
    'title' => 'Work Status',
    'callback' => 'sirius_node_tab_worker_work_status',
    'permissions' => array('sirius worker admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_status/ws_history'] = array(
    'title' => 'Work Status History',
    'callback' => 'sirius_node_tab_worker_work_status_history',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.page_work_status_history.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_worker']['sirius_worker_status/ms'] = array(
    'title' => 'Member Status',
    'callback' => 'sirius_node_tab_worker_industry',
    'permissions' => array('sirius worker admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_worker']['sirius_worker_status/ms_history'] = array(
    'title' => 'Member Status History',
    'callback' => 'sirius_node_tab_worker_industry_history',
    'permissions' => array('sirius worker admin'),
    'weight' => 2,
    'file' => 'sirius_worker.page_industry_history.inc',
    'weight' => 5,
  );

  $node_tabs['sirius_worker']['sirius_worker_schedule'] = array(
    'title' => 'Schedule',
    'callback' => 'sirius_worker_node_tab_schedule',
    'permissions' => array('sirius'),
    'component' => 'worker:schedule',
  );

  $node_tabs['sirius_worker']['sirius_worker_schedule/overview'] = array(
    'title' => 'Overview',
    'callback' => 'sirius_worker_node_tab_schedule',
    'permissions' => array('sirius'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'component' => 'worker:schedule',
  );

  $node_tabs['sirius_worker']['sirius_worker_schedule/tor'] = array(
    'title' => 'Time Off',
    'callback' => 'sirius_worker_node_tab_schedule_tor_worker_list',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => 'sirius_worker.page_schedule_tor_worker_list.inc',
    'component' => 'worker:schedule',
  );

  $node_tabs['sirius_worker']['sirius_worker_schedule/tor/add'] = array(
    'title' => 'Add',
    'callback' => 'sirius_worker_node_tab_schedule_tor_add',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => 'sirius_worker.page_schedule_tor_add.inc',
    'component' => 'worker:schedule',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill'] = array(
    'title' => 'Skills',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill',
    'permissions' => array('sirius'),
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill_view',
    'permissions' => array('sirius'),
    'weight' => 1,
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill_add',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 2,
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill/add2'] = array(
    'title' => 'Add Multiple',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill_add2',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 3,
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill/renew'] = array(
    'title' => 'Renew',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill_renew',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 4,
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_skill/remove'] = array(
    'title' => 'Remove',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_skill_remove',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 5,
    'component' => 'worker:skill',
  );

  $node_tabs['sirius_worker']['sirius_worker_contact'] = array(
    'title' => 'Contact',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact',
    'permissions' => array('sirius'),
    'rnode' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/contact'] = array(
    'title' => 'Contact',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact',
    'permissions' => array('sirius'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'rnode' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/phone'] = array(
    'title' => 'Phone',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact_phone',
    'file' => 'sirius_worker.page_contact_phone.inc',
    'permissions' => array('sirius'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/lang'] = array(
    'title' => 'Languages',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact_lang',
    'file' => 'sirius_worker.page_contact_lang.inc',
    'permissions' => array('sirius'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'component' => 'lang',
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/user'] = array(
    'title' => 'User',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_user',
    'permissions' => array('sirius worker admin'),
    'weight' => 2,
    'component' => 'worker:usersync',
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/customfields'] = array(
    'title' => 'Custom Fields',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_contact_customfields',
    'permissions' => array('sirius worker admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/tags'] = array(
    'title' => 'Tags',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_tags',
    'permissions' => array('sirius worker admin'), 
    'file' => 'sirius_worker.page_tags.inc',
    'weight' => 3,
  );


  $node_tabs['sirius_worker']['sirius_worker_contact/merge'] = array(
    'title' => 'Merge',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_merge',
    'permissions' => array('sirius worker admin'),
    'weight' => 4
  );

  $node_tabs['sirius_worker']['sirius_worker_grievances'] = array(
    'title' => 'Grievances',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_grievances',
    'permissions' => array('grievance view'),
    'file' => 'sirius_worker.page_grievances.inc',
    'rnode' => TRUE,
    'component' => 'grievance',
  );

  $node_tabs['sirius_worker']['sirius_worker_relationships'] = array(
    'title' => 'Relationships',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_relationships',
    'permissions' => array('sirius worker view'),
    'file' => 'sirius_worker.page_relationships.inc',
    'component' => 'contact:relationships',
    'rnode' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_relationships/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_relationships',
    'permissions' => array('sirius worker view'),
    'file' => 'sirius_worker.page_relationships.inc',
    'component' => 'contact:relationships',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_worker_relationships/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_relationship_add',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.page_relationship_add.inc',
    'component' => 'contact:relationships',
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_worker_schedule_tor'] = array(
    'title' => 'Time-Off',
    'description' => '',
    'callback' => 'sirius_worker_node_tab_schedule_tor_view',
    'permissions' => array('sirius'),
    'file' => 'sirius_worker.page_schedule_tor_view.inc',
  );

  $node_tabs['sirius_log']['sirius_worker_schedule_tor/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_worker_node_tab_schedule_tor_view',
    'permissions' => array('sirius'),
    'file' => 'sirius_worker.page_schedule_tor_view.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_log']['sirius_worker_schedule_tor/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_worker_node_tab_schedule_tor_edit',
    'permissions' => array('sirius'),
    'file' => 'sirius_worker.page_schedule_tor_edit.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_log_history'] = array(
    'title' => 'Manage',
    'callback' => 'sirius_node_tab_worker_log_history',
    'permissions' => array('sirius worker admin'),
    'weight' => 2,
    'file' => 'sirius_worker.page_log_history.inc',
  );
  

  $node_tabs['grievance_shop']['employer_workers/ein'] = array(
    'title' => 'EIN Lookup',
    'permissions' => array('sirius employer worker list'),
    'callback' =>'sirius_worker_node_tab_ein_lookup',
    'file' => 'sirius_worker.page_ein.inc',
    'component' => 'employer:ein',
    'weight' => 5,
  );

  $node_tabs['grievance_shop']['employer_workers/ein_set'] = array(
    'title' => 'EIN Assign',
    'file' => 'sirius_worker.page_ein.inc',
    'permissions' => array('sirius employer worker manage'),
    'callback' =>'sirius_worker_node_tab_ein_assign',
    'component' => 'employer:ein',
    'weight' => 6,
  );


  sirius_node_tab_set_module('sirius_worker', $node_tabs);
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

  $term_tabs['sirius_member_status']['sirius_member_status_tor'] = array(
    'title' => 'Time-Off Requests',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_member_status_tor',
    'permissions' => array('sirius worker admin'),
    'weight' => 4,
  );

  $term_tabs['sirius_work_status']['sirius_work_status_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_work_status_settings',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.term_tab_work_status.inc',
    'weight' => 1,
  );

  $term_tabs['sirius_worker_tor_type']['sirius_worker_tor_type_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_tor_settings',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.term_tab_tor_settings.inc',
    'weight' => 1,
  );

  $term_tabs['sirius_worker_tor_type']['sirius_worker_tor_type_statuses'] = array(
    'title' => 'Statuses',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_tor_statuses',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.term_tab_tor_statuses.inc',
    'weight' => 1,
  );

  /*
  $term_tabs['sirius_worker_tor_type']['sirius_worker_tor_type_worker_notifications'] = array(
    'title' => 'Worker Notifications',
    'description' => '',
    'callback' => 'sirius_worker_term_tab_tor_worker_notifications',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_worker.term_tab_tor_worker_notifications.inc',
    'weight' => 2,
  );
  */

  sirius_node_tab_set_module('sirius_worker', $term_tabs);

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

$items['sirius/skill/settings'] = array(
    'title' => 'Skill Settings',
    'description' => '',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_skill_settings',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_worker.page_skill_settings.inc',
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

  /*
  $items['sirius/worker/skill-renew'] = array(
    'title' => 'Renew a Skill',
    'description' => '',
    'page callback' => 'sirius_worker_skill_term_renew',
    'page arguments' => array(3),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );
  */

  $items['sirius/worker/config/user'] = array(
    'title' => 'Worker / User Configuration',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_worker_user_admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/config/create'] = array(
    'title' => 'Worker Creation Configuration',
    'description' => '',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_config_create',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'access' => array(
          'type' => 'worker',
          'action' => 'config',
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_worker.page_config_create.inc',
  );

  $items['sirius/worker/schedule/tor'] = array(
    'title' => 'Time-Off Entries',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_schedule_tor_list',
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_worker.page_schedule_tor_list.inc'
  );

  $items['sirius/worker/list'] = array(
    'title' => 'Workers',
    'access callback' => 'sirius_worker_page_list_access',
    'page callback' => 'sirius_worker_page_list',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/worker/search-by-phone-carrier'] = array(
    'title' => 'Worker Search by Phone Carrier',
    'access arguments' => array('sirius worker admin'),
    'page callback' => 'sirius_worker_page_search_by_phone_carrier',
    'file' => 'sirius_worker.page_search_by_phone_carrier.inc',
    'type' => MENU_NORMAL_ITEM,
  );


  $items['sirius/worker/proxy/%'] = array(
    'title' => 'Proxy Worker',
    'page callback' => 'sirius_worker_page_proxy_view',
    'page arguments' => array(3),
    'access callback' => 'sirius_page_access_wrap',
    'access arguments' => array('sirius_worker', 'sirius_worker.page_proxy_view.inc', 'sirius_worker_page_proxy_view_access', 3),
    'file' => 'sirius_worker.page_proxy_view.inc',
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



