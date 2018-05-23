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
  $view->name = 'grievance_alert_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Grievance Alert Types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Alert Types';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/admin/structure/taxonomy/grievance_alert_types/add?destination=grievance/admin/alert-types">Add an Alert Type</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Taxonomy term: Actor */
  $handler->display->display_options['fields']['field_grievance_actor']['id'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['table'] = 'field_data_field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['field'] = 'field_grievance_actor';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Action';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  $handler->display->display_options['fields']['description']['alter']['nl2br'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['strip_tags'] = TRUE;
  /* Field: Taxonomy term: Term edit link */
  $handler->display->display_options['fields']['edit_term']['id'] = 'edit_term';
  $handler->display->display_options['fields']['edit_term']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['edit_term']['field'] = 'edit_term';
  $handler->display->display_options['fields']['edit_term']['label'] = 'Edit';
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'grievance_alert_types' => 'grievance_alert_types',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/alert-types';
  $translatables['grievance_alert_types'] = array(
    t('Master'),
    t('Grievance Alert Types'),
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
    t('[<a href="/admin/structure/taxonomy/grievance_alert_types/add?destination=grievance/admin/alert-types">Add an Alert Type</a>]'),
    t('Actor'),
    t('Action'),
    t('Description'),
    t('Edit'),
    t('Page'),
  );
  $export['grievance_alert_types'] = $view;

  $view = new view();
  $view->name = 'grievance_attachment_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Grievance Attachment Types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Sort criterion: Taxonomy term: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'grievance_document_types' => 'grievance_document_types',
  );
  /* Filter criterion: Taxonomy term: Attachment? (field_grievance_can_attach) */
  $handler->display->display_options['filters']['field_grievance_can_attach_value']['id'] = 'field_grievance_can_attach_value';
  $handler->display->display_options['filters']['field_grievance_can_attach_value']['table'] = 'field_data_field_grievance_can_attach';
  $handler->display->display_options['filters']['field_grievance_can_attach_value']['field'] = 'field_grievance_can_attach_value';
  $handler->display->display_options['filters']['field_grievance_can_attach_value']['value'] = array(
    'Yes' => 'Yes',
  );

  /* Display: Term Reference */
  $handler = $view->new_display('term_reference', 'Term Reference', 'term_reference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'term_reference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'term_reference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['grievance_attachment_types'] = array(
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
    t('Term Reference'),
  );
  $export['grievance_attachment_types'] = $view;

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
  $handler->display->display_options['access']['perm'] = 'grievance user';
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
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['label'] = 'Description';
  /* Field: Content: Attachments */
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
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['label'] = 'Show only attachments of type:';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['operator'] = 'field_grievance_document_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['identifier'] = 'field_grievance_document_type_tid';
  $handler->display->display_options['filters']['field_grievance_document_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
    t('Description'),
    t('Attachments'),
    t('Uploaded On'),
    t('Uploaded By'),
    t('Edit'),
    t('All'),
    t('Show only attachments of type:'),
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
  $view->name = 'grievance_chapters';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Chapters';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Chapters';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
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
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-chapter?destination=grievance/admin/chapters">Add a chapter</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_grievance_address']['id'] = 'field_grievance_address';
  $handler->display->display_options['fields']['field_grievance_address']['table'] = 'field_data_field_grievance_address';
  $handler->display->display_options['fields']['field_grievance_address']['field'] = 'field_grievance_address';
  /* Field: Content: City */
  $handler->display->display_options['fields']['field_grievance_city']['id'] = 'field_grievance_city';
  $handler->display->display_options['fields']['field_grievance_city']['table'] = 'field_data_field_grievance_city';
  $handler->display->display_options['fields']['field_grievance_city']['field'] = 'field_grievance_city';
  /* Field: Content: State or Province */
  $handler->display->display_options['fields']['field_grievance_state']['id'] = 'field_grievance_state';
  $handler->display->display_options['fields']['field_grievance_state']['table'] = 'field_data_field_grievance_state';
  $handler->display->display_options['fields']['field_grievance_state']['field'] = 'field_grievance_state';
  /* Field: Content: Postal Code */
  $handler->display->display_options['fields']['field_grievance_zip']['id'] = 'field_grievance_zip';
  $handler->display->display_options['fields']['field_grievance_zip']['table'] = 'field_data_field_grievance_zip';
  $handler->display->display_options['fields']['field_grievance_zip']['field'] = 'field_grievance_zip';
  /* Field: Field: Phone */
  $handler->display->display_options['fields']['field_grievance_phone']['id'] = 'field_grievance_phone';
  $handler->display->display_options['fields']['field_grievance_phone']['table'] = 'field_data_field_grievance_phone';
  $handler->display->display_options['fields']['field_grievance_phone']['field'] = 'field_grievance_phone';
  /* Field: Content: Fax */
  $handler->display->display_options['fields']['field_grievance_co_fax']['id'] = 'field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['table'] = 'field_data_field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['field'] = 'field_grievance_co_fax';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
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
    'grievance_chapter' => 'grievance_chapter',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/chapters';
  $translatables['grievance_chapters'] = array(
    t('Master'),
    t('Grievance Chapters'),
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
    t('[<a href="/node/add/grievance-chapter?destination=grievance/admin/chapters">Add a chapter</a>]'),
    t('Title'),
    t('Address'),
    t('City'),
    t('State or Province'),
    t('Postal Code'),
    t('Phone'),
    t('Fax'),
    t('Edit'),
    t('Page'),
  );
  $export['grievance_chapters'] = $view;

  $view = new view();
  $view->name = 'grievance_classifications';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Grievance Classifications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Sort criterion: Taxonomy term: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'grievance_job_classification' => 'grievance_job_classification',
  );

  /* Display: Term Reference */
  $handler = $view->new_display('term_reference', 'Term Reference', 'term_reference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'term_reference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'term_reference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['grievance_classifications'] = array(
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
    t('Term Reference'),
  );
  $export['grievance_classifications'] = $view;

  $view = new view();
  $view->name = 'grievance_companies';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Companies';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Companies';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-company">Add a company</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'ID';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[<a href="/node/[nid]/edit">Edit</a>] [<a href="/node/[nid]/contacts">Contacts</a>]';
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
    'grievance_company' => 'grievance_company',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/shops/companies';
  $translatables['grievance_companies'] = array(
    t('Master'),
    t('Companies'),
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
    t('[<a href="/node/add/grievance-company">Add a company</a>]'),
    t('ID'),
    t('Title'),
    t('Actions'),
    t('[<a href="/node/[nid]/edit">Edit</a>] [<a href="/node/[nid]/contacts">Contacts</a>]'),
    t('Page'),
  );
  $export['grievance_companies'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_clause_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Section Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Article Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'Contract
Article
Section',
    'group-title_2' => array(
      'group' => '0',
      'filter_name' => 'title_2',
      'weight' => '-10',
    ),
    'group-field_grievance_cont_tplt_tags_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_cont_tplt_tags_tid',
      'weight' => '-9',
    ),
    'group-field_grievance_section_number_value' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_section_number_value',
      'weight' => '-7',
    ),
    'group-field_grievance_cont_sec_tags_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_cont_sec_tags_tid',
      'weight' => '-6',
    ),
    'group-title' => array(
      'group' => '1',
      'filter_name' => 'title',
      'weight' => '-8',
    ),
    'group-field_grievance_label_value' => array(
      'group' => '2',
      'filter_name' => 'field_grievance_label_value',
      'weight' => '-4',
    ),
    'group-title_1' => array(
      'group' => '2',
      'filter_name' => 'title_1',
      'weight' => '-5',
    ),
    'group-field_grievanct_cont_clse_tags_tid' => array(
      'group' => '2',
      'filter_name' => 'field_grievanct_cont_clse_tags_tid',
      'weight' => '-3',
    ),
  );
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title_2' => 'title_2',
    'field_grievance_cont_tplt_tags' => 'field_grievance_cont_tplt_tags',
    'field_grievance_section_number' => 'field_grievance_section_number',
    'title_1' => 'title_1',
    'field_grievance_cont_sec_tags' => 'field_grievance_cont_sec_tags',
    'field_grievance_label' => 'field_grievance_label',
    'title' => 'title',
    'field_grievanct_cont_clse_tags' => 'field_grievanct_cont_clse_tags',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_cont_tplt_tags' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_section_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_cont_sec_tags' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_label' => array(
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
    'field_grievanct_cont_clse_tags' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['id'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['field'] = 'field_grievance_contract_section_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['relationships']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['relationships']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['relationships']['field_grievance_contract_tplt_target_id']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_2']['id'] = 'title_2';
  $handler->display->display_options['fields']['title_2']['table'] = 'node';
  $handler->display->display_options['fields']['title_2']['field'] = 'title';
  $handler->display->display_options['fields']['title_2']['relationship'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['fields']['title_2']['label'] = 'Contract Template';
  /* Field: Content: Template Tags */
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['id'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['table'] = 'field_data_field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['field'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['relationship'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['label'] = 'Contract Tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['delta_offset'] = '0';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Article Title';
  /* Field: Content: Article Tags */
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['id'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['table'] = 'field_data_field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['field'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['delta_offset'] = '0';
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Section Title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Section Tags */
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['id'] = 'field_grievanct_cont_clse_tags';
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['table'] = 'field_data_field_grievanct_cont_clse_tags';
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['field'] = 'field_grievanct_cont_clse_tags';
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievanct_cont_clse_tags']['delta_offset'] = '0';
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
    'grievance_contract_clause' => 'grievance_contract_clause',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_2']['id'] = 'title_2';
  $handler->display->display_options['filters']['title_2']['table'] = 'node';
  $handler->display->display_options['filters']['title_2']['field'] = 'title';
  $handler->display->display_options['filters']['title_2']['relationship'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['filters']['title_2']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_2']['group'] = 1;
  $handler->display->display_options['filters']['title_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_2']['expose']['operator_id'] = 'title_2_op';
  $handler->display->display_options['filters']['title_2']['expose']['label'] = 'Contract Template';
  $handler->display->display_options['filters']['title_2']['expose']['operator'] = 'title_2_op';
  $handler->display->display_options['filters']['title_2']['expose']['identifier'] = 'title_2';
  $handler->display->display_options['filters']['title_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Template Tags (field_grievance_cont_tplt_tags) */
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['id'] = 'field_grievance_cont_tplt_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['table'] = 'field_data_field_grievance_cont_tplt_tags';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['field'] = 'field_grievance_cont_tplt_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['relationship'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['expose']['operator_id'] = 'field_grievance_cont_tplt_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['expose']['label'] = 'Contract Tags';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['expose']['operator'] = 'field_grievance_cont_tplt_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['expose']['identifier'] = 'field_grievance_cont_tplt_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['vocabulary'] = 'grievance_contract_template_tags';
  $handler->display->display_options['filters']['field_grievance_cont_tplt_tags_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Article Number (field_grievance_section_number) */
  $handler->display->display_options['filters']['field_grievance_section_number_value']['id'] = 'field_grievance_section_number_value';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['field'] = 'field_grievance_section_number_value';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_section_number_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_section_number_value']['expose']['operator_id'] = 'field_grievance_section_number_value_op';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['expose']['label'] = 'Article Number';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['expose']['operator'] = 'field_grievance_section_number_value_op';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['expose']['identifier'] = 'field_grievance_section_number_value';
  $handler->display->display_options['filters']['field_grievance_section_number_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Article Tags (field_grievance_cont_sec_tags) */
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['id'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['table'] = 'field_data_field_grievance_cont_sec_tags';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['field'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['operator_id'] = 'field_grievance_cont_sec_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['label'] = 'Article Tags';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['operator'] = 'field_grievance_cont_sec_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['identifier'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['vocabulary'] = 'grievance_contract_section_tags';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Article Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Label (field_grievance_label) */
  $handler->display->display_options['filters']['field_grievance_label_value']['id'] = 'field_grievance_label_value';
  $handler->display->display_options['filters']['field_grievance_label_value']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['filters']['field_grievance_label_value']['field'] = 'field_grievance_label_value';
  $handler->display->display_options['filters']['field_grievance_label_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_label_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_label_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_label_value']['expose']['operator_id'] = 'field_grievance_label_value_op';
  $handler->display->display_options['filters']['field_grievance_label_value']['expose']['label'] = 'Section Number';
  $handler->display->display_options['filters']['field_grievance_label_value']['expose']['operator'] = 'field_grievance_label_value_op';
  $handler->display->display_options['filters']['field_grievance_label_value']['expose']['identifier'] = 'field_grievance_label_value';
  $handler->display->display_options['filters']['field_grievance_label_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Section Title';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Section Tags (field_grievanct_cont_clse_tags) */
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['id'] = 'field_grievanct_cont_clse_tags_tid';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['table'] = 'field_data_field_grievanct_cont_clse_tags';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['field'] = 'field_grievanct_cont_clse_tags_tid';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['expose']['operator_id'] = 'field_grievanct_cont_clse_tags_tid_op';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['expose']['label'] = 'Section Tags';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['expose']['operator'] = 'field_grievanct_cont_clse_tags_tid_op';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['expose']['identifier'] = 'field_grievanct_cont_clse_tags_tid';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['vocabulary'] = 'grievance_contract_clause_tags';
  $handler->display->display_options['filters']['field_grievanct_cont_clse_tags_tid']['hierarchy'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/contract-templates/clause-search';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'grievance/admin/contract-templates/clause-search/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['grievance_contract_clause_search'] = array(
    t('Master'),
    t('Grievance Contract Article Search'),
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
    t('Displaying @start - @end of @total'),
    t('Content entity referenced from field_grievance_contract_section'),
    t('Content entity referenced from field_grievance_contract_tplt'),
    t('Contract Template'),
    t('Contract Tags'),
    t('Article Number'),
    t('Article Title'),
    t('Article Tags'),
    t('Label'),
    t('Section Title'),
    t('Section Tags'),
    t('Section Number'),
    t('Page'),
    t('Data export'),
  );
  $export['grievance_contract_clause_search'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_clauses';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Clauses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Sections';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance contract';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'nothing' => 'nothing',
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
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['id'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['relationships']['field_grievance_contract_section_target_id']['field'] = 'field_grievance_contract_section_target_id';
  /* Field: Draggableviews: Content */
  $handler->display->display_options['fields']['draggableviews']['id'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['table'] = 'node';
  $handler->display->display_options['fields']['draggableviews']['field'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['handler'] = 'draggableviews_handler_fieldapi';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['save_button_label'] = 'Save Order';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['ajax'] = 0;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['draggableviews_handler_fieldapi'] = array(
    'field' => 'field_data_field_grievance_weight:field_grievance_weight_value',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Brief Section';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Full Text */
  $handler->display->display_options['fields']['field_grievance_clause']['id'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['table'] = 'field_data_field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['field'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['exclude'] = TRUE;
  /* Field: Content: Recommended Statement of Grievance */
  $handler->display->display_options['fields']['field_grievance_type']['id'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['table'] = 'field_data_field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['field'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_type']['delta_limit'] = 'all';
  /* Field: Content: Recommended Remedy */
  $handler->display->display_options['fields']['field_grievance_remedy']['id'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['field'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_remedy']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_remedy']['delta_limit'] = 'all';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Weight (field_grievance_weight) */
  $handler->display->display_options['sorts']['field_grievance_weight_value']['id'] = 'field_grievance_weight_value';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['table'] = 'field_data_field_grievance_weight';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['field'] = 'field_grievance_weight_value';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Sort criterion: Content: Article Number (field_grievance_section_number) */
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['id'] = 'field_grievance_section_number_value';
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['field'] = 'field_grievance_section_number_value';
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Sort criterion: Content: Weight (field_grievance_weight) */
  $handler->display->display_options['sorts']['field_grievance_weight_value_1']['id'] = 'field_grievance_weight_value_1';
  $handler->display->display_options['sorts']['field_grievance_weight_value_1']['table'] = 'field_data_field_grievance_weight';
  $handler->display->display_options['sorts']['field_grievance_weight_value_1']['field'] = 'field_grievance_weight_value';
  /* Sort criterion: Content: Label (field_grievance_label) */
  $handler->display->display_options['sorts']['field_grievance_label_value']['id'] = 'field_grievance_label_value';
  $handler->display->display_options['sorts']['field_grievance_label_value']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['sorts']['field_grievance_label_value']['field'] = 'field_grievance_label_value';
  /* Contextual filter: Content: Contract Template (field_grievance_contract_tplt) */
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary_options']['items_per_page'] = '25';
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
    'grievance_contract_clause' => 'grievance_contract_clause',
  );

  /* Display: Entire Contract - Table */
  $handler = $view->new_display('block', 'Entire Contract - Table', 'block');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_section_number' => 'field_grievance_section_number',
    'field_grievance_contract_section' => 'field_grievance_contract_section',
    'field_grievance_label' => 'field_grievance_label',
    'title' => 'title',
    'edit_node' => 'edit_node',
    'fieldset' => 'fieldset',
    'field_grievance_type' => 'field_grievance_type',
    'field_grievance_remedy' => 'field_grievance_remedy',
    'field_grievance_clause' => 'field_grievance_clause',
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
    'field_grievance_section_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_contract_section' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_label' => array(
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
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fieldset' => array(
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
    'field_grievance_remedy' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_clause' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Brief Section';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Full Text */
  $handler->display->display_options['fields']['field_grievance_clause']['id'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['table'] = 'field_data_field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['field'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['exclude'] = TRUE;
  /* Field: Content: Recommended Statement of Grievance */
  $handler->display->display_options['fields']['field_grievance_type']['id'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['table'] = 'field_data_field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['field'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_type']['delta_limit'] = 'all';
  /* Field: Content: Recommended Remedy */
  $handler->display->display_options['fields']['field_grievance_remedy']['id'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['field'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_remedy']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_remedy']['delta_limit'] = 'all';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'field_grievance_label' => 'field_grievance_label',
    'title' => 'title',
    'nid' => 0,
    'field_grievance_section_number' => 0,
    'field_grievance_contract_section' => 0,
    'field_grievance_type' => 0,
    'edit_node' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['label'] = '';
  $handler->display->display_options['fields']['field_grievance_contract_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['label'] = '';
  $handler->display->display_options['fields']['field_grievance_label']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
    'grievance_contract_clause' => 'grievance_contract_clause',
  );

  /* Display: Entire Contract - List */
  $handler = $view->new_display('block', 'Entire Contract - List', 'block_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['fields']['field_grievance_section_number']['type'] = 'number_integer';
  $handler->display->display_options['fields']['field_grievance_section_number']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['label'] = 'Clause Number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Brief Clause';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Full Text */
  $handler->display->display_options['fields']['field_grievance_clause']['id'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['table'] = 'field_data_field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['field'] = 'field_grievance_clause';
  $handler->display->display_options['fields']['field_grievance_clause']['label'] = 'Full Clause';
  $handler->display->display_options['fields']['field_grievance_clause']['exclude'] = TRUE;
  /* Field: Content: Recommended Statement of Grievance */
  $handler->display->display_options['fields']['field_grievance_type']['id'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['table'] = 'field_data_field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['field'] = 'field_grievance_type';
  $handler->display->display_options['fields']['field_grievance_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_type']['delta_limit'] = 'all';
  /* Field: Content: Recommended Remedy */
  $handler->display->display_options['fields']['field_grievance_remedy']['id'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['field'] = 'field_grievance_remedy';
  $handler->display->display_options['fields']['field_grievance_remedy']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_remedy']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_remedy']['delta_limit'] = 'all';

  /* Display: By Section - Table */
  $handler = $view->new_display('block', 'By Section - Table', 'block_2');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Contract Template (field_grievance_contract_tplt) */
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Article (field_grievance_contract_section) */
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['id'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['field'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary_options']['items_per_page'] = '25';

  /* Display: Entire Contract - CSV */
  $handler = $view->new_display('views_data_export', 'Entire Contract - CSV', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Brief Section';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['path'] = 'node/%/clauses/csv';
  $handler->display->display_options['displays'] = array(
    'block' => 'block',
    'default' => 0,
    'block_1' => 0,
    'block_2' => 0,
  );

  /* Display: By Section - CSV */
  $handler = $view->new_display('views_data_export', 'By Section - CSV', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title - %2-title.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['relationship'] = 'field_grievance_contract_section_target_id';
  /* Field: Content: Article */
  $handler->display->display_options['fields']['field_grievance_contract_section']['id'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['field'] = 'field_grievance_contract_section';
  $handler->display->display_options['fields']['field_grievance_contract_section']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Label */
  $handler->display->display_options['fields']['field_grievance_label']['id'] = 'field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['table'] = 'field_data_field_grievance_label';
  $handler->display->display_options['fields']['field_grievance_label']['field'] = 'field_grievance_label';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Brief Section';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Contract Template (field_grievance_contract_tplt) */
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Article (field_grievance_contract_section) */
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['id'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['table'] = 'field_data_field_grievance_contract_section';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['field'] = 'field_grievance_contract_section_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_section_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'node/%/clauses/%/csv';
  $handler->display->display_options['displays'] = array(
    'block_2' => 'block_2',
    'default' => 0,
    'block' => 0,
    'block_1' => 0,
  );
  $translatables['grievance_contract_clauses'] = array(
    t('Master'),
    t('Grievance Contract Sections'),
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
    t('Displaying @start - @end of @total'),
    t('Content entity referenced from field_grievance_contract_section'),
    t('Content'),
    t('Nid'),
    t('Article Number'),
    t('Article'),
    t('Label'),
    t('Brief Section'),
    t('Full Text'),
    t('Recommended Statement of Grievance'),
    t('Recommended Remedy'),
    t('Edit'),
    t('All'),
    t('Entire Contract - Table'),
    t('Entity Reference'),
    t('Entire Contract - List'),
    t('Clause Number'),
    t('Brief Clause'),
    t('Full Clause'),
    t('By Section - Table'),
    t('Entire Contract - CSV'),
    t('By Section - CSV'),
  );
  $export['grievance_contract_clauses'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_rules';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Rules';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Rules';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'nothing' => 'nothing',
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
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  /* Field: Draggableviews: Content */
  $handler->display->display_options['fields']['draggableviews']['id'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['table'] = 'node';
  $handler->display->display_options['fields']['draggableviews']['field'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['handler'] = 'draggableviews_handler_fieldapi';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['save_button_label'] = 'Save Order';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['ajax'] = 0;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['draggableviews_handler_fieldapi'] = array(
    'field' => 'field_data_field_grievance_weight:field_grievance_weight_value',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: From Status */
  $handler->display->display_options['fields']['field_grievance_from_status']['id'] = 'field_grievance_from_status';
  $handler->display->display_options['fields']['field_grievance_from_status']['table'] = 'field_data_field_grievance_from_status';
  $handler->display->display_options['fields']['field_grievance_from_status']['field'] = 'field_grievance_from_status';
  $handler->display->display_options['fields']['field_grievance_from_status']['label'] = 'When a grievance has this status';
  $handler->display->display_options['fields']['field_grievance_from_status']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_from_status']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_from_status']['delta_offset'] = '0';
  /* Field: Content: Days */
  $handler->display->display_options['fields']['field_grievance_days']['id'] = 'field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['table'] = 'field_data_field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['field'] = 'field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_days']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Type of Days */
  $handler->display->display_options['fields']['field_grievance_days_type']['id'] = 'field_grievance_days_type';
  $handler->display->display_options['fields']['field_grievance_days_type']['table'] = 'field_data_field_grievance_days_type';
  $handler->display->display_options['fields']['field_grievance_days_type']['field'] = 'field_grievance_days_type';
  $handler->display->display_options['fields']['field_grievance_days_type']['label'] = 'Days Type';
  $handler->display->display_options['fields']['field_grievance_days_type']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'This many days are available';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[field_grievance_days] [field_grievance_days_type]';
  /* Field: Taxonomy term: Actor */
  $handler->display->display_options['fields']['field_grievance_actor']['id'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['table'] = 'field_data_field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['field'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['relationship'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['fields']['field_grievance_actor']['label'] = 'Before this actor';
  /* Field: Content: Next Step */
  $handler->display->display_options['fields']['field_grievance_alert_tid']['id'] = 'field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['field'] = 'field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['label'] = 'Must perform this action';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Next Status */
  $handler->display->display_options['fields']['field_grievance_next_status']['id'] = 'field_grievance_next_status';
  $handler->display->display_options['fields']['field_grievance_next_status']['table'] = 'field_data_field_grievance_next_status';
  $handler->display->display_options['fields']['field_grievance_next_status']['field'] = 'field_grievance_next_status';
  $handler->display->display_options['fields']['field_grievance_next_status']['label'] = 'After which the status will be';
  $handler->display->display_options['fields']['field_grievance_next_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Weight (field_grievance_weight) */
  $handler->display->display_options['sorts']['field_grievance_weight_value']['id'] = 'field_grievance_weight_value';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['table'] = 'field_data_field_grievance_weight';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['field'] = 'field_grievance_weight_value';
  /* Contextual filter: Content: Contract Template (field_grievance_contract_tplt) */
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_grievance_classaction_value']['limit'] = '0';
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
    'grievance_contract_rule' => 'grievance_contract_rule',
  );

  /* Display: Actions */
  $handler = $view->new_display('block', 'Actions', 'block');

  /* Display: Notifications */
  $handler = $view->new_display('block', 'Notifications', 'block_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['label'] = 'When a grievance has this status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Field: Notification Subject */
  $handler->display->display_options['fields']['field_grievance_notify_subject']['id'] = 'field_grievance_notify_subject';
  $handler->display->display_options['fields']['field_grievance_notify_subject']['table'] = 'field_data_field_grievance_notify_subject';
  $handler->display->display_options['fields']['field_grievance_notify_subject']['field'] = 'field_grievance_notify_subject';
  /* Field: Field: Notification Body */
  $handler->display->display_options['fields']['field_grievance_notify_body']['id'] = 'field_grievance_notify_body';
  $handler->display->display_options['fields']['field_grievance_notify_body']['table'] = 'field_data_field_grievance_notify_body';
  $handler->display->display_options['fields']['field_grievance_notify_body']['field'] = 'field_grievance_notify_body';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  $translatables['grievance_contract_rules'] = array(
    t('Master'),
    t('Grievance Contract Rules'),
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
    t('term from field_grievance_alert_tid'),
    t('Content'),
    t('Nid'),
    t('When a grievance has this status'),
    t('Days'),
    t('Days Type'),
    t('This many days are available'),
    t('[field_grievance_days] [field_grievance_days_type]'),
    t('Before this actor'),
    t('Must perform this action'),
    t('After which the status will be'),
    t('Edit'),
    t('All'),
    t('Actions'),
    t('Notifications'),
    t('Notification Subject'),
    t('Notification Body'),
  );
  $export['grievance_contract_rules'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_section_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Article Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Article Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_grievance_contract_tplt' => 'field_grievance_contract_tplt',
    'field_grievance_cont_tplt_tags' => 'field_grievance_cont_tplt_tags',
    'field_grievance_section_number' => 'field_grievance_section_number',
    'title' => 'title',
    'field_grievance_cont_sec_tags' => 'field_grievance_cont_sec_tags',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_grievance_contract_tplt' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_cont_tplt_tags' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_section_number' => array(
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
    'field_grievance_cont_sec_tags' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* Field: Content: Contract Template */
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['id'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['field'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Template Tags */
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['id'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['table'] = 'field_data_field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['field'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['label'] = 'Contract Tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['delta_offset'] = '0';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Article Tags */
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['id'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['table'] = 'field_data_field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['field'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['delta_offset'] = '0';
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
    'grievance_contract_section' => 'grievance_contract_section',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Article Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Article Tags (field_grievance_cont_sec_tags) */
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['id'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['table'] = 'field_data_field_grievance_cont_sec_tags';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['field'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['operator_id'] = 'field_grievance_cont_sec_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['label'] = 'Article Tag';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['operator'] = 'field_grievance_cont_sec_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['identifier'] = 'field_grievance_cont_sec_tags_tid';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    8 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_cont_sec_tags_tid']['vocabulary'] = 'grievance_contract_section_tags';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'skip_permission_check' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          'title' => 'title',
          'grievance_contract_section::field_grievance_section_number' => 'grievance_contract_section::field_grievance_section_number',
          'grievance_contract_section::field_grievance_cont_sec_tags' => 'grievance_contract_section::field_grievance_cont_sec_tags',
        ),
      ),
    ),
  );
  /* Field: Content: Contract Template */
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['id'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['field'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Template Tags */
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['id'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['table'] = 'field_data_field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['field'] = 'field_grievance_cont_tplt_tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['label'] = 'Contract Tags';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_tplt_tags']['delta_offset'] = '0';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Article Tags */
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['id'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['table'] = 'field_data_field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['field'] = 'field_grievance_cont_sec_tags';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_cont_sec_tags']['delta_offset'] = '0';
  $handler->display->display_options['path'] = 'grievance/admin/contract-templates/section-search';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'grievance/admin/contract-templates/section-search';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['grievance_contract_section_search'] = array(
    t('Master'),
    t('Grievance Contract Article Search'),
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
    t('Displaying @start - @end of @total'),
    t('Contract Template'),
    t('Contract Tags'),
    t('Article Number'),
    t('Title'),
    t('Article Tags'),
    t('Article Title'),
    t('Article Tag'),
    t('Page'),
    t('Nid'),
    t('Content'),
    t('- Choose an operation -'),
    t('Data export'),
  );
  $export['grievance_contract_section_search'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_sections';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Articles';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Articles';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance contract';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'nothing' => 'nothing',
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
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* Field: Draggableviews: Content */
  $handler->display->display_options['fields']['draggableviews']['id'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['table'] = 'node';
  $handler->display->display_options['fields']['draggableviews']['field'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['handler'] = 'draggableviews_handler_fieldapi';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['save_button_label'] = 'Save Order';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['ajax'] = 0;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['draggableviews_handler_fieldapi'] = array(
    'field' => 'field_data_field_grievance_weight:field_grievance_weight_value',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Sections';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]/clauses">sections</a>';
  /* Sort criterion: Content: Weight (field_grievance_weight) */
  $handler->display->display_options['sorts']['field_grievance_weight_value']['id'] = 'field_grievance_weight_value';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['table'] = 'field_data_field_grievance_weight';
  $handler->display->display_options['sorts']['field_grievance_weight_value']['field'] = 'field_grievance_weight_value';
  /* Sort criterion: Content: Article Number (field_grievance_section_number) */
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['id'] = 'field_grievance_section_number_value';
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['sorts']['field_grievance_section_number_value']['field'] = 'field_grievance_section_number_value';
  /* Contextual filter: Content: Contract Template (field_grievance_contract_tplt) */
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['id'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['field'] = 'field_grievance_contract_tplt_target_id';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_contract_tplt_target_id']['summary_options']['items_per_page'] = '25';
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
    'grievance_contract_section' => 'grievance_contract_section',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'field_grievance_section_number' => 'field_grievance_section_number',
    'title' => 'title',
    'nid' => 0,
    'edit_node' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title Articles.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Article Number */
  $handler->display->display_options['fields']['field_grievance_section_number']['id'] = 'field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['table'] = 'field_data_field_grievance_section_number';
  $handler->display->display_options['fields']['field_grievance_section_number']['field'] = 'field_grievance_section_number';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['path'] = 'node/%/sections/csv';
  $handler->display->display_options['displays'] = array(
    'block' => 'block',
    'default' => 0,
  );
  $translatables['grievance_contract_sections'] = array(
    t('Master'),
    t('Grievance Contract Articles'),
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
    t('Displaying @start - @end of @total'),
    t('Content'),
    t('Nid'),
    t('Article Number'),
    t('Title'),
    t('Edit'),
    t('Sections'),
    t('<a href="/node/[nid]/clauses">sections</a>'),
    t('All'),
    t('Block'),
    t('Entity Reference'),
    t('Data export'),
  );
  $export['grievance_contract_sections'] = $view;

  $view = new view();
  $view->name = 'grievance_contract_templates';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Contract Templates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Contract Templates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
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
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-contract-template">Add a Template</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'skip_permission_check' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          'grievance_contract_template::field_grievance_holidays' => 'grievance_contract_template::field_grievance_holidays',
          'grievance_contract_template::field_grievance_departments' => 'grievance_contract_template::field_grievance_departments',
          'grievance_contract_template::field_grievance_classifications' => 'grievance_contract_template::field_grievance_classifications',
          'grievance_contract_template::field_grievance_pullclause' => 'grievance_contract_template::field_grievance_pullclause',
          'grievance_contract_template::field_grievance_artsel' => 'grievance_contract_template::field_grievance_artsel',
        ),
      ),
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Notes';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[<a href="/node/[nid]">View</a>] 
[<a href="/node/[nid]/edit">Edit</a>] 
[<a href="/node/[nid]/sections">Articles</a>] 
[<a href="/node/[nid]/clauses">Sections</a>] 
[<a href="/node/[nid]/rules">Rules</a>] ';
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
    'grievance_contract_template' => 'grievance_contract_template',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/contract-templates';
  $translatables['grievance_contract_templates'] = array(
    t('Master'),
    t('Grievance Contract Templates'),
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
    t('[<a href="/node/add/grievance-contract-template">Add a Template</a>]'),
    t('Displaying @start - @end of @total'),
    t('Content'),
    t('- Choose an operation -'),
    t('Nid'),
    t('Title'),
    t('Notes'),
    t('Actions'),
    t('[<a href="/node/[nid]">View</a>] 
[<a href="/node/[nid]/edit">Edit</a>] 
[<a href="/node/[nid]/sections">Articles</a>] 
[<a href="/node/[nid]/clauses">Sections</a>] 
[<a href="/node/[nid]/rules">Rules</a>] '),
    t('Page'),
  );
  $export['grievance_contract_templates'] = $view;

  $view = new view();
  $view->name = 'grievance_corrected';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Corrected';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Corrected';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The following members have corrected information which needs to be uploaded into the member management system.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no grievances with pending member data that needs to be corrected.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'grievance::field_grievance_corrected' => 'grievance::field_grievance_corrected',
        ),
      ),
    ),
  );
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_grievance_address']['id'] = 'field_grievance_address';
  $handler->display->display_options['fields']['field_grievance_address']['table'] = 'field_data_field_grievance_address';
  $handler->display->display_options['fields']['field_grievance_address']['field'] = 'field_grievance_address';
  /* Field: Content: Address 2 */
  $handler->display->display_options['fields']['field_grievance_co_address_2']['id'] = 'field_grievance_co_address_2';
  $handler->display->display_options['fields']['field_grievance_co_address_2']['table'] = 'field_data_field_grievance_co_address_2';
  $handler->display->display_options['fields']['field_grievance_co_address_2']['field'] = 'field_grievance_co_address_2';
  /* Field: Content: City */
  $handler->display->display_options['fields']['field_grievance_city']['id'] = 'field_grievance_city';
  $handler->display->display_options['fields']['field_grievance_city']['table'] = 'field_data_field_grievance_city';
  $handler->display->display_options['fields']['field_grievance_city']['field'] = 'field_grievance_city';
  /* Field: Content: State or Province */
  $handler->display->display_options['fields']['field_grievance_co_state']['id'] = 'field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['table'] = 'field_data_field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['field'] = 'field_grievance_co_state';
  /* Field: Content: Postal Code */
  $handler->display->display_options['fields']['field_grievance_co_zip']['id'] = 'field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['table'] = 'field_data_field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['field'] = 'field_grievance_co_zip';
  /* Field: Content: Date of Hire */
  $handler->display->display_options['fields']['field_grievance_hire_date']['id'] = 'field_grievance_hire_date';
  $handler->display->display_options['fields']['field_grievance_hire_date']['table'] = 'field_data_field_grievance_hire_date';
  $handler->display->display_options['fields']['field_grievance_hire_date']['field'] = 'field_grievance_hire_date';
  $handler->display->display_options['fields']['field_grievance_hire_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Department */
  $handler->display->display_options['fields']['field_grievance_department']['id'] = 'field_grievance_department';
  $handler->display->display_options['fields']['field_grievance_department']['table'] = 'field_data_field_grievance_department';
  $handler->display->display_options['fields']['field_grievance_department']['field'] = 'field_grievance_department';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_grievance_email']['id'] = 'field_grievance_email';
  $handler->display->display_options['fields']['field_grievance_email']['table'] = 'field_data_field_grievance_email';
  $handler->display->display_options['fields']['field_grievance_email']['field'] = 'field_grievance_email';
  $handler->display->display_options['fields']['field_grievance_email']['type'] = 'email_plain';
  /* Field: Content: Office Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone']['id'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['table'] = 'field_data_field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['field'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['label'] = 'Phone';
  /* Field: Content: Employee ID # */
  $handler->display->display_options['fields']['field_grievance_ein']['id'] = 'field_grievance_ein';
  $handler->display->display_options['fields']['field_grievance_ein']['table'] = 'field_data_field_grievance_ein';
  $handler->display->display_options['fields']['field_grievance_ein']['field'] = 'field_grievance_ein';
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
  /* Filter criterion: Content: Data Corrected (field_grievance_corrected) */
  $handler->display->display_options['filters']['field_grievance_corrected_value']['id'] = 'field_grievance_corrected_value';
  $handler->display->display_options['filters']['field_grievance_corrected_value']['table'] = 'field_data_field_grievance_corrected';
  $handler->display->display_options['filters']['field_grievance_corrected_value']['field'] = 'field_grievance_corrected_value';
  $handler->display->display_options['filters']['field_grievance_corrected_value']['value'] = array(
    'Pending' => 'Pending',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/corrected';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'grievance/admin/corrected/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $translatables['grievance_corrected'] = array(
    t('Master'),
    t('Grievance Corrected'),
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
    t('The following members have corrected information which needs to be uploaded into the member management system.'),
    t('There are no grievances with pending member data that needs to be corrected.'),
    t('Content'),
    t('- Choose an operation -'),
    t('First Name'),
    t('Last Name'),
    t('Address'),
    t('Address 2'),
    t('City'),
    t('State or Province'),
    t('Postal Code'),
    t('Date of Hire'),
    t('Department'),
    t('Email'),
    t('Phone'),
    t('Employee ID #'),
    t('Page'),
    t('Data export'),
  );
  $export['grievance_corrected'] = $view;

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
  $view->name = 'grievance_holidays';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Holidays';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Holidays';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
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
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-holiday?destination=grievance/admin/holidays">Add a holiday</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_grievance_date']['id'] = 'field_grievance_date';
  $handler->display->display_options['fields']['field_grievance_date']['table'] = 'field_data_field_grievance_date';
  $handler->display->display_options['fields']['field_grievance_date']['field'] = 'field_grievance_date';
  $handler->display->display_options['fields']['field_grievance_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Annual? */
  $handler->display->display_options['fields']['field_grievance_annual']['id'] = 'field_grievance_annual';
  $handler->display->display_options['fields']['field_grievance_annual']['table'] = 'field_data_field_grievance_annual';
  $handler->display->display_options['fields']['field_grievance_annual']['field'] = 'field_grievance_annual';
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
    'grievance_holiday' => 'grievance_holiday',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/holidays';
  $translatables['grievance_holidays'] = array(
    t('Master'),
    t('Grievance Holidays'),
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
    t('[<a href="/node/add/grievance-holiday?destination=grievance/admin/holidays">Add a holiday</a>]'),
    t('Title'),
    t('Date'),
    t('Annual?'),
    t('Page'),
  );
  $export['grievance_holidays'] = $view;

  $view = new view();
  $view->name = 'grievance_information_request_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Grievance Information Request Types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Sort criterion: Taxonomy term: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'grievance_document_types' => 'grievance_document_types',
  );
  /* Filter criterion: Taxonomy term: Information Request? (field_grievance_can_ir) */
  $handler->display->display_options['filters']['field_grievance_can_ir_value']['id'] = 'field_grievance_can_ir_value';
  $handler->display->display_options['filters']['field_grievance_can_ir_value']['table'] = 'field_data_field_grievance_can_ir';
  $handler->display->display_options['filters']['field_grievance_can_ir_value']['field'] = 'field_grievance_can_ir_value';
  $handler->display->display_options['filters']['field_grievance_can_ir_value']['value'] = array(
    'Yes' => 'Yes',
  );

  /* Display: Term Reference */
  $handler = $view->new_display('term_reference', 'Term Reference', 'term_reference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'term_reference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'term_reference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['grievance_information_request_types'] = array(
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
    t('Term Reference'),
  );
  $export['grievance_information_request_types'] = $view;

  $view = new view();
  $view->name = 'grievance_letter_templates';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Letter Templates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Letter Templates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-letter-template?destination=grievance/admin/letter-templates">Add a letter template</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
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
    'grievance_letter_template' => 'grievance_letter_template',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/letter-templates';
  $translatables['grievance_letter_templates'] = array(
    t('Master'),
    t('Grievance Letter Templates'),
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
    t('[<a href="/node/add/grievance-letter-template?destination=grievance/admin/letter-templates">Add a letter template</a>]'),
    t('Title'),
    t('Edit link'),
    t('Page'),
  );
  $export['grievance_letter_templates'] = $view;

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
  $handler->display->display_options['access']['perm'] = 'grievance user';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'field_grievance_log_type_tid' => array(
      'bef_format' => 'bef',
      'more_options' => array(
        'bef_select_all_none' => 1,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            1 => 'vocabulary',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'break_row';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_grievance_status_date' => 'field_grievance_status_date',
    'field_grievance_log_type' => 'field_grievance_log_type',
    'name' => 'name',
    'field_grievance_status' => 'field_grievance_status',
    'edit_node' => 'edit_node',
    'field_grievance_attachments' => 'field_grievance_attachments',
    'body' => 'body',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_status_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_log_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_attachments' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => 'nothing',
      'separator2' => '',
      'colspan' => '6',
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_grievance_status_date']['id'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['field'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Grievance Log Type */
  $handler->display->display_options['fields']['field_grievance_log_type']['id'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['table'] = 'field_data_field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['field'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['label'] = 'Entry Type';
  $handler->display->display_options['fields']['field_grievance_log_type']['type'] = 'taxonomy_term_reference_plain';
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
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  /* Field: Content: Attachments */
  $handler->display->display_options['fields']['field_grievance_attachments']['id'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['table'] = 'field_data_field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['field'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_attachments']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_attachments']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_attachments']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Body';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<pre>[body][field_grievance_attachments]</pre><hr>';
  /* Sort criterion: Content: Date (field_grievance_status_date) */
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['id'] = 'field_grievance_status_date_value';
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['field'] = 'field_grievance_status_date_value';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['operator_id'] = 'field_grievance_log_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['label'] = 'Show only log entries of type:';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['operator'] = 'field_grievance_log_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['identifier'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['vocabulary'] = 'grievance_log_types';

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
    t('Advanced options'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('author'),
    t('Title'),
    t('Date'),
    t('Entry Type'),
    t('Author'),
    t('Status'),
    t('Edit link'),
    t('Attachments'),
    t('Body'),
    t('<pre>[body][field_grievance_attachments]</pre><hr>'),
    t('All'),
    t('Show only log entries of type:'),
    t('Block'),
  );
  $export['grievance_log'] = $view;

  $view = new view();
  $view->name = 'grievance_log_all';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Log All';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Log';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance statuslog';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'input_required' => 1,
      'text_input_required' => array(
        'text_input_required' => array(
          'value' => 'Select any filter and click on Apply to see results',
          'format' => 'full_html',
        ),
      ),
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'secondary_collapse_override' => '0',
    ),
    'field_grievance_log_type_tid' => array(
      'bef_format' => 'bef',
      'more_options' => array(
        'bef_select_all_none' => 1,
        'bef_collapsible' => 0,
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            1 => 'vocabulary',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'field_grievance_first_name_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'field_grievance_id_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'field_grievance_last_name_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'title' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'break_row';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'nid_1' => 'nid_1',
    'field_grievance_status_date' => 'field_grievance_status_date',
    'field_grievance_id' => 'field_grievance_id',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'title_1' => 'title_1',
    'nothing' => 'nothing',
    'name' => 'name',
    'title' => 'title',
    'field_grievance_log_type' => 'field_grievance_log_type',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_attachments' => 'field_grievance_attachments',
    'body' => 'body',
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
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_status_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_first_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_last_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_log_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_attachments' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => 'nothing_1',
      'separator2' => '',
      'colspan' => '7',
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
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
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['nid_1']['label'] = 'Grievance Nid';
  $handler->display->display_options['fields']['nid_1']['exclude'] = TRUE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_grievance_status_date']['id'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['field'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_id']['alter']['text'] = '<a href="/node/[nid_1]">[field_grievance_id]</a>';
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_first_name']['exclude'] = TRUE;
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_last_name']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Employer';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>[title_1]';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Grievance Log Type */
  $handler->display->display_options['fields']['field_grievance_log_type']['id'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['table'] = 'field_data_field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['field'] = 'field_grievance_log_type';
  $handler->display->display_options['fields']['field_grievance_log_type']['label'] = 'Entry Type';
  $handler->display->display_options['fields']['field_grievance_log_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Attachments */
  $handler->display->display_options['fields']['field_grievance_attachments']['id'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['table'] = 'field_data_field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['field'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_attachments']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_attachments']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_attachments']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Body';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<pre>[body][field_grievance_attachments]</pre><hr>';
  /* Sort criterion: Content: Date (field_grievance_status_date) */
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['id'] = 'field_grievance_status_date_value';
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['field'] = 'field_grievance_status_date_value';
  $handler->display->display_options['sorts']['field_grievance_status_date_value']['order'] = 'DESC';
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
    'grievance_log' => 'grievance_log',
  );
  /* Filter criterion: Content: Grievance Log Type (field_grievance_log_type) */
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['id'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['table'] = 'field_data_field_grievance_log_type';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['field'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['operator_id'] = 'field_grievance_log_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['label'] = 'Show only log entries of type:';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['operator'] = 'field_grievance_log_type_tid_op';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['identifier'] = 'field_grievance_log_type_tid';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_log_type_tid']['vocabulary'] = 'grievance_log_types';
  /* Filter criterion: Content: First Name (field_grievance_first_name) */
  $handler->display->display_options['filters']['field_grievance_first_name_value']['id'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['field'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['operator_id'] = 'field_grievance_first_name_value_op';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['label'] = 'First Name';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['operator'] = 'field_grievance_first_name_value_op';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['identifier'] = 'field_grievance_first_name_value';
  $handler->display->display_options['filters']['field_grievance_first_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance ID';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Last Name (field_grievance_last_name) */
  $handler->display->display_options['filters']['field_grievance_last_name_value']['id'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['field'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['operator_id'] = 'field_grievance_last_name_value_op';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['label'] = 'Last Name';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['operator'] = 'field_grievance_last_name_value_op';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['identifier'] = 'field_grievance_last_name_value';
  $handler->display->display_options['filters']['field_grievance_last_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date (field_grievance_status_date) */
  $handler->display->display_options['filters']['field_grievance_status_date_value']['id'] = 'field_grievance_status_date_value';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['field'] = 'field_grievance_status_date_value';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_date_value']['expose']['operator_id'] = 'field_grievance_status_date_value_op';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['expose']['label'] = 'Log Date';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['expose']['operator'] = 'field_grievance_status_date_value_op';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['expose']['identifier'] = 'field_grievance_status_date_value';
  $handler->display->display_options['filters']['field_grievance_status_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_status_date_value']['form_type'] = 'date_popup';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'grievances/log';
  $translatables['grievance_log_all'] = array(
    t('Master'),
    t('Grievance Log'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Select any filter and click on Apply to see results'),
    t('Advanced options'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('author'),
    t('Grievance'),
    t('Shop'),
    t('Nid'),
    t('Grievance Nid'),
    t('Date'),
    t('ID'),
    t('<a href="/node/[nid_1]">[field_grievance_id]</a>'),
    t('First Name'),
    t('Last Name'),
    t('Employer'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>[title_1]'),
    t('Author'),
    t('Title'),
    t('Entry Type'),
    t('Status'),
    t('Attachments'),
    t('Body'),
    t('<pre>[body][field_grievance_attachments]</pre><hr>'),
    t('Show only log entries of type:'),
    t('Grievance ID'),
    t('Log Date'),
    t('Page'),
  );
  $export['grievance_log_all'] = $view;

  $view = new view();
  $view->name = 'grievance_log_status';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Log Status';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Log Status';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'full_html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_status' => 'field_grievance_status',
    'created' => 'created',
    'field_grievance_status_date' => 'field_grievance_status_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_rep_organizer' => 'field_grievance_rep_organizer',
    'field_grievance_id' => 'field_grievance_id',
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
    'field_grievance_status_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_shop' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_organizer' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_id' => array(
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
  $handler->display->display_options['header']['area']['content'] = 'The view below searches the grievance log for entries with a specific status.  In other words, it searches for all grievances that have <em>ever</em> entered the specified status.  It displays the intake date, the date that the grievance first entered the selected status, and also the <em>current</em> status that the grievance has.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_target_id']['id'] = 'field_grievance_target_id';
  $handler->display->display_options['relationships']['field_grievance_target_id']['table'] = 'field_data_field_grievance';
  $handler->display->display_options['relationships']['field_grievance_target_id']['field'] = 'field_grievance_target_id';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['nid']['label'] = 'Grievance NID';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_status']['label'] = 'Status Now';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: MIN(Content: Date) */
  $handler->display->display_options['fields']['field_grievance_status_date']['id'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['table'] = 'field_data_field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['field'] = 'field_grievance_status_date';
  $handler->display->display_options['fields']['field_grievance_status_date']['group_type'] = 'min';
  $handler->display->display_options['fields']['field_grievance_status_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'custom_date_format' => '',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
  $handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
  );
  /* Field: Field: Organizer */
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['id'] = 'field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['table'] = 'field_data_field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['field'] = 'field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['label'] = 'Specialist';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_offset'] = '0';
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_id']['label'] = 'Grievance ID';
  $handler->display->display_options['fields']['field_grievance_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_id']['alter']['path'] = 'node/[nid]';
  /* Sort criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['sorts']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['sorts']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['sorts']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['sorts']['field_grievance_id_value']['relationship'] = 'field_grievance_target_id';
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
  /* Filter criterion: Content: Status (field_grievance_status) */
  $handler->display->display_options['filters']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['filters']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator_id'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['label'] = 'Where the status was, at some point:';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['identifier'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    8 => 0,
    9 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_status_tid']['vocabulary'] = 'grievance_status';
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Where the intake date is between:';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    8 => 0,
    9 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievances/log/status';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'grievances/log/status/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $translatables['grievance_log_status'] = array(
    t('Master'),
    t('Grievance Log Status'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Select any filter and click on Apply to see results'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('The view below searches the grievance log for entries with a specific status.  In other words, it searches for all grievances that have <em>ever</em> entered the specified status.  It displays the intake date, the date that the grievance first entered the selected status, and also the <em>current</em> status that the grievance has.'),
    t('Content entity referenced from field_grievance'),
    t('Grievance NID'),
    t('Status Now'),
    t('Intake Date'),
    t('Date'),
    t('Employer'),
    t('Specialist'),
    t('Grievance ID'),
    t('Where the status was, at some point:'),
    t('Where the intake date is between:'),
    t('Page'),
    t('Data export'),
  );
  $export['grievance_log_status'] = $view;

  $view = new view();
  $view->name = 'grievance_news';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance News';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'News and Announcements';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance user';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
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
    'grievance_news' => 'grievance_news',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/news';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/grievance/news">All news</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';

  /* Display: RSS Feed */
  $handler = $view->new_display('feed', 'RSS Feed', 'feed_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'grievance/news/rss';
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: JSON Feed */
  $handler = $view->new_display('page', 'JSON Feed', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['remove_newlines'] = 0;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['style_options']['object_arrays'] = 0;
  $handler->display->display_options['style_options']['numeric_strings'] = 0;
  $handler->display->display_options['style_options']['bigint_string'] = 0;
  $handler->display->display_options['style_options']['pretty_print'] = 0;
  $handler->display->display_options['style_options']['unescaped_slashes'] = 0;
  $handler->display->display_options['style_options']['unescaped_unicode'] = 0;
  $handler->display->display_options['style_options']['char_encoding'] = array();
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'grievance/news/json';
  $translatables['grievance_news'] = array(
    t('Master'),
    t('News and Announcements'),
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
    t('Body'),
    t('Nid'),
    t('Page'),
    t('Block'),
    t('[<a href="/grievance/news">All news</a>]'),
    t('RSS Feed'),
    t('JSON Feed'),
  );
  $export['grievance_news'] = $view;

  $view = new view();
  $view->name = 'grievance_open_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Open Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Open Grievances Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance view';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'views_aggregator';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_rep_organizer' => 'field_grievance_rep_organizer',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_id' => 'field_grievance_id',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'created' => 'created',
    'field_grievance_class_tid' => 'field_grievance_class_tid',
    'field_grievance_clauseref' => 'field_grievance_clauseref',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
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
    'field_grievance_rep_organizer' => array(
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
    'field_grievance_shop' => array(
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
      'has_aggr_column' => 1,
      'aggr_column' => 'views_aggregator_replace',
      'aggr_par_column' => 'Total',
    ),
    'field_grievance_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'has_aggr' => 0,
      'aggr' => array(
        'views_aggregator_count' => 'views_aggregator_count',
      ),
      'aggr_par' => '',
      'has_aggr_column' => 1,
      'aggr_column' => 'views_aggregator_count',
      'aggr_par_column' => '',
    ),
    'field_grievance_first_name' => array(
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
    'field_grievance_last_name' => array(
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
    'created' => array(
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
    'field_grievance_class_tid' => array(
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
    'field_grievance_clauseref' => array(
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
  /* Relationship: Content: Status (field_grievance_status) */
  $handler->display->display_options['relationships']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['label'] = 'Status Term';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['id'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['table'] = 'field_data_field_grievance_rep_organizer';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['field'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['label'] = 'Specialist';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['relationships']['field_grievance_shop_target_id']['label'] = 'Shop';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Field: Organizer */
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['id'] = 'field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['table'] = 'field_data_field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['field'] = 'field_grievance_rep_organizer';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['label'] = '';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['empty'] = '[No Specialist Assigned]';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['settings'] = array(
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_limit'] = 'all';
  $handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_offset'] = '0';
  /* Field: Content: Employer */
  $handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Classification */
  $handler->display->display_options['fields']['field_grievance_class_tid']['id'] = 'field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['table'] = 'field_data_field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['field'] = 'field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: CBA Provision(s) Violated */
  $handler->display->display_options['fields']['field_grievance_clauseref']['id'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['table'] = 'field_data_field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['field'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['settings'] = array(
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_clauseref']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_grievance_clauseref']['separator'] = '<br>';
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'field_grievance_rep_organizer_target_id';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['sorts']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['sorts']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['sorts']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  /* Contextual filter: Field: Organizer (field_grievance_rep_organizer) */
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['id'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['table'] = 'field_data_field_grievance_rep_organizer';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['field'] = 'field_grievance_rep_organizer_target_id';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['default_argument_options']['argument'] = 'NULL';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_rep_organizer_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['default_argument_options']['argument'] = '1+2+3';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_grievance_shop_target_id']['break_phrase'] = TRUE;
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
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['value'] = array(
    1 => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/report/open/ignore';
  $translatables['grievance_open_report'] = array(
    t('Master'),
    t('Open Grievances Report'),
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
    t('Status Term'),
    t('Specialist'),
    t('Shop'),
    t('Nid'),
    t('[No Specialist Assigned]'),
    t('Employer'),
    t('ID'),
    t('First Name'),
    t('Last Name'),
    t('Intake Date'),
    t('Classification'),
    t('CBA Provision(s) Violated'),
    t('All'),
    t('Page'),
  );
  $export['grievance_open_report'] = $view;

  $view = new view();
  $view->name = 'grievance_prior_grievances';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievance Prior Grievances';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievance Prior Grievances';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance view';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_grievance_id' => 'field_grievance_id',
    'field_grievance_date_1' => 'field_grievance_date_1',
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
    'field_grievance_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_date_1' => array(
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
  $handler->display->display_options['header']['area']['content'] = 'The list below shows other grievances which were filed by this member (member ID="<strong>!1</strong>").';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no prior grievances for this member (member ID="<strong>!1</strong>").';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['empty']['area']['tokenize'] = TRUE;
  /* Relationship: Content: Status (field_grievance_status) */
  $handler->display->display_options['relationships']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['label'] = 'Status Term';
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['label'] = 'Grievance ID';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: CBA Provision(s) Violated */
  $handler->display->display_options['fields']['field_grievance_clauseref']['id'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['table'] = 'field_data_field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['field'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['settings'] = array(
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_clauseref']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_grievance_clauseref']['separator'] = '<br>';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Taxonomy term: Unresolved? */
  $handler->display->display_options['fields']['field_grievance_open']['id'] = 'field_grievance_open';
  $handler->display->display_options['fields']['field_grievance_open']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['fields']['field_grievance_open']['field'] = 'field_grievance_open';
  $handler->display->display_options['fields']['field_grievance_open']['relationship'] = 'field_grievance_status_tid';
  $handler->display->display_options['fields']['field_grievance_open']['label'] = 'Open?';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'View';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '<a href="/node/[nid]">View</a>';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['arguments']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['arguments']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['arguments']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['arguments']['field_grievance_min_value']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_grievance_min_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_grievance_min_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_grievance_min_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_grievance_min_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_grievance_min_value']['limit'] = '0';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['not'] = TRUE;
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

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['grievance_prior_grievances'] = array(
    t('Master'),
    t('Grievance Prior Grievances'),
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
    t('The list below shows other grievances which were filed by this member (member ID="<strong>!1</strong>").'),
    t('There are no prior grievances for this member (member ID="<strong>!1</strong>").'),
    t('Status Term'),
    t('Grievance ID'),
    t('Intake Date'),
    t('CBA Provision(s) Violated'),
    t('Status'),
    t('Open?'),
    t('View'),
    t('<a href="/node/[nid]">View</a>'),
    t('All'),
    t('Block'),
  );
  $export['grievance_prior_grievances'] = $view;

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
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'full_html';
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
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['fields']['title']['label'] = 'Employer';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_id']['label'] = 'Grievance ID';
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['relationship'] = 'field_grievance_target_id';
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['relationship'] = 'field_grievance_target_id';
  /* Field: Content: Classification */
  $handler->display->display_options['fields']['field_grievance_class_tid']['id'] = 'field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['table'] = 'field_data_field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['field'] = 'field_grievance_class_tid';
  $handler->display->display_options['fields']['field_grievance_class_tid']['relationship'] = 'field_grievance_target_id';
  $handler->display->display_options['fields']['field_grievance_class_tid']['type'] = 'taxonomy_term_reference_plain';
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
  /* Field: Content: Settlement Tags */
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['id'] = 'field_grievance_settlement_tags';
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['table'] = 'field_data_field_grievance_settlement_tags';
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['field'] = 'field_grievance_settlement_tags';
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_settlement_tags']['delta_offset'] = '0';
  /* Sort criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['sorts']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['sorts']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['sorts']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  /* Sort criterion: Content: First Name (field_grievance_first_name) */
  $handler->display->display_options['sorts']['field_grievance_first_name_value']['id'] = 'field_grievance_first_name_value';
  $handler->display->display_options['sorts']['field_grievance_first_name_value']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['sorts']['field_grievance_first_name_value']['field'] = 'field_grievance_first_name_value';
  $handler->display->display_options['sorts']['field_grievance_first_name_value']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Last Name (field_grievance_last_name) */
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['id'] = 'field_grievance_last_name_value';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['field'] = 'field_grievance_last_name_value';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['relationship'] = 'field_grievance_target_id';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_target_id';
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Settlement Tags (field_grievance_settlement_tags) */
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['id'] = 'field_grievance_settlement_tags_tid';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['table'] = 'field_data_field_grievance_settlement_tags';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['field'] = 'field_grievance_settlement_tags_tid';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['expose']['operator_id'] = 'field_grievance_settlement_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['expose']['label'] = 'Settlement Tag';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['expose']['operator'] = 'field_grievance_settlement_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['expose']['identifier'] = 'field_grievance_settlement_tags_tid';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    8 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_settlement_tags_tid']['vocabulary'] = 'grievance_settlement_tags';

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
    t('Select any filter and click on Apply to see results'),
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
    t('Employer'),
    t('Grievance ID'),
    t('First Name'),
    t('Last Name'),
    t('Classification'),
    t('Settlement Amount'),
    t('Amount Received'),
    t('Date Received'),
    t('Settlement Tags'),
    t('Settlement Tag'),
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
  $handler->display->display_options['access']['perm'] = 'grievance contact';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '2000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'field_grievance_co_name' => 'field_grievance_co_name',
    'field_grievance_co_role' => 'field_grievance_co_role',
    'field_grievance_company' => 'field_grievance_company',
    'field_grievance_shops' => 'field_grievance_shops',
    'field_grievance_co_phone' => 'field_grievance_co_phone',
    'field_grievance_co_email' => 'field_grievance_co_email',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_grievance_co_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_co_role' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_company' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_shops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_co_phone' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_co_email' => array(
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
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-shop-contact">Add a contact</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Field: Content: Contact Type */
  $handler->display->display_options['fields']['field_grievance_contact_types']['id'] = 'field_grievance_contact_types';
  $handler->display->display_options['fields']['field_grievance_contact_types']['table'] = 'field_data_field_grievance_contact_types';
  $handler->display->display_options['fields']['field_grievance_contact_types']['field'] = 'field_grievance_contact_types';
  $handler->display->display_options['fields']['field_grievance_contact_types']['label'] = 'Type';
  $handler->display->display_options['fields']['field_grievance_contact_types']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_grievance_contact_types']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_contact_types']['delta_offset'] = '0';
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
  /* Field: Field: Employer */
  $handler->display->display_options['fields']['field_grievance_shops']['id'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['field'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_grievance_shops']['delta_offset'] = '0';
  /* Field: Content: Office Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone']['id'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['table'] = 'field_data_field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['field'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['label'] = 'Phone';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_grievance_co_email']['id'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['table'] = 'field_data_field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['field'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['type'] = 'email_plain';
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
  /* Filter criterion: Content: Contact Type (field_grievance_contact_types) */
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['id'] = 'field_grievance_contact_types_tid';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['table'] = 'field_data_field_grievance_contact_types';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['field'] = 'field_grievance_contact_types_tid';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['expose']['operator_id'] = 'field_grievance_contact_types_tid_op';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['expose']['operator'] = 'field_grievance_contact_types_tid_op';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['expose']['identifier'] = 'field_grievance_contact_types_tid';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_contact_types_tid']['vocabulary'] = 'grievance_contact_types';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/shops/contacts';

  /* Display: By Shop */
  $handler = $view->new_display('block', 'By Shop', 'block_1');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field: Employer (field_grievance_shops) */
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

  /* Display: All Fields */
  $handler = $view->new_display('page', 'All Fields', 'page_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
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
  /* Field: Field: Employer */
  $handler->display->display_options['fields']['field_grievance_shops']['id'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['field'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_grievance_shops']['delta_offset'] = '0';
  /* Field: Content: Office Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone']['id'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['table'] = 'field_data_field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['field'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['label'] = 'Phone';
  /* Field: Content: Personal Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['id'] = 'field_grievance_co_phone_2';
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['table'] = 'field_data_field_grievance_co_phone_2';
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['field'] = 'field_grievance_co_phone_2';
  /* Field: Content: Fax */
  $handler->display->display_options['fields']['field_grievance_co_fax']['id'] = 'field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['table'] = 'field_data_field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['field'] = 'field_grievance_co_fax';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_grievance_co_email']['id'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['table'] = 'field_data_field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['field'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['type'] = 'email_plain';
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_grievance_co_address']['id'] = 'field_grievance_co_address';
  $handler->display->display_options['fields']['field_grievance_co_address']['table'] = 'field_data_field_grievance_co_address';
  $handler->display->display_options['fields']['field_grievance_co_address']['field'] = 'field_grievance_co_address';
  /* Field: Content: City */
  $handler->display->display_options['fields']['field_grievance_co_city']['id'] = 'field_grievance_co_city';
  $handler->display->display_options['fields']['field_grievance_co_city']['table'] = 'field_data_field_grievance_co_city';
  $handler->display->display_options['fields']['field_grievance_co_city']['field'] = 'field_grievance_co_city';
  /* Field: Content: State or Province */
  $handler->display->display_options['fields']['field_grievance_co_state']['id'] = 'field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['table'] = 'field_data_field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['field'] = 'field_grievance_co_state';
  /* Field: Content: Postal Code */
  $handler->display->display_options['fields']['field_grievance_co_zip']['id'] = 'field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['table'] = 'field_data_field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['field'] = 'field_grievance_co_zip';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  $handler->display->display_options['path'] = 'grievance/admin/shops/contacts/all';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
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
  /* Field: Field: Employer */
  $handler->display->display_options['fields']['field_grievance_shops']['id'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['field'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_grievance_shops']['delta_offset'] = '0';
  /* Field: Field: Employer */
  $handler->display->display_options['fields']['field_grievance_shops_1']['id'] = 'field_grievance_shops_1';
  $handler->display->display_options['fields']['field_grievance_shops_1']['table'] = 'field_data_field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops_1']['field'] = 'field_grievance_shops';
  $handler->display->display_options['fields']['field_grievance_shops_1']['label'] = 'Shop External ID';
  $handler->display->display_options['fields']['field_grievance_shops_1']['settings'] = array(
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_shops_1']['delta_offset'] = '0';
  /* Field: Content: Office Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone']['id'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['table'] = 'field_data_field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['field'] = 'field_grievance_co_phone';
  $handler->display->display_options['fields']['field_grievance_co_phone']['label'] = 'Phone';
  /* Field: Content: Personal Phone */
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['id'] = 'field_grievance_co_phone_2';
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['table'] = 'field_data_field_grievance_co_phone_2';
  $handler->display->display_options['fields']['field_grievance_co_phone_2']['field'] = 'field_grievance_co_phone_2';
  /* Field: Content: Fax */
  $handler->display->display_options['fields']['field_grievance_co_fax']['id'] = 'field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['table'] = 'field_data_field_grievance_co_fax';
  $handler->display->display_options['fields']['field_grievance_co_fax']['field'] = 'field_grievance_co_fax';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_grievance_co_email']['id'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['table'] = 'field_data_field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['field'] = 'field_grievance_co_email';
  $handler->display->display_options['fields']['field_grievance_co_email']['type'] = 'email_plain';
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_grievance_co_address']['id'] = 'field_grievance_co_address';
  $handler->display->display_options['fields']['field_grievance_co_address']['table'] = 'field_data_field_grievance_co_address';
  $handler->display->display_options['fields']['field_grievance_co_address']['field'] = 'field_grievance_co_address';
  /* Field: Content: Address 2 */
  $handler->display->display_options['fields']['field_grievance_co_address_2']['id'] = 'field_grievance_co_address_2';
  $handler->display->display_options['fields']['field_grievance_co_address_2']['table'] = 'field_data_field_grievance_co_address_2';
  $handler->display->display_options['fields']['field_grievance_co_address_2']['field'] = 'field_grievance_co_address_2';
  /* Field: Content: City */
  $handler->display->display_options['fields']['field_grievance_co_city']['id'] = 'field_grievance_co_city';
  $handler->display->display_options['fields']['field_grievance_co_city']['table'] = 'field_data_field_grievance_co_city';
  $handler->display->display_options['fields']['field_grievance_co_city']['field'] = 'field_grievance_co_city';
  /* Field: Content: State or Province */
  $handler->display->display_options['fields']['field_grievance_co_state']['id'] = 'field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['table'] = 'field_data_field_grievance_co_state';
  $handler->display->display_options['fields']['field_grievance_co_state']['field'] = 'field_grievance_co_state';
  /* Field: Content: Postal Code */
  $handler->display->display_options['fields']['field_grievance_co_zip']['id'] = 'field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['table'] = 'field_data_field_grievance_co_zip';
  $handler->display->display_options['fields']['field_grievance_co_zip']['field'] = 'field_grievance_co_zip';
  $handler->display->display_options['path'] = 'grievance/admin/shops/contacts/all/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'block_1' => 'block_1',
    'block_2' => 'block_2',
    'page_1' => 'page_1',
    'default' => 0,
  );
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
    t('[<a href="/node/add/grievance-shop-contact">Add a contact</a>]'),
    t('Type'),
    t('Name'),
    t('Role'),
    t('Company'),
    t('Employer'),
    t('Phone'),
    t('Email'),
    t('Edit'),
    t('Page'),
    t('By Shop'),
    t('All'),
    t('By Company'),
    t('All Fields'),
    t('Personal Phone'),
    t('Fax'),
    t('Address'),
    t('City'),
    t('State or Province'),
    t('Postal Code'),
    t('Data export'),
    t('Shop External ID'),
    t('Address 2'),
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
  $handler->display->display_options['title'] = 'Grievance Employers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance contact';
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
  $handler->display->display_options['footer']['area']['content'] = '[<a href="/node/add/grievance-shop">Add an item</a>]';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
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
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
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
    'link' => 0,
  );
  /* Field: Content: Chapter */
  $handler->display->display_options['fields']['field_grievance_chapter']['id'] = 'field_grievance_chapter';
  $handler->display->display_options['fields']['field_grievance_chapter']['table'] = 'field_data_field_grievance_chapter';
  $handler->display->display_options['fields']['field_grievance_chapter']['field'] = 'field_grievance_chapter';
  $handler->display->display_options['fields']['field_grievance_chapter']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Employer';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Contract */
  $handler->display->display_options['fields']['field_grievance_contract']['id'] = 'field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['table'] = 'field_data_field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['field'] = 'field_grievance_contract';
  $handler->display->display_options['fields']['field_grievance_contract']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_grievance_contract']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_contract']['group_column'] = 'entity_id';
  /* Field: Content: Attachments */
  $handler->display->display_options['fields']['field_grievance_attachments']['id'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['table'] = 'field_data_field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['field'] = 'field_grievance_attachments';
  $handler->display->display_options['fields']['field_grievance_attachments']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_grievance_attachments']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_grievance_attachments']['separator'] = '<br>';
  /* Field: Content: Contract Template */
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['id'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['table'] = 'field_data_field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['field'] = 'field_grievance_contract_tplt';
  $handler->display->display_options['fields']['field_grievance_contract_tplt']['settings'] = array(
    'link' => 1,
  );
  /* Field: Content: Company */
  $handler->display->display_options['fields']['field_grievance_company_1']['id'] = 'field_grievance_company_1';
  $handler->display->display_options['fields']['field_grievance_company_1']['table'] = 'field_data_field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company_1']['field'] = 'field_grievance_company';
  $handler->display->display_options['fields']['field_grievance_company_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_company_1']['type'] = 'entityreference_entity_id';
  $handler->display->display_options['fields']['field_grievance_company_1']['settings'] = array(
    'link' => 0,
  );
  /* Field: Field: External ID */
  $handler->display->display_options['fields']['field_grievance_external_id']['id'] = 'field_grievance_external_id';
  $handler->display->display_options['fields']['field_grievance_external_id']['table'] = 'field_data_field_grievance_external_id';
  $handler->display->display_options['fields']['field_grievance_external_id']['field'] = 'field_grievance_external_id';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[<a href="/node/[nid]/edit">Edit</a>] [<a href="/node/[nid]/contacts">Contacts</a>]';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Employer';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title_1']['id'] = 'title_1';
  $handler->display->display_options['sorts']['title_1']['table'] = 'node';
  $handler->display->display_options['sorts']['title_1']['field'] = 'title';
  $handler->display->display_options['sorts']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['sorts']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['title_1']['expose']['label'] = 'Company';
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
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_grievance_company_target_id';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Company';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Chapter (field_grievance_chapter) */
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['id'] = 'field_grievance_chapter_target_id';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['table'] = 'field_data_field_grievance_chapter';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['field'] = 'field_grievance_chapter_target_id';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['expose']['operator_id'] = 'field_grievance_chapter_target_id_op';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['expose']['label'] = 'Chapter';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['expose']['operator'] = 'field_grievance_chapter_target_id_op';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['expose']['identifier'] = 'field_grievance_chapter_target_id';
  $handler->display->display_options['filters']['field_grievance_chapter_target_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'grievance/admin/shops';
  $translatables['grievance_shops'] = array(
    t('Master'),
    t('Grievance Employers'),
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
    t('[<a href="/node/add/grievance-shop">Add an item</a>]'),
    t('Company'),
    t('Nid'),
    t('Content'),
    t('- Choose an operation -'),
    t('Chapter'),
    t('Employer'),
    t('Contract'),
    t('Attachments'),
    t('Contract Template'),
    t('External ID'),
    t('Actions'),
    t('[<a href="/node/[nid]/edit">Edit</a>] [<a href="/node/[nid]/contacts">Contacts</a>]'),
    t('Tags'),
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
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing' => 'nothing',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing_2' => 'nothing_2',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_3' => 'nothing_3',
    'field_grievance_comments' => 'field_grievance_comments',
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
    'field_grievance_id' => array(
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
    'nothing' => array(
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
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
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
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]';
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
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['sorts']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['sorts']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['sorts']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance view';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'General
Status
Company Representatives
Union Representatives
Employee
Grievance Form',
    'group-field_grievance_first_name_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_first_name_value',
      'weight' => '-20',
    ),
    'group-field_grievance_last_name_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_last_name_value',
      'weight' => '-19',
    ),
    'group-field_grievance_status_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_status_tid',
      'weight' => '-15',
    ),
    'group-uid' => array(
      'group' => '3',
      'filter_name' => 'uid',
      'weight' => '-20',
    ),
    'group-uid_1' => array(
      'group' => '3',
      'filter_name' => 'uid_1',
      'weight' => '-19',
    ),
    'group-title' => array(
      'group' => '2',
      'filter_name' => 'title',
      'weight' => '-20',
    ),
    'group-title_1' => array(
      'group' => '2',
      'filter_name' => 'title_1',
      'weight' => '-19',
    ),
    'group-field_grievance_alert_date_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_alert_date_value',
      'weight' => '-21',
    ),
    'group-field_grievance_open_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_open_value',
      'weight' => '-20',
    ),
    'group-field_grievance_remedy_tid' => array(
      'group' => '5',
      'filter_name' => 'field_grievance_remedy_tid',
      'weight' => '-20',
    ),
    'group-field_grievance_violation_value' => array(
      'group' => '5',
      'filter_name' => 'field_grievance_violation_value',
      'weight' => '-19',
    ),
    'group-field_grievance_date_1_value' => array(
      'group' => '5',
      'filter_name' => 'field_grievance_date_1_value',
      'weight' => '-18',
    ),
    'group-field_grievance_id_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_id_value',
      'weight' => '-19',
    ),
    'group-field_grievance_tags_tid' => array(
      'group' => '2',
      'filter_name' => 'field_grievance_tags_tid',
      'weight' => '-18',
    ),
    'group-field_grievance_classaction_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_classaction_value',
      'weight' => '-18',
    ),
    'group-uid_2' => array(
      'group' => '3',
      'filter_name' => 'uid_2',
      'weight' => '-18',
    ),
    'group-field_grievance_broughtby_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_broughtby_tid',
      'weight' => '-17',
    ),
    'group-field_grievance_min_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_min_value',
      'weight' => '-18',
    ),
    'group-field_grievance_comments_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_comments_value',
      'weight' => '-16',
    ),
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Grievance Specialist';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Recommended Remedy (field_grievance_remedy) */
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['id'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['field'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator_id'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['label'] = 'Remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['identifier'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['vocabulary'] = 'grievance_remedies';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Violation (field_grievance_violation) */
  $handler->display->display_options['filters']['field_grievance_violation_value']['id'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['table'] = 'field_data_field_grievance_violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['field'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_violation_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_violation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator_id'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['label'] = 'Violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['identifier'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date of Grievance';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+10';
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_id_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance Number';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Shop Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';
  /* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'Class Action or General Grievance?';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['group'] = 1;
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Assignee';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_min_value']['group'] = 1;
  $handler->display->display_options['filters']['field_grievance_min_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator_id'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['label'] = 'Member ID #';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['identifier'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Comments (field_grievance_comments) */
  $handler->display->display_options['filters']['field_grievance_comments_value']['id'] = 'field_grievance_comments_value';
  $handler->display->display_options['filters']['field_grievance_comments_value']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['filters']['field_grievance_comments_value']['field'] = 'field_grievance_comments_value';
  $handler->display->display_options['filters']['field_grievance_comments_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_comments_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_comments_value']['expose']['operator_id'] = 'field_grievance_comments_value_op';
  $handler->display->display_options['filters']['field_grievance_comments_value']['expose']['label'] = 'Comments';
  $handler->display->display_options['filters']['field_grievance_comments_value']['expose']['operator'] = 'field_grievance_comments_value_op';
  $handler->display->display_options['filters']['field_grievance_comments_value']['expose']['identifier'] = 'field_grievance_comments_value';
  $handler->display->display_options['filters']['field_grievance_comments_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    8 => 0,
  );
  $handler->display->display_options['path'] = 'ignore/views-grievances-page';
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]'),
    t('Filed By'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('All Grievances'),
    t('The list below shows all grievances on the system.  Use the form to filer by grievant name, status, etc.'),
    t('No grievances match your search criteria.'),
    t('Grievance Specialist'),
    t('Remedy'),
    t('Violation'),
    t('Date of Grievance'),
    t('Grievance Number'),
    t('Shop Tags'),
    t('Class Action or General Grievance?'),
    t('Brought By'),
    t('Member ID #'),
    t('Comments'),
  );
  $export['grievances'] = $view;

  $view = new view();
  $view->name = 'grievances_mine';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'My Grievances';
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
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'created' => 'created',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing' => 'nothing',
    'nothing_4' => 'nothing_4',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_5' => 'nothing_5',
    'field_grievance_comments' => 'field_grievance_comments',
    'nothing_3' => 'nothing_3',
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
    'created' => array(
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
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_4' => array(
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
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_5' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  /* Field: Content: Employer */
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
  /* Field: Content: Recommended Statement of Grievance */
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
  /* Field: Taxonomy term: Actor */
  $handler->display->display_options['fields']['field_grievance_actor']['id'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['table'] = 'field_data_field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['field'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['relationship'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['fields']['field_grievance_actor']['label'] = 'Next Actor';
  /* Field: Content: Next Step */
  $handler->display->display_options['fields']['field_grievance_alert_tid']['id'] = 'field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['field'] = 'field_grievance_alert_tid';
  $handler->display->display_options['fields']['field_grievance_alert_tid']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Next Deadline */
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
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['sorts']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['sorts']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['sorts']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: My Grievances */
  $handler = $view->new_display('page', 'My Grievances', 'page_1');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance user';
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
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
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
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_5']['id'] = 'uid_current_5';
  $handler->display->display_options['filters']['uid_current_5']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_5']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_5']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_current_5']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_5']['group'] = 2;
  /* Filter criterion: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['value']['value'] = '999';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['group'] = 2;
  $handler->display->display_options['path'] = 'ignore/grievances-mine/page';
  $handler->display->display_options['menu']['title'] = 'My Unresolved Grievances';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Grievances';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Variant: Exclude Intake */
  $handler = $view->new_display('page', 'Variant: Exclude Intake', 'page_2');
  $handler->display->display_options['display_description'] = 'Exclude "Intake Staff" (aka "Filed By") from the list.';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance user';
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
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
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
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_5']['id'] = 'uid_current_5';
  $handler->display->display_options['filters']['uid_current_5']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_5']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_5']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_current_5']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_5']['group'] = 2;
  /* Filter criterion: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['value']['value'] = '999';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['group'] = 2;
  $handler->display->display_options['path'] = 'ignore/grievances-mine/page-2';
  $handler->display->display_options['menu']['title'] = 'My Unresolved Grievances';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Grievances';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Tickler Report */
  $handler = $view->new_display('page', 'Tickler Report', 'page_3');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance user';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'break_row';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_grievance_shop',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing_1' => 'nothing_1',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing' => 'nothing',
    'nothing_4' => 'nothing_4',
    'field_grievance_status' => 'field_grievance_status',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_5' => 'nothing_5',
    'field_grievance_comments' => 'field_grievance_comments',
    'nothing_3' => 'nothing_3',
    'field_grievance_clauseref' => 'field_grievance_clauseref',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_alert_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_shop' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_first_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_last_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing_4' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing_5' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => '0',
      'separator2' => '',
      'colspan' => '',
    ),
    'field_grievance_clauseref' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'break' => 'field_grievance_clauseref',
      'separator2' => '',
      'colspan' => '7',
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
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
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
  $handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
  $handler->display->display_options['fields']['field_grievance_shop']['label'] = '';
  $handler->display->display_options['fields']['field_grievance_shop']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_shop']['element_label_colon'] = FALSE;
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
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Field: Content: CBA Provision(s) Violated */
  $handler->display->display_options['fields']['field_grievance_clauseref']['id'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['table'] = 'field_data_field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['field'] = 'field_grievance_clauseref';
  $handler->display->display_options['fields']['field_grievance_clauseref']['settings'] = array(
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_clauseref']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['field_grievance_clauseref']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_5']['id'] = 'uid_current_5';
  $handler->display->display_options['filters']['uid_current_5']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_5']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_5']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_current_5']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_5']['group'] = 2;
  /* Filter criterion: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['value']['value'] = '999';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['group'] = 2;
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = 'not empty';
  $handler->display->display_options['path'] = 'ignore/grievances-mine/page_3';
  $handler->display->display_options['menu']['title'] = 'My Unresolved Grievances';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Grievances';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['grievances_mine'] = array(
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Filed By'),
    t('Recommended Statement of Grievance'),
    t('Next Actor'),
    t('Next Step'),
    t('Next Deadline'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('My Grievances'),
    t('The list below includes only <strong>unresolved</strong> grievances which were filed by the <strong>current user</strong> (or for which the current user is the organizer, lead organizer, manager, or is watching, or filed by another organizer at your shop).'),
    t('There are no unreseloved grievances which are assigned to you, or which you are watching.'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]
'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Variant: Exclude Intake'),
    t('Exclude "Intake Staff" (aka "Filed By") from the list.'),
    t('Tickler Report'),
    t('CBA Provision(s) Violated'),
  );
  $export['grievances_mine'] = $view;

  $view = new view();
  $view->name = 'grievances_without_brought_by';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievances Without Brought By';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievances with no "Brought By"';
  $handler->display->display_options['use_ajax'] = TRUE;
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
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing' => 'nothing',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing_2' => 'nothing_2',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_3' => 'nothing_3',
    'field_grievance_comments' => 'field_grievance_comments',
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
    'field_grievance_id' => array(
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
    'nothing' => array(
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
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]';
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
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report missing broughtby';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'Status
Company Representatives
Union Representatives
Employee
Grievance Form',
    'group-field_grievance_first_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_first_name_value',
      'weight' => '-20',
    ),
    'group-field_grievance_last_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_last_name_value',
      'weight' => '-19',
    ),
    'group-field_grievance_status_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_status_tid',
      'weight' => '-19',
    ),
    'group-uid' => array(
      'group' => '2',
      'filter_name' => 'uid',
      'weight' => '-20',
    ),
    'group-uid_1' => array(
      'group' => '2',
      'filter_name' => 'uid_1',
      'weight' => '-19',
    ),
    'group-title' => array(
      'group' => '1',
      'filter_name' => 'title',
      'weight' => '-19',
    ),
    'group-title_1' => array(
      'group' => '1',
      'filter_name' => 'title_1',
      'weight' => '-18',
    ),
    'group-field_grievance_alert_date_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_alert_date_value',
      'weight' => '-18',
    ),
    'group-field_grievance_open_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_open_value',
      'weight' => '-17',
    ),
    'group-field_grievance_remedy_tid' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_remedy_tid',
      'weight' => '-13',
    ),
    'group-field_grievance_violation_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_violation_value',
      'weight' => '-14',
    ),
    'group-field_grievance_date_1_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_date_1_value',
      'weight' => '-12',
    ),
    'group-field_grievance_id_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_id_value',
      'weight' => '-16',
    ),
    'group-field_grievance_tags_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_tags_tid',
      'weight' => '-17',
    ),
    'group-field_grievance_classaction_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_classaction_value',
      'weight' => '-15',
    ),
    'group-uid_2' => array(
      'group' => '2',
      'filter_name' => 'uid_2',
      'weight' => '-17',
    ),
    'group-field_grievance_broughtby_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_broughtby_tid',
      'weight' => '-20',
    ),
    'group-field_grievance_min_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_min_value',
      'weight' => '-18',
    ),
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The list below shows grievances where the "Member ID" field is empty.  ';
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Grievance Specialist';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Recommended Remedy (field_grievance_remedy) */
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['id'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['field'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator_id'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['label'] = 'Remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['identifier'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['vocabulary'] = 'grievance_remedies';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Violation (field_grievance_violation) */
  $handler->display->display_options['filters']['field_grievance_violation_value']['id'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['table'] = 'field_data_field_grievance_violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['field'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_violation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator_id'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['label'] = 'Violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['identifier'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date of Grievance';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+10';
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance Number';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Shop Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';
  /* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'Class Action or General Grievance?';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Assignee';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_min_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator_id'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['label'] = 'Member ID #';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['identifier'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['id'] = 'field_grievance_broughtby_tid_1';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['operator'] = 'empty';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid_1']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['path'] = 'grievance/report/missing-broughtby';
  $handler->display->display_options['menu']['title'] = 'All Grievances';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['grievances_without_brought_by'] = array(
    t('Master'),
    t('Grievances with no "Brought By"'),
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]'),
    t('Filed By'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('All Grievances'),
    t('The list below shows grievances where the "Member ID" field is empty.  '),
    t('No grievances match your search criteria.'),
    t('Grievance Specialist'),
    t('Remedy'),
    t('Violation'),
    t('Date of Grievance'),
    t('Grievance Number'),
    t('Shop Tags'),
    t('Class Action or General Grievance?'),
    t('Brought By'),
    t('Member ID #'),
  );
  $export['grievances_without_brought_by'] = $view;

  $view = new view();
  $view->name = 'grievances_without_category';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievances Without Category';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievances with no Category';
  $handler->display->display_options['use_ajax'] = TRUE;
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
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing' => 'nothing',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing_2' => 'nothing_2',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_3' => 'nothing_3',
    'field_grievance_comments' => 'field_grievance_comments',
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
    'field_grievance_id' => array(
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
    'nothing' => array(
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
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]';
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
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report missing broughtby';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'Status
Company Representatives
Union Representatives
Employee
Grievance Form',
    'group-field_grievance_first_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_first_name_value',
      'weight' => '-20',
    ),
    'group-field_grievance_last_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_last_name_value',
      'weight' => '-19',
    ),
    'group-field_grievance_status_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_status_tid',
      'weight' => '-19',
    ),
    'group-uid' => array(
      'group' => '2',
      'filter_name' => 'uid',
      'weight' => '-20',
    ),
    'group-uid_1' => array(
      'group' => '2',
      'filter_name' => 'uid_1',
      'weight' => '-19',
    ),
    'group-title' => array(
      'group' => '1',
      'filter_name' => 'title',
      'weight' => '-19',
    ),
    'group-title_1' => array(
      'group' => '1',
      'filter_name' => 'title_1',
      'weight' => '-18',
    ),
    'group-field_grievance_alert_date_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_alert_date_value',
      'weight' => '-18',
    ),
    'group-field_grievance_open_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_open_value',
      'weight' => '-17',
    ),
    'group-field_grievance_remedy_tid' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_remedy_tid',
      'weight' => '-13',
    ),
    'group-field_grievance_violation_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_violation_value',
      'weight' => '-14',
    ),
    'group-field_grievance_date_1_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_date_1_value',
      'weight' => '-12',
    ),
    'group-field_grievance_id_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_id_value',
      'weight' => '-16',
    ),
    'group-field_grievance_tags_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_tags_tid',
      'weight' => '-17',
    ),
    'group-field_grievance_classaction_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_classaction_value',
      'weight' => '-15',
    ),
    'group-uid_2' => array(
      'group' => '2',
      'filter_name' => 'uid_2',
      'weight' => '-17',
    ),
    'group-field_grievance_broughtby_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_broughtby_tid',
      'weight' => '-20',
    ),
    'group-field_grievance_min_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_min_value',
      'weight' => '-18',
    ),
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The list below shows grievances where the "Member ID" field is empty.  ';
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Grievance Specialist';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Recommended Remedy (field_grievance_remedy) */
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['id'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['field'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator_id'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['label'] = 'Remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['identifier'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['vocabulary'] = 'grievance_remedies';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Violation (field_grievance_violation) */
  $handler->display->display_options['filters']['field_grievance_violation_value']['id'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['table'] = 'field_data_field_grievance_violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['field'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_violation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator_id'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['label'] = 'Violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['identifier'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date of Grievance';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+10';
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance Number';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Shop Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';
  /* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'Class Action or General Grievance?';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Assignee';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_min_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator_id'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['label'] = 'Member ID #';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['identifier'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Category (field_grievance_category) */
  $handler->display->display_options['filters']['field_grievance_category_tid']['id'] = 'field_grievance_category_tid';
  $handler->display->display_options['filters']['field_grievance_category_tid']['table'] = 'field_data_field_grievance_category';
  $handler->display->display_options['filters']['field_grievance_category_tid']['field'] = 'field_grievance_category_tid';
  $handler->display->display_options['filters']['field_grievance_category_tid']['operator'] = 'empty';
  $handler->display->display_options['filters']['field_grievance_category_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_category_tid']['vocabulary'] = 'grievance_category';
  $handler->display->display_options['path'] = 'grievance/report/missing-category';
  $handler->display->display_options['menu']['title'] = 'All Grievances';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['grievances_without_category'] = array(
    t('Master'),
    t('Grievances with no Category'),
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]'),
    t('Filed By'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('All Grievances'),
    t('The list below shows grievances where the "Member ID" field is empty.  '),
    t('No grievances match your search criteria.'),
    t('Grievance Specialist'),
    t('Remedy'),
    t('Violation'),
    t('Date of Grievance'),
    t('Grievance Number'),
    t('Shop Tags'),
    t('Class Action or General Grievance?'),
    t('Brought By'),
    t('Member ID #'),
  );
  $export['grievances_without_category'] = $view;

  $view = new view();
  $view->name = 'grievances_without_member_id';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievances Without Member ID';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievances with no Member ID';
  $handler->display->display_options['use_ajax'] = TRUE;
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
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing' => 'nothing',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing_2' => 'nothing_2',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_3' => 'nothing_3',
    'field_grievance_comments' => 'field_grievance_comments',
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
    'field_grievance_id' => array(
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
    'nothing' => array(
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
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]';
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
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report missing member id';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'Status
Company Representatives
Union Representatives
Employee
Grievance Form',
    'group-field_grievance_first_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_first_name_value',
      'weight' => '-20',
    ),
    'group-field_grievance_last_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_last_name_value',
      'weight' => '-19',
    ),
    'group-field_grievance_status_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_status_tid',
      'weight' => '-19',
    ),
    'group-uid' => array(
      'group' => '2',
      'filter_name' => 'uid',
      'weight' => '-20',
    ),
    'group-uid_1' => array(
      'group' => '2',
      'filter_name' => 'uid_1',
      'weight' => '-19',
    ),
    'group-title' => array(
      'group' => '1',
      'filter_name' => 'title',
      'weight' => '-19',
    ),
    'group-title_1' => array(
      'group' => '1',
      'filter_name' => 'title_1',
      'weight' => '-18',
    ),
    'group-field_grievance_alert_date_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_alert_date_value',
      'weight' => '-18',
    ),
    'group-field_grievance_open_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_open_value',
      'weight' => '-17',
    ),
    'group-field_grievance_remedy_tid' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_remedy_tid',
      'weight' => '-13',
    ),
    'group-field_grievance_violation_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_violation_value',
      'weight' => '-14',
    ),
    'group-field_grievance_date_1_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_date_1_value',
      'weight' => '-12',
    ),
    'group-field_grievance_id_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_id_value',
      'weight' => '-16',
    ),
    'group-field_grievance_tags_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_tags_tid',
      'weight' => '-17',
    ),
    'group-field_grievance_classaction_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_classaction_value',
      'weight' => '-15',
    ),
    'group-uid_2' => array(
      'group' => '2',
      'filter_name' => 'uid_2',
      'weight' => '-17',
    ),
    'group-field_grievance_broughtby_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_broughtby_tid',
      'weight' => '-20',
    ),
    'group-field_grievance_min_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_min_value',
      'weight' => '-18',
    ),
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The list below shows grievances where the "Member ID" field is empty.  ';
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Grievance Specialist';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Recommended Remedy (field_grievance_remedy) */
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['id'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['field'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator_id'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['label'] = 'Remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['identifier'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['vocabulary'] = 'grievance_remedies';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Violation (field_grievance_violation) */
  $handler->display->display_options['filters']['field_grievance_violation_value']['id'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['table'] = 'field_data_field_grievance_violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['field'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_violation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator_id'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['label'] = 'Violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['identifier'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date of Grievance';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+10';
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance Number';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Shop Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';
  /* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'Class Action or General Grievance?';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Assignee';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_min_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator_id'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['label'] = 'Member ID #';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['identifier'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value_1']['id'] = 'field_grievance_min_value_1';
  $handler->display->display_options['filters']['field_grievance_min_value_1']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value_1']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value_1']['operator'] = 'empty';
  $handler->display->display_options['path'] = 'grievance/report/missing-member-id';
  $handler->display->display_options['menu']['title'] = 'All Grievances';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['grievances_without_member_id'] = array(
    t('Master'),
    t('Grievances with no Member ID'),
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]'),
    t('Filed By'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('All Grievances'),
    t('The list below shows grievances where the "Member ID" field is empty.  '),
    t('No grievances match your search criteria.'),
    t('Grievance Specialist'),
    t('Remedy'),
    t('Violation'),
    t('Date of Grievance'),
    t('Grievance Number'),
    t('Shop Tags'),
    t('Class Action or General Grievance?'),
    t('Brought By'),
    t('Member ID #'),
  );
  $export['grievances_without_member_id'] = $view;

  $view = new view();
  $view->name = 'grievances_without_status';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Grievances Without Status';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Grievances with no Status';
  $handler->display->display_options['use_ajax'] = TRUE;
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
    'field_grievance_id' => 'field_grievance_id',
    'created' => 'created',
    'nothing' => 'nothing',
    'field_grievance_alert_date' => 'field_grievance_alert_date',
    'field_grievance_shop' => 'field_grievance_shop',
    'field_grievance_first_name' => 'field_grievance_first_name',
    'field_grievance_last_name' => 'field_grievance_last_name',
    'nothing_2' => 'nothing_2',
    'field_grievance_rep_filed' => 'field_grievance_rep_filed',
    'field_grievance_rep_assignee' => 'field_grievance_rep_assignee',
    'field_grievance_assignee_notes' => 'field_grievance_assignee_notes',
    'nothing_3' => 'nothing_3',
    'field_grievance_comments' => 'field_grievance_comments',
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
    'field_grievance_id' => array(
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
    'nothing' => array(
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
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_filed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_rep_assignee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_assignee_notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_grievance_comments' => array(
      'sortable' => 0,
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
  /* Relationship: Content: Next Step (field_grievance_alert_tid) */
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['id'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['table'] = 'field_data_field_grievance_alert_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['field'] = 'field_grievance_alert_tid_tid';
  $handler->display->display_options['relationships']['field_grievance_alert_tid_tid']['label'] = 'Alert';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['id'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['field'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['relationships']['field_grievance_rep_assignee_target_id']['label'] = 'Assignee';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Intake Date';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Intake Date / ID';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[created]<br>[field_grievance_id]';
  /* Field: Content: Next Deadline */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['label'] = 'Due Date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
    'format_type' => 'date_only_sortable',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Employer */
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
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Grievant';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]';
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
  /* Field: Content: Assignee */
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['id'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['table'] = 'field_data_field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['field'] = 'field_grievance_rep_assignee';
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_grievance_rep_assignee']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Assignee Notes */
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['id'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['table'] = 'field_data_field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['field'] = 'field_grievance_assignee_notes';
  $handler->display->display_options['fields']['field_grievance_assignee_notes']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Assigned To';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_grievance_comments']['id'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['table'] = 'field_data_field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['field'] = 'field_grievance_comments';
  $handler->display->display_options['fields']['field_grievance_comments']['label'] = 'Key Dates';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: All Grievances */
  $handler = $view->new_display('page', 'All Grievances', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance report';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'views_exposed_groups';
  $handler->display->display_options['exposed_form']['options']['views_exposed_groups'] = array(
    'format_groups' => 'vertical_tabs',
    'groups' => 'Status
Company Representatives
Union Representatives
Employee
Grievance Form',
    'group-field_grievance_first_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_first_name_value',
      'weight' => '-20',
    ),
    'group-field_grievance_last_name_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_last_name_value',
      'weight' => '-19',
    ),
    'group-field_grievance_status_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_status_tid',
      'weight' => '-19',
    ),
    'group-uid' => array(
      'group' => '2',
      'filter_name' => 'uid',
      'weight' => '-20',
    ),
    'group-uid_1' => array(
      'group' => '2',
      'filter_name' => 'uid_1',
      'weight' => '-19',
    ),
    'group-title' => array(
      'group' => '1',
      'filter_name' => 'title',
      'weight' => '-19',
    ),
    'group-title_1' => array(
      'group' => '1',
      'filter_name' => 'title_1',
      'weight' => '-18',
    ),
    'group-field_grievance_alert_date_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_alert_date_value',
      'weight' => '-18',
    ),
    'group-field_grievance_open_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_open_value',
      'weight' => '-17',
    ),
    'group-field_grievance_remedy_tid' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_remedy_tid',
      'weight' => '-13',
    ),
    'group-field_grievance_violation_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_violation_value',
      'weight' => '-14',
    ),
    'group-field_grievance_date_1_value' => array(
      'group' => '4',
      'filter_name' => 'field_grievance_date_1_value',
      'weight' => '-12',
    ),
    'group-field_grievance_id_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_id_value',
      'weight' => '-16',
    ),
    'group-field_grievance_tags_tid' => array(
      'group' => '1',
      'filter_name' => 'field_grievance_tags_tid',
      'weight' => '-17',
    ),
    'group-field_grievance_classaction_value' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_classaction_value',
      'weight' => '-15',
    ),
    'group-uid_2' => array(
      'group' => '2',
      'filter_name' => 'uid_2',
      'weight' => '-17',
    ),
    'group-field_grievance_broughtby_tid' => array(
      'group' => '0',
      'filter_name' => 'field_grievance_broughtby_tid',
      'weight' => '-20',
    ),
    'group-field_grievance_min_value' => array(
      'group' => '3',
      'filter_name' => 'field_grievance_min_value',
      'weight' => '-18',
    ),
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'The list below shows grievances where the "Status" field is empty.';
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
  $handler->display->display_options['filters']['field_grievance_status_tid']['operator'] = 'empty';
  $handler->display->display_options['filters']['field_grievance_status_tid']['group'] = 1;
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
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Grievance Specialist';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
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
  /* Filter criterion: Content: Next Deadline (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Union Deadline Before';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
  $handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
  $handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_grievance_open_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator_id'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['label'] = 'Show Unresolved Grievances?';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['operator'] = 'field_grievance_open_value_op';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['identifier'] = 'field_grievance_open_value';
  $handler->display->display_options['filters']['field_grievance_open_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Recommended Remedy (field_grievance_remedy) */
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['id'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['table'] = 'field_data_field_grievance_remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['field'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator_id'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['label'] = 'Remedy';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['operator'] = 'field_grievance_remedy_tid_op';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['identifier'] = 'field_grievance_remedy_tid';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['vocabulary'] = 'grievance_remedies';
  $handler->display->display_options['filters']['field_grievance_remedy_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Violation (field_grievance_violation) */
  $handler->display->display_options['filters']['field_grievance_violation_value']['id'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['table'] = 'field_data_field_grievance_violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['field'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_violation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator_id'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['label'] = 'Violation';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['operator'] = 'field_grievance_violation_value_op';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['identifier'] = 'field_grievance_violation_value';
  $handler->display->display_options['filters']['field_grievance_violation_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Date Received (field_grievance_date_1) */
  $handler->display->display_options['filters']['field_grievance_date_1_value']['id'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['table'] = 'field_data_field_grievance_date_1';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['field'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator_id'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['label'] = 'Date of Grievance';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['operator'] = 'field_grievance_date_1_value_op';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['identifier'] = 'field_grievance_date_1_value';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_date_1_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_date_1_value']['year_range'] = '-10:+10';
  /* Filter criterion: Content: ID (field_grievance_id) */
  $handler->display->display_options['filters']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['filters']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator_id'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['label'] = 'Grievance Number';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['operator'] = 'field_grievance_id_value_op';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['identifier'] = 'field_grievance_id_value';
  $handler->display->display_options['filters']['field_grievance_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Tags (field_grievance_tags) */
  $handler->display->display_options['filters']['field_grievance_tags_tid']['id'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['table'] = 'field_data_field_grievance_tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['field'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['relationship'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator_id'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['label'] = 'Shop Tags';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['operator'] = 'field_grievance_tags_tid_op';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['identifier'] = 'field_grievance_tags_tid';
  $handler->display->display_options['filters']['field_grievance_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_tags_tid']['vocabulary'] = 'grievance_tags';
  /* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
  $handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'Class Action or General Grievance?';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
  $handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_grievance_rep_assignee_target_id';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Assignee';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Brought By (field_grievance_broughtby) */
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
  $handler->display->display_options['filters']['field_grievance_broughtby_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Member ID # (field_grievance_min) */
  $handler->display->display_options['filters']['field_grievance_min_value']['id'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['table'] = 'field_data_field_grievance_min';
  $handler->display->display_options['filters']['field_grievance_min_value']['field'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_grievance_min_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator_id'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['label'] = 'Member ID #';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['operator'] = 'field_grievance_min_value_op';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['identifier'] = 'field_grievance_min_value';
  $handler->display->display_options['filters']['field_grievance_min_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    7 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'grievance/report/missing-status';
  $handler->display->display_options['menu']['title'] = 'All Grievances';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['grievances_without_status'] = array(
    t('Master'),
    t('Grievances with no Status'),
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
    t('Alert'),
    t('Assignee'),
    t('Nid'),
    t('ID'),
    t('Intake Date'),
    t('Intake Date / ID'),
    t('[created]<br>[field_grievance_id]'),
    t('Due Date'),
    t('Employer'),
    t('First Name'),
    t('Last Name'),
    t('Grievant'),
    t('[field_grievance_first_name] [field_grievance_last_name]<br>
[field_grievance_shop]'),
    t('Filed By'),
    t('Assignee Notes'),
    t('Assigned To'),
    t('[field_grievance_rep_assignee]<br>[field_grievance_assignee_notes]'),
    t('Key Dates'),
    t('Actions'),
    t('[<a href="/node/[nid]">view</a>]&nbsp;[<a href="/node/[nid]/edit">edit</a>]<br>
[<a href="/node/[nid]/statuslog">log</a>]&nbsp;[<a href="/node/[nid]/letter">letters</a>]'),
    t('Union Deadline Before'),
    t('Show Unresolved Grievances?'),
    t('All Grievances'),
    t('The list below shows grievances where the "Status" field is empty.'),
    t('No grievances match your search criteria.'),
    t('Grievance Specialist'),
    t('Remedy'),
    t('Violation'),
    t('Date of Grievance'),
    t('Grievance Number'),
    t('Shop Tags'),
    t('Class Action or General Grievance?'),
    t('Brought By'),
    t('Member ID #'),
  );
  $export['grievances_without_status'] = $view;

  return $export;
}
