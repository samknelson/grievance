<?php

function sirius_trust_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits'] = array(
    'title' => 'Benefits',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_benefits',
    'permissions' => array('sirius'),
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

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/cobra'] = array(
    'title' => 'COBRA',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_cobra',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_cobra.inc',
    'weight' => 15,
    'component' => 'trust:cobra',
    'type' => MENU_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_benefits/scan'] = array(
    'title' => 'Scan',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_benefit_scan',
    'permissions' => array('sirius trust staff'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_benefit_scan.inc',
    'weight' => 20,
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
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
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => 'sirius_trust.page_worker_election.inc',
    'component' => 'trust',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election_list',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => 'sirius_trust.page_worker_election_list.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_election/wizard'] = array(
    'title' => 'Change My Election',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_election_wizard',
    'permissions' => array('sirius'),
    'weight' => 3,
    'file' => 'sirius_trust.page_worker_election_wizard.inc',
    'component' => 'trust',
    'type' => MENU_LOCAL_TASK,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_trust_worker_beneficiary'] = array(
    'title' => 'Beneficiaries',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_worker_beneficiary',
    'permissions' => array('sirius'),
    'weight' => 14,
    'file' => 'sirius_trust.page_worker_beneficiary.inc',
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

  $node_tabs['sirius_log']['sirius_trust_election_wizard'] = array(
    'title' => 'Election Wizard',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_election_wizard',
    'permissions' => array('sirius'),
    'file' => 'sirius_trust.page_election_wizard.inc',
    'weight' => 3,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_trust_benefit']['sirius_trust_benefit_dashboard_settings'] = array(
    'title' => 'Dashboard Settings',
    'description' => '',
    'callback' => 'sirius_trust_node_tab_benefit_dashboard_settings',
    'permissions' => array('sirius trust admin'),
    'file' => 'sirius_trust.page_benefit_dashboard_settings.inc',
    'weight' => 3,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_trust_provider']['sirius_trust_provider_edi_plugins'] = array(
    'title' => 'EDI',
    'callback' => 'sirius_node_tab_entity_plugins',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
    'file' => 'sirius.page_entity_plugins.inc',
    'callback arguments' => array('trust_provider_edi'),
    'module' => 'sirius',
    'component' => 'trust:provider:edi',
  );

  sirius_node_tab_set_module('sirius_trust', $node_tabs);

  return $node_tabs;
}

function sirius_trust_term_tabs() {
  $term_tabs = array();

  /*
  $term_tabs['sirius_trust_election_type']['sirius_trust_election_relationships'] = array(
    'title' => 'Relationships',
    'description' => '',
    'callback' => 'sirius_trust_term_tab_election_relationships',
    'permissions' => array('sirius trust admin'),
    'weight' => 10,
    'file' => 'sirius_trust.term_tab_election_relationships.inc',
    'component' => 'trust',
  );
  */

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

  $items['sirius/trust/cobra'] = array(
    'title' => 'COBRA Configuration',
    'page callback' => 'sirius_trust_page_cobra_config',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_cobra_config.inc',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust:cobra',
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

  $items['sirius/trust/benefits/benefits'] = array(
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
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius_trust.page_benefits.inc',
  );

  $items['sirius/trust/benefits/scan'] = array(
    'title' => 'Scan',
    'page callback' => 'sirius_trust_page_wb_scan_status',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_trust.page_wb_scan_status.inc',
  );

  $items['sirius/trust/benefits/scan/status'] = array(
    'title' => 'Status',
    'page callback' => 'sirius_trust_page_wb_scan_status',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius_trust.page_wb_scan_status.inc',
  );

  $items['sirius/trust/benefits/scan/changelog'] = array(
    'title' => 'Change Log',
    'page callback' => 'sirius_trust_page_wb_scan_changelog',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_trust.page_wb_scan_changelog.inc',
  );

  /*
  $items['sirius/trust/election/wizard/launch'] = array(
    'title' => 'Create an Election',
    'page callback' => 'sirius_trust_page_election_wizard_launch',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_election_wizard_launch.inc',
    'weight' => 1,
  );
  */

  $items['sirius/trust/election/wizard'] = array(
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

  $items['sirius/trust/election/wizard/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_trust_page_election_wizard_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust staff',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_trust.page_election_wizard_list.inc',
    'weight' => 1,
  );

  $items['sirius/trust/election/wizard/config'] = array(
    'title' => 'Configuration',
    'page callback' => 'sirius_trust_page_election_wizard_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_trust.page_election_wizard_config.inc',
    'weight' => 2,
  );

  $items['sirius/trust/changequeue'] = array(
    'title' => 'Change Queue',
    'page callback' => 'sirius_trust_page_changequeue',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust admin',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_changequeue.inc',
  );

  $items['sirius/trust/geog/tester'] = array(
    'title' => 'Geography tester',
    'page callback' => 'sirius_trust_page_geog_tester',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust staff',
        'component' => 'trust',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_trust.page_geog_tester.inc',
  );

  $cq_options = Sirius_Trust_Changequeue::getInstance()->status_options();
  $weight = 0;
  foreach ($cq_options as $key => $name) {
    ++$weight;
    if ($weight == 1) {
      $type = MENU_DEFAULT_LOCAL_TASK;
    } else {
      $type = MENU_LOCAL_TASK;
    }
    $items['sirius/trust/changequeue/' . $key] = array(
      'title' => $name,
      'page callback' => 'sirius_trust_page_changequeue',
      'page arguments' => array(3),
      'access callback' => 'sirius_menu_access_callback',
      'access arguments' => array(
        array(
          'permission' => 'sirius trust admin',
          'component' => 'trust',
        ),
      ),
      'type' => $type,
      'file' => 'sirius_trust.page_changequeue.inc',
      'weight' => $weight,
    );
  }

  return $items;
}
