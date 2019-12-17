<?php

/**
 * @file
 * grievance_content_types.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function grievance_content_types_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sirius_departments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Sirius Departments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Departments';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius dispatch admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'tid',
    'name' => 'name',
    'field_sirius_name_short' => 'field_sirius_name_short',
    'field_sirius_open_editable' => 'field_sirius_open_editable',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
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
    'field_sirius_name_short' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_sirius_open_editable' => array(
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
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['content'] = '<a href="/admin/structure/taxonomy/grievance_department/add?destination=[current_path]" class="action-button">Add a Department</a>';
  $handler->display->display_options['header']['area_text_custom']['tokenize'] = TRUE;
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Field: Industry */
  $handler->display->display_options['fields']['field_sirius_industry']['id'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['table'] = 'field_data_field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['field'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_sirius_industry']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_industry']['delta_offset'] = '0';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Field: ID */
  $handler->display->display_options['fields']['field_sirius_id']['id'] = 'field_sirius_id';
  $handler->display->display_options['fields']['field_sirius_id']['table'] = 'field_data_field_sirius_id';
  $handler->display->display_options['fields']['field_sirius_id']['field'] = 'field_sirius_id';
  $handler->display->display_options['fields']['field_sirius_id']['label'] = 'External ID';
  /* Field: Taxonomy term: Available for Dispatch? */
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['id'] = 'field_sirius_dispatch_available';
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['table'] = 'field_data_field_sirius_dispatch_available';
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['field'] = 'field_sirius_dispatch_available';
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['label'] = 'Use for Dispatch?';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=[current_path]" class="tool-button">Edit</a>';
  /* Sort criterion: Field: Industry (field_sirius_industry) */
  $handler->display->display_options['sorts']['field_sirius_industry_tid']['id'] = 'field_sirius_industry_tid';
  $handler->display->display_options['sorts']['field_sirius_industry_tid']['table'] = 'field_data_field_sirius_industry';
  $handler->display->display_options['sorts']['field_sirius_industry_tid']['field'] = 'field_sirius_industry_tid';
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'grievance_department' => 'grievance_department',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'sirius/admin/departments';
  $translatables['sirius_departments'] = array(
    t('Master'),
    t('Departments'),
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
    t('<a href="/admin/structure/taxonomy/grievance_department/add?destination=[current_path]" class="action-button">Add a Department</a>'),
    t('Current Path'),
    t('Term ID'),
    t('.'),
    t('Industry'),
    t('Name'),
    t('External ID'),
    t('Use for Dispatch?'),
    t('Tools'),
    t('<a href="/taxonomy/term/[tid]/edit?destination=[current_path]" class="tool-button">Edit</a>'),
    t('Page'),
  );
  $export['sirius_departments'] = $view;

  $view = new view();
  $view->name = 'sirius_event_participants';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius Event Participants';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius Event Participants';
  $handler->display->display_options['css_class'] = 'sirius_view_multitable';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'current_path' => 'current_path',
    'nid' => 'nid',
    'field_sirius_contact' => 'field_sirius_contact',
    'field_event_prole' => 'field_event_prole',
    'field_sirius_event_pstatus' => 'field_sirius_event_pstatus',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'current_path' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_sirius_contact' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_event_prole' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_sirius_event_pstatus' => array(
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
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No participants found.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_sirius_contact_target_id']['id'] = 'field_sirius_contact_target_id';
  $handler->display->display_options['relationships']['field_sirius_contact_target_id']['table'] = 'field_data_field_sirius_contact';
  $handler->display->display_options['relationships']['field_sirius_contact_target_id']['field'] = 'field_sirius_contact_target_id';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::sirius_event_participant_status' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'skip_permission_check' => 1,
      'override_label' => 1,
      'label' => 'Change Status',
    ),
  );
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Field: Contact */
  $handler->display->display_options['fields']['field_sirius_contact']['id'] = 'field_sirius_contact';
  $handler->display->display_options['fields']['field_sirius_contact']['table'] = 'field_data_field_sirius_contact';
  $handler->display->display_options['fields']['field_sirius_contact']['field'] = 'field_sirius_contact';
  $handler->display->display_options['fields']['field_sirius_contact']['settings'] = array(
    'bypass_access' => 1,
    'link' => 0,
  );
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_sirius_event_pstatus']['id'] = 'field_sirius_event_pstatus';
  $handler->display->display_options['fields']['field_sirius_event_pstatus']['table'] = 'field_data_field_sirius_event_pstatus';
  $handler->display->display_options['fields']['field_sirius_event_pstatus']['field'] = 'field_sirius_event_pstatus';
  $handler->display->display_options['fields']['field_sirius_event_pstatus']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Start Date / Time */
  $handler->display->display_options['fields']['field_sirius_datetime']['id'] = 'field_sirius_datetime';
  $handler->display->display_options['fields']['field_sirius_datetime']['table'] = 'field_data_field_sirius_datetime';
  $handler->display->display_options['fields']['field_sirius_datetime']['field'] = 'field_sirius_datetime';
  $handler->display->display_options['fields']['field_sirius_datetime']['label'] = 'Registration Date';
  $handler->display->display_options['fields']['field_sirius_datetime']['settings'] = array(
    'format_type' => 'date_and_time_sortable',
    'custom_date_format' => '',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]/edit?destination=[current_path]" class="tool-button">Edit</a>';
  /* Sort criterion: Content: Status (field_sirius_event_pstatus) */
  $handler->display->display_options['sorts']['field_sirius_event_pstatus_tid']['id'] = 'field_sirius_event_pstatus_tid';
  $handler->display->display_options['sorts']['field_sirius_event_pstatus_tid']['table'] = 'field_data_field_sirius_event_pstatus';
  $handler->display->display_options['sorts']['field_sirius_event_pstatus_tid']['field'] = 'field_sirius_event_pstatus_tid';
  /* Sort criterion: Name Components: Family */
  $handler->display->display_options['sorts']['field_sirius_name_family']['id'] = 'field_sirius_name_family';
  $handler->display->display_options['sorts']['field_sirius_name_family']['table'] = 'field_data_field_sirius_name';
  $handler->display->display_options['sorts']['field_sirius_name_family']['field'] = 'field_sirius_name_family';
  $handler->display->display_options['sorts']['field_sirius_name_family']['relationship'] = 'field_sirius_contact_target_id';
  /* Sort criterion: Name Components: Given */
  $handler->display->display_options['sorts']['field_sirius_name_given']['id'] = 'field_sirius_name_given';
  $handler->display->display_options['sorts']['field_sirius_name_given']['table'] = 'field_data_field_sirius_name';
  $handler->display->display_options['sorts']['field_sirius_name_given']['field'] = 'field_sirius_name_given';
  $handler->display->display_options['sorts']['field_sirius_name_given']['relationship'] = 'field_sirius_contact_target_id';
  /* Sort criterion: Name Components: Middle name(s) */
  $handler->display->display_options['sorts']['field_sirius_name_middle']['id'] = 'field_sirius_name_middle';
  $handler->display->display_options['sorts']['field_sirius_name_middle']['table'] = 'field_data_field_sirius_name';
  $handler->display->display_options['sorts']['field_sirius_name_middle']['field'] = 'field_sirius_name_middle';
  $handler->display->display_options['sorts']['field_sirius_name_middle']['relationship'] = 'field_sirius_contact_target_id';
  /* Contextual filter: Content: Event (field_sirius_event) */
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['id'] = 'field_sirius_event_target_id';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['table'] = 'field_data_field_sirius_event';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['field'] = 'field_sirius_event_target_id';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_event_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Role (field_sirius_event_prole) */
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['id'] = 'field_sirius_event_prole_tid';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['table'] = 'field_data_field_sirius_event_prole';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['field'] = 'field_sirius_event_prole_tid';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_event_prole_tid']['summary_options']['items_per_page'] = '25';
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
    'sirius_event_participant' => 'sirius_event_participant',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius event admin';
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
  $handler->display->display_options['path'] = 'node/%/sirius_event_participants/csv/%';
  $handler->display->display_options['displays'] = array(
    'block' => 'block',
    'default' => 0,
  );
  $translatables['sirius_event_participants'] = array(
    t('Master'),
    t('Sirius Event Participants'),
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
    t('No participants found.'),
    t('Content entity referenced from field_sirius_contact'),
    t('Content'),
    t('- Choose an operation -'),
    t('Change Status'),
    t('Current Path'),
    t('Nid'),
    t('Contact'),
    t('Status'),
    t('Registration Date'),
    t('Tools'),
    t('<a href="/node/[nid]/edit?destination=[current_path]" class="tool-button">Edit</a>'),
    t('All'),
    t('Block'),
    t('Data export'),
  );
  $export['sirius_event_participants'] = $view;

  $view = new view();
  $view->name = 'sirius_events';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Sirius Events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'sirius';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no events of this type.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_sirius_daterepeat']['id'] = 'field_sirius_daterepeat';
  $handler->display->display_options['fields']['field_sirius_daterepeat']['table'] = 'field_data_field_sirius_daterepeat';
  $handler->display->display_options['fields']['field_sirius_daterepeat']['field'] = 'field_sirius_daterepeat';
  $handler->display->display_options['fields']['field_sirius_daterepeat']['settings'] = array(
    'format_type' => 'short',
    'custom_date_format' => '',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_daterepeat']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['field_sirius_daterepeat']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_sirius_daterepeat']['multi_type'] = 'ul';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Tools';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]/sirius_event_schedule" class="tool-button" target="_blank">Schedule</a>
<a href="/node/[nid]/sirius_event_participants_current" class="tool-button" target="_blank">Participants</a>
<a href="/node/[nid]/sirius_event_participants_register" class="tool-button" target="_blank">Register</a>
';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Field: Domain (field_sirius_domain) */
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['id'] = 'field_sirius_domain_target_id';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['table'] = 'field_data_field_sirius_domain';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['field'] = 'field_sirius_domain_target_id';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Type (field_sirius_event_type) */
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['id'] = 'field_sirius_event_type_tid';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['table'] = 'field_data_field_sirius_event_type';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['field'] = 'field_sirius_event_type_tid';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_event_type_tid']['summary_options']['items_per_page'] = '25';
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
    'sirius_event' => 'sirius_event',
  );

  /* Display: All */
  $handler = $view->new_display('block', 'All', 'block_1');

  /* Display: Upcoming */
  $handler = $view->new_display('block', 'Upcoming', 'block_2');
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'sirius_event' => 'sirius_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Date -  start date (field_sirius_daterepeat) */
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['id'] = 'field_sirius_daterepeat_value';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['table'] = 'field_data_field_sirius_daterepeat';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['field'] = 'field_sirius_daterepeat_value';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['group'] = 2;
  $handler->display->display_options['filters']['field_sirius_daterepeat_value']['default_date'] = 'now';
  /* Filter criterion: Content: Date - end date (field_sirius_daterepeat:value2) */
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['id'] = 'field_sirius_daterepeat_value2';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['table'] = 'field_data_field_sirius_daterepeat';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['field'] = 'field_sirius_daterepeat_value2';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['operator'] = '>=';
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['group'] = 2;
  $handler->display->display_options['filters']['field_sirius_daterepeat_value2']['default_date'] = 'now';
  $translatables['sirius_events'] = array(
    t('Master'),
    t('Events'),
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
    t('There are no events of this type.'),
    t('Nid'),
    t('Title'),
    t('Date'),
    t('Tools'),
    t('<a href="/node/[nid]/sirius_event_schedule" class="tool-button" target="_blank">Schedule</a>
<a href="/node/[nid]/sirius_event_participants_current" class="tool-button" target="_blank">Participants</a>
<a href="/node/[nid]/sirius_event_participants_register" class="tool-button" target="_blank">Register</a>
'),
    t('All'),
    t('Upcoming'),
  );
  $export['sirius_events'] = $view;

  $view = new view();
  $view->name = 'sirius_taxonomy';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Sirius Taxonomy';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirius Taxonomy';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'tid',
    'weight' => 'weight',
    'hdepth' => 'hdepth',
    'expression' => 'expression',
    'name' => 'name',
    'current_path' => 'current_path',
    'nothing' => 'nothing',
    'hweight' => 'hweight',
  );
  $handler->display->display_options['style_options']['class'] = '';
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'weight' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hdepth' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expression' => array(
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
    'current_path' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hweight' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['footer']['area']['content'] = '<a href="/admin/structure/taxonomy/!1/add?destination=sirius/taxonomy/!1&sirius_domain=!2" class="tool-button">Add a New Term</a>';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  $handler->display->display_options['footer']['area']['tokenize'] = TRUE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';
  /* Sort criterion: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['sorts']['hweight']['id'] = 'hweight';
  $handler->display->display_options['sorts']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['hweight']['field'] = 'hweight';
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Contextual filter: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['arguments']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['arguments']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['arguments']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['arguments']['machine_name']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['machine_name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['machine_name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['machine_name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['machine_name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['machine_name']['limit'] = '0';
  /* Contextual filter: Field: Domain (field_sirius_domain) */
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['id'] = 'field_sirius_domain_target_id';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['table'] = 'field_data_field_sirius_domain';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['field'] = 'field_sirius_domain_target_id';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['default_argument_options']['argument'] = 'CURRENT_DOMAIN';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_sirius_domain_target_id']['summary_options']['items_per_page'] = '25';

  /* Display: Generic Block */
  $handler = $view->new_display('block', 'Generic Block', 'block');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Term Reference */
  $handler = $view->new_display('term_reference', 'Term Reference', 'term_reference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'term_reference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'name' => 'name',
    'tid' => 0,
    'weight' => 0,
    'current_path' => 0,
    'nothing' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'term_reference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;

  /* Display: Skills */
  $handler = $view->new_display('block', 'Skills', 'block_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Abbreviation */
  $handler->display->display_options['fields']['field_sirius_name_short']['id'] = 'field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['field'] = 'field_sirius_name_short';
  /* Field: Field: Industry */
  $handler->display->display_options['fields']['field_sirius_industry']['id'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['table'] = 'field_data_field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['field'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_sirius_industry']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_industry']['delta_offset'] = '0';
  /* Field: Field: External ID */
  $handler->display->display_options['fields']['field_grievance_external_id']['id'] = 'field_grievance_external_id';
  $handler->display->display_options['fields']['field_grievance_external_id']['table'] = 'field_data_field_grievance_external_id';
  $handler->display->display_options['fields']['field_grievance_external_id']['field'] = 'field_grievance_external_id';
  /* Field: Field: ID */
  $handler->display->display_options['fields']['field_sirius_id']['id'] = 'field_sirius_id';
  $handler->display->display_options['fields']['field_sirius_id']['table'] = 'field_data_field_sirius_id';
  $handler->display->display_options['fields']['field_sirius_id']['field'] = 'field_sirius_id';
  /* Field: Field: Number of Days */
  $handler->display->display_options['fields']['field_grievance_days']['id'] = 'field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['table'] = 'field_data_field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['field'] = 'field_grievance_days';
  $handler->display->display_options['fields']['field_grievance_days']['label'] = 'Certification Duration';
  $handler->display->display_options['fields']['field_grievance_days']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Alert Types */
  $handler = $view->new_display('block', 'Alert Types', 'block_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Taxonomy term: Actor */
  $handler->display->display_options['fields']['field_grievance_actor']['id'] = 'field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['table'] = 'field_data_field_grievance_actor';
  $handler->display->display_options['fields']['field_grievance_actor']['field'] = 'field_grievance_actor';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Tags */
  $handler = $view->new_display('block', 'Tags', 'block_3');

  /* Display: Departments */
  $handler = $view->new_display('block', 'Departments', 'block_4');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Industry */
  $handler->display->display_options['fields']['field_sirius_industry']['id'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['table'] = 'field_data_field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['field'] = 'field_sirius_industry';
  $handler->display->display_options['fields']['field_sirius_industry']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_sirius_industry']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_industry']['delta_offset'] = '0';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Doc Types */
  $handler = $view->new_display('block', 'Doc Types', 'block_5');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Information Request? */
  $handler->display->display_options['fields']['field_grievance_can_ir']['id'] = 'field_grievance_can_ir';
  $handler->display->display_options['fields']['field_grievance_can_ir']['table'] = 'field_data_field_grievance_can_ir';
  $handler->display->display_options['fields']['field_grievance_can_ir']['field'] = 'field_grievance_can_ir';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Log Types */
  $handler = $view->new_display('block', 'Log Types', 'block_6');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Taxonomy term: Show on Timeline? */
  $handler->display->display_options['fields']['field_grievance_timeline_show']['id'] = 'field_grievance_timeline_show';
  $handler->display->display_options['fields']['field_grievance_timeline_show']['table'] = 'field_data_field_grievance_timeline_show';
  $handler->display->display_options['fields']['field_grievance_timeline_show']['field'] = 'field_grievance_timeline_show';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';
  /* Field: Field: Roles */
  $handler->display->display_options['fields']['field_grievance_roles']['id'] = 'field_grievance_roles';
  $handler->display->display_options['fields']['field_grievance_roles']['table'] = 'field_data_field_grievance_roles';
  $handler->display->display_options['fields']['field_grievance_roles']['field'] = 'field_grievance_roles';
  $handler->display->display_options['fields']['field_grievance_roles']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_grievance_roles']['delta_offset'] = '0';

  /* Display: Industries */
  $handler = $view->new_display('block', 'Industries', 'block_7');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Abbreviation */
  $handler->display->display_options['fields']['field_sirius_name_short']['id'] = 'field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['table'] = 'field_data_field_sirius_name_short';
  $handler->display->display_options['fields']['field_sirius_name_short']['field'] = 'field_sirius_name_short';
  /* Field: Field: Default Dispatch Notes */
  $handler->display->display_options['fields']['field_sirius_dispatch_notesdef']['id'] = 'field_sirius_dispatch_notesdef';
  $handler->display->display_options['fields']['field_sirius_dispatch_notesdef']['table'] = 'field_data_field_sirius_dispatch_notesdef';
  $handler->display->display_options['fields']['field_sirius_dispatch_notesdef']['field'] = 'field_sirius_dispatch_notesdef';
  /* Field: Taxonomy term: Days to Retain Position */
  $handler->display->display_options['fields']['field_sirius_dispatch_days']['id'] = 'field_sirius_dispatch_days';
  $handler->display->display_options['fields']['field_sirius_dispatch_days']['table'] = 'field_data_field_sirius_dispatch_days';
  $handler->display->display_options['fields']['field_sirius_dispatch_days']['field'] = 'field_sirius_dispatch_days';
  $handler->display->display_options['fields']['field_sirius_dispatch_days']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Taxonomy term: Threestrikes Enable */
  $handler->display->display_options['fields']['field_sirius_dispatch_3sen']['id'] = 'field_sirius_dispatch_3sen';
  $handler->display->display_options['fields']['field_sirius_dispatch_3sen']['table'] = 'field_data_field_sirius_dispatch_3sen';
  $handler->display->display_options['fields']['field_sirius_dispatch_3sen']['field'] = 'field_sirius_dispatch_3sen';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Work Status */
  $handler = $view->new_display('block', 'Work Status', 'block_8');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Available for Dispatch? */
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['id'] = 'field_sirius_dispatch_available';
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['table'] = 'field_data_field_sirius_dispatch_available';
  $handler->display->display_options['fields']['field_sirius_dispatch_available']['field'] = 'field_sirius_dispatch_available';
  /* Field: Taxonomy term: Member? */
  $handler->display->display_options['fields']['field_sirius_member_active']['id'] = 'field_sirius_member_active';
  $handler->display->display_options['fields']['field_sirius_member_active']['table'] = 'field_data_field_sirius_member_active';
  $handler->display->display_options['fields']['field_sirius_member_active']['field'] = 'field_sirius_member_active';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';

  /* Display: Participant Statuses */
  $handler = $view->new_display('block', 'Participant Statuses', 'block_9');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical depth */
  $handler->display->display_options['fields']['hdepth']['id'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hdepth']['field'] = 'hdepth';
  $handler->display->display_options['fields']['hdepth']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hdepth']['separator'] = '';
  /* Field: Taxonomy term: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['weight']['separator'] = '';
  /* Field: Taxonomy term: Hierarchical weight */
  $handler->display->display_options['fields']['hweight']['id'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['hweight']['field'] = 'hweight';
  $handler->display->display_options['fields']['hweight']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hweight']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[hdepth]*35';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Taxonomy term: Participant Roles */
  $handler->display->display_options['fields']['field_sirius_event_proles']['id'] = 'field_sirius_event_proles';
  $handler->display->display_options['fields']['field_sirius_event_proles']['table'] = 'field_data_field_sirius_event_proles';
  $handler->display->display_options['fields']['field_sirius_event_proles']['field'] = 'field_sirius_event_proles';
  $handler->display->display_options['fields']['field_sirius_event_proles']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_sirius_event_proles']['settings'] = array(
    'field_delimiter' => '',
  );
  $handler->display->display_options['fields']['field_sirius_event_proles']['delta_offset'] = '0';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Global: Current Path */
  $handler->display->display_options['fields']['current_path']['id'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['table'] = 'views';
  $handler->display->display_options['fields']['current_path']['field'] = 'current_path';
  $handler->display->display_options['fields']['current_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['current_path']['qs_support_fieldset'] = array(
    'query_string_support' => NULL,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>';
  $translatables['sirius_taxonomy'] = array(
    t('Master'),
    t('Sirius Taxonomy'),
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
    t('<a href="/admin/structure/taxonomy/!1/add?destination=sirius/taxonomy/!1&sirius_domain=!2" class="tool-button">Add a New Term</a>'),
    t('Term ID'),
    t('.'),
    t('Hierarchical depth'),
    t('Weight'),
    t('Hierarchical weight'),
    t('Math expression'),
    t(','),
    t('Name'),
    t('<div style="display: inline-block; width: [expression]px;">&nbsp;</div>[name]'),
    t('Description'),
    t('Current Path'),
    t('Actions'),
    t('<a href="/taxonomy/term/[tid]/edit?destination=sirius/taxonomy/!1" class="tool-button">Edit</a>'),
    t('All'),
    t('Generic Block'),
    t('Term Reference'),
    t('Skills'),
    t('Abbreviation'),
    t('Industry'),
    t('External ID'),
    t('ID'),
    t('Certification Duration'),
    t('Alert Types'),
    t('Actor'),
    t('Tags'),
    t('Departments'),
    t('Doc Types'),
    t('Information Request?'),
    t('Log Types'),
    t('Show on Timeline?'),
    t('Roles'),
    t('Industries'),
    t('Default Dispatch Notes'),
    t('Days to Retain Position'),
    t('Threestrikes Enable'),
    t('Work Status'),
    t('Available for Dispatch?'),
    t('Member?'),
    t('Participant Statuses'),
    t('Participant Roles'),
  );
  $export['sirius_taxonomy'] = $view;

  return $export;
}
