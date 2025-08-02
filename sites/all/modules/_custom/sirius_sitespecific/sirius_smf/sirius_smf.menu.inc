<?php

/**
 * Implements hook_node_tabs
 */

function sirius_smf_node_tabs() {
	$node_tabs = array();

  $node_tabs['grievance_shop']['sirius_employer/smf_kaiser'] = array(
    'title' => 'Kaiser Immediate Eligibility',
    'description' => '',
    'callback' => 'sirius_smf_page_employer_kaiser_settings',
    'permissions' => array('sirius employer admin'),
    'file' => 'sirius_smf.page_employer_kaiser_settings.inc',
    'weight' => 4,
  );

  $node_tabs['grievance_shop']['sirius_smf_ehrs'] = array(
    'title' => 'Upload Wizard',
    'description' => '',
    'callback' => 'sirius_smf_node_tab_employer_ehrs',
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => 'sirius_smf.page_employer_ehrs.inc',
  );

  $node_tabs['sirius_worker']['sirius_worker_hours/view'] = array(
    'title' => 'Hours',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_worker_hours',
    'permissions' => array('sirius hour admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_worker_hours/smf_buildup'] = array(
    'title' => 'Buildup Settings',
    'callback' => 'sirius_smf_node_tab_worker_buildup',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_worker_buildup.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_hours/smf_hour_calc'] = array(
    'title' => 'Recompute Work History',
    'callback' => 'sirius_smf_node_tab_worker_hour_calc',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_worker_hour_calc.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );

  $node_tabs['sirius_worker']['sirius_worker_ledger/smf_cobra_rates'] = array(
    'title' => 'COBRA Rate',
    'callback' => 'sirius_smf_node_tab_worker_cobra_rates',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_worker_cobra_rates.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $node_tabs['sirius_worker']['sirius_worker_contact/smf_autotag'] = array(
    'title' => 'Autotag',
    'callback' => 'sirius_smf_node_tab_worker_autotag',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_worker_autotag.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $node_tabs['sirius_worker']['smf_autotag_history'] = array(
    'title' => 'Monthly History',
    'callback' => 'sirius_smf_node_tab_worker_autotag_history',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_worker_autotag_history.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => -1,
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition/smf_automsg_test'] = array(
    'title' => 'Test',
    'callback' => 'sirius_smf_node_tab_automsg_test',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_automsg_test.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition/smf_automsg_config'] = array(
    'title' => 'Config',
    'callback' => 'sirius_smf_node_tab_automsg_config',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_automsg_config.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 11,
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition/smf_policy_industry'] = array(
    'title' => 'Industry',
    'callback' => 'sirius_smf_node_tab_policy_industry',
    'permissions' => array('sirius trust admin'),
    'file' => 'sirius_smf.page_policy_industry.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 11,
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition/smf_automsg_config'] = array(
    'title' => 'Config',
    'callback' => 'sirius_smf_node_tab_automsg_config',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.page_automsg_config.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 11,
  );

  $node_tabs['sirius_log']['sirius_smf_ehrs_wizard'] = array(
    'title' => 'Wizard',
    'description' => '',
    'callback' => 'sirius_smf_node_tab_ehrs_wizard',
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => 'sirius_smf.page_ehrs_wizard.inc',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['sirius_smf_ehrs_wizard_correction'] = array(
    'title' => 'Corrections',
    'description' => '',
    'callback' => 'sirius_smf_node_tab_ehrs_wizard_corrections',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => 'sirius_smf.page_ehrs_wizard_corrections.inc',
    'skip_node_view_check' => TRUE,
  );

  //
  // Oneoffs
  //

  /*
  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/oneoff_benefit_cleanup'] = array(
    'title' => 'Oneoff Benefit Cleanup',
    'callback' => 'sirius_smf_node_tab_worker_oneoff_benefit_cleanup',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.oneoff_benefit_cleanup.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/oneoff_dentwell_election'] = array(
    'title' => 'Oneoff Dentwell Election',
    'callback' => 'sirius_smf_node_tab_worker_oneoff_dentwell_election',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.oneoff_dentwell_election.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/oneoff_election_relationships'] = array(
    'title' => 'Oneoff Election Relationships',
    'callback' => 'sirius_smf_node_tab_worker_oneoff_election_relationships',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.oneoff_election_relationships.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );
  */

  $node_tabs['sirius_trust_worker_election']['oneoff_linked_benefit'] = array(
    'title' => 'Oneoff Linked Benefits',
    'callback' => 'sirius_smf_node_tab_election_oneoff_linked_benefit',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_smf.oneoff_linked_election.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );





  sirius_node_tab_set_module('sirius_smf', $node_tabs);
	return $node_tabs;
}

/**
 * Implements hook_term_tabs
 */

function sirius_smf_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_member_status']['sirius_smf_ms_threshold'] = array(
    'title' => 'Hours Threshold',
    'description' => '',
    'callback' => 'sirius_smf_term_tab_ms_threshold',
    'permissions' => array('sirius trust admin'),
    'weight' => 1,
    'file' => 'sirius_smf.page_ms_threshold.inc',
    'component' => 'trust',
  );

  sirius_node_tab_set_module('sirius_smf', $term_tabs);

  return $term_tabs;
}


/***
 * Implements hook_menu
 */

function sirius_smf_menu() {
  $items = array();

  $items['sirius/smf/views-taxonomy-restrict-by-parent-config'] = array(
    'title' => 'SMF Views Taxonomy Restrict by Parent',
    'page callback' => 'sirius_smf_page_views_taxonomy_restrict_by_parent_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_smf.page_views_taxonomy_restrict_by_parent.inc',
  );

  $items['sirius/smf/modal/wym/%node'] = array(
    'title'            => 'Worker Year / Month',
    'page callback'    => 'sirius_smf_page_wym_modal',
    'page arguments'   => array(4),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permissions' => array('sirius trust staff')
      ),
    ),
    'type'             => MENU_NORMAL_ITEM,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
    'file' => 'sirius_smf.page_wym_modal.inc',
  );

  $items['smf/register'] = array(
    'title'            => 'Register a User Account',
    'page callback'    => 'sirius_smf_page_register',
    'page arguments'   => array(4),
    'access callback' => TRUE,
    'type'             => MENU_NORMAL_ITEM,
    'file' => 'sirius_smf.page_register.inc',
  );

  $items['sirius/smf/ehrs'] = array(
    'title'            => 'Employer Upload Status',
    'page callback'    => 'sirius_smf_page_ehrs_status',
    'page arguments'   => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permissions' => array('sirius trust staff')
      ),
    ),
    'type'             => MENU_NORMAL_ITEM,
    'file' => 'sirius_smf.page_ehrs_status.inc',
  );
  
  return $items;
}


/**
 * Implments hook_modal_paths
 */

function sirius_smf_modal_paths() {
  $paths = array();

  $paths['sirius/smf/modal/wym/%node'] = array(
    'style' => 'sirius-modal-style',
  );

  return $paths;
}

