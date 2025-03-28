<?php

function sirius_trust_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits'] = array(
    'title' => 'Benefits',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_benefits',
    'permissions' => array('sirius trust staff'),
    'weight' => 13,
    'file' => 'sirius_trust.page_worker_benefits.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_benefits',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_benefits.inc',
    'component' => 'trust',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_benefit_add',
    'permissions' => array('sirius trust staff'),
    'weight' => 2,
    'file' => 'sirius_trust.page_worker_benefit_add.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/eligibility'] = array(
    'title' => 'Eligibility',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_eligibility',
    'permissions' => array('sirius trust staff'),
    'weight' => 13,
    'file' => 'sirius_trust.page_worker_eligibility.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/beneficiaries'] = array(
    'title' => 'Beneficiaries',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary',
    'permissions' => array('sirius trust staff'),
    'weight' => 14,
    'file' => 'sirius_trust.page_worker_beneficiary.inc',
    'component' => 'trust:beneficiary',
  );




  $node_tabs['sirius_worker']['sirius_trust_worker_election'] = array(
    'title' => 'Elections',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election',
    'permissions' => array('sirius'),
    'weight' => 13,
    'file' => 'sirius_trust.page_worker_election.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/current'] = array(
    'title' => 'Current Election',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_election.inc',
    'component' => 'trust',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election_list',
    'permissions' => array('sirius trust staff'),
    'weight' => 2,
    'file' => 'sirius_trust.page_worker_election_list.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election_add',
    'permissions' => array('sirius trust staff'),
    'weight' => 3,
    'file' => 'sirius_trust.page_worker_election_add.inc',
    'component' => 'trust',
  );



  $node_tabs['sirius_worker']['sirius_trust_worker_beneficiary'] = array(
    'title' => 'Beneficiaries   ',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary',
    'permissions' => array('sirius'),
    'weight' => 14,
    'file' => 'sirius_trust.page_worker_beneficiary.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_beneficiary/current'] = array(
    'title' => 'Current Beneficiary',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_beneficiary.inc',
    'component' => 'trust',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_beneficiary/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary_list',
    'permissions' => array('sirius trust staff'),
    'weight' => 2,
    'file' => 'sirius_trust.page_worker_beneficiary_list.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_beneficiary/add'] = array(
    'title' => 'Add',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary_add',
    'permissions' => array('sirius trust staff'),
    'weight' => 3,
    'file' => 'sirius_trust.page_worker_beneficiary_add.inc',
    'component' => 'trust',
  );




  $node_tabs['sirius_json_definition']['sirius_trust_policy_benefits'] = array(
    'title' => 'Benefits',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_policy_benefits',
    'permissions' => array('sirius trust admin'),
    'weight' => 2,
    'file' => 'sirius_trust.page_policy_benefits.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_json_definition']['sirius_trust_policy_election'] = array(
    'title' => 'Elections',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_policy_election',
    'permissions' => array('sirius trust admin'),
    'weight' => 2,
    'file' => 'sirius_trust.page_policy_election.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_json_definition']['sirius_trust_policy_election/wizard_intro'] = array(
    'title' => 'Wizard Intro',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_policy_election',
    'permissions' => array('sirius trust admin'),
    'weight' => 2,
    'file' => 'sirius_trust.page_policy_election.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_json_definition']['sirius_trust_policy_service_rates'] = array(
    'title' => 'Service Rates',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_policy_service_rates',
    'permissions' => array('sirius trust admin'),
    'weight' => 2,
    'file' => 'sirius_trust.page_policy_service_rates.inc',
    'component' => 'trust:provider',
  );

  $node_tabs['sirius_trust_worker_election']['sirius_trust_election_wizard_link'] = array(
    'title' => 'Wizard',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_election_wizard_link',
    'permissions' => array('sirius trust admin'),
    'weight' => 1,
    'file' => 'sirius_trust.page_election_wizard_link.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_trust_worker_election']['sirius_trust_election_wbs'] = array(
    'title' => 'Benefits',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_election_wbs',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_election_wbs.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_trust_worker_election']['sirius_trust_election_relationships'] = array(
    'title' => 'Relationships',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_election_relationships',
    'permissions' => array('sirius trust admin'),
    'weight' => 1,
    'file' => 'sirius_trust.page_election_relationships.inc',
    'component' => 'trust',
  );

  $node_tabs['sirius_trust_benefit']['sirius_trust_benefit_beneficiary'] = array(
    'title' => 'Beneficiaries',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_benefit_beneficiary',
    'permissions' => array('sirius trust admin'),
    'weight' => 13,
    'file' => 'sirius_trust.page_benefit_beneficiary.inc',
    'component' => 'trust:beneficiary',
  );

  $node_tabs['sirius_log']['sirius_trust_election_wizard'] = array(
    'title' => 'Election Wizard',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_election_wizard',
    'permissions' => array('sirius'),
    'file' => 'sirius_trust.page_election_wizard.inc',
    'weight' => 3,
    'skip_node_view_check' => TRUE,
  );

  sirius_node_tab_set_module('sirius_trust', $node_tabs);

  return $node_tabs;
}

function sirius_trust_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_trust_election_type']['sirius_trust_election_relationships'] = array(
    'title' => 'Relationships',
    'description' => '',
    'callback' => 'sirius_trust_term_tab_election_relationships',
    'permissions' => array('sirius trust admin'),
    'weight' => 10,
    'file' => 'sirius_trust.term_tab_election_relationships.inc',
    'component' => 'trust',
  );

  sirius_node_tab_set_module('sirius_trust', $term_tabs);

  return $term_tabs;
}

function sirius_trust_menu() {
  $items = array();

  $items['sirius/trust/config'] = array(
    'title' => 'Trust General Configuration',
    'page callback' => 'sirius_trust_page_config',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_config.inc',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
  );

  $items['sirius/trust/benefits'] = array(
    'title' => 'Benefits',
    'page callback' => 'sirius_trust_page_benefits',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_benefits.inc'
  );

  $items['sirius/trust/election/wizard/launch'] = array(
    'title' => 'Create an Election',
    'page callback' => 'sirius_trust_page_election_wizard_launch',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_election_wizard_launch.inc',
    'weight' => 1,
  );

  $items['sirius/trust/election/wizard/list'] = array(
    'title' => 'Election Wizards',
    'page callback' => 'sirius_trust_page_election_wizard_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust staff',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_election_wizard_list.inc',
    'weight' => 1,
  );

  return $items;
}
