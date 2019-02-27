<?php

/**
 * @file
 * sirius.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sirius_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sirius_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius Log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_sirius_category']['id'] = 'field_sirius_category';
  $handler->display->display_options['fields']['field_sirius_category']['table'] = 'field_data_field_sirius_category';
  $handler->display->display_options['fields']['field_sirius_category']['field'] = 'field_sirius_category';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['field_sirius_type']['id'] = 'field_sirius_type';
  $handler->display->display_options['fields']['field_sirius_type']['table'] = 'field_data_field_sirius_type';
  $handler->display->display_options['fields']['field_sirius_type']['field'] = 'field_sirius_type';
  /* Field: Content: Message */
  $handler->display->display_options['fields']['field_sirius_message']['id'] = 'field_sirius_message';
  $handler->display->display_options['fields']['field_sirius_message']['table'] = 'field_data_field_sirius_message';
  $handler->display->display_options['fields']['field_sirius_message']['field'] = 'field_sirius_message';
  $handler->display->display_options['fields']['field_sirius_message']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_sirius_message']['alter']['nl2br'] = TRUE;
  /* Field: Content: Attachments */
  $handler->display->display_options['fields']['field_sirius_attachments']['id'] = 'field_sirius_attachments';
  $handler->display->display_options['fields']['field_sirius_attachments']['table'] = 'field_data_field_sirius_attachments';
  $handler->display->display_options['fields']['field_sirius_attachments']['field'] = 'field_sirius_attachments';
  $handler->display->display_options['fields']['field_sirius_attachments']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_sirius_attachments']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_attachments']['delta_offset'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>';
  /* Sort criterion: Content: Nid */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'node';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
  /* Contextual filter: Content: Handler (field_sirius_log_handler) */
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['id'] = 'field_sirius_log_handler_target_id';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['table'] = 'field_data_field_sirius_log_handler';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['field'] = 'field_sirius_log_handler_target_id';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_log_handler_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Category (field_sirius_category) */
  $handler->display->display_options['arguments']['field_sirius_category_value']['id'] = 'field_sirius_category_value';
  $handler->display->display_options['arguments']['field_sirius_category_value']['table'] = 'field_data_field_sirius_category';
  $handler->display->display_options['arguments']['field_sirius_category_value']['field'] = 'field_sirius_category_value';
  $handler->display->display_options['arguments']['field_sirius_category_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_category_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_category_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_category_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_sirius_category_value']['limit'] = '0';
  $handler->display->display_options['arguments']['field_sirius_category_value']['break_phrase'] = TRUE;
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
    'sirius_log' => 'sirius_log',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['sirius_log'] = array(
    t('Master'),
    t('Sirius Log'),
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
    t('Nid'),
    t('Date'),
    t('Category'),
    t('Type'),
    t('Message'),
    t('Attachments'),
    t('Tools'),
    t('<a href="/node/[nid]" class="tool-button">View</a>'),
    t('All'),
    t('Block'),
  );
  $export['sirius_log'] = $view;

  $view = new view();
  $view->name = 'sirius_log_admin';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius Log Admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius Log Admin';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_sirius_category']['id'] = 'field_sirius_category';
  $handler->display->display_options['fields']['field_sirius_category']['table'] = 'field_data_field_sirius_category';
  $handler->display->display_options['fields']['field_sirius_category']['field'] = 'field_sirius_category';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['field_sirius_type']['id'] = 'field_sirius_type';
  $handler->display->display_options['fields']['field_sirius_type']['table'] = 'field_data_field_sirius_type';
  $handler->display->display_options['fields']['field_sirius_type']['field'] = 'field_sirius_type';
  /* Field: Content: Message */
  $handler->display->display_options['fields']['field_sirius_message']['id'] = 'field_sirius_message';
  $handler->display->display_options['fields']['field_sirius_message']['table'] = 'field_data_field_sirius_message';
  $handler->display->display_options['fields']['field_sirius_message']['field'] = 'field_sirius_message';
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['field_sirius_notes']['id'] = 'field_sirius_notes';
  $handler->display->display_options['fields']['field_sirius_notes']['table'] = 'field_data_field_sirius_notes';
  $handler->display->display_options['fields']['field_sirius_notes']['field'] = 'field_sirius_notes';
  /* Field: Content: Handler */
  $handler->display->display_options['fields']['field_sirius_log_handler_1']['id'] = 'field_sirius_log_handler_1';
  $handler->display->display_options['fields']['field_sirius_log_handler_1']['table'] = 'field_data_field_sirius_log_handler';
  $handler->display->display_options['fields']['field_sirius_log_handler_1']['field'] = 'field_sirius_log_handler';
  $handler->display->display_options['fields']['field_sirius_log_handler_1']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_log_handler_1']['delta_offset'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['link_to_node'] = TRUE;
  /* Field: Content: JSON */
  $handler->display->display_options['fields']['field_sirius_json']['id'] = 'field_sirius_json';
  $handler->display->display_options['fields']['field_sirius_json']['table'] = 'field_data_field_sirius_json';
  $handler->display->display_options['fields']['field_sirius_json']['field'] = 'field_sirius_json';
  $handler->display->display_options['fields']['field_sirius_json']['type'] = 'sirius_jsonfield_formatter_single';
  $handler->display->display_options['fields']['field_sirius_json']['settings'] = array(
    'sub_name' => 'phone',
  );
  /* Sort criterion: Content: Nid */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'node';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
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
    'sirius_log' => 'sirius_log',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'sirius/log';
  $translatables['sirius_log_admin'] = array(
    t('Master'),
    t('Sirius Log Admin'),
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
    t('Post date'),
    t('Category'),
    t('Type'),
    t('Message'),
    t('Notes'),
    t('Handler'),
    t('Nid'),
    t('JSON'),
    t('Page'),
  );
  $export['sirius_log_admin'] = $view;

  $view = new view();
  $view->name = 'sirius_news';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius News';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius News';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'ds';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  $handler->display->display_options['row_options']['alternating'] = 0;
  $handler->display->display_options['row_options']['grouping'] = 0;
  $handler->display->display_options['row_options']['advanced'] = 0;
  $handler->display->display_options['row_options']['delta_fieldset']['delta_fields'] = array();
  $handler->display->display_options['row_options']['grouping_fieldset']['group_field'] = 'node|created';
  $handler->display->display_options['row_options']['default_fieldset']['view_mode'] = 'full';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Roles (field_sirius_roles) */
  $handler->display->display_options['arguments']['field_sirius_roles_value']['id'] = 'field_sirius_roles_value';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['table'] = 'field_data_field_sirius_roles';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['field'] = 'field_sirius_roles_value';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['default_argument_options']['argument'] = 'There is no news.';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_sirius_roles_value']['break_phrase'] = TRUE;
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
    'sirius_news' => 'sirius_news',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $translatables['sirius_news'] = array(
    t('Master'),
    t('Sirius News'),
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
    t('All'),
    t('Block'),
  );
  $export['sirius_news'] = $view;

  return $export;
}
