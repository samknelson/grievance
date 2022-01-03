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
  $view->name = 'bulk_log_entry_deadline_extension';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Bulk Log Entry / Deadline Extension';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bulk Log Entry / Deadline Extension';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'grievance admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'input_required' => 0,
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
    'sort' => array(
      'bef_format' => 'default',
      'advanced' => array(
        'collapsible' => 0,
        'collapsible_label' => 'Sort options',
        'combine' => 0,
        'combine_param' => 'sort_bef_combine',
        'combine_rewrite' => '',
        'reset' => 0,
        'reset_label' => '',
        'is_secondary' => 0,
        'autosubmit' => 0,
      ),
    ),
  );
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<strong>This tool is for bulk adding log entries to grievance records.</strong>  The most common usage is extending multiple deadlines at once.  Select each grievance you want to add the log to, then click the Grievance Log button.  Be sure you know what type of log entry you want to use, usually "Grievance - Timeline Extended".';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<br><br>Displaying @start - @end of @total grievances.';
  /* Relationship: Content: Status (field_grievance_status) */
  $handler->display->display_options['relationships']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['relationships']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
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
    'action::grievance_vbo_log' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'skip_permission_check' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: ID */
  $handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
  $handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
  /* Field: Content: Next Deadline Date */
  $handler->display->display_options['fields']['field_grievance_alert_date']['id'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['field'] = 'field_grievance_alert_date';
  $handler->display->display_options['fields']['field_grievance_alert_date']['settings'] = array(
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
  $handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
  );
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
  /* Field: Content: First Name */
  $handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
  $handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
  $handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
  /* Sort criterion: Content: Next Deadline Date (field_grievance_alert_date) */
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['order'] = 'DESC';
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_grievance_alert_date_value']['expose']['label'] = 'Next Deadline Date';
  /* Sort criterion: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['sorts']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['sorts']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['sorts']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['sorts']['field_grievance_shop_target_id']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_grievance_shop_target_id']['expose']['label'] = 'Employer';
  /* Sort criterion: Content: Last Name (field_grievance_last_name) */
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['id'] = 'field_grievance_last_name_value';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['table'] = 'field_data_field_grievance_last_name';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['field'] = 'field_grievance_last_name_value';
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_grievance_last_name_value']['expose']['label'] = 'Last Name';
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
    'grievance' => 'grievance',
  );
  /* Filter criterion: Content: Employer (field_grievance_shop) */
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['operator_id'] = 'field_grievance_shop_target_id_op';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['label'] = 'Employer';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['operator'] = 'field_grievance_shop_target_id_op';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['identifier'] = 'field_grievance_shop_target_id';
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_shop_target_id']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Next Deadline Date (field_grievance_alert_date) */
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['id'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['table'] = 'field_data_field_grievance_alert_date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['field'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator_id'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['label'] = 'Next Deadline Date';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['description'] = 'between; enter both dates.';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['operator'] = 'field_grievance_alert_date_value_op';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['identifier'] = 'field_grievance_alert_date_value';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_grievance_alert_date_value']['year_range'] = '2010:+1';
  /* Filter criterion: Content: Status (field_grievance_status) */
  $handler->display->display_options['filters']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
  $handler->display->display_options['filters']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator_id'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['operator'] = 'field_grievance_status_tid_op';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['identifier'] = 'field_grievance_status_tid';
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_grievance_status_tid']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_grievance_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_grievance_status_tid']['vocabulary'] = 'grievance_status';
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
  $handler->display->display_options['path'] = 'sirius/grievance/bulk-log-entry-deadline-extension';
  $translatables['bulk_log_entry_deadline_extension'] = array(
    t('Master'),
    t('Bulk Log Entry / Deadline Extension'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Select any filter and click on Apply to see results'),
    t('Advanced options'),
    t('Sort options'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<strong>This tool is for bulk adding log entries to grievance records.</strong>  The most common usage is extending multiple deadlines at once.  Select each grievance you want to add the log to, then click the Grievance Log button.  Be sure you know what type of log entry you want to use, usually "Grievance - Timeline Extended".'),
    t('<br><br>Displaying @start - @end of @total grievances.'),
    t('term from field_grievance_status'),
    t('Content'),
    t('- Choose an operation -'),
    t('ID'),
    t('Next Deadline Date'),
    t('Employer'),
    t('Last Name'),
    t('First Name'),
    t('Status'),
    t('between; enter both dates.'),
    t('Page'),
  );
  $export['bulk_log_entry_deadline_extension'] = $view;

  return $export;
}
