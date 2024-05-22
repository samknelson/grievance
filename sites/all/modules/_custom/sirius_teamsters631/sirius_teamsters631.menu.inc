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
          // 'action' => 'employer_dashboard',
          'action' => 'staff',
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

  return $items;
}
