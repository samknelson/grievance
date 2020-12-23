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
  $ds_fieldsetting->id = 'node|grievance_contract_clause|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance_contract_clause';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '2',
      'label' => 'inline',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'div',
        'class' => '',
        'ft' => array(
          'lb' => 'Brief Statement',
        ),
      ),
    ),
  );
  $export['node|grievance_contract_clause|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance_log|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance_log';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'edit_link' => array(
      'weight' => '9',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'author' => array(
      'weight' => '3',
      'label' => 'inline',
      'format' => 'author',
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Submitted By',
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
  );
  $export['node|grievance_log|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'grievance_timeline_short' => array(
      'weight' => '7',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'field_grievance_alert_waived' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Deadlines Waived As Of',
        ),
      ),
    ),
    'field_grievance_id' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance ID',
        ),
      ),
    ),
  );
  $export['node|grievance|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|grievance_form_226';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'grievance_form_226';
  $ds_fieldsetting->settings = array(
    'grievance_company_rep_ad_combine' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'grievant_name' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'field_grievance_rep_organizer' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance Specialist',
        ),
      ),
    ),
    'field_grievance_rep_lead' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Grievance Director',
        ),
      ),
    ),
    'field_grievance_clauseref' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'lb-el' => 'div',
          'lb-def-at' => TRUE,
          'lbw' => 1,
          'lbw-el' => 'h2',
          'lbw-cl' => 'field-label',
          'prefix' => '<div class="field-name-field-grievance-clauseref-wrapper">',
          'suffix' => 'And all other pertinent provisions of the Collective Bargaining Agreement.</div>',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => '',
          'ow-def-cl' => TRUE,
          'ow-at' => '',
          'ow-def-at' => TRUE,
          'fis' => TRUE,
          'fis-el' => 'div',
          'fis-cl' => '',
          'fis-at' => '',
          'fis-def-at' => TRUE,
          'fi' => TRUE,
          'fi-el' => 'div',
          'fi-cl' => '',
          'fi-at' => '',
          'fi-def-at' => TRUE,
          'fi-odd-even' => TRUE,
          'fi-first-last' => TRUE,
        ),
      ),
    ),
  );
  $export['node|grievance|grievance_form_226'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|grievance|grievance_gform';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'grievance';
  $ds_fieldsetting->view_mode = 'grievance_gform';
  $ds_fieldsetting->settings = array(
    'grievance_timeline_short' => array(
      'weight' => '0',
      'label' => 'hidden',
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
    'field_grievance_alert_waived' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Deadlines Waived As Of',
        ),
      ),
    ),
    'field_grievance_clauseref' => array(
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

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_contact|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_contact';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'field_sirius_address_geo' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|sirius_contact|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_dispatch_facility|sirius_worker';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_dispatch_facility';
  $ds_fieldsetting->view_mode = 'sirius_worker';
  $ds_fieldsetting->settings = array(
    'field_sirius_address_geo' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|sirius_dispatch_facility|sirius_worker'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_dispatch_job|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_dispatch_job';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'field_sirius_events' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|sirius_dispatch_job|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_log|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_log';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'author' => array(
      'weight' => '8',
      'label' => 'inline',
      'format' => 'author',
    ),
    'post_date' => array(
      'weight' => '7',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
  );
  $export['node|sirius_log|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_news|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_news';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'author' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'author',
    ),
    'post_date' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'ds_post_date_long',
    ),
  );
  $export['node|sirius_news|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|sirius_worker|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'sirius_worker';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'eins' => array(
      'weight' => '9',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'grievance_timeline' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'field_sirius_aat' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Access Token',
        ),
      ),
    ),
  );
  $export['node|sirius_worker|default'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function grievance_content_types_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'edit_link';
  $ds_field->label = 'Edit Link';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '<p>[<a href="/node/[node:nid]/edit?destination=[current-page:url:path]">Edit</a>]</p>
',
      'format' => 'full_html',
    ),
    'use_token' => 1,
  );
  $export['edit_link'] = $ds_field;

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
  $ds_field->field = 'grievance_company_rep_ad_combine';
  $ds_field->label = 'Employer\'s Address';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '<p>[node:field_grievance_co_address]&nbsp;[node:field_grievance_co_address_2]<br />
[node:field_grievance_co_city], [node:field_grievance_co_state] [node:field_grievance_co_zip]</p>
',
      'format' => 'full_html',
    ),
    'use_token' => 1,
  );
  $export['grievance_company_rep_ad_combine'] = $ds_field;

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
  $ds_field->field = 'grievance_timeline_short';
  $ds_field->label = 'Timeline';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '<!-- replace me -->',
      'format' => 'full_html',
    ),
    'use_token' => 0,
  );
  $export['grievance_timeline_short'] = $ds_field;

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
        1 => 'field_grievance_label',
        2 => 'field_grievance_attachments',
      ),
    ),
    'fields' => array(
      'field_grievance_document_type' => 'ds_content',
      'field_grievance_label' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_attachment|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance_contract_clause|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance_contract_clause';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_grievance_contract_section',
        1 => 'field_grievance_label',
        2 => 'title',
        3 => 'field_grievance_clause',
        4 => 'field_grievance_type',
        5 => 'field_grievance_remedy',
        6 => 'field_grievance_dummy',
      ),
    ),
    'fields' => array(
      'field_grievance_contract_section' => 'ds_content',
      'field_grievance_label' => 'ds_content',
      'title' => 'ds_content',
      'field_grievance_clause' => 'ds_content',
      'field_grievance_type' => 'ds_content',
      'field_grievance_remedy' => 'ds_content',
      'field_grievance_dummy' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_contract_clause|default'] = $ds_layout;

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
        0 => 'title',
        1 => 'field_grievance_status_date',
        2 => 'field_grievance_time',
        3 => 'author',
        4 => 'field_grievance_log_type',
        5 => 'field_grievance_log_tags',
        6 => 'field_grievance_status',
        7 => 'body',
        8 => 'field_grievance_attachments',
        9 => 'edit_link',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'field_grievance_status_date' => 'ds_content',
      'field_grievance_time' => 'ds_content',
      'author' => 'ds_content',
      'field_grievance_log_type' => 'ds_content',
      'field_grievance_log_tags' => 'ds_content',
      'field_grievance_status' => 'ds_content',
      'body' => 'ds_content',
      'field_grievance_attachments' => 'ds_content',
      'edit_link' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_log|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance_shop_contact|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance_shop_contact';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_grievance_contact_types',
        1 => 'field_grievance_co_name',
        2 => 'field_grievance_co_role',
        3 => 'field_grievance_co_email',
        4 => 'field_grievance_co_phone',
        5 => 'field_grievance_co_address',
        6 => 'field_grievance_co_address_2',
        7 => 'field_grievance_co_city',
        8 => 'field_grievance_co_state',
        9 => 'field_grievance_co_zip',
      ),
    ),
    'fields' => array(
      'field_grievance_contact_types' => 'ds_content',
      'field_grievance_co_name' => 'ds_content',
      'field_grievance_co_role' => 'ds_content',
      'field_grievance_co_email' => 'ds_content',
      'field_grievance_co_phone' => 'ds_content',
      'field_grievance_co_address' => 'ds_content',
      'field_grievance_co_address_2' => 'ds_content',
      'field_grievance_co_city' => 'ds_content',
      'field_grievance_co_state' => 'ds_content',
      'field_grievance_co_zip' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_shop_contact|teaser'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|grievance_shop|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance_shop';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'group_sirius_identity',
        1 => 'group_sirius_contract',
        2 => 'field_sirius_active',
        3 => 'group_sirius_grievance',
        4 => 'field_sirius_name_tts',
        5 => 'group_sirius_dispatch',
        6 => 'field_grievance_contract_tplt',
        7 => 'field_grievance_contract',
        8 => 'field_sirius_id',
        9 => 'field_grievance_attachments',
        10 => 'field_grievance_external_id',
        11 => 'field_grievance_company',
        12 => 'field_grievance_rep_assignee',
        13 => 'field_sirius_dispatch_job_types',
        14 => 'field_sirius_industry',
        15 => 'field_sirius_skill',
        16 => 'field_grievance_chapter',
        17 => 'field_grievance_assignee_notes',
        18 => 'field_grievance_tags',
        19 => 'field_sirius_attachments',
        20 => 'field_grievance_rep_filed',
        21 => 'field_grievance_rep_organizer',
        22 => 'field_grievance_rep_lead',
        23 => 'field_grievance_rep_manager',
        24 => 'field_grievance_rep_watching',
      ),
    ),
    'fields' => array(
      'group_sirius_identity' => 'ds_content',
      'group_sirius_contract' => 'ds_content',
      'field_sirius_active' => 'ds_content',
      'group_sirius_grievance' => 'ds_content',
      'field_sirius_name_tts' => 'ds_content',
      'group_sirius_dispatch' => 'ds_content',
      'field_grievance_contract_tplt' => 'ds_content',
      'field_grievance_contract' => 'ds_content',
      'field_sirius_id' => 'ds_content',
      'field_grievance_attachments' => 'ds_content',
      'field_grievance_external_id' => 'ds_content',
      'field_grievance_company' => 'ds_content',
      'field_grievance_rep_assignee' => 'ds_content',
      'field_sirius_dispatch_job_types' => 'ds_content',
      'field_sirius_industry' => 'ds_content',
      'field_sirius_skill' => 'ds_content',
      'field_grievance_chapter' => 'ds_content',
      'field_grievance_assignee_notes' => 'ds_content',
      'field_grievance_tags' => 'ds_content',
      'field_sirius_attachments' => 'ds_content',
      'field_grievance_rep_filed' => 'ds_content',
      'field_grievance_rep_organizer' => 'ds_content',
      'field_grievance_rep_lead' => 'ds_content',
      'field_grievance_rep_manager' => 'ds_content',
      'field_grievance_rep_watching' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|grievance_shop|default'] = $ds_layout;

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
        0 => 'field_grievance_id',
        2 => 'field_grievance_external_id',
        4 => 'field_grievance_shop',
        5 => 'field_grievance_contract_tplt',
        7 => 'field_grievance_category',
        11 => 'field_grievance_classaction',
        13 => 'field_grievance_status',
        15 => 'field_grievance_alert_tid',
        17 => 'field_grievance_alert_date',
        18 => 'field_grievance_alert_waived',
      ),
      'footer' => array(
        1 => 'field_grievance_arbitration_a',
        3 => 'field_grievance_arbitration_b',
        8 => 'field_grievance_arbitration_c',
        10 => 'field_grievance_arbitration_d',
        14 => 'field_grievance_comments',
        16 => 'grievance_timeline_short',
        28 => 'group_grievance_form',
        30 => 'group_grievance_inforeq',
        31 => 'group_grievance_comments',
        34 => 'group_grievance_arbitration',
        36 => 'group_grievance_timeline',
        37 => 'group_grievance_resproc',
        46 => 'field_grievance_broughtby',
        47 => 'field_grievance_violation',
        48 => 'field_grievance_clauseref',
        50 => 'field_grievance_type',
        52 => 'field_grievance_resproc_lh',
        53 => 'field_grievance_type_other',
        55 => 'field_grievance_resproc_an',
        56 => 'field_grievance_remedy',
        57 => 'field_grievance_resproc_er',
        59 => 'field_grievance_inforeq',
        60 => 'field_grievance_remedy_other',
        62 => 'field_grievance_resproc_ur',
        64 => 'field_field_grievance_resproc_ua',
        65 => 'field_grievance_document_types',
        66 => 'field_grievance_resproc_ea',
        84 => 'field_grievance_resproc_hd',
        85 => 'field_grievance_resproc_cd',
        86 => 'field_grievance_outcome',
      ),
      'right' => array(
        6 => 'field_grievance_supervisor_title',
        9 => 'field_grievance_supervisor_name',
        12 => 'field_grievance_supervisor_unit',
        21 => 'field_grievance_co_name',
        23 => 'group_grievance_company',
        24 => 'field_grievance_co_role',
        25 => 'group_grievance_supervisor',
        26 => 'field_grievance_co_email',
        27 => 'field_grievance_co_phone',
        29 => 'field_grievance_co_address',
        32 => 'field_grievance_co_address_2',
        33 => 'field_grievance_co_city',
        35 => 'field_grievance_co_state',
        38 => 'field_grievance_co_zip',
      ),
      'left' => array(
        19 => 'group_grievance_union_rep',
        20 => 'group_grievance_st',
        22 => 'group_grievance_employee',
        39 => 'field_grievance_rep_assignee',
        40 => 'field_grievance_assignee_notes',
        41 => 'field_grievance_rep_filed',
        42 => 'field_grievance_rep_organizer',
        43 => 'field_grievance_rep_lead',
        44 => 'field_grievance_rep_manager',
        45 => 'field_grievance_rep_watching',
        49 => 'field_grievance_min',
        51 => 'field_grievance_first_name',
        54 => 'field_grievance_last_name',
        58 => 'field_grievance_phone',
        61 => 'field_grievance_email',
        63 => 'field_grievance_address',
        67 => 'field_grievance_address_2',
        68 => 'field_grievance_city',
        69 => 'field_grievance_state',
        70 => 'field_grievance_zip',
        71 => 'field_grievance_hire_date',
        72 => 'field_grievance_gender',
        73 => 'field_grievance_class_tid',
        74 => 'field_grievance_classification',
        75 => 'field_grievance_department',
        76 => 'field_grievance_department_tid',
        77 => 'field_grievance_work_status',
        78 => 'field_grievance_shift',
        79 => 'field_grievance_daysoff',
        80 => 'field_grievance_ein',
        81 => 'field_grievance_st_name',
        82 => 'field_grievance_st_email',
        83 => 'field_grievance_st_phone',
      ),
    ),
    'fields' => array(
      'field_grievance_id' => 'header',
      'field_grievance_arbitration_a' => 'footer',
      'field_grievance_external_id' => 'header',
      'field_grievance_arbitration_b' => 'footer',
      'field_grievance_shop' => 'header',
      'field_grievance_contract_tplt' => 'header',
      'field_grievance_supervisor_title' => 'right',
      'field_grievance_category' => 'header',
      'field_grievance_arbitration_c' => 'footer',
      'field_grievance_supervisor_name' => 'right',
      'field_grievance_arbitration_d' => 'footer',
      'field_grievance_classaction' => 'header',
      'field_grievance_supervisor_unit' => 'right',
      'field_grievance_status' => 'header',
      'field_grievance_comments' => 'footer',
      'field_grievance_alert_tid' => 'header',
      'grievance_timeline_short' => 'footer',
      'field_grievance_alert_date' => 'header',
      'field_grievance_alert_waived' => 'header',
      'group_grievance_union_rep' => 'left',
      'group_grievance_st' => 'left',
      'field_grievance_co_name' => 'right',
      'group_grievance_employee' => 'left',
      'group_grievance_company' => 'right',
      'field_grievance_co_role' => 'right',
      'group_grievance_supervisor' => 'right',
      'field_grievance_co_email' => 'right',
      'field_grievance_co_phone' => 'right',
      'group_grievance_form' => 'footer',
      'field_grievance_co_address' => 'right',
      'group_grievance_inforeq' => 'footer',
      'group_grievance_comments' => 'footer',
      'field_grievance_co_address_2' => 'right',
      'field_grievance_co_city' => 'right',
      'group_grievance_arbitration' => 'footer',
      'field_grievance_co_state' => 'right',
      'group_grievance_timeline' => 'footer',
      'group_grievance_resproc' => 'footer',
      'field_grievance_co_zip' => 'right',
      'field_grievance_rep_assignee' => 'left',
      'field_grievance_assignee_notes' => 'left',
      'field_grievance_rep_filed' => 'left',
      'field_grievance_rep_organizer' => 'left',
      'field_grievance_rep_lead' => 'left',
      'field_grievance_rep_manager' => 'left',
      'field_grievance_rep_watching' => 'left',
      'field_grievance_broughtby' => 'footer',
      'field_grievance_violation' => 'footer',
      'field_grievance_clauseref' => 'footer',
      'field_grievance_min' => 'left',
      'field_grievance_type' => 'footer',
      'field_grievance_first_name' => 'left',
      'field_grievance_resproc_lh' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_last_name' => 'left',
      'field_grievance_resproc_an' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_resproc_er' => 'footer',
      'field_grievance_phone' => 'left',
      'field_grievance_inforeq' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'field_grievance_email' => 'left',
      'field_grievance_resproc_ur' => 'footer',
      'field_grievance_address' => 'left',
      'field_field_grievance_resproc_ua' => 'footer',
      'field_grievance_document_types' => 'footer',
      'field_grievance_resproc_ea' => 'footer',
      'field_grievance_address_2' => 'left',
      'field_grievance_city' => 'left',
      'field_grievance_state' => 'left',
      'field_grievance_zip' => 'left',
      'field_grievance_hire_date' => 'left',
      'field_grievance_gender' => 'left',
      'field_grievance_class_tid' => 'left',
      'field_grievance_classification' => 'left',
      'field_grievance_department' => 'left',
      'field_grievance_department_tid' => 'left',
      'field_grievance_work_status' => 'left',
      'field_grievance_shift' => 'left',
      'field_grievance_daysoff' => 'left',
      'field_grievance_ein' => 'left',
      'field_grievance_st_name' => 'left',
      'field_grievance_st_email' => 'left',
      'field_grievance_st_phone' => 'left',
      'field_grievance_resproc_hd' => 'footer',
      'field_grievance_resproc_cd' => 'footer',
      'field_grievance_outcome' => 'footer',
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
  $ds_layout->id = 'node|grievance|grievance_form_226';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'grievance';
  $ds_layout->view_mode = 'grievance_form_226';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'group_grievance_226_1',
        1 => 'field_grievance_shop',
        4 => 'field_grievance_co_name',
        7 => 'field_grievance_co_role',
        8 => 'grievance_company_rep_ad_combine',
      ),
      'right' => array(
        2 => 'group_grievance_226_2',
        3 => 'grievant_name',
        6 => 'field_grievance_class_tid',
        9 => 'field_grievance_rep_organizer',
        12 => 'field_grievance_rep_lead',
      ),
      'footer' => array(
        5 => 'field_grievance_clauseref',
        10 => 'field_grievance_date_1',
        11 => 'field_grievance_date_2',
        13 => 'field_grievance_type',
        14 => 'field_grievance_type_other',
        15 => 'field_grievance_remedy',
        16 => 'field_grievance_document_types',
        17 => 'field_grievance_remedy_other',
        18 => 'group_grievance_226_3',
        19 => 'field_grievance_inforeq',
      ),
    ),
    'fields' => array(
      'group_grievance_226_1' => 'left',
      'field_grievance_shop' => 'left',
      'group_grievance_226_2' => 'right',
      'grievant_name' => 'right',
      'field_grievance_co_name' => 'left',
      'field_grievance_clauseref' => 'footer',
      'field_grievance_class_tid' => 'right',
      'field_grievance_co_role' => 'left',
      'grievance_company_rep_ad_combine' => 'left',
      'field_grievance_rep_organizer' => 'right',
      'field_grievance_date_1' => 'footer',
      'field_grievance_date_2' => 'footer',
      'field_grievance_rep_lead' => 'right',
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_document_types' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'group_grievance_226_3' => 'footer',
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
  $export['node|grievance|grievance_form_226'] = $ds_layout;

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
        0 => 'grievance_timeline_short',
        1 => 'field_grievance_meeting_date',
        2 => 'field_grievance_alert_waived',
        34 => 'field_grievance_rep_filed',
      ),
      'left' => array(
        3 => 'field_grievance_co_state',
        4 => 'field_grievance_co_zip',
        5 => 'field_grievance_co_email',
        6 => 'field_grievance_co_phone',
        7 => 'field_grievance_shop',
        8 => 'field_grievance_co_name',
        9 => 'field_grievance_co_address',
        10 => 'field_grievance_co_address_2',
        11 => 'field_grievance_co_city',
      ),
      'right' => array(
        12 => 'field_grievance_supervisor_name',
        13 => 'field_grievance_supervisor_title',
        14 => 'field_grievance_supervisor_unit',
        15 => 'field_grievance_first_name',
        16 => 'field_grievance_last_name',
        17 => 'field_grievance_address',
        18 => 'field_grievance_address_2',
        19 => 'field_grievance_city',
        20 => 'field_grievance_state',
        21 => 'field_grievance_zip',
        22 => 'field_grievance_phone',
        23 => 'field_grievance_email',
        24 => 'field_grievance_classification',
        25 => 'field_grievance_department',
      ),
      'footer' => array(
        26 => 'field_grievance_type',
        27 => 'field_grievance_type_other',
        28 => 'field_grievance_violation',
        29 => 'field_grievance_clauseref',
        30 => 'field_grievance_remedy',
        31 => 'field_grievance_remedy_other',
        32 => 'field_grievance_document_types',
        33 => 'field_grievance_inforeq',
      ),
    ),
    'fields' => array(
      'grievance_timeline_short' => 'header',
      'field_grievance_meeting_date' => 'header',
      'field_grievance_alert_waived' => 'header',
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
      'field_grievance_type' => 'footer',
      'field_grievance_type_other' => 'footer',
      'field_grievance_violation' => 'footer',
      'field_grievance_clauseref' => 'footer',
      'field_grievance_remedy' => 'footer',
      'field_grievance_remedy_other' => 'footer',
      'field_grievance_document_types' => 'footer',
      'field_grievance_inforeq' => 'footer',
      'field_grievance_rep_filed' => 'header',
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

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch_facility|sirius_worker';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch_facility';
  $ds_layout->view_mode = 'sirius_worker';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_address',
        1 => 'field_sirius_address_geo',
      ),
    ),
    'fields' => array(
      'field_sirius_address' => 'ds_content',
      'field_sirius_address_geo' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_dispatch_facility|sirius_worker'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch_job|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch_job';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'two_column_bricks';
  $ds_layout->settings = array(
    'regions' => array(
      'above_left' => array(
        0 => 'group_sirius_employer_info',
        2 => 'field_grievance_shop',
        3 => 'field_sirius_industry',
        4 => 'group_sirius_job_details',
        5 => 'field_sirius_id',
        7 => 'field_sirius_id2',
        17 => 'field_sirius_skill',
        22 => 'field_grievance_departments',
        25 => 'field_sirius_datetime',
        28 => 'field_sirius_dispatch_job_group',
        29 => 'field_sirius_dispatch_facility',
        30 => 'field_sirius_dispatch_hall',
        32 => 'field_sirius_address_notes',
        33 => 'field_sirius_dispatch_job_tags',
        35 => 'field_sirius_events',
      ),
      'above_right' => array(
        1 => 'group_sirius_job_info',
        9 => 'group_sirius_notify',
        11 => 'field_sirius_dispatch_job_type',
        13 => 'field_sirius_notify',
        14 => 'field_sirius_datetime_created',
        18 => 'field_sirius_dispatch_job_nfcns',
        21 => 'field_sirius_dispatch_job_status',
        24 => 'field_sirius_representatives',
        27 => 'field_sirius_emails',
      ),
      'middle' => array(
        6 => 'group_sirius_employer_notes',
        10 => 'field_sirius_notes',
        12 => 'field_sirius_address_parking',
        15 => 'field_sirius_attachments',
      ),
      'below_left' => array(
        8 => 'group_sirius_dispatch_details',
        31 => 'field_sirius_member_status',
        34 => 'field_sirius_dispatch_eba',
        36 => 'field_sirius_dispatch_eba_dates',
        37 => 'field_sirius_summary',
        38 => 'field_sirius_timelimit',
        39 => 'field_sirius_batch_max',
        40 => 'field_sirius_batch_min',
        41 => 'field_sirius_batch_ratio',
        42 => 'field_sirius_datetime_completed',
      ),
      'below_right' => array(
        16 => 'field_sirius_count',
        19 => 'field_sirius_count_notified',
        20 => 'group_sirius_worker_counts',
        23 => 'field_sirius_count_yes',
        26 => 'field_sirius_count_no',
      ),
    ),
    'fields' => array(
      'group_sirius_employer_info' => 'above_left',
      'group_sirius_job_info' => 'above_right',
      'field_grievance_shop' => 'above_left',
      'field_sirius_industry' => 'above_left',
      'group_sirius_job_details' => 'above_left',
      'field_sirius_id' => 'above_left',
      'group_sirius_employer_notes' => 'middle',
      'field_sirius_id2' => 'above_left',
      'group_sirius_dispatch_details' => 'below_left',
      'group_sirius_notify' => 'above_right',
      'field_sirius_notes' => 'middle',
      'field_sirius_dispatch_job_type' => 'above_right',
      'field_sirius_address_parking' => 'middle',
      'field_sirius_notify' => 'above_right',
      'field_sirius_datetime_created' => 'above_right',
      'field_sirius_attachments' => 'middle',
      'field_sirius_count' => 'below_right',
      'field_sirius_skill' => 'above_left',
      'field_sirius_dispatch_job_nfcns' => 'above_right',
      'field_sirius_count_notified' => 'below_right',
      'group_sirius_worker_counts' => 'below_right',
      'field_sirius_dispatch_job_status' => 'above_right',
      'field_grievance_departments' => 'above_left',
      'field_sirius_count_yes' => 'below_right',
      'field_sirius_representatives' => 'above_right',
      'field_sirius_datetime' => 'above_left',
      'field_sirius_count_no' => 'below_right',
      'field_sirius_emails' => 'above_right',
      'field_sirius_dispatch_job_group' => 'above_left',
      'field_sirius_dispatch_facility' => 'above_left',
      'field_sirius_dispatch_hall' => 'above_left',
      'field_sirius_member_status' => 'below_left',
      'field_sirius_address_notes' => 'above_left',
      'field_sirius_dispatch_job_tags' => 'above_left',
      'field_sirius_dispatch_eba' => 'below_left',
      'field_sirius_events' => 'above_left',
      'field_sirius_dispatch_eba_dates' => 'below_left',
      'field_sirius_summary' => 'below_left',
      'field_sirius_timelimit' => 'below_left',
      'field_sirius_batch_max' => 'below_left',
      'field_sirius_batch_min' => 'below_left',
      'field_sirius_batch_ratio' => 'below_left',
      'field_sirius_datetime_completed' => 'below_left',
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
  $export['node|sirius_dispatch_job|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch_job|sirius_worker';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch_job';
  $ds_layout->view_mode = 'sirius_worker';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_industry',
        1 => 'field_grievance_shop',
        2 => 'field_sirius_dispatch_job_type',
        3 => 'field_sirius_skill',
        4 => 'field_sirius_id',
        5 => 'field_sirius_datetime',
        6 => 'field_sirius_dispatch_facility',
        7 => 'field_sirius_dispatch_hall',
        8 => 'field_sirius_address_notes',
        9 => 'field_sirius_address_parking',
        10 => 'field_sirius_notes',
      ),
    ),
    'fields' => array(
      'field_sirius_industry' => 'ds_content',
      'field_grievance_shop' => 'ds_content',
      'field_sirius_dispatch_job_type' => 'ds_content',
      'field_sirius_skill' => 'ds_content',
      'field_sirius_id' => 'ds_content',
      'field_sirius_datetime' => 'ds_content',
      'field_sirius_dispatch_facility' => 'ds_content',
      'field_sirius_dispatch_hall' => 'ds_content',
      'field_sirius_address_notes' => 'ds_content',
      'field_sirius_address_parking' => 'ds_content',
      'field_sirius_notes' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_dispatch_job|sirius_worker'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch_job|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch_job';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_industry',
        1 => 'field_sirius_skill',
        2 => 'field_grievance_shop',
        3 => 'field_sirius_datetime',
        4 => 'field_sirius_dispatch_job_type',
        5 => 'field_sirius_dispatch_job_status',
        6 => 'field_sirius_count',
        7 => 'field_sirius_datetime_completed',
        8 => 'field_sirius_dispatch_facility',
        9 => 'field_sirius_dispatch_hall',
      ),
    ),
    'fields' => array(
      'field_sirius_industry' => 'ds_content',
      'field_sirius_skill' => 'ds_content',
      'field_grievance_shop' => 'ds_content',
      'field_sirius_datetime' => 'ds_content',
      'field_sirius_dispatch_job_type' => 'ds_content',
      'field_sirius_dispatch_job_status' => 'ds_content',
      'field_sirius_count' => 'ds_content',
      'field_sirius_datetime_completed' => 'ds_content',
      'field_sirius_dispatch_facility' => 'ds_content',
      'field_sirius_dispatch_hall' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_dispatch_job|teaser'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch|sirius_worker';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch';
  $ds_layout->view_mode = 'sirius_worker';
  $ds_layout->layout = 'two_column_bricks';
  $ds_layout->settings = array(
    'regions' => array(
      'above_left' => array(
        0 => 'field_sirius_payrate',
        1 => 'field_sirius_dispatch_status',
        2 => 'field_sirius_dispatch_cbn',
        3 => 'field_sirius_datetime',
        4 => 'field_sirius_datetime_completed',
      ),
      'above_right' => array(
        5 => 'field_sirius_worker',
      ),
    ),
    'fields' => array(
      'field_sirius_payrate' => 'above_left',
      'field_sirius_dispatch_status' => 'above_left',
      'field_sirius_dispatch_cbn' => 'above_left',
      'field_sirius_datetime' => 'above_left',
      'field_sirius_datetime_completed' => 'above_left',
      'field_sirius_worker' => 'above_right',
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
  $export['node|sirius_dispatch|sirius_worker'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_dispatch|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_dispatch';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_datetime',
        1 => 'field_sirius_dispatch_status',
        2 => 'field_sirius_datetime_completed',
      ),
    ),
    'fields' => array(
      'field_sirius_datetime' => 'ds_content',
      'field_sirius_dispatch_status' => 'ds_content',
      'field_sirius_datetime_completed' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_dispatch|teaser'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_log|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_log';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_log_handler',
        1 => 'field_sirius_category',
        2 => 'field_sirius_type',
        3 => 'field_sirius_message',
        4 => 'field_sirius_notes',
        5 => 'field_sirius_summary',
        6 => 'field_sirius_attachments',
        7 => 'post_date',
        8 => 'author',
        9 => 'field_sirius_domain',
      ),
    ),
    'fields' => array(
      'field_sirius_log_handler' => 'ds_content',
      'field_sirius_category' => 'ds_content',
      'field_sirius_type' => 'ds_content',
      'field_sirius_message' => 'ds_content',
      'field_sirius_notes' => 'ds_content',
      'field_sirius_summary' => 'ds_content',
      'field_sirius_attachments' => 'ds_content',
      'post_date' => 'ds_content',
      'author' => 'ds_content',
      'field_sirius_domain' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_log|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_news|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_news';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'post_date',
        2 => 'author',
        3 => 'body',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'post_date' => 'ds_content',
      'author' => 'ds_content',
      'body' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_news|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_worker|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_worker';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'two_column_bricks';
  $ds_layout->settings = array(
    'regions' => array(
      'above_left' => array(
        0 => 'grievance_timeline',
        1 => 'group_sirius_worker_identity',
        3 => 'field_sirius_name',
        4 => 'group_sirius_worker_contact',
        5 => 'field_sirius_id',
        8 => 'field_sirius_ssn',
        10 => 'field_sirius_id2',
        11 => 'field_sirius_id3',
        13 => 'field_sirius_aat',
        15 => 'field_sirius_gender',
        17 => 'eins',
        18 => 'field_sirius_skill',
        21 => 'field_sirius_phone',
        22 => 'field_sirius_phone_alt',
        23 => 'field_sirius_phone_fax',
        24 => 'field_sirius_email',
        25 => 'field_sirius_address',
        40 => 'group_skills',
      ),
      'above_right' => array(
        2 => 'group_sirius_worker_status',
        6 => 'field_sirius_member_status',
        7 => 'group_sirius_worker_preferences',
        9 => 'field_sirius_work_status',
        12 => 'field_sirius_worker_dispstatus',
        14 => 'field_sirius_dispatch_availdate',
        16 => 'field_grievance_shop',
        19 => 'field_sirius_dispatch_dncs',
        20 => 'field_sirius_industry',
        26 => 'field_sirius_dispatch_medium',
        27 => 'field_sirius_dispatch_asi',
        29 => 'field_sirius_dispatch_sib_status',
        30 => 'field_sirius_dispatch_cbn',
        31 => 'field_sirius_skills_avail',
        32 => 'field_sirius_skills_availx',
        33 => 'field_grievance_departments',
        34 => 'field_sirius_dispatch_dncs2',
        35 => 'field_sirius_dispatch_hfe',
        36 => 'field_sirius_dispatch_hfe_until',
        37 => 'field_sirius_aat_required',
        38 => 'field_sirius_dispatch_eba',
        39 => 'field_sirius_dispatch_eba_dates',
      ),
      'header' => array(
        28 => 'field_sirius_headshot',
      ),
    ),
    'fields' => array(
      'grievance_timeline' => 'above_left',
      'group_sirius_worker_identity' => 'above_left',
      'group_sirius_worker_status' => 'above_right',
      'field_sirius_name' => 'above_left',
      'group_sirius_worker_contact' => 'above_left',
      'field_sirius_id' => 'above_left',
      'field_sirius_member_status' => 'above_right',
      'group_sirius_worker_preferences' => 'above_right',
      'field_sirius_ssn' => 'above_left',
      'field_sirius_work_status' => 'above_right',
      'field_sirius_id2' => 'above_left',
      'field_sirius_id3' => 'above_left',
      'field_sirius_worker_dispstatus' => 'above_right',
      'field_sirius_aat' => 'above_left',
      'field_sirius_dispatch_availdate' => 'above_right',
      'field_sirius_gender' => 'above_left',
      'field_grievance_shop' => 'above_right',
      'eins' => 'above_left',
      'field_sirius_skill' => 'above_left',
      'field_sirius_dispatch_dncs' => 'above_right',
      'field_sirius_industry' => 'above_right',
      'field_sirius_phone' => 'above_left',
      'field_sirius_phone_alt' => 'above_left',
      'field_sirius_phone_fax' => 'above_left',
      'field_sirius_email' => 'above_left',
      'field_sirius_address' => 'above_left',
      'field_sirius_dispatch_medium' => 'above_right',
      'field_sirius_dispatch_asi' => 'above_right',
      'field_sirius_headshot' => 'header',
      'field_sirius_dispatch_sib_status' => 'above_right',
      'field_sirius_dispatch_cbn' => 'above_right',
      'field_sirius_skills_avail' => 'above_right',
      'field_sirius_skills_availx' => 'above_right',
      'field_grievance_departments' => 'above_right',
      'field_sirius_dispatch_dncs2' => 'above_right',
      'field_sirius_dispatch_hfe' => 'above_right',
      'field_sirius_dispatch_hfe_until' => 'above_right',
      'field_sirius_aat_required' => 'above_right',
      'field_sirius_dispatch_eba' => 'above_right',
      'field_sirius_dispatch_eba_dates' => 'above_right',
      'group_skills' => 'above_left',
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
  $export['node|sirius_worker|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_worker|sirius_worker';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_worker';
  $ds_layout->view_mode = 'sirius_worker';
  $ds_layout->layout = 'two_column_bricks';
  $ds_layout->settings = array(
    'regions' => array(
      'above_left' => array(
        0 => 'group_sirius_worker_identity',
        1 => 'field_sirius_name',
        2 => 'field_sirius_id',
        3 => 'field_sirius_ssn',
      ),
      'above_right' => array(
        4 => 'group_sirius_worker_contact',
        5 => 'field_sirius_phone',
        6 => 'field_sirius_phone_alt',
        7 => 'field_sirius_email',
        8 => 'field_sirius_address',
      ),
    ),
    'fields' => array(
      'group_sirius_worker_identity' => 'above_left',
      'field_sirius_name' => 'above_left',
      'field_sirius_id' => 'above_left',
      'field_sirius_ssn' => 'above_left',
      'group_sirius_worker_contact' => 'above_right',
      'field_sirius_phone' => 'above_right',
      'field_sirius_phone_alt' => 'above_right',
      'field_sirius_email' => 'above_right',
      'field_sirius_address' => 'above_right',
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
  $export['node|sirius_worker|sirius_worker'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|sirius_worker|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'sirius_worker';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_sirius_name',
        1 => 'field_sirius_phone',
        2 => 'field_sirius_id',
        3 => 'field_sirius_ssn',
      ),
    ),
    'fields' => array(
      'field_sirius_name' => 'ds_content',
      'field_sirius_phone' => 'ds_content',
      'field_sirius_id' => 'ds_content',
      'field_sirius_ssn' => 'ds_content',
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
    'layout_disable_css' => FALSE,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|sirius_worker|teaser'] = $ds_layout;

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
  $ds_view_mode->view_mode = 'grievance_form_226';
  $ds_view_mode->label = 'Grievance Form 226';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['grievance_form_226'] = $ds_view_mode;

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
