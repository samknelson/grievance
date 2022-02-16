<?php

/**
 * @file
 * hotelsdb_content_types.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function hotelsdb_content_types_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hdb_airports';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'HDB Airports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'HDB Airports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'unite hotels admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/hdb-airport" class="tool-button">Add an Airport</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Field: Abbreviation */
  $handler->display->display_options['fields']['field_sirius_name_short']['id'] = 'field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['field'] = 'field_sirius_name_short';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Canonical Address */
  $handler->display->display_options['fields']['field_address_canon']['id'] = 'field_address_canon';
  $handler->display->display_options['fields']['field_address_canon']['table'] = 'field_data_field_address_canon';
  $handler->display->display_options['fields']['field_address_canon']['field'] = 'field_address_canon';
  $handler->display->display_options['fields']['field_address_canon']['label'] = 'Address';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>
<a href="/node/[nid]/edit" class="tool-button">Edit</a>';
  /* Sort criterion: Field: Abbreviation (field_sirius_name_short) */
  $handler->display->display_options['sorts']['field_sirius_name_short_value']['id'] = 'field_sirius_name_short_value';
  $handler->display->display_options['sorts']['field_sirius_name_short_value']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['sorts']['field_sirius_name_short_value']['field'] = 'field_sirius_name_short_value';
  /* Filter criterion: Content: Published status */
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
    'hdb_airport' => 'hdb_airport',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Field: Abbreviation (field_sirius_name_short) */
  $handler->display->display_options['filters']['field_sirius_name_short_value']['id'] = 'field_sirius_name_short_value';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['field'] = 'field_sirius_name_short_value';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['group'] = 1;
  $handler->display->display_options['filters']['field_sirius_name_short_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['operator_id'] = 'field_sirius_name_short_value_op';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['label'] = 'Abbreviation';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['operator'] = 'field_sirius_name_short_value_op';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['identifier'] = 'field_sirius_name_short_value';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_field'] = 'field_sirius_name_short';
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['field_sirius_name_short_value']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['description'] = '(substring ok)';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['title']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['title']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['title']['expose']['autocomplete_field'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['title']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['title']['expose']['autocomplete_dependent'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'airports';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'airports/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'nid' => 'nid',
    'field_sirius_name_short' => 'field_sirius_name_short',
    'title' => 'title',
    'field_address_canon' => 0,
    'nothing' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_sirius_name_short' => 'field_sirius_name_short',
    'title' => 'title',
  );
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Field: Abbreviation */
  $handler->display->display_options['fields']['field_sirius_name_short']['id'] = 'field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['field'] = 'field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['label'] = '';
  $handler->display->display_options['fields']['field_sirius_name_short']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published status */
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
    'hdb_airport' => 'hdb_airport',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $export['hdb_airports'] = $view;

  return $export;
}
