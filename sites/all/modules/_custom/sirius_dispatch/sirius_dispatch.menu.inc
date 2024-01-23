<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_dispatch_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_dispatch']['sirius_dispatch_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_manage',
    'permissions' => array('sirius dispatch staff'),
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_worker_view'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_view',
    'skip_node_view_check' => TRUE,
    'permissions' => array(),
    'component' => 'dispatch:live',
    'weight' => 0,
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_prior'] = array(
    'title' => 'Prior Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_prior',
    'skip_node_view_check' => TRUE,
    'permissions' => array(),
    'component' => 'dispatch:live',
    'weight' => 1,
    'file' => 'sirius_dispatch.page_prior.inc',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_manage',
    'permissions' => array('sirius dispatch employer'),
    'skip_node_view_check' => TRUE,
    'weight' => 4,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_dispatches'] = array(
    'title' => 'Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_dispatches',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 5, 
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_dispatches/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_dispatches',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_dispatches/create'] = array(
    'title' => 'Create',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_dispatches_create',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 2,
    'file' => 'sirius_dispatch.page_job_dispatches_create.inc',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_bullpen'] = array(
    'title' => 'Bullpen',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_bullpen',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 6,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_events'] = array(
    'title' => 'Events',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_events',
    'permissions' => array('sirius dispatch employer'),
    'skip_node_view_check' => TRUE,
    'weight' => 6,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_attachments'] = array(
    'title' => 'Attachments',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_attachments',
    'permissions' => array('sirius dispatch employer'),
    'skip_node_view_check' => TRUE,
    'weight' => 7,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_payrate'] = array(
    'title' => 'Pay Rate',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_payrate',
    'permissions' => array('sirius dispatch payrate override'),
    'weight' => 8,
    'file' => 'sirius_dispatch.page_job_payrate.inc',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_1for1'] = array(
    'title' => 'One-for-One',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_1for1_list',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 1,
    'file' => 'sirius_dispatch.page_job_1for1_list.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_1for1/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_1for1_list',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 1,
    'file' => 'sirius_dispatch.page_job_1for1_list.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_1for1/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_1for1_add',
    'permissions' => array('sirius'),
    'skip_node_view_check' => TRUE,
    'weight' => 2,
    'file' => 'sirius_dispatch.page_job_1for1_add.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_payrate'] = array(
    'title' => 'Pay Rate',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_payrate',
    'permissions' => array('sirius dispatch payrate override'),
    'weight' => 8,
    'file' => 'sirius_dispatch.page_payrate.inc',
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_fore'] = array(
    'title' => 'Foreperson',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_fore',
    'permissions' => array('sirius dispatch employer'),
    'skip_node_view_check' => TRUE,
    'weight' => 8,
  );

  $node_tabs['sirius_dispatch_facility']['sirius_dispatch_facility_halls'] = array(
    'title' => 'Halls',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_facility_halls',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches',
    'permissions' => array('sirius dispatch worker'),
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/status'] = array(
    'title' => 'Dispatch Status',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatch_status',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 0,
    'component' => 'dispatch:live',
  );

  /*
  $node_tabs['sirius_worker']['sirius_worker_dispatches/current'] = array(
    'title' => 'Current Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_current',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 1,
  );
  */

  $node_tabs['sirius_worker']['sirius_worker_dispatches/archive'] = array(
    'title' => 'Prior Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_archive',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 2,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/all'] = array(
    'title' => 'All Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_all',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 3,
    'component' => 'dispatch:live',
  );

  /*
  $node_tabs['sirius_worker']['sirius_worker_dispatches/sirius_worker_dispatch_preferences'] = array(
    'title' => 'Dispatch Preferences',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatch_preferences',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 4,
  );
  */

  $node_tabs['sirius_worker']['sirius_worker_dispatches/notifications'] = array(
    'title' => 'Notifications',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_notifications',
    'permissions' => array('sirius dispatch pref notifications'),
    'weight' => 4,
    'file' => 'sirius_dispatch.page_worker_pref_notifications.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/asi'] = array(
    'title' => 'Auto Sign-In',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_asi',
    'permissions' => array('sirius dispatch pref asi'),
    'weight' => 5,
    'file' => 'sirius_dispatch.page_worker_pref_asi.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/skill'] = array(
    'title' => 'Skill',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_skill',
    'permissions' => array('sirius dispatch pref skill'),
    'weight' => 6,
    'file' => 'sirius_dispatch.page_worker_pref_skill.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/dept'] = array(
    'title' => 'Department',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_dept',
    'permissions' => array('sirius dispatch pref dept'),
    'weight' => 7,
    'file' => 'sirius_dispatch.page_worker_pref_dept.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/dnc2'] = array(
    'title' => 'Excluded Employers',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_dnc2',
    'permissions' => array('sirius dispatch pref dnc2'),
    'weight' => 8,
    'file' => 'sirius_dispatch.page_worker_pref_dnc2.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/hfe'] = array(
    'title' => 'Hold For Employer',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_hfe',
    'permissions' => array('sirius dispatch pref hfe'),
    'weight' => 9,
    'file' => 'sirius_dispatch.page_worker_pref_hfe.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/cbn'] = array(
    'title' => 'Call By Name',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_cbn',
    'permissions' => array('sirius dispatch pref cbn'),
    'weight' => 10,
    'file' => 'sirius_dispatch.page_worker_pref_cbn.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/eba'] = array(
    'title' => 'Employed but Available',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_pref_eba',
    'permissions' => array('sirius dispatch pref eba'),
    'weight' => 11,
    'file' => 'sirius_dispatch.page_worker_pref_eba.inc',
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/dnc'] = array(
    'title' => 'DNC',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dnc',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 15,
    'component' => 'dispatch:live',
  );

  $node_tabs['sirius_worker']['sirius_worker_emphist'] = array(
    'title' => 'Employment',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_worker_emphist',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_dispatch.page_worker_emphist.inc',
    'component' => 'dispatch:emphist',
  );

  $node_tabs['sirius_worker']['sirius_worker_emphist/list'] = array(
    'title' => 'List',
    'callback' => 'sirius_dispatch_node_tab_worker_emphist',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_dispatch.page_worker_emphist.inc',
    'component' => 'dispatch:emphist',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_worker_emphist/add'] = array(
    'title' => 'Add',
    'callback' => 'sirius_dispatch_node_tab_worker_emphist_add',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_dispatch.page_worker_emphist_add.inc',
    'component' => 'dispatch:emphist',
    'weight' => 2,
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_emphist'] = array(
    'title' => 'Employment',
    'description' => '',
    'callback' => 'sirius_dispatch_node_tab_emphist',
    'permissions' => array('sirius worker admin'),
    'file' => 'sirius_dispatch.page_emphist.inc',
    'component' => 'dispatch:emphist',
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_employer_dispatch_preferences',
    'permissions' => array('sirius dispatch employer'),
    'skip_node_view_check' => TRUE,
    'component' => 'dispatch:live',
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch/preferences'] = array(
    'title' => 'Preferences',
    'description' => '',
    'callback' => 'sirius_node_tab_employer_dispatch_preferences',
    'permissions' => array('sirius dispatch employer'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'skip_node_view_check' => TRUE,
    'component' => 'dispatch:live',
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch/attachments'] = array(
    'title' => 'Attachments',
    'description' => '',
    'callback' => 'sirius_node_tab_employer_dispatch_attachments',
    'permissions' => array('sirius dispatch employer'),
    'weight' => 2,
    'skip_node_view_check' => TRUE,
    'component' => 'dispatch:live',
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch/payrate'] = array(
    'title' => 'Payrate',
    'description' => '',
    'callback' => 'sirius_node_tab_employer_dispatch_payrate',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 3,
  );

  sirius_node_tab_set_module('sirius_dispatch', $node_tabs);

  return $node_tabs;
}

function sirius_dispatch_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_industry']['sirius_dispatch_industry'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_dispatch_term_tab_industry',
    'permissions' => array('sirius dispatch admin'),
    'weight' => 4,
  );

  $term_tabs['sirius_skill']['sirius_skill_dispatch'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_dispatch_term_tab_skill',
    'permissions' => array('sirius dispatch admin'),
    'weight' => 4,
    'file' => array('sirius_dispatch.term_tab_skill.inc'),
  );

  $job_type_tabs = array(
    'worker' => 'Eligible Workers',
    'form' => 'Job Form Settings',
    'run' => 'Run Settings',
    'event' => 'Event Settings',
  );
  $weight = 20;
  foreach ($job_type_tabs as $key => $name) {
    ++$weight;
    $term_tabs['sirius_dispatch_job_type']['sirius_dispatch_job_type_' . $key] = array(
      'title' => $name,
      'description' => '',
      'callback' => 'sirius_dispatch_term_tab_job_type_' . $key,
      'permissions' => array('sirius dispatch admin'),
      'weight' => $weight,
      'file' => array('sirius_dispatch.term_tab_job_type_' . $key . '.inc'),
    );
  }
  $plugins = sirius_dispatch_elig_info_all();
  foreach ($plugins as $key => $plugin) {
    ++$weight;
    $term_tabs['sirius_dispatch_job_type']['sirius_dispatch_job_type_plugin_' . $key] = array(
      'title' => $plugin['name'],
      'description' => '',
      'callback' => 'sirius_dispatch_term_tab_job_type_elig_plugin',
      'permissions' => array('sirius dispatch admin'),
      'weight' => $weight,
      'file' => 'sirius_dispatch.term_tab_job_type_elig_plugin.inc',
      'callback arguments' => array($key),
    );
  }

  sirius_node_tab_set_module('sirius_dispatch', $term_tabs);

  return $term_tabs;
}
/**
 * Implements hook_menu
 */

function sirius_dispatch_menu() {
  $items = array();

  $items['sirius/staff/dispatch/operator'] = array(
    'title' => 'Sirius: Dispatch: Operator',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/staff/dispatch/operator/ajax/%'] = array(
    'title' => 'Sirius: Dispatch: Operator: Ajax',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_operator_ajax',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/job/create'] = array(
    'title' => 'Create a Job',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_job_create_intro',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/job/create/%node/%'] = array(
    'title' => 'Create a Job',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_job_create',
    'page arguments' => array(4, 5),
    'access callback' => 'sirius_dispatch_page_job_create_access',
    'access arguments' => array(4, 5),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/job/create/%node/%/%'] = array(
    'title' => 'Create a Job',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_job_create',
    'page arguments' => array(4, 5, 6),
    'access callback' => 'sirius_dispatch_page_job_create_access',
    'access arguments' => array(4, 5, 6),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch'] = array(
    'title' => 'Sirius: Employer: Dispatch',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_landing',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/job/list'] = array(
    'title' => 'Sirius: Employer: Dispatch: Job: List',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_job_list',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/vlri'] = array(
    'title' => 'Sirius: Employer: Layoff Requests',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_vlri',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/vlri'] = array(
    'title' => 'Layoff Request Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_vlri_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/staff/dispatch'] = array(
    'title' => 'Sirius: Staff: Dispatch',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_landing',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/job/view/ajax/%node'] = array(
    'title' => 'Sirius: Employer: Dispatch: Job: View: Ajax',
    'description' => '',
    'page callback' => 'sirius_dispatch_job_employer_view_ajax',
    'page arguments' => array(6),
    'access callback' => 'sirius_node_tab_dispatch_job_employer_view_access',
    'access arguments' => array(6),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/run'] = array(
    'title' => 'Job Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_run_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/view'] = array(
    'title' => 'Dispatch View Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_view_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/threestrikes'] = array(
    'title' => 'Three Strikes',
    'description' => '',
    'access arguments' => array('sirius dispatch staff'),
    'page callback' => 'sirius_dispatch_threestrikes_page',
    'weight' => 1,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/threestrikes/test'] = array(
    'title' => 'Three Strikes Tester',
    'description' => '',
    'access arguments' => array('sirius dispatch staff'),
    'page callback' => 'sirius_dispatch_threestrikes_page',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['sirius/dispatch/threestrikes/config'] = array(
    'title' => 'Three-Strikes Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_threestrikes_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/dispatch/jwa'] = array(
    'title' => 'Job / Worker Assign',
    'page callback' => 'sirius_dispatch_page_jwa_assign',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/jwa/assign'] = array(
    'title' => 'Assign',
    'page callback' => 'sirius_dispatch_page_jwa_assign',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/dispatch/jwa/workers'] = array(
    'title' => 'Workers',
    'page callback' => 'sirius_dispatch_page_jwa_workers',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_dispatch.page_jwa_workers.inc',
    'weight' => 2,
  );

  $items['sirius/dispatch/jwa/jobs'] = array(
    'title' => 'Jobs',
    'page callback' => 'sirius_dispatch_page_jwa_jobs',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_dispatch.page_jwa_jobs.inc',
    'weight' => 3,
  );

  $items['sirius/dispatch/jwa/display'] = array(
    'title' => 'Display',
    'page callback' => 'sirius_dispatch_page_jwa_display',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_dispatch.page_jwa_display.inc',
    'weight' => 4,
  );

  $items['sirius/ajax/fore-quicksearch'] = array(
    'page callback' => 'grievance_quicksearch_autocomplete',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_CALLBACK,
  );

  $items['sirius/dispatch/job/stub'] = array(
    'title' => 'Stub Jobs',
    'page callback' => 'sirius_dispatch_page_job_stub',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_CALLBACK,
  );

  return $items;
}
