<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_event']['sirius_event_materials'] = array(
    'title' => 'Materials',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_materials',
    'permissions' => array('sirius'),
    'weight' => 20,
  );

  $node_tabs['sirius_worker']['sirius_worker_materials'] = array(
    'title' => 'Materials',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_materials',
    'permissions' => array('sirius'),
    'weight' => 20,
  );

  $node_tabs['grievance_shop']['grievance_shop_materials'] = array(
    'title' => 'Materials',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_materials',
    'permissions' => array('sirius'),
    'weight' => 20,
  );

  // 
  // Entity Log
  //

  $node_tabs['sirius_dispatch']['sirius_dispatch_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_facility']['sirius_dispatch_facility_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_facility_hall']['sirius_dispatch_facility_hall_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['grievance_shop']['grievance_shop_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_feed']['sirius_feed_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_payment']['sirius_payment_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius ledger admin'),
  );

  $node_tabs['sirius_worker']['sirius_worker_letter'] = array(
    'title' => 'Letters',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_letter',
    'permissions' => array('sirius admin'),
  );

  $node_tabs['grievance']['grievance_letter'] = array(
    'title' => 'Letters',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_letter',
    'permissions' => array('sirius admin'),
  );

  $node_tabs['grievance_letter_template']['sirius_letter_template_recipients'] = array(
    'title' => 'Recipients',
    'description' => '',
    'callback' => 'sirius_node_tab_letter_template_recipients',
    'permissions' => array('grievance admin'),
  );


  $node_tabs['sirius_feed']['sirius_feed_wizard'] = array(
    'title' => 'Wizard',
    'description' => '',
    'callback' => 'sirius_node_tab_feed_wizard',
    'callback arguments' => array(),
    'permissions' => array('sirius'),
  );

  $node_tabs['sirius_letterhead']['sirius_letterhead_preview'] = array(
    'title' => 'Preview',
    'description' => '',
    'callback' => 'sirius_node_tab_letterhead_preview',
    'permissions' => array('sirius dispatch letterhead edit'),
  );

  $node_tabs['sirius_domain']['sirius_domain_subdomains'] = array(
    'title' => 'Subdomains',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_subdomains',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_domain']['sirius_domain_users'] = array(
    'title' => 'Users',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_users',
    'permissions' => array('sirius domain admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_domain']['sirius_domain_clone'] = array(
    'title' => 'Clone',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_clone',
    'permissions' => array('sirius domain admin'),
    'weight' => 5,
  );

  $node_tabs['sirius_domain']['sirius_domain_delete'] = array(
    'title' => 'Delete',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_delete',
    'permissions' => array('sirius domain admin'),
    'weight' => 6,
  );

  $node_tabs['sirius_contact']['sirius_contact_json'] = array(
    'title' => 'Custom Fields',
    'description' => '',
    'callback' => 'sirius_node_tab_contact_json',
    'permissions' => array('sirius'),
    'weight' => 6,
  );

  return $node_tabs;
}

function sirius_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_event_type']['sirius_event_materials'] = array(
    'title' => 'Materials',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_materials',
    'permissions' => array('sirius'),
  );

  return $term_tabs;

}
/**
 * Implements hook_menu
 */

function sirius_menu() {
  $items = array();

  $node_tabs = sirius_node_tab_all('node_tabs');
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      if ($tab_record['type']) { $type = $tab_record['type']; } else { $type = MENU_LOCAL_TASK; }
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'page callback' => 'sirius_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => $type,
      );
      if ($tab_record['weight']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
      }
      if ($tab_record['description']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['description'];
      }
      if ($tab_record['page arguments']) {
        $items['node/%node/' . $tab_name]['page_arguments'] = array_merge($items['node/%node/' . $tab_name]['page_arguments'], $tab_record['page arguments']);
      }
    }
  }

  $term_tabs = sirius_term_tab_all('term_tabs');
  foreach ($term_tabs as $term_type => $term_record) {
    foreach ($term_record as $tab_name => $tab_record) {
      if ($tab_record['type']) { $type = $tab_record['type']; } else { $type = MENU_LOCAL_TASK; }
      $items['taxonomy/term/%taxonomy_term/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_term_tab_content',
        'page arguments' => array(2, $tab_name),
        'access callback' => 'sirius_term_tab_access',
        'access arguments' => array(2, $tab_name),
        'type' => $type,
      );
      if ($tab_record['weight']) {
        $items['taxonomy/term/%taxonomy_term/' . $tab_name]['weight'] = $tab_record['weight'];
      }
      if ($tab_record['page arguments']) {
        $items['taxonomy/term/%taxonomy_term/' . $tab_name]['page_arguments'] = array_merge($items['term/%term/' . $tab_name]['page_arguments'], $tab_record['page arguments']);
      }
    }
  }

  $items['sirius/news'] = array(
    'title' => 'News',
    'description' => '',
    'page callback' => 'sirius_page_news',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/news/admin'] = array(
    'title' => 'News',
    'description' => '',
    'page callback' => 'sirius_page_news_admin',
    'page arguments' => array(),
    'access arguments' => array('sirius news admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/banner'] = array(
    'title' => 'Banner Configuration ',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_banner_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/auditlog/config'] = array(
    'title' => 'Auditlog Configuration ',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_auditlog_config_form'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['d/%/%'] = array(
    'title' => 'Dispatch shortlink',
    'page callback' => 'sirius_aatlink',
    'page arguments' => array(1,2),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/specialtid'] = array(
    'title' => 'Special Terms',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_specialtid_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/specialrid'] = array(
    'title' => 'Special Roles',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_specialrid_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/email'] = array(
    'title' => 'Email Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/email/config'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/email/test'] = array(
    'title' => 'Test',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_test_form'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/phone'] = array(
    'title' => 'Phone Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_sms_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/phone/sms'] = array(
    'title' => 'SMS',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_sms_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -100,
  );

  $items['sirius/config/phone/callerid'] = array(
    'title' => 'Caller IDs',
    'page callback' => 'sirius_page_callerid',
    'page arguments' => array(),
    'access arguments' => array('sirius callerid admin'),
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/config/menu'] = array(
    'title' => 'Menu Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_menu_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/letter'] = array(
    'title' => 'Letter Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_letter_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/letter/general'] = array(
    'title' => 'General',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_letter_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );


  $items['sirius/voice/forward-to-operator'] = array(
    'title' => 'Incoming Calls',
    'page callback' => 'sirius_page_voice_forward_to_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius log view'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/voice/forward-to-operator/log'] = array(
    'title' => 'Incoming Calls',
    'page callback' => 'sirius_page_voice_forward_to_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius log view'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/voice/forward-to-operator/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'access arguments' => array('sirius log view'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_page_voice_forward_to_operator_settings'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/loopback/remote-ip'] = array(
    'title' => 'What is my IP?',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_loopback_remote_ip_print',
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/loopback/sample-html'] = array(
    'title' => 'Sample HTML Content',
    'description' => '',
    'access callback' => 'sirius_loopback_access',
    'page callback' => 'sirius_loopback_sample_html',
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/loopback/echo'] = array(
    'title' => 'Loopback Echo',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_loopback_echo',
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/letterhead/sample-html/%node'] = array(
    'title' => 'Sample HTML for Letterhead',
    'description' => '',
    'access callback' => 'sirius_loopback_access',
    'page callback' => 'sirius_letterhead_preview_loopback',
    'page arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/letterhead/sample-html/%node'] = array(
    'title' => 'Sample HTML for Letterhead',
    'description' => '',
    'access callback' => 'sirius_loopback_access',
    'page callback' => 'sirius_letterhead_preview_loopback',
    'page arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/feeds/launch/%'] = array(
    'title' => 'Start an Upload',
    'description' => '',
    'access callback' => 'sirius_feed_page_launch_access',
    'access arguments' => array(3),
    'page callback' => 'sirius_feed_page_launch',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/feeds/%'] = array(
    'title' => 'Feed List',
    'description' => '',
    'access callback' => 'sirius_feed_page_list_access',
    'access arguments' => array(2),
    'page callback' => 'sirius_feed_page_list',
    'page arguments' => array(2),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/feeds'] = array(
    'title' => 'Feeds',
    'description' => '',
    'access callback' => 'sirius_feed_page_landing_access',
    'access arguments' => array(),
    'page callback' => 'sirius_feed_page_landing',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/sns/incoming'] = array(
    'title' => 'SNS Incoming',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_sns_incoming',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/sns/config'] = array(
    'title' => 'SNS Configuration',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_sns_config'),
    'type' => MENU_NORMAL_ITEM,
  );


  $items['sirius/domain/config/usermanage'] = array(
    'title' => 'Domain User Manager Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_domain_usermanage_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domain/switch'] = array(
    'title' => 'Domain Switch',
    'page callback' => 'sirius_domain_page_switch',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domains'] = array(
    'title' => 'Domains',
    'page callback' => 'sirius_page_domains',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domain/users'] = array(
    'title' => 'Users',
    'page callback' => 'sirius_page_domain_users',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/contacts'] = array(
    'title' => 'Contacts',
    'page callback' => 'sirius_page_contacts',
    'page arguments' => array(),
    'access arguments' => array('sirius contact admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/contact/definition'] = array(
    'title' => 'Contact Fields',
    'page callback' => 'sirius_page_contact_definition_edit',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/contact/definition/edit'] = array(
    'title' => 'Edit',
    'page callback' => 'sirius_page_contact_definition_edit',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/contact/definition/preview'] = array(
    'title' => 'Preview',
    'page callback' => 'sirius_page_contact_definition_preview',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/contact/definition/exim'] = array(
    'title' => 'Export / Import',
    'page callback' => 'sirius_page_contact_definition_exim',
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/contact/definition/display'] = array(
    'title' => 'Display',
    'page callback' => 'sirius_page_contact_definition_display',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/taxonomy/%'] = array(
    'title' => 'Taxonomy',
    'page callback' => 'sirius_page_taxonomy',
    'page arguments' => array(2),
    'access callback' => 'sirius_page_taxonomy_access',
    'access arguments' => array(2),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/taxonomy/%/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_page_taxonomy_list',
    'page arguments' => array(2),
    'access callback' => 'sirius_page_taxonomy_access',
    'access arguments' => array(2),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/taxonomy/%/sort'] = array(
    'title' => 'Sort',
    'page callback' => 'sirius_page_taxonomy_sort_wrapper',
    'page arguments' => array(2),
    'access callback' => 'sirius_page_taxonomy_access',
    'access arguments' => array(2),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/taxonomy/%/clone'] = array(
    'title' => 'Clone',
    'page callback' => 'sirius_page_taxonomy_clone',
    'page arguments' => array(2),
    'access callback' => 'sirius_page_taxonomy_clone_access',
    'access arguments' => array(2),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/taxonomy'] = array(
    'title' => 'Drop-Down List Management',
    'page callback' => 'sirius_page_taxonomy_vocabulary_list',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/volunteer/admin'] = array(
    'title' => 'Volunteer Admin',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_page_volunteer_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/volunteer/%'] = array(
    'title' => 'Volunteer Login',
    'description' => '',
    'page callback' => 'sirius_page_volunteer_selfregister',
    'page arguments' => array(2),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/contact/resync/all'] = array(
    'title' => 'Resync All Contacts',
    'description' => '',
    'page callback' => 'sirius_page_contact_resync_all',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/field_overrides'] = array(
    'title' => 'Field Overrides',
    'page callback' => 'sirius_page_field_overrides',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/phonenumber'] = array(
    'title' => 'Manage Phone Numbers',
    'page callback' => 'sirius_page_phonenumber_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/phonenumber/list'] = array(
    'title' => 'Phone Number List',
    'page callback' => 'sirius_page_phonenumber_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/phonenumber/check'] = array(
    'title' => 'Phone Number Lookup',
    'page callback' => 'sirius_page_phonenumber_check',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/emailaddress'] = array(
    'title' => 'Manage Email Addresses',
    'page callback' => 'sirius_page_emailaddress_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/emailaddress/list'] = array(
    'title' => 'Email Address List',
    'page callback' => 'sirius_page_emailaddress_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );


  $items['sirius/config'] = array(
    'title' => 'Sirius Configuration',
    'page callback' => 'sirius_page_config',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/esignature/test'] = array(
    'title' => 'Sirius eSignature Test',
    'page callback' => 'sirius_page_esignature_test',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/esignature/test/%'] = array(
    'title' => 'Sirius eSignature Test',
    'page callback' => 'sirius_page_esignature_test',
    'page arguments' => array(3),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/esignature/stream/%/%'] = array(
    'title' => 'Sirius eSignature Stream',
    'page callback' => 'sirius_esignature_stream',
    'page arguments' => array(3,4),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  ); 

  $items['sirius/license'] = array(
    'title' => 'Sirius License',
    'page callback' => 'sirius_page_license',
    'page arguments' => array(3,4),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  ); 

  $items['grievance/license'] = array(
    'title' => 'Sirius License',
    'page callback' => 'sirius_page_license',
    'page arguments' => array(3,4),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  ); 

  $items['taxonomy/term/%/json_exim'] = array(
    'title' => 'Export / Import',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'sirius_page_term_exim',
    'page arguments' => array(2),
    'type' => MENU_LOCAL_TASK,
    'weight' => 60,
  );

  return $items;
}

function sirius_aatlink($nid, $aat) {
  drupal_goto('node/' . $nid . '/sirius_dispatch_worker_view', array('query' => array('aat' => $aat)));
}

function sirius_menu_block_view() {
  $block = array();
  $block['subject'] = '';
  $block['content'] = superfish_contents(3)['content'];
  return $block;
}

function sirius_menu_admin() {
  $role_options = array_map('check_plain', user_roles());
  $menu_options = menu_parent_options(menu_get_menus(), array('mlid' => 0));

  for ($delta = 1; $delta <=4; ++$delta) {
    $form[$delta] = array(
      '#type' => 'fieldset',
      '#title' => 'Menu #' . $delta,
    );

    $form[$delta]['superfish_menu_' . $delta] = array(
      '#type' => 'select',
      '#title' => t('Menu'),
      '#default_value' => variable_get('superfish_menu_' . $delta, 'main-menu:0'),
      '#options' => $menu_options,
    );

     // Per-role visibility.
    $default_role_options = db_query("SELECT rid FROM {block_role} WHERE module = 'superfish' AND delta = :delta", array(
      ':delta' => $delta,
    ))->fetchCol();
    $form[$delta]['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => $default_role_options,
      '#options' => $role_options,
    );
  }

  return system_settings_form($form);
}

