<?php
/**
 * @file
 * grievance_content_types.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function grievance_content_types_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance_log|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance_log';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'author' => array(
      'weight' => '1',
      'label' => 'inline',
      'format' => 'author',
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Submitted By',
        ),
      ),
    ),
    'post_date' => array(
      'weight' => '0',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Date',
        ),
      ),
    ),
    'body' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fi' => TRUE,
          'fi-el' => 'pre',
          'fi-cl' => '',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_grievance_log_type' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Entry Type',
        ),
      ),
    ),
    'field_grievance_status' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance Status',
        ),
      ),
    ),
    'field_grievance_alert' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance Next Step',
        ),
      ),
    ),
    'field_grievance_alert_date' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance Next Deadline',
        ),
      ),
    ),
  );
  $export['node|grievance_log|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|grievance_gform';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'grievance_gform';
  $ds_fieldsetting->settings = array(
    'grievance_form_signature' => array(
      'weight' => '30',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'grievance_timeline' => array(
      'weight' => '1',
      'label' => 'inline',
      'format' => 'default',
    ),
    'field_grievance_remedy_other' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Remedy is to make grievant(s) whole in every way, including but not limited to',
        ),
      ),
    ),
    'field_grievance_remedy' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Remedy is to make grievant(s) whole in every way, including but not limited to',
        ),
      ),
    ),
    'field_grievance_violation' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'This violates the contract, including but not limited to Article(s)',
        ),
      ),
    ),
  );
  $export['node|grievance|grievance_gform'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function grievance_content_types_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'grievance_form_signature';
  $ds_field->label = 'Grievance Form Signature';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '<section class="field field-name-field-company-signature field-type-text clearfix view-mode-grievance_gform" style="padding-bottom: 20px; padding-top: 20px;">
<div class="field-items"><div class="field-item even" style="border-bottom: 2px dashed #000000; min-width: 600px;">&nbsp;</div></div>
<h2 class="field-label">Company Representative Signature / date</h2>
</section>

<section class="field field-name-field-union-signature field-type-text clearfix view-mode-grievance_gform" style="padding-bottom: 20px; padding-top: 20px;">
<div class="field-items"><div class="field-item even" style="border-bottom: 2px dashed #000000; min-width: 600px;">&nbsp;</div></div>
<h2 class="field-label">Union Representative Signature / date</h2>
</section>',
      'format' => 'php_code',
    ),
    'use_token' => 0,
  );
  $export['grievance_form_signature'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'grievance_timeline';
  $ds_field->label = 'Timeline';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'block' => 'views|grievance_timeline-block_1',
    'block_render' => '2',
  );
  $export['grievance_timeline'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function grievance_content_types_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance_log|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance_log';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'post_date',
        1 => 'author',
        2 => 'field_grievance_log_type',
        3 => 'field_grievance_status',
        4 => 'field_grievance_alert',
        5 => 'field_grievance_alert_date',
        6 => 'body',
        7 => 'field_grievance_attachments',
      ),
    ),
    'fields' => array(
      'post_date' => 'ds_content',
      'author' => 'ds_content',
      'field_grievance_log_type' => 'ds_content',
      'field_grievance_status' => 'ds_content',
      'field_grievance_alert' => 'ds_content',
      'field_grievance_alert_date' => 'ds_content',
      'body' => 'ds_content',
      'field_grievance_attachments' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_log|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_grievance_shop',
        1 => 'field_grievance_status',
        2 => 'field_grievance_alert',
        4 => 'field_grievance_alert_date',
      ),
      'right' => array(
        3 => 'field_grievance_supervisor_title',
        6 => 'field_grievance_supervisor_name',
        8 => 'field_grievance_supervisor_unit',
        9 => 'group_grievance_company',
        10 => 'group_grievance_supervisor',
        22 => 'field_grievance_co_name',
        25 => 'field_grievance_co_email',
        28 => 'field_grievance_co_phone',
        29 => 'field_grievance_co_address',
        31 => 'field_grievance_co_address_2',
        34 => 'field_grievance_co_city',
        39 => 'field_grievance_co_state',
        41 => 'field_grievance_co_zip',
      ),
      'left' => array(
        5 => 'group_grievance_union_rep',
        7 => 'group_grievance_employee',
        12 => 'field_grievance_first_name',
        13 => 'field_grievance_last_name',
        15 => 'field_grievance_phone',
        18 => 'field_grievance_email',
        21 => 'field_grievance_address',
        24 => 'field_grievance_address_2',
        27 => 'field_grievance_city',
        30 => 'field_grievance_state',
        32 => 'field_grievance_zip',
        33 => 'field_grievance_rep_filed',
        35 => 'field_grievance_hire_date',
        36 => 'field_grievance_rep_organizer',
        37 => 'field_grievance_rep_lead',
        38 => 'field_grievance_department',
        40 => 'field_grievance_rep_manager',
        42 => 'field_grievance_classification',
        43 => 'field_grievance_rep_watching',
      ),
      'footer' => array(
        11 => 'group_grievance_form',
        14 => 'group_grievance_comments',
        16 => 'group_grievance_arbitration',
        17 => 'field_grievance_comments',
        19 => 'group_grievance_timeline',
        20 => 'field_grievance_attachments_a',
        23 => 'field_grievance_attachments_b',
        26 => 'field_grievance_attachments',
        44 => 'field_grievance_date_1',
        45 => 'field_grievance_arbitration_a',
        46 => 'field_grievance_date_2',
        47 => 'field_grievance_arbitration_b',
        48 => 'field_grievance_arbitration_c',
        49 => 'field_grievance_arbitration_d',
        50 => 'field_grievance_type',
        51 => 'field_grievance_type_other',
        52 => 'field_grievance_violation',
        53 => 'field_grievance_remedy',
        54 => 'field_grievance_remedy_other',
      ),
    ),
    'fields' => array(
      'field_grievance_shop' => 'header',
      'field_grievance_status' => 'header',
      'field_grievance_alert' => 'header',
      'field_grievance_supervisor_title' => 'right',
      'field_grievance_alert_date' => 'header',
      'group_grievance_union_rep' => 'left',
      'field_grievance_supervisor_name' => 'right',
      'group_grievance_employee' => 'left',
      'field_grievance_supervisor_unit' => 'right',
      'group_grievance_company' => 'right',
      'group_grievance_supervisor' => 'right',
      'group_grievance_form' => 'footer',
      'field_grievance_first_name' => 'left',
      'field_grievance_last_name' => 'left',
      'group_grievance_comments' => 'footer',
      'field_grievance_phone' => 'left',
      'group_grievance_arbitration' => 'footer',
      'field_grievance_comments' => 'footer',
      'field_grievance_email' => 'left',
      'group_grievance_timeline' => 'footer',
      'field_grievance_attachments_a' => 'footer',
      'field_grievance_address' => 'left',
      'field_grievance_co_name' => 'right',
      'field_grievance_attachments_b' => 'footer',
      'field_grievance_address_2' => 'left',
      'field_grievance_co_email' => 'right',
      'field_grievance_attachments' => 'footer',
      'field_grievance_city' => 'left',
      'field_grievance_co_phone' => 'right',
      'field_grievance_co_address' => 'right',
      'field_grievance_state' => 'left',
      'field_grievance_co_address_2' => 'right',
      'field_grievance_zip' => 'left',
      'field_grievance_rep_filed' => 'left',
      'field_grievance_co_city' => 'right',
      'field_grievance_hire_date' => 'left',
      'field_grievance_rep_organizer' => 'left',
      'field_grievance_rep_lead' => 'left',
      'field_grievance_department' => 'left',
      'field_grievance_co_state' => 'right',
      'field_grievance_rep_manager' => 'left',
      'field_grievance_co_zip' => 'right',
      'field_grievance_classification' => 'left',
      'field_grievance_rep_watching' => 'left',
      'field_grievance_date_1' => 'footer',
      'field_grievance_arbitration_a' => 'footer',
      'field_grievance_date_2' => 'footer',
      'field_grievance_arbitration_b' => 'footer',
      'field_grievance_arbitration_c' => 'footer',
      'field_grievance_arbitration_d' => 'footer',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_violation' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_remedy_other' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance|grievance_arbitrationform';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance';
  $ds_layout->view_mode = 'grievance_arbitrationform';
  $ds_layout->layout = 'two_column_bricks';
  $ds_layout->settings = array(
    'regions' => array(
      'footer' => array(
        0 => 'field_grievance_date_1',
        1 => 'field_grievance_date_2',
        2 => 'field_grievance_type',
        3 => 'field_grievance_type_other',
        5 => 'field_grievance_violation',
        8 => 'field_grievance_remedy',
        12 => 'field_grievance_remedy_other',
        13 => 'group_grievance_e',
        16 => 'group_grievance_f',
        19 => 'group_grievance_g',
        21 => 'group_grievance_h',
        30 => 'field_grievance_attachments_a',
        31 => 'field_grievance_attachments_b',
        32 => 'field_grievance_arbitration_a',
        33 => 'field_grievance_arbitration_b',
        34 => 'field_grievance_arbitration_c',
        35 => 'field_grievance_attachments',
        36 => 'field_grievance_arbitration_d',
      ),
      'above_left' => array(
        4 => 'group_grievance_b',
        37 => 'field_grievance_rep_filed',
        38 => 'field_grievance_rep_organizer',
        39 => 'field_grievance_rep_lead',
        40 => 'field_grievance_rep_manager',
        41 => 'field_grievance_rep_watching',
      ),
      'above_right' => array(
        6 => 'group_grievance_a',
        42 => 'field_grievance_shop',
        43 => 'field_grievance_co_name',
        44 => 'field_grievance_co_address',
        45 => 'field_grievance_co_address_2',
        46 => 'field_grievance_co_city',
        47 => 'field_grievance_co_state',
        48 => 'field_grievance_co_zip',
        49 => 'field_grievance_co_email',
        50 => 'field_grievance_co_phone',
      ),
      'below_left' => array(
        7 => 'group_grievance_c',
        9 => 'field_grievance_first_name',
        15 => 'field_grievance_last_name',
        18 => 'field_grievance_address',
        20 => 'field_grievance_address_2',
        22 => 'field_grievance_city',
        23 => 'field_grievance_state',
        24 => 'field_grievance_zip',
        25 => 'field_grievance_phone',
        26 => 'field_grievance_email',
        27 => 'field_grievance_hire_date',
        28 => 'field_grievance_classification',
        29 => 'field_grievance_department',
      ),
      'below_right' => array(
        10 => 'group_grievance_d',
        11 => 'field_grievance_supervisor_name',
        14 => 'field_grievance_supervisor_title',
        17 => 'field_grievance_supervisor_unit',
      ),
    ),
    'fields' => array(
      'field_grievance_date_1' => 'footer',
      'field_grievance_date_2' => 'footer',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'group_grievance_b' => 'above_left',
      'field_grievance_violation' => 'footer',
      'group_grievance_a' => 'above_right',
      'group_grievance_c' => 'below_left',
      'field_grievance_remedy' => 'footer',
      'field_grievance_first_name' => 'below_left',
      'group_grievance_d' => 'below_right',
      'field_grievance_supervisor_name' => 'below_right',
      'field_grievance_remedy_other' => 'footer',
      'group_grievance_e' => 'footer',
      'field_grievance_supervisor_title' => 'below_right',
      'field_grievance_last_name' => 'below_left',
      'group_grievance_f' => 'footer',
      'field_grievance_supervisor_unit' => 'below_right',
      'field_grievance_address' => 'below_left',
      'group_grievance_g' => 'footer',
      'field_grievance_address_2' => 'below_left',
      'group_grievance_h' => 'footer',
      'field_grievance_city' => 'below_left',
      'field_grievance_state' => 'below_left',
      'field_grievance_zip' => 'below_left',
      'field_grievance_phone' => 'below_left',
      'field_grievance_email' => 'below_left',
      'field_grievance_hire_date' => 'below_left',
      'field_grievance_classification' => 'below_left',
      'field_grievance_department' => 'below_left',
      'field_grievance_attachments_a' => 'footer',
      'field_grievance_attachments_b' => 'footer',
      'field_grievance_arbitration_a' => 'footer',
      'field_grievance_arbitration_b' => 'footer',
      'field_grievance_arbitration_c' => 'footer',
      'field_grievance_attachments' => 'footer',
      'field_grievance_arbitration_d' => 'footer',
      'field_grievance_rep_filed' => 'above_left',
      'field_grievance_rep_organizer' => 'above_left',
      'field_grievance_rep_lead' => 'above_left',
      'field_grievance_rep_manager' => 'above_left',
      'field_grievance_rep_watching' => 'above_left',
      'field_grievance_shop' => 'above_right',
      'field_grievance_co_name' => 'above_right',
      'field_grievance_co_address' => 'above_right',
      'field_grievance_co_address_2' => 'above_right',
      'field_grievance_co_city' => 'above_right',
      'field_grievance_co_state' => 'above_right',
      'field_grievance_co_zip' => 'above_right',
      'field_grievance_co_email' => 'above_right',
      'field_grievance_co_phone' => 'above_right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'above_left' => 'div',
      'above_right' => 'div',
      'middle' => 'div',
      'below_left' => 'div',
      'below_right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance|grievance_arbitrationform'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance|grievance_gform';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance';
  $ds_layout->view_mode = 'grievance_gform';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_grievance_date_1',
        1 => 'grievance_timeline',
      ),
      'left' => array(
        2 => 'field_grievance_shop',
        3 => 'field_grievance_co_name',
        4 => 'field_grievance_co_address',
        5 => 'field_grievance_co_address_2',
        6 => 'field_grievance_co_city',
        7 => 'field_grievance_co_state',
        8 => 'field_grievance_co_zip',
        9 => 'field_grievance_co_email',
        10 => 'field_grievance_co_phone',
      ),
      'right' => array(
        11 => 'field_grievance_first_name',
        12 => 'field_grievance_last_name',
        13 => 'field_grievance_address',
        14 => 'field_grievance_address_2',
        15 => 'field_grievance_city',
        16 => 'field_grievance_state',
        17 => 'field_grievance_zip',
        18 => 'field_grievance_phone',
        19 => 'field_grievance_email',
        20 => 'field_grievance_classification',
        21 => 'field_grievance_department',
        22 => 'field_grievance_supervisor_name',
        23 => 'field_grievance_supervisor_title',
        24 => 'field_grievance_supervisor_unit',
      ),
      'footer' => array(
        25 => 'field_grievance_violation',
        26 => 'field_grievance_type',
        27 => 'field_grievance_type_other',
        28 => 'field_grievance_remedy',
        29 => 'field_grievance_remedy_other',
        30 => 'grievance_form_signature',
      ),
    ),
    'fields' => array(
      'field_grievance_date_1' => 'header',
      'grievance_timeline' => 'header',
      'field_grievance_shop' => 'left',
      'field_grievance_co_name' => 'left',
      'field_grievance_co_address' => 'left',
      'field_grievance_co_address_2' => 'left',
      'field_grievance_co_city' => 'left',
      'field_grievance_co_state' => 'left',
      'field_grievance_co_zip' => 'left',
      'field_grievance_co_email' => 'left',
      'field_grievance_co_phone' => 'left',
      'field_grievance_first_name' => 'right',
      'field_grievance_last_name' => 'right',
      'field_grievance_address' => 'right',
      'field_grievance_address_2' => 'right',
      'field_grievance_city' => 'right',
      'field_grievance_state' => 'right',
      'field_grievance_zip' => 'right',
      'field_grievance_phone' => 'right',
      'field_grievance_email' => 'right',
      'field_grievance_classification' => 'right',
      'field_grievance_department' => 'right',
      'field_grievance_supervisor_name' => 'right',
      'field_grievance_supervisor_title' => 'right',
      'field_grievance_supervisor_unit' => 'right',
      'field_grievance_violation' => 'footer',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'grievance_form_signature' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance|grievance_gform'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function grievance_content_types_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'grievance_arbitrationform';
  $ds_view_mode->label = 'Grievance Arbitration Form';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['grievance_arbitrationform'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'grievance_gform';
  $ds_view_mode->label = 'Grievance Form';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['grievance_gform'] = $ds_view_mode;

  return $export;
}
