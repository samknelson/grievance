<?php

function sirius_teamsters631_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_teamsters631_badge'] = array(
    'title' => 'Badge',
    'description' => '',
    'callback' => 'sirius_teamsters631_badge',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch']['teamsters631_oneoff_end_dates'] = array(
    'title' => '[Temp. End Date Correct]',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_dispatch_oneoff_end_date_correct',
    'permissions' => array('sirius admin'),
    'file' => 'sirius_teamsters631.oneoff_dispatch_end_dates.inc',
    'weight' => 999
  );

  $node_tabs['sirius_dispatch_job_group']['sirius_teamsters631_dispatch_job_group_seniority'] = array(
    'title' => 'Seniority Settings',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_dispatch_job_group_seniority',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 4,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_dispatch_job_group_seniority.inc',
  );

  $node_tabs['grievance_shop']['grievance_shop_teamsters631_m12'] = array(
    'title' => '12th of the Month',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_employer_m12',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 10,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_employer_m12.inc',
  );

  $node_tabs['grievance_shop']['grievance_shop_teamsters631_m12/reports'] = array(
    'title' => 'Reports',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_employer_m12',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 1,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_employer_m12.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['grievance_shop']['grievance_shop_teamsters631_m12/emails'] = array(
    'title' => 'Email Addresses',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_employer_m12_emails',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 2,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_employer_m12_emails.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12'] = array(
    'title' => '12th of the Month',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 10,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 1,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/dispatches'] = array(
    'title' => 'Count Dispatches',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_dispatches',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 2,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_dispatches.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/received'] = array(
    'title' => 'Received',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_received',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 3,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_received.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/request_layoffs'] = array(
    'title' => 'Select Ineligible Workers by Name',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_request_layoffs',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 4,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_request_layoffs.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/request_layoffs2'] = array(
    'title' => 'Select Ineligible Workers by Dispatch',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_request_layoffs2',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 5,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_request_layoffs2.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/cancel_layoffs'] = array(
    'title' => 'Cancel Inelgible Workers',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_cancel_layoffs',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 6,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_cancel_layoffs.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/notify'] = array(
    'title' => 'Notify',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_notify',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 7,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_notify.inc',
  );

  $node_tabs['sirius_log']['sirius_log_teamsters631_m12/followup'] = array(
    'title' => 'Follow-Up',
    'description' => '',
    'callback' => 'sirius_teamsters631_node_tab_log_m12_followup',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 8,
    'component' => 'dispatch:live',
    'file' => 'sirius_teamsters631.page_log_m12_followup.inc',
  );

  sirius_node_tab_set_module('sirius_teamsters631', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_teamsters631_menu() {
  $items = array();

  $items['sirius/admin/teamsters631/ebnotify'] = array(
    'title' => 'Teamsters 631 Extraboard Notifications',
    'description' => '',
    'access arguments' => array('sirius teamsters631 admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_teamsters631_ebnotify_admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/front'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_teamsters631_badge_front',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/contents'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_contents',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/headshot'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_headshot',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['teamsters631/skills-correct'] = array(
    'title' => 'Skill Correction Tool',
    'description' => '',
    'access arguments' => array('sirius training admin'),
    'page callback' => 'sirius_teamsters631_oneoff_skills_correct',
    'page arguments' => array(''),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_teamsters631.oneoff_skills_correct.inc',
  );

  $items['teamsters631/skills-correct/muiltiadd'] = array(
    'title' => 'Multiply-Added Skills',
    'description' => '',
    'access arguments' => array('sirius training admin'),
    'page callback' => 'sirius_teamsters631_oneoff_skills_correct',
    'page arguments' => array(3),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_teamsters631.oneoff_skills_correct.inc',
    'weight' => 1,
  );

  $items['teamsters631/skills-correct/osha'] = array(
    'title' => 'OSHA Expiration Date',
    'description' => '',
    'access arguments' => array('sirius training admin'),
    'page callback' => 'sirius_teamsters631_oneoff_skills_correct_osha',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_teamsters631.oneoff_skills_correct_osha.inc',
    'weight' => 2,
  );

  $items['teamsters631/switch/leave'] = array(
    'title' => 'Site Switch - Leave',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_teamsters631_server_switch_leave',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_teamsters631.server_switch.inc',
  );

  $items['teamsters631/switch/arrive'] = array(
    'title' => 'Site Switch - Arrive',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_teamsters631_server_switch_arrive',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_teamsters631.server_switch.inc',
  );

  $items['sirius/edls/631compliance'] = array(
    'title' => 'Compliance',
    'page callback' => 'sirius_edls_page_631compliance',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'employer_dashboard',
          // 'action' => 'staff',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'file' => 'sirius_teamsters631.page_edls_compliance.inc'
  );

  $items['sirius/edls/631twilio'] = array(
    'title' => 'Twilio',
    'page callback' => 'sirius_edls_page_631twilio',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'admin',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 11,
    'file' => 'sirius_teamsters631.page_edls_twilio_settings.inc'
  );


  $items['sirius/teamsters631/m12'] = array(
    'title' => '12th of the Month',
    'page callback' => 'sirius_teamsters631_page_m12',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'dispatch:live',
        'permission' => 'sirius dispatch staff',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_teamsters631.page_m12.inc'
  );

  $items['sirius/teamsters631/m12/%'] = array(
    'title' => '12th of the Month',
    'page callback' => 'sirius_teamsters631_page_m12_month',
    'page arguments' => array(3,4),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'dispatch:live',
        'permission' => 'sirius dispatch staff',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_teamsters631.page_m12_month.inc'
  );

  $items['sirius/teamsters631/m12/%/all'] = array(
    'title' => 'All Convention Employers',
    'page callback' => 'sirius_teamsters631_page_m12_month',
    'page arguments' => array(3, array()),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'dispatch:live',
        'permission' => 'sirius dispatch staff',
      ),
    ),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_teamsters631.page_m12_month.inc'
  );

  $items['sirius/teamsters631/m12/%/active'] = array(
    'title' => 'Employers with Active Dispatches',
    'page callback' => 'sirius_teamsters631_page_m12_month',
    'page arguments' => array(3, array('active')),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'dispatch:live',
        'permission' => 'sirius dispatch staff',
      ),
    ),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_teamsters631.page_m12_month.inc'
  );

  $items['sirius/teamsters631/m12/%/active_missing'] = array(
    'title' => 'Employers with Active Dispatches and No Report',
    'page callback' => 'sirius_teamsters631_page_m12_month',
    'page arguments' => array(3, array('active', 'missing')),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'dispatch:live',
        'permission' => 'sirius dispatch staff',
      ),
    ),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_teamsters631.page_m12_month.inc'
  );

  return $items;
}
