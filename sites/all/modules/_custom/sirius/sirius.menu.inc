<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_node_tabs() {
  $node_tabs = array();

  $content_types = array('sirius_event', 'sirius_worker', 'grievance_shop', 'sirius_ledger_account', 'sirius_contact_relationship', 'sirius_dispatch_job_group');
  foreach ($content_types as $content_type) {
    $node_tabs[$content_type][$content_type . '_materials'] = array(
      'title' => 'Materials',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_materials',
      'permissions' => array('sirius'),
      'weight' => 20,
    );
  }

  $content_types = array('sirius_json_definition');
  foreach ($content_types as $content_type) {
    $node_tabs[$content_type][$content_type . '_machine_name'] = array(
      'title' => 'Machine Name',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_machine_name',
      'permissions' => array('sirius domain admin'),
      'weight' => 20,
      'file' => 'sirius.page_entity_machine_name.inc',
    );
  }

  $content_types = array('grievance_basic_page');
  foreach ($content_types as $content_type) {
    $node_tabs[$content_type][$content_type . '_machine_name'] = array(
      'title' => 'Restrict',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_restrict',
      'permissions' => array('sirius'),
      'weight' => 20,
      'file' => 'sirius.page_entity_restrict.inc',
    );
  }

  $entity_contact_files = array('sirius.page_entity_contacts_list.inc', 'sirius.page_entity_contacts_add_existing.inc', 'sirius.page_entity_contacts_add_new.inc');
  $node_tabs['sirius_ledger_account']['sirius_ledger_account_contacts'] = array(
    'title' => 'Contacts',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_contacts_list',
    'permissions' => array('sirius domain admin'),
    'file' => $entity_contact_files,
    'weight' => 5,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_contacts/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_contacts_list',
    'permissions' => array('sirius domain admin'),
    'file' => $entity_contact_files,
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_contacts/add-existing'] = array(
    'title' => 'Add Existing Contact',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_contacts_add_existing',
    'permissions' => array('sirius domain admin'),
    'file' => $entity_contact_files,
    'weight' => 5,
    'type' => MENU_LOCAL_TASK,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_contacts/add-new'] = array(
    'title' => 'Create New Contact',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_contacts_add_new',
    'permissions' => array('sirius domain admin'),
    'file' => $entity_contact_files,
    'weight' => 5,
    'type' => MENU_LOCAL_TASK,
  );

  $content_types = array('sirius_log');
  foreach ($content_types as $content_type) {
    $node_tabs[$content_type][$content_type . '_signatures'] = array(
      'title' => 'Signatures',
      'callback' => 'sirius_node_tab_entity_signatures',
      'permissions' => array('sirius domain admin'),
      'file' => 'sirius.page_entity_signatures.inc',
      'weight' => 5,
    );
  }

  $customfield_files = array(
    'sirius.page_entity_customfield.inc', 
    'sirius.page_entity_customfield_settings.inc', 
  );
  $customfield_content_types = sirius_customfield_content_types_all();
  foreach ($customfield_content_types as $content_type => $content_type_settings) {
    $node_tabs[$content_type][$content_type . '_customfield_settings'] = array(
      'title' => 'Custom Field Sets',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_customfield_settings',
      'permissions' => array($content_type_settings['permission']),
      'weight' => 10,
      'file' => $customfield_files,
      'type' => MENU_LOCAL_TASK,
    );

    $node_tabs[$content_type][$content_type . '_customfield_tab'] = array(
      'title' => 'Hidden Customfield tab',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_customfield',
      'permissions' => array('sirius'),
      'weight' => 100,
      'file' => $customfield_files,
      'type' => MENU_LOCAL_TASK,
    );
  }

  // 
  // Entity Log
  //

  /*
  $content_types = array(
    'sirius_dispatch' => 'sirius dispatch staff',
    'sirius_dispatch_job' => 'sirius dispatch staff',
    'sirius_dispatch_facility' => 'sirius dispatch staff',
    'sirius_dispatch_facility_hall' => 'sirius dispatch staff',
    'grievance_shop' => 'sirius dispatch staff',
    'sirius_payment' => 'sirius ledger admin',
    'sirius_payperiod' => 'sirius hour admin',
    'sirius_worker' => 'sirius dispatch staff',
    'sirius_contact' => 'sirius dispatch staff',
  );
  foreach ($content_types as $content_type => $permission) {
    $node_tabs[$content_type][$content_type . '_log'] = array(
      'title' => 'Log',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_log',
      'permissions' => array($permission),
      'weight' => 20,
    );
  }
  */

  $buckets = sirius_log_buckets();
  $permission_lookup = user_permission_get_modules();
  foreach ($buckets as $bucket_id => $bucket) {
    // Hack to prevent adding menu items for modules that don't exist.
    // @todo: this is an ugly and cumbersome way to do this, and something better should be found.
    $ok = TRUE;
    foreach ($bucket['permissions'] as $permission) {
      if (!$permission_lookup[$permission]) { $ok = FALSE; }
    }
    if (!$ok) {
      continue;
    }
    $node_tabs[$bucket['content_type']][$bucket['path']] = array(
      'title' => $bucket['title'],
      'description' => '',
      'callback' => 'sirius_node_tab_entity_log',
      'permissions' => $bucket['permissions'],
      'weight' => $bucket['weight'],
      'callback arguments' => array($bucket_id),
      'file' => 'sirius.page_entity_log.inc',
    );
  }

  foreach (sirius_letter_content_types_all() as $content_type => $content_type_name) {
    $node_tabs[$content_type][$content_type . '_letter'] = array(
      'title' => 'Letters',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_letter',
      'permissions' => array('sirius'),
    );
  }

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
    'permissions' => array('sirius letterhead edit'),
  );

  $node_tabs['sirius_domain']['sirius_domain_info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_info',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
    'file' => 'sirius.page_domain_info.inc',
    'landing' => TRUE,
    'type' => MENU_CALLBACK,
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

  $node_tabs['sirius_domain']['sirius_domain_components'] = array(
    'title' => 'Components',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_components',
    'permissions' => array('sirius admin'),
    'weight' => 6,
    'file' => 'sirius.page_domain_components.inc',
  );

  $node_tabs['sirius_domain']['sirius_domain_stats'] = array(
    'title' => 'Stats',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_stats_total',
    'permissions' => array('sirius domain admin'),
    'weight' => 6,
  );

  $node_tabs['sirius_domain']['sirius_domain_stats/view'] = array(
    'title' => 'Stats (Including Subdomains)',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_stats_total',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_domain']['sirius_domain_stats/domain_only'] = array(
    'title' => 'Stats (This Domain Only)',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_stats',
    'permissions' => array('sirius domain admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_domain']['sirius_domain_stats/calc'] = array(
    'title' => 'Recalc',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_stats_calc',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_contact']['sirius_contact_json'] = array(
    'title' => 'Custom Fields',
    'description' => '',
    'callback' => 'sirius_node_tab_contact_json',
    'permissions' => array('sirius'),
    'weight' => 9,
  );

  $node_tabs['sirius_domain']['sirius_domain_delete'] = array(
    'title' => 'Delete',
    'description' => '',
    'callback' => 'sirius_node_tab_sirius_domain_delete',
    'permissions' => array('sirius domain admin'),
    'weight' => 10,
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition_disclaimer'] = array(
    'title' => 'Disclaimer',
    'description' => '',
    'callback' => 'sirius_node_tab_json_definition_disclaimer',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_json_definition']['sirius_customfield_content_types'] = array(
    'title' => 'Content Types',
    'description' => '',
    'callback' => 'sirius_node_tab_customfield_content_types',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
    'file' => 'sirius.page_customfield_content_types.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_customfield_tab'] = array(
    'title' => 'Single-Value Tab',
    'description' => '',
    'callback' => 'sirius_node_tab_customfield_tab',
    'permissions' => array('sirius domain admin'),
    'weight' => 2,
    'file' => 'sirius.page_customfield_tab.inc',
  );

  /*
  $node_tabs['sirius_json_definition']['sirius_customfield_log'] = array(
    'title' => 'Multi-Value Tab',
    'description' => '',
    'callback' => 'sirius_node_tab_customfield_log',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
    'file' => 'sirius.page_customfield_log.inc',
  );
  */

  $node_tabs['sirius_phonenumber']['sirius_phonenumber_info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_node_tab_phonenumber_info',
    'permissions' => array('sirius domain admin'),
    'file' => 'sirius.page_phonenumber_info.inc',
    'weight' => 1,
    'landing' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $node_tabs['sirius_phonenumber']['sirius_phonenumber_manual_block'] = array(
    'title' => 'Administrative Block',
    'description' => '',
    'callback' => 'sirius_node_tab_phonenumber_manual_block',
    'permissions' => array('sirius domain admin'),
    'file' => 'sirius.page_phonenumber_manual_block.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_phonenumber']['sirius_phonenumber_scan'] = array(
    'title' => 'Scan',
    'description' => '',
    'callback' => 'sirius_node_tab_phonenumber_scan',
    'permissions' => array('sirius domain admin'),
    'file' => 'sirius.page_phonenumber_scan.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_contact']['sirius_contact_relationships'] = array(
    'title' => 'Relationships',
    'description' => '',
    'callback' => 'sirius_node_tab_contact_relationships',
    'permissions' => array('sirius contact admin'), 
    'file' => 'sirius.page_contact_relationships.inc',
    'component' => 'contact:relationships',
  );

  $content_types = array(
    'sirius_log', 
    'sirius_json_definition',
    'sirius_letterhead', 
    'sirius_contact', 
    'sirius_phonenumber', 
    'sirius_payperiod', 
    'sirius_dispatch',
    'sirius_trust_worker_benefit',
    'property',
    'actor',
    'property_actor',
  );
  foreach ($content_types as $content_type) {
    $node_tabs[$content_type][$content_type . '_rawjson'] = array(
      'title' => 'JSON',
      'description' => '',
      'callback' => 'sirius_node_tab_entity_json',
      'permissions' => array('sirius admin'),
      'weight' => 999,
      'file' => 'sirius.page_entity_json.inc',
    );
  }

  sirius_node_tab_set_module('sirius', $node_tabs);
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

  $customfield_files = array(
    'sirius.page_entity_customfield.inc', 
    'sirius.page_entity_customfield_settings.inc', 
  );

  $customfield_vocabs = array(
    'sirius_event_type' => array(
      'edit_permission' => 'sirius',
      'settings_permission' => 'sirius event admin',
      'can_edit' => TRUE
    ),
    'grievance_resproc' => array(
      'edit_permission' => 'grievance admin',
      'settings_permission' => 'grievance admin',
      'can_edit' => FALSE
    )
  );

  foreach ($customfield_vocabs as $vocabulary => $vocabulary_settings) {

    if ($vocabulary_settings['can_edit']) {
      $term_tabs[$vocabulary][$vocabulary . '_customfield'] = array(
        'title' => 'Custom Fields',
        'description' => '',
        'callback' => 'sirius_term_tab_entity_customfield',
        'permissions' => array($vocabulary_settings['edit_permission']),
        'weight' => 20,
        'file' => $customfield_files,
      );

      $term_tabs[$vocabulary][$vocabulary . '_customfield/edit'] = array(
        'title' => 'Edit',
        'description' => '',
        'callback' => 'sirius_term_tab_entity_customfield',
        'permissions' => array($vocabulary_settings['edit_permission']),
        'weight' => 1,
        'file' => $customfield_files,
        'type' => MENU_DEFAULT_LOCAL_TASK,
      );

      $term_tabs[$vocabulary][$vocabulary . '_customfield/settings'] = array(
        'title' => 'Settings',
        'description' => '',
        'callback' => 'sirius_term_tab_entity_customfield_settings',
        'permissions' => array($vocabulary_settings['settings_permission']),
        'weight' => 1,
        'file' => $customfield_files,
        'type' => MENU_LOCAL_TASK,
      );
    } else {
      $term_tabs[$vocabulary][$vocabulary . '_customfield'] = array(
        'title' => 'Custom Fields',
        'description' => '',
        'callback' => 'sirius_term_tab_entity_customfield_settings',
        'permissions' => array($vocabulary_settings['settings_permission']),
        'weight' => 20,
        'file' => $customfield_files,
      );
    }
  }

  sirius_node_tab_set_module('sirius', $term_tabs);
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
      if (isset($tab_record['type'])) { 
        $type = $tab_record['type'];
      } else {
        $type = MENU_LOCAL_TASK;
      }
      $item = array(
        'title' => $tab_record['title'],
        'page callback' => 'sirius_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => $type,
      );
      if ($tab_record['weight']) { $item['weight'] = $tab_record['weight']; }
      if ($tab_record['description']) { $item['description'] = $tab_record['description']; }
      if ($tab_record['page arguments']) { 
        $item['page_arguments'] = array_merge($items['node/%node/' . $tab_name]['page_arguments'], $tab_record['page arguments']);
      }
      if ($tab_record['title callback']) {
        $item['title callback'] = $tab_record['title callback'];
        unset($$item['title']);
      }
      if ($tab_record['title arguments']) { $item['title arguments'] = $tab_record['title arguments']; }
      $items['node/%node/' . $tab_name] = $item;

      /*
      if ($tab_record['rnode']) {
        $item['page callback'] = 'sirius_rnode_tab_content';
        $item['access callback'] = 'sirius_page_access_wrap';
        $item['access arguments'] = array('sirius.page_proxy_rnode.inc', 'sirius_rnode_tab_access', 1, $tab_name);
        $item['file'] = 'sirius.page_proxy_rnode.inc';
        $item['type'] = $type;
        $items['rnode/%/' . $tab_name] = $item;
      }
      */
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
      if (!$tab_record['description']) {
        $items['taxonomy/term/%taxonomy_term/' . $tab_name]['description'] = '';
      }
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
    'title' => 'News: Admin',
    'description' => '',
    'page callback' => 'sirius_page_news_admin',
    'page arguments' => array(),
    'access arguments' => array('sirius news admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/do_not_disturb'] = array(
    'title' => 'Do Not Disurb Configuration',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'sirius_page_do_not_disurb_config',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/do_not_disturb/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'sirius_page_do_not_disurb_config',
    'page arguments' => array(),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/admin/do_not_disturb/test'] = array(
    'title' => 'Test',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'sirius_page_do_not_disurb_test',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/admin/banner'] = array(
    'title' => 'Banner and Footer Configuration',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_banner_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/banner/banner'] = array(
    'title' => 'Banner',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_banner_config'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/admin/banner/footer'] = array(
    'title' => 'Footer',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_footer_config'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
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

  $items['sirius/config/feed'] = array(
    'title' => 'Feeds',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_feed_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/server-paths'] = array(
    'title' => 'Server Paths',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_server_paths_admin'),
    'access arguments' => array('administer site configuration'),
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
    'page arguments' => array('sirius_email_settings_form'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius.page_email_settings.inc',
  );

  $items['sirius/config/email/config'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_settings_form'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius.page_email_settings.inc',
  );

  $items['sirius/config/email/test'] = array(
    'title' => 'Test',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_test_form'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius.page_email_test.inc',
  );

  $items['sirius/config/email/global'] = array(
    'title' => 'Global Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_settings_global_form'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'sirius.page_email_settings_global.inc',
  );

  $items['sirius/config/phone'] = array(
    'title' => 'Phone Configuration',
    'page callback' => 'sirius_page_callerid',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/phone/callerid'] = array(
    'title' => 'Caller IDs',
    'page callback' => 'sirius_page_callerid',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/phone/sms_global_disable'] = array(
    'title' => 'SMS Global Disable',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_sms_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 5,
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
    'type' => MENU_CALLBACK,
  );

  $items['sirius/loopback/sample-html'] = array(
    'title' => 'Sample HTML Content',
    'description' => '',
    'access callback' => TRUE,
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
    'access callback' => TRUE,
    'page callback' => 'sirius_letterhead_preview_loopback',
    'page arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/letterhead/sample-html/%node'] = array(
    'title' => 'Sample HTML for Letterhead',
    'description' => '',
    'access callback' => TRUE,
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

  $items['sirius/phonetree/config'] = array(
    'title' => 'Phone Tree Configuration',
    'page callback' => 'sirius_page_phonetree_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius admin',
        'component' => 'phonetree',
      ),
    ),
    'file' => 'sirius.page_phonetree_config.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domain/config'] = array(
    'title' => 'Domain Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_domain_usermanage_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domain/config/usermanage'] = array(
    'title' => 'User Manager',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_domain_usermanage_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/domain/config/stats'] = array(
    'title' => 'Stats',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_domain_stats_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/domain/switch'] = array(
    'title' => 'Switch',
    'page callback' => 'sirius_domain_page_switch',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );



  $items['sirius/domain/list'] = array(
    'title' => 'Domains',
    'page callback' => 'sirius_domain_page_list',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/domain/list/tools'] = array(
    'title' => 'Tools',
    'page callback' => 'sirius_domain_page_list',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/domain/list/stats'] = array(
    'title' => 'Stats',
    'page callback' => 'sirius_domain_page_list_stats',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/domain/list/prefs'] = array(
    'title' => 'Preferences',
    'page callback' => 'sirius_domain_page_list_prefs',
    'page arguments' => array(),
    'access arguments' => array('sirius domain switch'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
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

  $items['sirius/map/admin'] = array(
    'title' => 'Map Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_map_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/signature/admin'] = array(
    'title' => 'Signature Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_signature_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/disclaimer'] = array(
    'title' => 'Disclaimers',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_disclaimer_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/disclaimer/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_disclaimer_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/disclaimer/list'] = array(
    'title' => 'List',
    'description' => '',
    'page callback' => 'sirius_disclaimer_page_list',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/disclaimer/list/add'] = array(
    'title' => 'Add a Disclaimer',
    'description' => '',
    'page callback' => 'sirius_disclaimer_add',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/pdf/settings'] = array(
    'title' => 'PDF Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_pdf_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
  );

  $items['sirius/mailattach'] = array(
    'title' => 'Mailattach',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_mailattach_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/mailattach/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_mailattach_admin'),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/mailattach/test'] = array(
    'title' => 'Test',
    'description' => '',
    'page callback' => 'sirius_mailattach_page_test',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/mailattach/test-all'] = array(
    'title' => 'Test (All Domains)',
    'description' => '',
    'page callback' => 'sirius_mailattach_page_test_all',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/contact/resync/all'] = array(
    'title' => 'Resync All Contacts',
    'description' => '',
    'page callback' => 'sirius_page_contact_resync_all',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  /*
  $items['sirius/field_overrides'] = array(
    'title' => 'Field Overrides',
    'page callback' => 'sirius_page_field_overrides',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );
  */

  $items['sirius/log/quicklog/config'] = array(
    'title' => 'Quicklog Settings',
    'description' => '',
    'page callback' => 'sirius_log_quicklog_config',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/modal/%node/quicklog'] = array(
    'title'            => 'Add Log Entry',
    'page callback'    => 'sirius_log_quicklog_page_add',
    'page arguments'   => array(2),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permissions' => array('sirius dispatch staff', 'sirius trust staff')
      ),
    ),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/phonenumber'] = array(
    'title' => 'Manage Phone Numbers',
    'page callback' => 'sirius_page_phonenumber_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/phonenumber/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_page_phonenumber_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/phonenumber/check'] = array(
    'title' => 'Lookup',
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

  $items['sirius/config/by-module/all'] = array(
    'title' => 'All',
    'page callback' => 'sirius_page_config',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $links = sirius_config_links();
  $weight = 1;
  foreach ($links as $key => $record) {
    ++$weight;
    $items["sirius/config/by-module/$key"] = array(
      'title' => $record['title'],
      'page callback' => 'sirius_page_config',
      'page arguments' => array(3),
      'access arguments' => array('sirius domain admin'),
      'type' => MENU_LOCAL_TASK,
      'weight' => $weight,
    );
  }


  $items['sirius/customfield/definition/list'] = array(
    'title' => 'Custom Field Definitions',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'sirius_customfield_definition_list',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius.page_customfield_definitions.inc',
  );

  $items['sirius/esignature/admin'] = array(
    'title' => 'eSignature Settings',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_esignature_admin'),
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
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
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

  $items['s/%'] = array(
    'title' => 'Shortlink',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_shortlink_clicked',
    'page arguments' => array(1),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['user/%/esig'] = array(
    'title' => 'Signature Preferences',
    'page callback' => 'sirius_user_tab_esignature',
    'page arguments' => array(1),
    'access callback' => 'sirius_user_tab_esignature_access',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 8,
  );

  $items['user/%/sirius_proxy'] = array(
    'title' => 'Proxy',
    'page callback' => 'sirius_user_tab_proxy',
    'page arguments' => array(1),
    'access callback' => 'sirius_user_tab_proxy_access',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 8,
  );

  $items['user/%/current_location'] = array(
    'title' => 'Current Location',
    'page callback' => 'sirius_user_tab_map_current_location',
    'page arguments' => array(1),
    'access callback' => 'sirius_user_tab_map_current_location_access',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );

  /*
  $items['rnode/%'] = array(
    'title' => 'View',
    'page callback' => 'sirius_rnode_tab_content',
    'page arguments' => array(1),
    'access callback' => 'sirius_page_access_wrap',
    'access arguments' => array('sirius.page_proxy_rnode.inc', 'sirius_rnode_tab_access', 1),
    'file' => 'sirius.page_proxy_rnode.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['rnode/%/view'] = array(
    'title' => 'View',
    'page callback' => 'sirius_rnode_tab_content',
    'page arguments' => array(1),
    'access callback' => 'sirius_page_access_wrap',
    'access arguments' => array('sirius.page_proxy_rnode.inc', 'sirius_rnode_tab_access', 1),
    'file' => 'sirius.page_proxy_rnode.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  */

  $items['sirius/ajax/map/current_location/checkin'] = array(
    'page callback'    => 'sirius_map_current_location_checkin',
    'page arguments'   => array(),
    'access arguments' => array('sirius'),
    'type'             => MENU_CALLBACK,
  );

  $items['sirius/ajax/contact-autocomplete'] = array(
    'page callback' => 'sirius_contact_autocomplete',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_CALLBACK,
  );

  $items['sirius/domain/components/root'] = array(
    'title' => 'Root Domain Components',
    'page callback' => 'sirius_page_domain_components_root',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'file' => 'sirius.page_domain_components_root.inc',
    'type' => MENU_NORMAL_ITEM,
  );


  /*
  $items['public'] = array(
    'page callback' => 'sirius_page_public',
    'page arguments' => array(),
    'access callback' => 'sirius_page_public_access',
    'access arguments' => array(),
    'type' => MENU_CALLBACK,
  );
  */

  return $items;
}

function sirius_page_access_wrap($module, $file, $callback, $arg, $arg2 = NULL) {
  sirius_require($module, $file);
  return $callback($arg, $arg2);
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

/**
 * Implments hook_modal_paths
 */

function sirius_modal_paths() {
  $paths = array();

  $paths['sirius/modal/%node/quicklog'] = array(
    'style' => 'sirius-modal-style',
  );

  return $paths;
}

function sirius_modal_styles() {
  $styles = array();

  $styles['sirius-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 600,
      'height' => 500,
    ),
  );

  $styles['sirius-modal-style-small'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 400,
      'height' => 200,
    ),
  );

  return $styles;
}

/**
 * Utility function to wrap permission, component, and other restrictions
 */

function sirius_menu_access_callback($options) {
  // Simple permission
  if ($options['permission']) {
    if (!user_access($options['permission'])) {
      return FALSE; 
    }
  }

  // Multiple permissions
  if ($options['permissions']) {
    $found = FALSE;
    foreach ($options['permissions'] as $permission) {
      if (user_access($permission)) { $found = TRUE; }
    }
    if (!$found) { return FALSE; }
  } 

  // Component
  if ($options['component']) {
    if (!sirius_domain_component_enabled($options['component'])) {
      return FALSE;
    }
  }

  // Passed all tests
  return TRUE;
}
