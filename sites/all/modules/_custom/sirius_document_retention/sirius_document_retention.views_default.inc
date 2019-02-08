<?php
/**
 * @file
 * sirius_document_retention.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sirius_document_retention_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sirius_document_retention_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius Document Retention Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius Document Retention Log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius document retention admin';
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
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Key */
  $handler->display->display_options['fields']['field_sirius_docret_key']['id'] = 'field_sirius_docret_key';
  $handler->display->display_options['fields']['field_sirius_docret_key']['table'] = 'field_data_field_sirius_docret_key';
  $handler->display->display_options['fields']['field_sirius_docret_key']['field'] = 'field_sirius_docret_key';
  /* Field: Content: Mode */
  $handler->display->display_options['fields']['field_sirius_docret_mode']['id'] = 'field_sirius_docret_mode';
  $handler->display->display_options['fields']['field_sirius_docret_mode']['table'] = 'field_data_field_sirius_docret_mode';
  $handler->display->display_options['fields']['field_sirius_docret_mode']['field'] = 'field_sirius_docret_mode';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[<a href="/node/[nid]">View</a>]';
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
    'sirius_document_retention_log' => 'sirius_document_retention_log',
  );
  /* Filter criterion: Content: Key (field_sirius_docret_key) */
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['id'] = 'field_sirius_docret_key_value';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['table'] = 'field_data_field_sirius_docret_key';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['field'] = 'field_sirius_docret_key_value';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['expose']['operator_id'] = 'field_sirius_docret_key_value_op';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['expose']['label'] = 'Key';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['expose']['operator'] = 'field_sirius_docret_key_value_op';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['expose']['identifier'] = 'field_sirius_docret_key_value';
  $handler->display->display_options['filters']['field_sirius_docret_key_value']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Mode (field_sirius_docret_mode) */
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['id'] = 'field_sirius_docret_mode_value';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['table'] = 'field_data_field_sirius_docret_mode';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['field'] = 'field_sirius_docret_mode_value';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['expose']['operator_id'] = 'field_sirius_docret_mode_value_op';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['expose']['label'] = 'Mode';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['expose']['operator'] = 'field_sirius_docret_mode_value_op';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['expose']['identifier'] = 'field_sirius_docret_mode_value';
  $handler->display->display_options['filters']['field_sirius_docret_mode_value']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['path'] = 'sirius/document-retention/log';
  $translatables['sirius_document_retention_log'] = array(
    t('Master'),
    t('Sirius Document Retention Log'),
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
    t('Created'),
    t('Key'),
    t('Mode'),
    t('Tools'),
    t('[<a href="/node/[nid]">View</a>]'),
    t('Page'),
  );
  $export['sirius_document_retention_log'] = $view;

  return $export;
}
