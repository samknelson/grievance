<?php
/**
 * @file
 * grievance_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function grievance_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'grievance_attachments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Attachments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Attachments';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'field_grievance_log_type' => 'field_grievance_log_type',
    'title' => 'title',
    'name' => 'name',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_alert' => 'field_grievance_alert',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_log_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_alert' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_alert_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Field: Grievance Document Type */
  $handler->display->display_options['fields']['field_grievance_document_type']['id'] = 'field_grievance_document_type';
  $handler->display->display_options['fields']['field_grievance_document_type']['table'] = 'field_data_field_grievance_document_type';
  $handler->display->display_options['fields']['field_grievance_document_type']['field'] = 'field_grievance_document_type';
  $handler->display->display_options['fields']['field_grievance_document_type']['label'] = 'Type';
  $handler->display->display_options['fields']['field_grievance_document_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Attachment */
  $handler->display->display_options['fields']['field_grievance_attachments']['id'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['table'] = 'field_data_field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['field'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_attachments']['delta_offset'] = '0';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Uploaded On';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Uploaded By';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Grievance (field_grievance) */
  $handler->display->display_options['arguments']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['arguments']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_attachment' => 'grievance_attachment',
  );
  /* Filter criterion: Field: Grievance Document Type (field_grievance_document_type) */
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['id'] = 'field_grievance_document_type_tid';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['table'] = 'field_data_field_grievance_document_type';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['field'] = 'field_grievance_document_type_tid';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['operator_id'] = 'field_grievance_document_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['label'] = 'Attachment Type';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['operator'] = 'field_grievance_document_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['identifier'] = 'field_grievance_document_type_tid';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['vocabulary'] = 'grievance_document_types';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['hierarchy'] = 1;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['grievance_attachments'] = array(
    t('Master'),
    t('Grievance Attachments'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('author'),
    t('Type'),
    t('Attachment'),
    t('Uploaded On'),
    t('Uploaded By'),
    t('Edit'),
    t('All'),
    t('Attachment Type'),
    t('Block'),
  );
  $export['grievance_attachments'] = $view;

  $view = new view();
  $view->name = 'grievance_basic_pages';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Basic Pages';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Basic Pages';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_basic_page' => 'grievance_basic_page',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/basic-pages';
  $translatables['grievance_basic_pages'] = array(
    t('Master'),
    t('Grievance Basic Pages'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Title'),
    t('Edit'),
    t('Path'),
    t('Page'),
  );
  $export['grievance_basic_pages'] = $view;

  $view = new view();
  $view->name = 'grievance_field_overrides';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Field Overrides';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Field Overrides';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'The list below shows "field overrides".  Sometimes the grievance system uses a field label which is not appropriate to your installation.  For example, the grievance software refers to the union staff person assigned to a grievance as the "Organizer", whereas a particular installation might refer to them as a "Steward".  The list below can be used to override field labels or descriptions.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-field-overrides">Add an override</a>]
[<a href="/admin/config/grievances/fieldoverrides/apply">Apply Overrides</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Entity Type */
  $handler->display->display_options['fields']['field_grievance_entity_type']['id'] = 'field_grievance_entity_type';
  $handler->display->display_options['fields']['field_grievance_entity_type']['table'] = 'field_data_field_grievance_entity_type';
  $handler->display->display_options['fields']['field_grievance_entity_type']['field'] = 'field_grievance_entity_type';
  /* Field: Content: Bundle */
  $handler->display->display_options['fields']['field_grievance_bundle']['id'] = 'field_grievance_bundle';
  $handler->display->display_options['fields']['field_grievance_bundle']['table'] = 'field_data_field_grievance_bundle';
  $handler->display->display_options['fields']['field_grievance_bundle']['field'] = 'field_grievance_bundle';
  /* Field: Content: Field Name */
  $handler->display->display_options['fields']['field_grievance_field_name']['id'] = 'field_grievance_field_name';
  $handler->display->display_options['fields']['field_grievance_field_name']['table'] = 'field_data_field_grievance_field_name';
  $handler->display->display_options['fields']['field_grievance_field_name']['field'] = 'field_grievance_field_name';
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Description */
  $handler->display->display_options['fields']['field_grievance_description']['id'] = 'field_grievance_description';
  $handler->display->display_options['fields']['field_grievance_description']['table'] = 'field_data_field_grievance_description';
  $handler->display->display_options['fields']['field_grievance_description']['field'] = 'field_grievance_description';
  /* Field: Content: Default Value */
  $handler->display->display_options['fields']['field_grievance_default']['id'] = 'field_grievance_default';
  $handler->display->display_options['fields']['field_grievance_default']['table'] = 'field_data_field_grievance_default';
  $handler->display->display_options['fields']['field_grievance_default']['field'] = 'field_grievance_default';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_field_overrides' => 'grievance_field_overrides',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/config/grievances/fieldoverrides';
  $translatables['grievance_field_overrides'] = array(
    t('Master'),
    t('Grievance Field Overrides'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('The list below shows "field overrides".  Sometimes the grievance system uses a field label which is not appropriate to your installation.  For example, the grievance software refers to the union staff person assigned to a grievance as the "Organizer", whereas a particular installation might refer to them as a "Steward".  The list below can be used to override field labels or descriptions.'),
    t('[<a href="/node/add/grievance-field-overrides">Add an override</a>]
[<a href="/admin/config/grievances/fieldoverrides/apply">Apply Overrides</a>]'),
    t('Entity Type'),
    t('Bundle'),
    t('Field Name'),
    t('Label'),
    t('Description'),
    t('Default Value'),
    t('Edit'),
    t('Block'),
    t('Page'),
  );
  $export['grievance_field_overrides'] = $view;

  $view = new view();
  $view->name = 'grievance_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Log';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Grievance Log Type */
  $handler->display->display_options['fields']['field_grievance_log_type']['id'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['table'] = 'field_data_field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['field'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['label'] = 'Entry Type';
  $handler->display->display_options['fields']['field_grievance_log_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Next Step in the Grievance Procedure for the Union */
  $handler->display->display_options['fields']['field_grievance_alert']['id'] = 'field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['table'] = 'field_data_field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['field'] = 'field_grievance_alert';
  /* Field: Content: Next Deadline Date */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Grievance (field_grievance) */
  $handler->display->display_options['arguments']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['arguments']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_log' => 'grievance_log',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['grievance_log'] = array(
    t('Master'),
    t('Grievance Log'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('author'),
    t('Post date'),
    t('Entry Type'),
    t('Title'),
    t('Author'),
    t('Status'),
    t('Next Step in the Grievance Procedure for the Union'),
    t('Next Deadline Date'),
    t('All'),
    t('Block'),
  );
  $export['grievance_log'] = $view;

  $view = new view();
  $view->name = 'grievance_settlement_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Settlement Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Settlement Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10000';
  $handler->display->display_options['style_plugin'] = 'views_calc';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_grievance' => 'field_grievance',
    'field_grievance_amt' => 'field_grievance_amt',
    'field_grievance_amt_rcvd' => 'field_grievance_amt_rcvd',
    'field_grievance_date_1' => 'field_grievance_date_1',
    'field_grievance_comments' => 'field_grievance_comments',
    'field_grievance_checkno' => 'field_grievance_checkno',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_grievance' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 0,
      'calc' => array(),
    ),
    'field_grievance_amt' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 1,
      'calc' => array(
        'SUM' => 'SUM',
      ),
    ),
    'field_grievance_amt_rcvd' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 1,
      'calc' => array(
        'SUM' => 'SUM',
      ),
    ),
    'field_grievance_date_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 0,
      'calc' => array(),
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 0,
      'calc' => array(),
    ),
    'field_grievance_checkno' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_calc' => 0,
      'calc' => array(),
    ),
  );
  $handler->display->display_options['style_options']['detailed_values'] = '0';
  $handler->display->display_options['style_options']['precision'] = '0';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['relationships']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['relationships']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  $handler->display->display_options['relationships']['field_grievance_target_id']['label'] = 'Grievance';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['label'] = 'Shop';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['id'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['field'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['label'] = 'Company';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Company';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['fields']['title']['label'] = 'Shop';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Grievance */
  $handler->display->display_options['fields']['field_grievance']['id'] = 'field_grievance';
  $handler->display->display_options['fields']['field_grievance']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['fields']['field_grievance']['field'] = 'field_grievance';
  $handler->display->display_options['fields']['field_grievance']['settings'] = array(
    'link' => 1,
  );
  /* Field: Content: Settlement Amount */
  $handler->display->display_options['fields']['field_grievance_amt']['id'] = 'field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['table'] = 'field_data_field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['field'] = 'field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount Received */
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['id'] = 'field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['table'] = 'field_data_field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['field'] = 'field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Date Received */
  $handler->display->display_options['fields']['field_grievance_date_1']['id'] = 'field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['field'] = 'field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  /* Field: Content: Check Number */
  $handler->display->display_options['fields']['field_grievance_checkno']['id'] = 'field_grievance_checkno';
  $handler->display->display_options['fields']['field_grievance_checkno']['table'] = 'field_data_field_grievance_checkno';
  $handler->display->display_options['fields']['field_grievance_checkno']['field'] = 'field_grievance_checkno';
  /* Sort criterion: Content: Grievance (field_grievance) */
  $handler->display->display_options['sorts']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['sorts']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['sorts']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_settlement' => 'grievance_settlement',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Shop';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Company';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date Received';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/report/settlement';
  $translatables['grievance_settlement_report'] = array(
    t('Master'),
    t('Grievance Settlement Report'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Grievance'),
    t('Shop'),
    t('Company'),
    t('Settlement Amount'),
    t('Amount Received'),
    t('Date Received'),
    t('Comments'),
    t('Check Number'),
    t('Page'),
  );
  $export['grievance_settlement_report'] = $view;

  $view = new view();
  $view->name = 'grievance_settlements';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Settlements';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Settlements';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_grievance_document_type' => 'field_grievance_document_type',
    'field_grievance_attachments' => 'field_grievance_attachments',
    'created' => 'created',
    'name' => 'name',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_grievance_document_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_attachments' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Settlement Amount */
  $handler->display->display_options['fields']['field_grievance_amt']['id'] = 'field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['table'] = 'field_data_field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['field'] = 'field_grievance_amt';
  $handler->display->display_options['fields']['field_grievance_amt']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount Received */
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['id'] = 'field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['table'] = 'field_data_field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['field'] = 'field_grievance_amt_rcvd';
  $handler->display->display_options['fields']['field_grievance_amt_rcvd']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Check Number */
  $handler->display->display_options['fields']['field_grievance_checkno']['id'] = 'field_grievance_checkno';
  $handler->display->display_options['fields']['field_grievance_checkno']['table'] = 'field_data_field_grievance_checkno';
  $handler->display->display_options['fields']['field_grievance_checkno']['field'] = 'field_grievance_checkno';
  /* Field: Content: Date Received */
  $handler->display->display_options['fields']['field_grievance_date_1']['id'] = 'field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['field'] = 'field_grievance_date_1';
  $handler->display->display_options['fields']['field_grievance_date_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Grievance (field_grievance) */
  $handler->display->display_options['arguments']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['arguments']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_settlement' => 'grievance_settlement',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['grievance_settlements'] = array(
    t('Master'),
    t('Settlements'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('author'),
    t('Settlement Amount'),
    t('Amount Received'),
    t('Check Number'),
    t('Date Received'),
    t('Comments'),
    t('Edit'),
    t('All'),
    t('Block'),
  );
  $export['grievance_settlements'] = $view;

  $view = new view();
  $view->name = 'grievance_shop_contacts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Shop Contacts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Shop Contacts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-shop-contact">Add a Contact</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Name */
  $handler->display->display_options['fields']['field_grievance_co_name']['id'] = 'field_grievance_co_name';
  $handler->display->display_options['fields']['field_grievance_co_name']['table'] = 'field_data_field_grievance_co_name';
  $handler->display->display_options['fields']['field_grievance_co_name']['field'] = 'field_grievance_co_name';
  /* Field: Content: Role */
  $handler->display->display_options['fields']['field_grievance_co_role']['id'] = 'field_grievance_co_role';
  $handler->display->display_options['fields']['field_grievance_co_role']['table'] = 'field_data_field_grievance_co_role';
  $handler->display->display_options['fields']['field_grievance_co_role']['field'] = 'field_grievance_co_role';
  /* Field: Content: Company */
  $handler->display->display_options['fields']['field_grievance_company']['id'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['field'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['settings'] = array(
    'link' => 1,
  );
  /* Field: Field: Shop */
  $handler->display->display_options['fields']['field_grievance_shops']['id'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['field'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['label'] = 'Shops';
  $handler->display->display_options['fields']['field_grievance_shops']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_grievance_shops']['delta_offset'] = '0';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_shop_contact' => 'grievance_shop_contact',
  );
  /* Filter criterion: Content: Name (field_grievance_co_name) */
  $handler->display->display_options['filters']['field_grievance_co_name_value']['id'] = 'field_grievance_co_name_value';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['table'] = 'field_data_field_grievance_co_name';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['field'] = 'field_grievance_co_name_value';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_co_name_value']['expose']['operator_id'] = 'field_grievance_co_name_value_op';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['expose']['operator'] = 'field_grievance_co_name_value_op';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['expose']['identifier'] = 'field_grievance_co_name_value';
  $handler->display->display_options['filters']['field_grievance_co_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Role (field_grievance_co_role) */
  $handler->display->display_options['filters']['field_grievance_co_role_value']['id'] = 'field_grievance_co_role_value';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['table'] = 'field_data_field_grievance_co_role';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['field'] = 'field_grievance_co_role_value';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_co_role_value']['expose']['operator_id'] = 'field_grievance_co_role_value_op';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['expose']['label'] = 'Role';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['expose']['operator'] = 'field_grievance_co_role_value_op';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['expose']['identifier'] = 'field_grievance_co_role_value';
  $handler->display->display_options['filters']['field_grievance_co_role_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Statuses (field_grievance_statuses) */
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['id'] = 'field_grievance_statuses_tid';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['table'] = 'field_data_field_grievance_statuses';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['field'] = 'field_grievance_statuses_tid';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['expose']['operator_id'] = 'field_grievance_statuses_tid_op';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['expose']['operator'] = 'field_grievance_statuses_tid_op';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['expose']['identifier'] = 'field_grievance_statuses_tid';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['vocabulary'] = 'grievance_status';
  $handler->display->display_options['filters']['field_grievance_statuses_tid']['hierarchy'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/shops/contacts';

  /* Display: By Shop */
  $handler = $view->new_display('block', 'By Shop', 'block_1');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field: Shop (field_grievance_shops) */
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['id'] = 'field_grievance_shops_target_id';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['field'] = 'field_grievance_shops_target_id';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_shops_target_id']['summary_options']['items_per_page'] = '25';

  /* Display: By Company */
  $handler = $view->new_display('block', 'By Company', 'block_2');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Company (field_grievance_company) */
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['id'] = 'field_grievance_company_target_id';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['field'] = 'field_grievance_company_target_id';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_company_target_id']['summary_options']['items_per_page'] = '25';
  $translatables['grievance_shop_contacts'] = array(
    t('Master'),
    t('Grievance Shop Contacts'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('[<a href="/node/add/grievance-shop-contact">Add a Contact</a>]'),
    t('Name'),
    t('Role'),
    t('Company'),
    t('Shops'),
    t('Edit'),
    t('Status'),
    t('Page'),
    t('By Shop'),
    t('All'),
    t('By Company'),
  );
  $export['grievance_shop_contacts'] = $view;

  $view = new view();
  $view->name = 'grievance_shops';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Shops';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Shops';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'views_aggregator';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'field_grievance_contract' => 'field_grievance_contract',
    'field_grievance_co_name' => 'field_grievance_co_name',
    'name' => 'name',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 1,
      'aggr' => array(
        'views_aggregator_group_and_compress' => 'views_aggregator_group_and_compress',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'views_bulk_operations' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_first' => 'views_aggregator_first',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_first' => 'views_aggregator_first',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'field_grievance_contract' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_first' => 'views_aggregator_first',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'field_grievance_co_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_first' => 'views_aggregator_first',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ',',
      'empty_column' => 0,
      'has_aggr' => 1,
      'aggr' => array(
        'views_aggregator_enumerate' => 'views_aggregator_enumerate',
      ),
      'aggr_par' => ', ',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
    'edit_node' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_first' => 'views_aggregator_first',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 0,
      'aggr_column' => 'views_aggregator_sum',
      'aggr_par_column' => '',
    ),
  );
  $handler->display->display_options['style_options']['column_aggregation']['totals_per_page'] = '1';
  $handler->display->display_options['style_options']['column_aggregation']['totals_row_position'] = array(
    1 => 0,
    2 => '2',
  );
  $handler->display->display_options['style_options']['column_aggregation']['precision'] = '2';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-shop">Add a shop</a>]
[<a href="/node/add/grievance-company">Add a company</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_grievance_shops_user']['id'] = 'reverse_field_grievance_shops_user';
  $handler->display->display_options['relationships']['reverse_field_grievance_shops_user']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_grievance_shops_user']['field'] = 'reverse_field_grievance_shops_user';
  $handler->display->display_options['relationships']['reverse_field_grievance_shops_user']['label'] = 'User';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['id'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['field'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['label'] = 'Company';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
  );
  /* Field: Content: Company */
  $handler->display->display_options['fields']['field_grievance_company']['id'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['field'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company']['settings'] = array(
    'link' => 1,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Shop';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Contract */
  $handler->display->display_options['fields']['field_grievance_contract']['id'] = 'field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['table'] = 'field_data_field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['field'] = 'field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_grievance_contract']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_contract']['group_column'] = 'entity_id';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'reverse_field_grievance_shops_user';
  $handler->display->display_options['fields']['name']['label'] = 'Steward';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = TRUE;
  /* Field: Content: Company */
  $handler->display->display_options['fields']['field_grievance_company_1']['id'] = 'field_grievance_company_1';
  $handler->display->display_options['fields']['field_grievance_company_1']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company_1']['field'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_company_1']['type'] = 'entityreference_entity_id';
  $handler->display->display_options['fields']['field_grievance_company_1']['settings'] = array(
    'link' => 0,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit Company';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[field_grievance_company_1]/edit?destination=grievance/admin/shops">edit company</a>';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Edit Shop';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<a href="/node/[nid]/edit?destination=grievance/admin/shops">edit shop</a>';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_shop' => 'grievance_shop',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Shop Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Company Name';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/shops';
  $translatables['grievance_shops'] = array(
    t('Master'),
    t('Grievance Shops'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('[<a href="/node/add/grievance-shop">Add a shop</a>]
[<a href="/node/add/grievance-company">Add a company</a>]'),
    t('User'),
    t('Company'),
    t('Nid'),
    t('Content'),
    t('- Choose an operation -'),
    t('Shop'),
    t('Contract'),
    t('Steward'),
    t('Edit Company'),
    t('<a href="/node/[field_grievance_company_1]/edit?destination=grievance/admin/shops">edit company</a>'),
    t('Edit Shop'),
    t('<a href="/node/[nid]/edit?destination=grievance/admin/shops">edit shop</a>'),
    t('Shop Name'),
    t('Company Name'),
    t('Page'),
  );
  $export['grievance_shops'] = $view;

  $view = new view();
  $view->name = 'grievance_timeline';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Timeline';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'field_grievance_status' => 'field_grievance_status',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_grievance_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Status (field_grievance_status) */
  $handler->display->display_options['relationships']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: MIN(Content: Post date) */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['group_type'] = 'min';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'n/j/Y';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Sort criterion: MIN(Content: Post date) */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['group_type'] = 'min';
  /* Contextual filter: Content: Grievance (field_grievance) */
  $handler->display->display_options['arguments']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['arguments']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance_log' => 'grievance_log',
  );
  /* Filter criterion: Content: Grievance Log Type (field_grievance_log_type) */
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['id'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['table'] = 'field_data_field_grievance_log_type';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['field'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['value'] = array(
    44 => '44',
  );
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['vocabulary'] = 'grievance_log_types';
  /* Filter criterion: Taxonomy term: Show on Timeline? (field_grievance_timeline_show) */
  $handler->display->display_options['filters']['field_grievance_timeline_show_value']['id'] = 'field_grievance_timeline_show_value';
  $handler->display->display_options['filters']['field_grievance_timeline_show_value']['table'] = 'field_data_field_grievance_timeline_show';
  $handler->display->display_options['filters']['field_grievance_timeline_show_value']['field'] = 'field_grievance_timeline_show_value';
  $handler->display->display_options['filters']['field_grievance_timeline_show_value']['relationship'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_timeline_show_value']['value'] = array(
    'Yes' => 'Yes',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $translatables['grievance_timeline'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('term from field_grievance_status'),
    t('Status'),
    t('Post date'),
    t('All'),
    t('Block'),
  );
  $export['grievance_timeline'] = $view;

  $view = new view();
  $view->name = 'grievances';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievances';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievances';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_type' => 'field_grievance_type',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_alert' => 'field_grievance_alert',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'nothing_1' => 'nothing_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_shop' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_alert' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_alert_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance">File a new grievance</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_lead_target_id']['id'] = 'field_grievance_rep_lead_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_lead_target_id']['table'] = 'field_data_field_grievance_rep_lead';
  $handler->display->display_options['relationships']['field_grievance_rep_lead_target_id']['field'] = 'field_grievance_rep_lead_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_lead_target_id']['label'] = 'Lead';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['id'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['table'] = 'field_data_field_grievance_rep_organizer';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['field'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['label'] = 'Organizer';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_manager_target_id']['id'] = 'field_grievance_rep_manager_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_manager_target_id']['table'] = 'field_data_field_grievance_rep_manager';
  $handler->display->display_options['relationships']['field_grievance_rep_manager_target_id']['field'] = 'field_grievance_rep_manager_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_manager_target_id']['label'] = 'Manager';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_watching_target_id']['id'] = 'field_grievance_rep_watching_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_watching_target_id']['table'] = 'field_data_field_grievance_rep_watching';
  $handler->display->display_options['relationships']['field_grievance_rep_watching_target_id']['field'] = 'field_grievance_rep_watching_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_watching_target_id']['label'] = 'Watching';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_filed_target_id']['id'] = 'field_grievance_rep_filed_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_filed_target_id']['table'] = 'field_data_field_grievance_rep_filed';
  $handler->display->display_options['relationships']['field_grievance_rep_filed_target_id']['field'] = 'field_grievance_rep_filed_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_filed_target_id']['label'] = 'Filed';
  /* Relationship: Content: Taxonomy terms on node */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['label'] = 'Status';
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'grievance_status' => 'grievance_status',
    'grievance_log_types' => 0,
    'grievance_types' => 0,
    'tags' => 0,
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['label'] = 'Shop';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['id'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['field'] = 'field_grievance_company_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_company_target_id']['label'] = 'Company';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Shop */
  $handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  /* Field: Content: Filed By */
  $handler->display->display_options['fields']['field_grievance_rep_filed']['id'] = 'field_grievance_rep_filed';
  $handler->display->display_options['fields']['field_grievance_rep_filed']['table'] = 'field_data_field_grievance_rep_filed';
  $handler->display->display_options['fields']['field_grievance_rep_filed']['field'] = 'field_grievance_rep_filed';
  $handler->display->display_options['fields']['field_grievance_rep_filed']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_grievance_rep_filed']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_grievance_rep_filed']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_grievance_rep_filed']['delta_offset'] = '0';
  /* Field: Content: Statement of Grievance */
  $handler->display->display_options['fields']['field_grievance_type']['id'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['table'] = 'field_data_field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['field'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_type']['delta_offset'] = '0';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Next Step in the Grievance Procedure for the Union */
  $handler->display->display_options['fields']['field_grievance_alert']['id'] = 'field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['table'] = 'field_data_field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['field'] = 'field_grievance_alert';
  /* Field: Content: Next Deadline Date */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/gform">form</a>]&nbsp;[<a href="/node/[nid]/notification">notify</a>]
';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance' => 'grievance',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: First Name (field_grievance_first_name) */
  $handler->display->display_options['filters']['field_grievance_first_name_value']['id'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['field'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_first_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['operator_id'] = 'field_grievance_first_name_value_op';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['label'] = 'First Name';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['operator'] = 'field_grievance_first_name_value_op';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['identifier'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Last Name (field_grievance_last_name) */
  $handler->display->display_options['filters']['field_grievance_last_name_value']['id'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['field'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_last_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['operator_id'] = 'field_grievance_last_name_value_op';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['label'] = 'Last Name';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['operator'] = 'field_grievance_last_name_value_op';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['identifier'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Status (field_grievance_status) */
  $handler->display->display_options['filters']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['filters']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator_id'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['identifier'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_status_tid']['vocabulary'] = 'grievance_status';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'field_grievance_rep_filed_target_id';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Filed By';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['group'] = 1;
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Organizer';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Shop';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Company Name';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: My Grievances */
  $handler = $view->new_display('page', 'My Grievances', 'page_1');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
    5 => '5',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'The list below includes only <strong>unresolved</strong> grievances which were filed by the <strong>current user</strong> (or for which the current user is the organizer, lead organizer, manager, or is watching, or filed by another organizer at your shop).';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no unreseloved grievances which are assigned to you, or which you are watching.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Next Deadline Date */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Next Deadline Date */
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['id'] = 'field_grievance_alert_date_1';
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['label'] = 'Time Remaining';
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['type'] = 'format_interval';
  $handler->display->display_options['fields']['field_grievance_alert_date_1']['settings'] = array(
    'interval' => '2',
    'interval_display' => 'inverse time span',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Next Deadline';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_alert_date]<br>([field_grievance_alert_date_1])';
  /* Field: Content: Next Step in the Grievance Procedure for the Union */
  $handler->display->display_options['fields']['field_grievance_alert']['id'] = 'field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['table'] = 'field_data_field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['field'] = 'field_grievance_alert';
  $handler->display->display_options['fields']['field_grievance_alert']['label'] = 'Next Step';
  $handler->display->display_options['fields']['field_grievance_alert']['exclude'] = TRUE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Status';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[field_grievance_status]<br>
Union must: [field_grievance_alert]';
  /* Field: Content: Shop */
  $handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['exclude'] = TRUE;
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]
';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/gform">form</a>]&nbsp;[<a href="/node/[nid]/notification">notify</a>]';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Next Deadline Date (field_grievance_alert_date) */
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'grievance' => 'grievance',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_grievance_open_value']['group'] = 1;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_4']['id'] = 'uid_current_4';
  $handler->display->display_options['filters']['uid_current_4']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_4']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_4']['relationship'] = 'field_grievance_rep_filed_target_id';
  $handler->display->display_options['filters']['uid_current_4']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_4']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_1']['id'] = 'uid_current_1';
  $handler->display->display_options['filters']['uid_current_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_1']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_1']['relationship'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['filters']['uid_current_1']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_1']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'field_grievance_rep_lead_target_id';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_3']['id'] = 'uid_current_3';
  $handler->display->display_options['filters']['uid_current_3']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_3']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_3']['relationship'] = 'field_grievance_rep_watching_target_id';
  $handler->display->display_options['filters']['uid_current_3']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_3']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_2']['id'] = 'uid_current_2';
  $handler->display->display_options['filters']['uid_current_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_2']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_2']['relationship'] = 'field_grievance_rep_manager_target_id';
  $handler->display->display_options['filters']['uid_current_2']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_2']['group'] = 2;
  /* Filter criterion: Content: Shop (field_grievance_shop) */
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['value']['value'] = '999';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['group'] = 2;
  $handler->display->display_options['path'] = 'grievances/mine';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'My Unresolved Grievances';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Grievances';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The list below shows all grievances on the system.  Use the form to filer by grievant name, status, etc.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No grievances match your search criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['path'] = 'grievances/all';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'All Grievances';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['grievances'] = array(
    t('Master'),
    t('Grievances'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('[<a href="/node/add/grievance">File a new grievance</a>]'),
    t('Lead'),
    t('Organizer'),
    t('Manager'),
    t('Watching'),
    t('Filed'),
    t('Status'),
    t('Shop'),
    t('Company'),
    t('Nid'),
    t('First Name'),
    t('Last Name'),
    t('Filed By'),
    t('Statement of Grievance'),
    t('Next Step in the Grievance Procedure for the Union'),
    t('Next Deadline Date'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/gform">form</a>]&nbsp;[<a href="/node/[nid]/notification">notify</a>]
'),
    t('Company Name'),
    t('My Grievances'),
    t('The list below includes only <strong>unresolved</strong> grievances which were filed by the <strong>current user</strong> (or for which the current user is the organizer, lead organizer, manager, or is watching, or filed by another organizer at your shop).'),
    t('There are no unreseloved grievances which are assigned to you, or which you are watching.'),
    t('Time Remaining'),
    t('Next Deadline'),
    t('[field_grievance_alert_date]<br>([field_grievance_alert_date_1])'),
    t('Next Step'),
    t('[field_grievance_status]<br>
Union must: [field_grievance_alert]'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]
'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/gform">form</a>]&nbsp;[<a href="/node/[nid]/notification">notify</a>]'),
    t('All Grievances'),
    t('The list below shows all grievances on the system.  Use the form to filer by grievant name, status, etc.'),
    t('No grievances match your search criteria.'),
  );
  $export['grievances'] = $view;

  return $export;
}
