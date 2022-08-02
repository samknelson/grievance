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
  $view->name = 'hdb_actors';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'HDB Actors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Actors';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'unite hotels user';
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Actor Type */
  $handler->display->display_options['fields']['field_actor_type']['id'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['table'] = 'field_data_field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['field'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Phone */
  $handler->display->display_options['fields']['field_phone']['id'] = 'field_phone';
  $handler->display->display_options['fields']['field_phone']['table'] = 'field_data_field_phone';
  $handler->display->display_options['fields']['field_phone']['field'] = 'field_phone';
  /* Field: Content: Raw Address - Thoroughfare (i.e. Street address) */
  $handler->display->display_options['fields']['field_address_thoroughfare']['id'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_thoroughfare']['field'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['label'] = 'Address';
  /* Field: Content: Raw Address - Premise (i.e. Apartment / Suite number) */
  $handler->display->display_options['fields']['field_address_premise']['id'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_premise']['field'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['label'] = 'Address 2';
  /* Field: Content: Raw Address - Locality (i.e. City) */
  $handler->display->display_options['fields']['field_address_locality']['id'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_locality']['field'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['label'] = 'City';
  /* Field: Content: Raw Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['fields']['field_address_administrative_area']['id'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_administrative_area']['field'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['label'] = 'State';
  $handler->display->display_options['fields']['field_address_administrative_area']['display_name'] = 1;
  /* Field: Content: Raw Address - Postal code */
  $handler->display->display_options['fields']['field_address_postal_code']['id'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_postal_code']['field'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['label'] = 'Postal Code';
  /* Field: Content: Raw Address - Country */
  $handler->display->display_options['fields']['field_address_country']['id'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_country']['field'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['label'] = 'Country';
  $handler->display->display_options['fields']['field_address_country']['display_name'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'actor' => 'actor',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Actor Type (field_actor_type) */
  $handler->display->display_options['filters']['field_actor_type_tid']['id'] = 'field_actor_type_tid';
  $handler->display->display_options['filters']['field_actor_type_tid']['table'] = 'field_data_field_actor_type';
  $handler->display->display_options['filters']['field_actor_type_tid']['field'] = 'field_actor_type_tid';
  $handler->display->display_options['filters']['field_actor_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_actor_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_actor_type_tid']['expose']['operator_id'] = 'field_actor_type_tid_op';
  $handler->display->display_options['filters']['field_actor_type_tid']['expose']['label'] = 'Actor Type';
  $handler->display->display_options['filters']['field_actor_type_tid']['expose']['operator'] = 'field_actor_type_tid_op';
  $handler->display->display_options['filters']['field_actor_type_tid']['expose']['identifier'] = 'field_actor_type_tid';
  $handler->display->display_options['filters']['field_actor_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_actor_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_actor_type_tid']['vocabulary'] = 'actor_type';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
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
  /* Filter criterion: Content: Canonical Title (field_title_canon) */
  $handler->display->display_options['filters']['field_title_canon_value']['id'] = 'field_title_canon_value';
  $handler->display->display_options['filters']['field_title_canon_value']['table'] = 'field_data_field_title_canon';
  $handler->display->display_options['filters']['field_title_canon_value']['field'] = 'field_title_canon_value';
  $handler->display->display_options['filters']['field_title_canon_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_title_canon_value']['group'] = 1;
  $handler->display->display_options['filters']['field_title_canon_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['operator_id'] = 'field_title_canon_value_op';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['label'] = 'Canonical Name';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['description'] = '(substring ok)';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['operator'] = 'field_title_canon_value_op';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['identifier'] = 'field_title_canon_value';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_field'] = 'field_title_canon';
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['field_title_canon_value']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Content: Phone (field_phone) */
  $handler->display->display_options['filters']['field_phone_value']['id'] = 'field_phone_value';
  $handler->display->display_options['filters']['field_phone_value']['table'] = 'field_data_field_phone';
  $handler->display->display_options['filters']['field_phone_value']['field'] = 'field_phone_value';
  $handler->display->display_options['filters']['field_phone_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_phone_value']['group'] = 1;
  $handler->display->display_options['filters']['field_phone_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_phone_value']['expose']['operator_id'] = 'field_phone_value_op';
  $handler->display->display_options['filters']['field_phone_value']['expose']['label'] = 'Phone';
  $handler->display->display_options['filters']['field_phone_value']['expose']['description'] = '(substring ok)';
  $handler->display->display_options['filters']['field_phone_value']['expose']['operator'] = 'field_phone_value_op';
  $handler->display->display_options['filters']['field_phone_value']['expose']['identifier'] = 'field_phone_value';
  $handler->display->display_options['filters']['field_phone_value']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_phone_value']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['field_phone_value']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['field_phone_value']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['field_phone_value']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['field_phone_value']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Content: Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['id'] = 'field_sirius_address_administrative_area';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['table'] = 'field_data_field_sirius_address';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['field'] = 'field_sirius_address_administrative_area';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['group'] = 1;
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['operator_id'] = 'field_sirius_address_administrative_area_op';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['label'] = 'State';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['operator'] = 'field_sirius_address_administrative_area_op';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['identifier'] = 'field_sirius_address_administrative_area';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['field_sirius_address_administrative_area']['expose']['autocomplete_dependent'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'actors/default';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Default';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Actors';
  $handler->display->display_options['tab_options']['weight'] = '0';

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
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Canonical Title */
  $handler->display->display_options['fields']['field_title_canon']['id'] = 'field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['table'] = 'field_data_field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['field'] = 'field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['label'] = 'Canonical Name';
  /* Field: Content: Actor Type */
  $handler->display->display_options['fields']['field_actor_type']['id'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['table'] = 'field_data_field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['field'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Phone */
  $handler->display->display_options['fields']['field_phone']['id'] = 'field_phone';
  $handler->display->display_options['fields']['field_phone']['table'] = 'field_data_field_phone';
  $handler->display->display_options['fields']['field_phone']['field'] = 'field_phone';
  /* Field: Content: Raw Address - Thoroughfare (i.e. Street address) */
  $handler->display->display_options['fields']['field_address_thoroughfare']['id'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_thoroughfare']['field'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['label'] = 'Address';
  /* Field: Content: Raw Address - Premise (i.e. Apartment / Suite number) */
  $handler->display->display_options['fields']['field_address_premise']['id'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_premise']['field'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['label'] = 'Address 2';
  /* Field: Content: Raw Address - Locality (i.e. City) */
  $handler->display->display_options['fields']['field_address_locality']['id'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_locality']['field'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['label'] = 'City';
  /* Field: Content: Raw Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['fields']['field_address_administrative_area']['id'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_administrative_area']['field'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['label'] = 'State';
  $handler->display->display_options['fields']['field_address_administrative_area']['display_name'] = 1;
  /* Field: Content: Raw Address - Postal code */
  $handler->display->display_options['fields']['field_address_postal_code']['id'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_postal_code']['field'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['label'] = 'Postal Code';
  /* Field: Content: Raw Address - Country */
  $handler->display->display_options['fields']['field_address_country']['id'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_country']['field'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['label'] = 'Country';
  $handler->display->display_options['fields']['field_address_country']['display_name'] = 0;
  $handler->display->display_options['path'] = 'actors/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
    'page_1' => 'page_1',
    'page_2' => 'page_2',
  );
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '500';

  /* Display: Resave */
  $handler = $view->new_display('page', 'Resave', 'page_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Update';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_save_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'skip_permission_check' => 1,
      'override_label' => 1,
      'label' => 'Re-save',
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Actor Type */
  $handler->display->display_options['fields']['field_actor_type']['id'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['table'] = 'field_data_field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['field'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Phone */
  $handler->display->display_options['fields']['field_phone']['id'] = 'field_phone';
  $handler->display->display_options['fields']['field_phone']['table'] = 'field_data_field_phone';
  $handler->display->display_options['fields']['field_phone']['field'] = 'field_phone';
  /* Field: Content: Raw Address - Thoroughfare (i.e. Street address) */
  $handler->display->display_options['fields']['field_address_thoroughfare']['id'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_thoroughfare']['field'] = 'field_address_thoroughfare';
  $handler->display->display_options['fields']['field_address_thoroughfare']['label'] = 'Address';
  /* Field: Content: Raw Address - Premise (i.e. Apartment / Suite number) */
  $handler->display->display_options['fields']['field_address_premise']['id'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_premise']['field'] = 'field_address_premise';
  $handler->display->display_options['fields']['field_address_premise']['label'] = 'Address 2';
  /* Field: Content: Raw Address - Locality (i.e. City) */
  $handler->display->display_options['fields']['field_address_locality']['id'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_locality']['field'] = 'field_address_locality';
  $handler->display->display_options['fields']['field_address_locality']['label'] = 'City';
  /* Field: Content: Raw Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['fields']['field_address_administrative_area']['id'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_administrative_area']['field'] = 'field_address_administrative_area';
  $handler->display->display_options['fields']['field_address_administrative_area']['label'] = 'State';
  $handler->display->display_options['fields']['field_address_administrative_area']['display_name'] = 1;
  /* Field: Content: Raw Address - Postal code */
  $handler->display->display_options['fields']['field_address_postal_code']['id'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_postal_code']['field'] = 'field_address_postal_code';
  $handler->display->display_options['fields']['field_address_postal_code']['label'] = 'Postal Code';
  /* Field: Content: Raw Address - Country */
  $handler->display->display_options['fields']['field_address_country']['id'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address_country']['field'] = 'field_address_country';
  $handler->display->display_options['fields']['field_address_country']['label'] = 'Country';
  $handler->display->display_options['fields']['field_address_country']['display_name'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>';
  $handler->display->display_options['path'] = 'actors/update';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Resave';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Canonical Names */
  $handler = $view->new_display('page', 'Canonical Names', 'page_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Canonical Title */
  $handler->display->display_options['fields']['field_title_canon']['id'] = 'field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['table'] = 'field_data_field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['field'] = 'field_title_canon';
  $handler->display->display_options['fields']['field_title_canon']['label'] = 'Canonical Name';
  /* Field: Content: Actor Type */
  $handler->display->display_options['fields']['field_actor_type']['id'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['table'] = 'field_data_field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['field'] = 'field_actor_type';
  $handler->display->display_options['fields']['field_actor_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>';
  $handler->display->display_options['path'] = 'actors/canonical';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Canonical Names';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['hdb_actors'] = $view;

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

  $view = new view();
  $view->name = 'hdb_convention_centers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'HDB Convention Centers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Convention Centers';
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
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/hdb-convention-center" class="tool-button">Add a Convention Center</a>';
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
    'hdb_convention_center' => 'hdb_convention_center',
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
  $handler->display->display_options['path'] = 'convention-centers';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'convention-centers/csv';
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
    'hdb_convention_center' => 'hdb_convention_center',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $export['hdb_convention_centers'] = $view;

  $view = new view();
  $view->name = 'hotelsdb_downloads';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'HotelsDB - Downloads';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Search Download Log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'unite hotels admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Content author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
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
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['field_sirius_notes']['id'] = 'field_sirius_notes';
  $handler->display->display_options['fields']['field_sirius_notes']['table'] = 'field_data_field_sirius_notes';
  $handler->display->display_options['fields']['field_sirius_notes']['field'] = 'field_sirius_notes';
  $handler->display->display_options['fields']['field_sirius_notes']['label'] = 'Count';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>
<a href="/mightysearch?saved=[nid]" class="tool-button">Load</a>';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'sirius_log' => 'sirius_log',
  );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Date';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Category (field_sirius_category) */
  $handler->display->display_options['filters']['field_sirius_category_value']['id'] = 'field_sirius_category_value';
  $handler->display->display_options['filters']['field_sirius_category_value']['table'] = 'field_data_field_sirius_category';
  $handler->display->display_options['filters']['field_sirius_category_value']['field'] = 'field_sirius_category_value';
  $handler->display->display_options['filters']['field_sirius_category_value']['value'] = 'hotelsdb';
  $handler->display->display_options['filters']['field_sirius_category_value']['expose']['operator_id'] = 'field_sirius_category_value_op';
  $handler->display->display_options['filters']['field_sirius_category_value']['expose']['label'] = 'Category (field_sirius_category)';
  $handler->display->display_options['filters']['field_sirius_category_value']['expose']['operator'] = 'field_sirius_category_value_op';
  $handler->display->display_options['filters']['field_sirius_category_value']['expose']['identifier'] = 'field_sirius_category_value';
  /* Filter criterion: Content: Type (field_sirius_type) */
  $handler->display->display_options['filters']['field_sirius_type_value']['id'] = 'field_sirius_type_value';
  $handler->display->display_options['filters']['field_sirius_type_value']['table'] = 'field_data_field_sirius_type';
  $handler->display->display_options['filters']['field_sirius_type_value']['field'] = 'field_sirius_type_value';
  $handler->display->display_options['filters']['field_sirius_type_value']['value'] = 'search:export';
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['description'] = '(substring ok)';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_field'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['mail']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'hotelsdb/search-download-log';

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
  $handler->display->display_options['path'] = 'hotelsdb/search-download-log/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $export['hotelsdb_downloads'] = $view;

  $view = new view();
  $view->name = 'recently_updated';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Recently Updated';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recently Updated';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Content author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  /* Relationship: Content revision: User */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node_revision';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Editor';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Created by';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Modified';
  $handler->display->display_options['fields']['changed']['date_format'] = 'raw time ago';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name_1']['label'] = 'Modified By';
  /* Field: Content: Link to edit content */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  /* Filter criterion: Content: Published status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created Between';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['operator'] = 'between';
  $handler->display->display_options['filters']['changed']['group'] = 1;
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Modified Between';
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  $handler->display->display_options['filters']['changed']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
  );
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'uid';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['group'] = 2;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Created By';
  $handler->display->display_options['filters']['name']['expose']['description'] = 'substring ok';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
  );
  $handler->display->display_options['filters']['name']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['name']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['name']['expose']['autocomplete_field'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['name']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['name']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name_1']['id'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['table'] = 'users';
  $handler->display->display_options['filters']['name_1']['field'] = 'name';
  $handler->display->display_options['filters']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['name_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_1']['group'] = 2;
  $handler->display->display_options['filters']['name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator_id'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['label'] = 'Most Recently Modified By';
  $handler->display->display_options['filters']['name_1']['expose']['description'] = 'substring ok';
  $handler->display->display_options['filters']['name_1']['expose']['operator'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['identifier'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
  );
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_field'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['name_1']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'actor' => 'actor',
    'hdb_airport' => 'hdb_airport',
    'hdb_convention_center' => 'hdb_convention_center',
    'property' => 'property',
    'property_actor' => 'property_actor',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'report/updated';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Report - Updated';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'report/updated/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );

  /* Display: Recently Updated By You */
  $handler = $view->new_display('block', 'Recently Updated By You', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recently Updated';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_1']['id'] = 'uid_current_1';
  $handler->display->display_options['filters']['uid_current_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_1']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_1']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['uid_current_1']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_1']['group'] = 2;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'actor' => 'actor',
    'page' => 'page',
    'contract' => 'contract',
    'local' => 'local',
    'property' => 'property',
  );
  $export['recently_updated'] = $view;

  return $export;
}
