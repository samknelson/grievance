<?php
/**
 * @file
 * grievance_menus.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function grievance_menus_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_add-a-company:node/add/grievance-company.
  $menu_links['main-menu_add-a-company:node/add/grievance-company'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance-company',
    'router_path' => 'node/add/grievance-company',
    'link_title' => 'Add a Company',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_add-a-company:node/add/grievance-company',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_companies:grievance/admin/shops/companies',
  );
  // Exported menu link: main-menu_add-a-contact:node/add/grievance-shop-contact.
  $menu_links['main-menu_add-a-contact:node/add/grievance-shop-contact'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance-shop-contact',
    'router_path' => 'node/add/grievance-shop-contact',
    'link_title' => 'Add a Contact',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_add-a-contact:node/add/grievance-shop-contact',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_contacts:grievance/admin/shops/contacts',
  );
  // Exported menu link: main-menu_add-a-shop:node/add/grievance-shop.
  $menu_links['main-menu_add-a-shop:node/add/grievance-shop'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance-shop',
    'router_path' => 'node/add/grievance-shop',
    'link_title' => 'Add a Shop',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_add-a-shop:node/add/grievance-shop',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_employers:grievance/admin/shops',
  );
  // Exported menu link: main-menu_add:admin/structure/custom_help_text/add.
  $menu_links['main-menu_add:admin/structure/custom_help_text/add'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/custom_help_text/add',
    'router_path' => 'admin/structure',
    'link_title' => 'Add',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_add:admin/structure/custom_help_text/add',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_help-text:admin/structure/custom_help_text',
  );
  // Exported menu link: main-menu_add:node/add/grievance-basic-page.
  $menu_links['main-menu_add:node/add/grievance-basic-page'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance-basic-page',
    'router_path' => 'node/add/grievance-basic-page',
    'link_title' => 'Add',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_add:node/add/grievance-basic-page',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_basic-pages:grievance/admin/basic-pages',
  );
  // Exported menu link: main-menu_add:node/add/grievance-news.
  $menu_links['main-menu_add:node/add/grievance-news'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance-news',
    'router_path' => 'node/add/grievance-news',
    'link_title' => 'Add',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_add:node/add/grievance-news',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_news:grievance/news',
  );
  // Exported menu link: main-menu_alert-types:grievance/admin/alert-types.
  $menu_links['main-menu_alert-types:grievance/admin/alert-types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/alert-types',
    'router_path' => 'grievance/admin/alert-types',
    'link_title' => 'Alert Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_alert-types:grievance/admin/alert-types',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_all-grievances:grievances/all.
  $menu_links['main-menu_all-grievances:grievances/all'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievances/all',
    'router_path' => 'grievances/all',
    'link_title' => 'All Grievances',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_all-grievances:grievances/all',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_arbitration-form-settings:admin/config/grievance/arbitrationform.
  $menu_links['main-menu_arbitration-form-settings:admin/config/grievance/arbitrationform'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/arbitrationform',
    'router_path' => 'admin/config',
    'link_title' => 'Arbitration Form Settings',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_arbitration-form-settings:admin/config/grievance/arbitrationform',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_article-search:grievance/admin/contract-templates/section-search.
  $menu_links['main-menu_article-search:grievance/admin/contract-templates/section-search'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/contract-templates/section-search',
    'router_path' => 'grievance/admin/contract-templates/section-search',
    'link_title' => 'Article Search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_article-search:grievance/admin/contract-templates/section-search',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_contracts:grievance/admin/contract-templates',
  );
  // Exported menu link: main-menu_assignee-notes:admin/structure/taxonomy/grievance_assignee_notes.
  $menu_links['main-menu_assignee-notes:admin/structure/taxonomy/grievance_assignee_notes'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_assignee_notes',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Assignee Notes',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_assignee-notes:admin/structure/taxonomy/grievance_assignee_notes',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_attachment-types:admin/structure/taxonomy/grievance_document_types.
  $menu_links['main-menu_attachment-types:admin/structure/taxonomy/grievance_document_types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_document_types',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Attachment Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_attachment-types:admin/structure/taxonomy/grievance_document_types',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_basic-pages:grievance/admin/basic-pages.
  $menu_links['main-menu_basic-pages:grievance/admin/basic-pages'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/basic-pages',
    'router_path' => 'grievance/admin/basic-pages',
    'link_title' => 'Basic Pages',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_basic-pages:grievance/admin/basic-pages',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_web:node/1',
  );
  // Exported menu link: main-menu_brought-by:admin/structure/taxonomy/grievance_broughtby.
  $menu_links['main-menu_brought-by:admin/structure/taxonomy/grievance_broughtby'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_broughtby',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Brought By',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_brought-by:admin/structure/taxonomy/grievance_broughtby',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_bulk-re-assign:grievance/admin/shops/bulk-reassign.
  $menu_links['main-menu_bulk-re-assign:grievance/admin/shops/bulk-reassign'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/shops/bulk-reassign',
    'router_path' => 'grievance/admin/shops/bulk-reassign',
    'link_title' => 'Bulk Re-assign',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_bulk-re-assign:grievance/admin/shops/bulk-reassign',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_employers:grievance/admin/shops',
  );
  // Exported menu link: main-menu_chapters:grievance/admin/chapters.
  $menu_links['main-menu_chapters:grievance/admin/chapters'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/chapters',
    'router_path' => 'grievance/admin/chapters',
    'link_title' => 'Chapters',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_chapters:grievance/admin/chapters',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_classifications:admin/structure/taxonomy/grievance_job_classification.
  $menu_links['main-menu_classifications:admin/structure/taxonomy/grievance_job_classification'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_job_classification',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Classifications',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_classifications:admin/structure/taxonomy/grievance_job_classification',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_companies:grievance/admin/shops/companies.
  $menu_links['main-menu_companies:grievance/admin/shops/companies'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/shops/companies',
    'router_path' => 'grievance/admin/shops/companies',
    'link_title' => 'Companies',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_companies:grievance/admin/shops/companies',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_employers:grievance/admin/shops',
  );
  // Exported menu link: main-menu_config:admin/config/grievance/core.
  $menu_links['main-menu_config:admin/config/grievance/core'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/core',
    'router_path' => 'admin/config',
    'link_title' => 'Config',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_config:admin/config/grievance/core',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_contact-configuration:admin/config/grievance/contacts.
  $menu_links['main-menu_contact-configuration:admin/config/grievance/contacts'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/contacts',
    'router_path' => 'admin/config',
    'link_title' => 'Contact Configuration',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_contact-configuration:admin/config/grievance/contacts',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_contact-types:admin/structure/taxonomy/grievance_contact_types.
  $menu_links['main-menu_contact-types:admin/structure/taxonomy/grievance_contact_types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_contact_types',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Contact Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_contact-types:admin/structure/taxonomy/grievance_contact_types',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_contacts:grievance/admin/shops/contacts.
  $menu_links['main-menu_contacts:grievance/admin/shops/contacts'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/shops/contacts',
    'router_path' => 'grievance/admin/shops/contacts',
    'link_title' => 'Contacts',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_contacts:grievance/admin/shops/contacts',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_employers:grievance/admin/shops',
  );
  // Exported menu link: main-menu_contracts:grievance/admin/contract-templates.
  $menu_links['main-menu_contracts:grievance/admin/contract-templates'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/contract-templates',
    'router_path' => 'grievance/admin/contract-templates',
    'link_title' => 'Contracts',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_contracts:grievance/admin/contract-templates',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_corrected-member-data:grievance/admin/corrected.
  $menu_links['main-menu_corrected-member-data:grievance/admin/corrected'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/corrected',
    'router_path' => 'grievance/admin/corrected',
    'link_title' => 'Corrected Member Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_corrected-member-data:grievance/admin/corrected',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_delivery-options:admin/structure/taxonomy/grievance_delivery.
  $menu_links['main-menu_delivery-options:admin/structure/taxonomy/grievance_delivery'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_delivery',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Delivery Options',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_delivery-options:admin/structure/taxonomy/grievance_delivery',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_departments:admin/structure/taxonomy/grievance_department.
  $menu_links['main-menu_departments:admin/structure/taxonomy/grievance_department'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_department',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Departments',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_departments:admin/structure/taxonomy/grievance_department',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_drop-down-lists:admin/structure/taxonomy.
  $menu_links['main-menu_drop-down-lists:admin/structure/taxonomy'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy',
    'router_path' => 'admin/structure/taxonomy',
    'link_title' => 'Drop-down Lists',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_employers:grievance/admin/shops.
  $menu_links['main-menu_employers:grievance/admin/shops'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/shops',
    'router_path' => 'grievance/admin/shops',
    'link_title' => 'Employers',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_employers:grievance/admin/shops',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_field-overrides:admin/config/grievances/fieldoverrides.
  $menu_links['main-menu_field-overrides:admin/config/grievances/fieldoverrides'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievances/fieldoverrides',
    'router_path' => 'admin/config/grievances/fieldoverrides',
    'link_title' => 'Field Overrides',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_field-overrides:admin/config/grievances/fieldoverrides',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_file-a-grievance:node/add/grievance.
  $menu_links['main-menu_file-a-grievance:node/add/grievance'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/grievance',
    'router_path' => 'node/add/grievance',
    'link_title' => 'File a Grievance',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_file-a-grievance:node/add/grievance',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_flood-control:admin/config/system/flood-control.
  $menu_links['main-menu_flood-control:admin/config/system/flood-control'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/system/flood-control',
    'router_path' => 'admin/config/system/flood-control',
    'link_title' => 'Flood Control',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_flood-control:admin/config/system/flood-control',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_users:admin/people',
  );
  // Exported menu link: main-menu_flood-unblock:admin/config/system/flood-unblock.
  $menu_links['main-menu_flood-unblock:admin/config/system/flood-unblock'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/system/flood-unblock',
    'router_path' => 'admin/config/system/flood-unblock',
    'link_title' => 'Flood Unblock',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_flood-unblock:admin/config/system/flood-unblock',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_users:admin/people',
  );
  // Exported menu link: main-menu_grievance-form-settings:admin/config/grievance/gform.
  $menu_links['main-menu_grievance-form-settings:admin/config/grievance/gform'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/gform',
    'router_path' => 'admin/config',
    'link_title' => 'Grievance Form Settings',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_grievance-form-settings:admin/config/grievance/gform',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_grievance-types:admin/structure/taxonomy/grievance_types.
  $menu_links['main-menu_grievance-types:admin/structure/taxonomy/grievance_types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_types',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Grievance Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_grievance-types:admin/structure/taxonomy/grievance_types',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_help-text:admin/structure/custom_help_text.
  $menu_links['main-menu_help-text:admin/structure/custom_help_text'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/custom_help_text',
    'router_path' => 'admin/structure',
    'link_title' => 'Help Text',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_help-text:admin/structure/custom_help_text',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_web:node/1',
  );
  // Exported menu link: main-menu_holidays:grievance/admin/holidays.
  $menu_links['main-menu_holidays:grievance/admin/holidays'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/holidays',
    'router_path' => 'grievance/admin/holidays',
    'link_title' => 'Holidays',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_holidays:grievance/admin/holidays',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_intake-log:admin/config/grievance/intake-log.
  $menu_links['main-menu_intake-log:admin/config/grievance/intake-log'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/intake-log',
    'router_path' => 'admin/config',
    'link_title' => 'Intake Log',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_intake-log:admin/config/grievance/intake-log',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_letter-templates:grievance/admin/letter-templates.
  $menu_links['main-menu_letter-templates:grievance/admin/letter-templates'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/letter-templates',
    'router_path' => 'grievance/admin/letter-templates',
    'link_title' => 'Letter Templates',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_letter-templates:grievance/admin/letter-templates',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_log-search:grievances/log.
  $menu_links['main-menu_log-search:grievances/log'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievances/log',
    'router_path' => 'grievances/log',
    'link_title' => 'Log Search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_log-search:grievances/log',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_log-tags:admin/structure/taxonomy/grievance_log_tags.
  $menu_links['main-menu_log-tags:admin/structure/taxonomy/grievance_log_tags'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_log_tags',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Log Tags',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_log-tags:admin/structure/taxonomy/grievance_log_tags',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_log-types:admin/structure/taxonomy/grievance_log_types.
  $menu_links['main-menu_log-types:admin/structure/taxonomy/grievance_log_types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_log_types',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Log Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_log-types:admin/structure/taxonomy/grievance_log_types',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_my-grievances:grievances.
  $menu_links['main-menu_my-grievances:grievances'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievances',
    'router_path' => 'grievances',
    'link_title' => 'My Grievances',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_my-grievances:grievances',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_news:grievance/news.
  $menu_links['main-menu_news:grievance/news'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/news',
    'router_path' => 'grievance/news',
    'link_title' => 'News',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_news:grievance/news',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_web:node/1',
  );
  // Exported menu link: main-menu_outcomes:admin/structure/taxonomy/grievance_outcome.
  $menu_links['main-menu_outcomes:admin/structure/taxonomy/grievance_outcome'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_outcome',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Outcomes',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_outcomes:admin/structure/taxonomy/grievance_outcome',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_phaxio:admin/config/grievance/phaxio.
  $menu_links['main-menu_phaxio:admin/config/grievance/phaxio'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/phaxio',
    'router_path' => 'admin/config',
    'link_title' => 'Phaxio',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_phaxio:admin/config/grievance/phaxio',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_remedies:admin/structure/taxonomy/grievance_remedies.
  $menu_links['main-menu_remedies:admin/structure/taxonomy/grievance_remedies'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_remedies',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Remedies',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_remedies:admin/structure/taxonomy/grievance_remedies',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -37,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_report-average-time-between-statuses:grievance/report/average-time.
  $menu_links['main-menu_report-average-time-between-statuses:grievance/report/average-time'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/average-time',
    'router_path' => 'grievance/report/average-time',
    'link_title' => 'Report: Average Time Between Statuses',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-average-time-between-statuses:grievance/report/average-time',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_report-grievances-with-no-status:grievance/report/missing-status.
  $menu_links['main-menu_report-grievances-with-no-status:grievance/report/missing-status'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/missing-status',
    'router_path' => 'grievance/report/missing-status',
    'link_title' => 'Report: Grievances with no Status',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-grievances-with-no-status:grievance/report/missing-status',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_report-open-grievances-by-specialist:grievance/report/open.
  $menu_links['main-menu_report-open-grievances-by-specialist:grievance/report/open'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/open',
    'router_path' => 'grievance/report/open',
    'link_title' => 'Report: Open Grievances by Specialist',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-open-grievances-by-specialist:grievance/report/open',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_report-settlements:grievance/report/settlement.
  $menu_links['main-menu_report-settlements:grievance/report/settlement'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/settlement',
    'router_path' => 'grievance/report/settlement',
    'link_title' => 'Report: Settlements',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-settlements:grievance/report/settlement',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_report-type-and-status:grievance/report/bytype.
  $menu_links['main-menu_report-type-and-status:grievance/report/bytype'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/bytype',
    'router_path' => 'grievance/report/bytype',
    'link_title' => 'Report: Type and Status',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-type-and-status:grievance/report/bytype',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_report-yearly-summary:grievance/report/year-summary.
  $menu_links['main-menu_report-yearly-summary:grievance/report/year-summary'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/report/year-summary',
    'router_path' => 'grievance/report/year-summary',
    'link_title' => 'Report: Yearly Summary',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_report-yearly-summary:grievance/report/year-summary',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_my-grievances:grievances',
  );
  // Exported menu link: main-menu_resolution-procedures:admin/structure/taxonomy/grievance_resproc.
  $menu_links['main-menu_resolution-procedures:admin/structure/taxonomy/grievance_resproc'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_resproc',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Resolution Procedures',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_resolution-procedures:admin/structure/taxonomy/grievance_resproc',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -36,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_section-search:grievance/admin/contract-templates/clause-search.
  $menu_links['main-menu_section-search:grievance/admin/contract-templates/clause-search'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'grievance/admin/contract-templates/clause-search',
    'router_path' => 'grievance/admin/contract-templates/clause-search',
    'link_title' => 'Section Search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_section-search:grievance/admin/contract-templates/clause-search',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_contracts:grievance/admin/contract-templates',
  );
  // Exported menu link: main-menu_shift:admin/structure/taxonomy/grievance_shift.
  $menu_links['main-menu_shift:admin/structure/taxonomy/grievance_shift'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_shift',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Shift',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_shift:admin/structure/taxonomy/grievance_shift',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -35,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_statuses:admin/structure/taxonomy/grievance_status.
  $menu_links['main-menu_statuses:admin/structure/taxonomy/grievance_status'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_status',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Statuses',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_statuses:admin/structure/taxonomy/grievance_status',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -34,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );
  // Exported menu link: main-menu_tags:admin/structure/taxonomy/grievance_tags.
  $menu_links['main-menu_tags:admin/structure/taxonomy/grievance_tags'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_tags',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Tags',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_tags:admin/structure/taxonomy/grievance_tags',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_timss-configuration:admin/config/grievance/timss.
  $menu_links['main-menu_timss-configuration:admin/config/grievance/timss'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/grievance/timss',
    'router_path' => 'admin/config',
    'link_title' => 'TIMSS Configuration',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_timss-configuration:admin/config/grievance/timss',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_two-factor-authentication:admin/config/people/tfa.
  $menu_links['main-menu_two-factor-authentication:admin/config/people/tfa'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/people/tfa',
    'router_path' => 'admin/config/people',
    'link_title' => 'Two-Factor Authentication',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_two-factor-authentication:admin/config/people/tfa',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_two-factor-trusted-ips:admin/config/people/tfa-trusted-ip.
  $menu_links['main-menu_two-factor-trusted-ips:admin/config/people/tfa-trusted-ip'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/config/people/tfa-trusted-ip',
    'router_path' => 'admin/config/people',
    'link_title' => 'Two-Factor Trusted IPs',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_two-factor-trusted-ips:admin/config/people/tfa-trusted-ip',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -37,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_config:admin/config/grievance/core',
  );
  // Exported menu link: main-menu_users:admin/people.
  $menu_links['main-menu_users:admin/people'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/people',
    'router_path' => 'admin/people',
    'link_title' => 'Users',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_users:admin/people',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_web:node/1.
  $menu_links['main-menu_web:node/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/1',
    'router_path' => 'node/%',
    'link_title' => 'Web',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_web:node/1',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_work-status:admin/structure/taxonomy/grievance_work_status.
  $menu_links['main-menu_work-status:admin/structure/taxonomy/grievance_work_status'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'admin/structure/taxonomy/grievance_work_status',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Work Status',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'main-menu_work-status:admin/structure/taxonomy/grievance_work_status',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -33,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
    'parent_identifier' => 'main-menu_drop-down-lists:admin/structure/taxonomy',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Add');
  t('Add a Company');
  t('Add a Contact');
  t('Add a Shop');
  t('Alert Types');
  t('All Grievances');
  t('Arbitration Form Settings');
  t('Article Search');
  t('Assignee Notes');
  t('Attachment Types');
  t('Basic Pages');
  t('Brought By');
  t('Bulk Re-assign');
  t('Chapters');
  t('Classifications');
  t('Companies');
  t('Config');
  t('Contact Configuration');
  t('Contact Types');
  t('Contacts');
  t('Contracts');
  t('Corrected Member Data');
  t('Delivery Options');
  t('Departments');
  t('Drop-down Lists');
  t('Employers');
  t('Field Overrides');
  t('File a Grievance');
  t('Flood Control');
  t('Flood Unblock');
  t('Grievance Form Settings');
  t('Grievance Types');
  t('Help Text');
  t('Holidays');
  t('Intake Log');
  t('Letter Templates');
  t('Log Search');
  t('Log Tags');
  t('Log Types');
  t('My Grievances');
  t('News');
  t('Outcomes');
  t('Phaxio');
  t('Remedies');
  t('Report: Average Time Between Statuses');
  t('Report: Grievances with no Status');
  t('Report: Open Grievances by Specialist');
  t('Report: Settlements');
  t('Report: Type and Status');
  t('Report: Yearly Summary');
  t('Resolution Procedures');
  t('Section Search');
  t('Shift');
  t('Statuses');
  t('TIMSS Configuration');
  t('Tags');
  t('Two-Factor Authentication');
  t('Two-Factor Trusted IPs');
  t('Users');
  t('Web');
  t('Work Status');

  return $menu_links;
}
