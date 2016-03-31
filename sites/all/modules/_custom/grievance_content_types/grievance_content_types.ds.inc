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
  $ds_fieldsetting->id = 'node|grievance|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'grievance_timeline' => array(
      'weight' => '21',
      'label' => 'inline',
      'format' => 'default',
    ),
  );
  $export['node|grievance|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|grievance_gform';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'grievance_gform';
  $ds_fieldsetting->settings = array(
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

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|grievance_gform_a';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'grievance_gform_a';
  $ds_fieldsetting->settings = array(
    'grievance_address_combined' => array(
      'weight' => '2',
      'label' => 'inline',
      'format' => 'default',
    ),
    'grievance_timeline' => array(
      'weight' => '10',
      'label' => 'inline',
      'format' => 'default',
    ),
    'grievant_name' => array(
      'weight' => '0',
      'label' => 'inline',
      'format' => 'default',
    ),
    'field_grievance_shop' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Employer',
        ),
      ),
    ),
    'field_grievance_supervisor_name' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'SUPERVISOR OR FOREMAN INVOLVED',
        ),
      ),
    ),
    'field_grievance_rep_organizer' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'STEWARD INVOLVED',
        ),
      ),
    ),
    'field_grievance_rep_lead' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'BUSINESS AGENT INVOLVED',
        ),
      ),
    ),
    'field_grievance_co_name' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'REPORTED TO',
        ),
      ),
    ),
    'field_grievance_remedy_other' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'REMEDY SOUGHT',
        ),
      ),
    ),
    'field_grievance_remedy' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'REMEDY SOUGHT',
        ),
      ),
    ),
    'field_grievance_type_other' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'FACTS OF THE CASE',
        ),
      ),
    ),
    'field_grievance_violation' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'ARTICLES OF THE CONTRACT VIOLATED',
        ),
      ),
    ),
    'field_grievance_inforeq' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'INFORMATION REQUEST',
        ),
      ),
    ),
  );
  $export['node|grievance|grievance_gform_a'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function grievance_content_types_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'grievance_address_combined';
  $ds_field->label = 'Grievant\'s Address';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '[node:field_grievance_address]<br />
[node:field_grievance_city], [node:field_grievance_state] [node:field_grievance_zip]',
      'format' => 'full_html',
    ),
    'use_token' => 1,
  );
  $export['grievance_address_combined'] = $ds_field;

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

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'grievant_name';
  $ds_field->label = 'Grievant Name';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '[node:field_grievance_first_name] [node:field_grievance_last_name]
',
      'format' => 'full_html',
    ),
    'use_token' => 1,
  );
  $export['grievant_name'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function grievance_content_types_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance_attachment|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance_attachment';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_grievance_document_type',
        1 => 'field_grievance_attachments',
      ),
    ),
    'fields' => array(
      'field_grievance_document_type' => 'ds_content',
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
  $export['node|grievance_attachment|default'] = $ds_layout;

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
        3 => 'field_grievance_alert',
        5 => 'field_grievance_alert_date',
      ),
      'footer' => array(
        2 => 'field_grievance_arbitration_a',
        4 => 'field_grievance_arbitration_b',
        8 => 'field_grievance_arbitration_c',
        12 => 'field_grievance_arbitration_d',
        15 => 'group_grievance_form',
        17 => 'group_grievance_comments',
        20 => 'group_grievance_arbitration',
        21 => 'group_grievance_timeline',
        23 => 'field_grievance_comments',
        46 => 'field_grievance_date_1',
        47 => 'grievance_timeline',
        48 => 'field_grievance_type',
        49 => 'field_grievance_type_other',
        50 => 'field_grievance_violation',
        51 => 'field_grievance_remedy',
        52 => 'field_grievance_remedy_other',
        53 => 'field_grievance_inforeq',
      ),
      'right' => array(
        6 => 'field_grievance_supervisor_title',
        9 => 'field_grievance_supervisor_name',
        11 => 'field_grievance_supervisor_unit',
        13 => 'group_grievance_company',
        14 => 'group_grievance_supervisor',
        24 => 'field_grievance_co_name',
        26 => 'field_grievance_co_email',
        29 => 'field_grievance_co_phone',
        30 => 'field_grievance_co_address',
        33 => 'field_grievance_co_address_2',
        37 => 'field_grievance_co_city',
        40 => 'field_grievance_co_state',
        41 => 'field_grievance_co_zip',
      ),
      'left' => array(
        7 => 'group_grievance_union_rep',
        10 => 'group_grievance_employee',
        16 => 'field_grievance_first_name',
        18 => 'field_grievance_last_name',
        19 => 'field_grievance_phone',
        22 => 'field_grievance_email',
        25 => 'field_grievance_address',
        27 => 'field_grievance_address_2',
        28 => 'field_grievance_city',
        31 => 'field_grievance_state',
        32 => 'field_grievance_zip',
        34 => 'field_grievance_rep_filed',
        35 => 'field_grievance_rep_organizer',
        36 => 'field_grievance_hire_date',
        38 => 'field_grievance_department',
        39 => 'field_grievance_rep_lead',
        42 => 'field_grievance_classification',
        43 => 'field_grievance_rep_manager',
        44 => 'field_grievance_rep_watching',
        45 => 'field_grievance_ein',
      ),
    ),
    'fields' => array(
      'field_grievance_shop' => 'header',
      'field_grievance_status' => 'header',
      'field_grievance_arbitration_a' => 'footer',
      'field_grievance_alert' => 'header',
      'field_grievance_arbitration_b' => 'footer',
      'field_grievance_alert_date' => 'header',
      'field_grievance_supervisor_title' => 'right',
      'group_grievance_union_rep' => 'left',
      'field_grievance_arbitration_c' => 'footer',
      'field_grievance_supervisor_name' => 'right',
      'group_grievance_employee' => 'left',
      'field_grievance_supervisor_unit' => 'right',
      'field_grievance_arbitration_d' => 'footer',
      'group_grievance_company' => 'right',
      'group_grievance_supervisor' => 'right',
      'group_grievance_form' => 'footer',
      'field_grievance_first_name' => 'left',
      'group_grievance_comments' => 'footer',
      'field_grievance_last_name' => 'left',
      'field_grievance_phone' => 'left',
      'group_grievance_arbitration' => 'footer',
      'group_grievance_timeline' => 'footer',
      'field_grievance_email' => 'left',
      'field_grievance_comments' => 'footer',
      'field_grievance_co_name' => 'right',
      'field_grievance_address' => 'left',
      'field_grievance_co_email' => 'right',
      'field_grievance_address_2' => 'left',
      'field_grievance_city' => 'left',
      'field_grievance_co_phone' => 'right',
      'field_grievance_co_address' => 'right',
      'field_grievance_state' => 'left',
      'field_grievance_zip' => 'left',
      'field_grievance_co_address_2' => 'right',
      'field_grievance_rep_filed' => 'left',
      'field_grievance_rep_organizer' => 'left',
      'field_grievance_hire_date' => 'left',
      'field_grievance_co_city' => 'right',
      'field_grievance_department' => 'left',
      'field_grievance_rep_lead' => 'left',
      'field_grievance_co_state' => 'right',
      'field_grievance_co_zip' => 'right',
      'field_grievance_classification' => 'left',
      'field_grievance_rep_manager' => 'left',
      'field_grievance_rep_watching' => 'left',
      'field_grievance_ein' => 'left',
      'field_grievance_date_1' => 'footer',
      'grievance_timeline' => 'footer',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_violation' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'field_grievance_inforeq' => 'footer',
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
        2 => 'field_grievance_co_state',
        3 => 'field_grievance_co_zip',
        4 => 'field_grievance_co_email',
        5 => 'field_grievance_co_phone',
        6 => 'field_grievance_shop',
        7 => 'field_grievance_co_name',
        8 => 'field_grievance_co_address',
        9 => 'field_grievance_co_address_2',
        10 => 'field_grievance_co_city',
      ),
      'right' => array(
        11 => 'field_grievance_supervisor_name',
        12 => 'field_grievance_supervisor_title',
        13 => 'field_grievance_supervisor_unit',
        14 => 'field_grievance_first_name',
        15 => 'field_grievance_last_name',
        16 => 'field_grievance_address',
        17 => 'field_grievance_address_2',
        18 => 'field_grievance_city',
        19 => 'field_grievance_state',
        20 => 'field_grievance_zip',
        21 => 'field_grievance_phone',
        22 => 'field_grievance_email',
        23 => 'field_grievance_classification',
        24 => 'field_grievance_department',
      ),
      'footer' => array(
        25 => 'field_grievance_violation',
        26 => 'field_grievance_type',
        27 => 'field_grievance_type_other',
        28 => 'field_grievance_remedy',
        29 => 'field_grievance_remedy_other',
      ),
    ),
    'fields' => array(
      'field_grievance_date_1' => 'header',
      'grievance_timeline' => 'header',
      'field_grievance_co_state' => 'left',
      'field_grievance_co_zip' => 'left',
      'field_grievance_co_email' => 'left',
      'field_grievance_co_phone' => 'left',
      'field_grievance_shop' => 'left',
      'field_grievance_co_name' => 'left',
      'field_grievance_co_address' => 'left',
      'field_grievance_co_address_2' => 'left',
      'field_grievance_co_city' => 'left',
      'field_grievance_supervisor_name' => 'right',
      'field_grievance_supervisor_title' => 'right',
      'field_grievance_supervisor_unit' => 'right',
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
      'field_grievance_violation' => 'footer',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
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
  $export['node|grievance|grievance_gform'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance|grievance_gform_a';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance';
  $ds_layout->view_mode = 'grievance_gform_a';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'grievant_name',
        1 => 'field_grievance_shop',
        2 => 'grievance_address_combined',
        3 => 'field_grievance_classification',
      ),
      'right' => array(
        4 => 'field_grievance_date_1',
        5 => 'grievance_timeline',
        6 => 'field_grievance_ein',
        7 => 'field_grievance_hire_date',
        8 => 'field_grievance_phone',
      ),
      'footer' => array(
        9 => 'field_grievance_type_other',
        10 => 'field_grievance_violation',
        11 => 'field_grievance_remedy',
        12 => 'field_grievance_remedy_other',
        13 => 'field_grievance_supervisor_name',
        14 => 'field_grievance_co_name',
        15 => 'field_grievance_rep_organizer',
        16 => 'field_grievance_rep_lead',
        17 => 'field_grievance_inforeq',
      ),
    ),
    'fields' => array(
      'grievant_name' => 'left',
      'field_grievance_shop' => 'left',
      'grievance_address_combined' => 'left',
      'field_grievance_classification' => 'left',
      'field_grievance_date_1' => 'right',
      'grievance_timeline' => 'right',
      'field_grievance_ein' => 'right',
      'field_grievance_hire_date' => 'right',
      'field_grievance_phone' => 'right',
      'field_grievance_type_other' => 'footer',
      'field_grievance_violation' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'field_grievance_supervisor_name' => 'footer',
      'field_grievance_co_name' => 'footer',
      'field_grievance_rep_organizer' => 'footer',
      'field_grievance_rep_lead' => 'footer',
      'field_grievance_inforeq' => 'footer',
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
  $export['node|grievance|grievance_gform_a'] = $ds_layout;

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

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'grievance_gform_a';
  $ds_view_mode->label = 'Grievance Form A';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['grievance_gform_a'] = $ds_view_mode;

  return $export;
}
