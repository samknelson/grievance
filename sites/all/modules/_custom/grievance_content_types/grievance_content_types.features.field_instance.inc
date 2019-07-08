<?php

/**
 * @file
 * grievance_content_types.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function grievance_content_types_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'file-grievance_upload-field_grievance_document_type'.
  $field_instances['file-grievance_upload-field_grievance_document_type'] = array(
    'bundle' => 'grievance_upload',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'file',
    'field_name' => 'field_grievance_document_type',
    'label' => 'Grievance Document Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-grievance-field_field_grievance_resproc_ua'.
  $field_instances['node-grievance-field_field_grievance_resproc_ua'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 38,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_field_grievance_resproc_ua',
    'label' => 'Union Attorney',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 84,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_address'.
  $field_instances['node-grievance-field_grievance_address'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 38,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 29,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 53,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_address_2'.
  $field_instances['node-grievance-field_grievance_address_2'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 39,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 30,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 56,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_address_2',
    'label' => 'Address 2',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_alert'.
  $field_instances['node-grievance-field_grievance_alert'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the next deadline that the Union must meet -- i.e. "File Step 2" or "File for Arbitration".',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 69,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 88,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 50,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 74,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert',
    'label' => 'Next Step in the Grievance Procedure',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_alert_date'.
  $field_instances['node-grievance-field_grievance_alert_date'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 1,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 8,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 70,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 89,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 51,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 1,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 18,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_date',
    'label' => 'Next Deadline Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 39,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_alert_tid'.
  $field_instances['node-grievance-field_grievance_alert_tid'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 7,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 61,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 78,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 17,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_tid',
    'label' => 'Next Step in the Grievance Procedure',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 35,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_alert_waived'.
  $field_instances['node-grievance-field_grievance_alert_waived'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => 'Deadlines have been waived as of this date.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 9,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 2,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 19,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_waived',
    'label' => 'Deadlines Waived',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_arbitration_a'.
  $field_instances['node-grievance-field_grievance_arbitration_a'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 71,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 90,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 52,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_arbitration_a',
    'label' => 'Labor charges or other relevant shop issues',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 77,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_arbitration_b'.
  $field_instances['node-grievance-field_grievance_arbitration_b'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 68,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 87,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 49,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_arbitration_b',
    'label' => 'Can and should this case be settled?',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 78,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_arbitration_c'.
  $field_instances['node-grievance-field_grievance_arbitration_c'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 21,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 67,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 86,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 48,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_arbitration_c',
    'label' => 'Building the Union with this case',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 79,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_arbitration_d'.
  $field_instances['node-grievance-field_grievance_arbitration_d'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 66,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 85,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 47,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 14,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_arbitration_d',
    'label' => 'Lead\'s Recommendaiton',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 80,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_assignee_notes'.
  $field_instances['node-grievance-field_grievance_assignee_notes'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 24,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 30,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_assignee_notes',
    'label' => 'Assignee Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'available_options' => '',
        'available_options_php' => '',
        'markup_available_options_php' => '&lt;none&gt;',
        'other' => 'Other',
        'other_size' => 60,
        'other_title' => '',
        'other_unknown_defaults' => 'other',
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_broughtby'.
  $field_instances['node-grievance-field_grievance_broughtby'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 30,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 69,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 36,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_broughtby',
    'label' => 'Brought By',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 36,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_category'.
  $field_instances['node-grievance-field_grievance_category'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_category',
    'label' => 'Category',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_city'.
  $field_instances['node-grievance-field_grievance_city'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 40,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 31,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 58,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_city',
    'label' => 'City',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_class_tid'.
  $field_instances['node-grievance-field_grievance_class_tid'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 45,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 65,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 63,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_class_tid',
    'label' => 'Classification',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => 'term_reference_1',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => 'grievance_classifications',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'content_taxonomy_autocomplete_new_terms' => 'deny',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_classaction'.
  $field_instances['node-grievance-field_grievance_classaction'] = array(
    'bundle' => 'grievance',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 5,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 49,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 66,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 13,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_classaction',
    'label' => 'General Grievance?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_classification'.
  $field_instances['node-grievance-field_grievance_classification'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 46,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 24,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 64,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_classification',
    'label' => 'Classification',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 39,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_clauseref'.
  $field_instances['node-grievance-field_grievance_clauseref'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 33,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 3,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 29,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 38,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_clauseref',
    'label' => 'CBA Provision(s) Violated',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 36,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_address'.
  $field_instances['node-grievance-field_grievance_co_address'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 16,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 49,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 24,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_address',
    'label' => 'Company Rep Address',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 54,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_address_2'.
  $field_instances['node-grievance-field_grievance_co_address_2'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 50,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 25,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_address_2',
    'label' => 'Company Rep Address 2',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 55,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_city'.
  $field_instances['node-grievance-field_grievance_co_city'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 51,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 26,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_city',
    'label' => 'Company Rep City',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 56,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_email'.
  $field_instances['node-grievance-field_grievance_co_email'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 14,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 54,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 5,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 22,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_email',
    'label' => 'Company Rep Email',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'email',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'email_textfield',
      'weight' => 51,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_name'.
  $field_instances['node-grievance-field_grievance_co_name'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 12,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 48,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 20,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_name',
    'label' => 'Company Rep Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 49,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_phone'.
  $field_instances['node-grievance-field_grievance_co_phone'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 55,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 23,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_phone',
    'label' => 'Company Rep Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 52,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_role'.
  $field_instances['node-grievance-field_grievance_co_role'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 47,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 21,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_role',
    'label' => 'Company Rep Title',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 50,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_state'.
  $field_instances['node-grievance-field_grievance_co_state'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 52,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 27,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_state',
    'label' => 'Company Rep State or Province',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 2,
      ),
      'type' => 'text_textfield',
      'weight' => 57,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_co_zip'.
  $field_instances['node-grievance-field_grievance_co_zip'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 53,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 28,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_zip',
    'label' => 'Company Rep Postal Code',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 10,
      ),
      'type' => 'text_textfield',
      'weight' => 58,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_comments'.
  $field_instances['node-grievance-field_grievance_comments'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 72,
      ),
      'grievance_gform' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 91,
      ),
      'grievance_gform_a' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 53,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 16,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_comments',
    'label' => 'Comments',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 60,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_contact_selector'.
  $field_instances['node-grievance-field_grievance_contact_selector'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 58,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 76,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 76,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contact_selector',
    'label' => 'Select a Contact',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'markup',
      'settings' => array(),
      'type' => 'markup',
      'weight' => 48,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_contract_tplt'.
  $field_instances['node-grievance-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 3,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 55,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 74,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_corrected'.
  $field_instances['node-grievance-field_grievance_corrected'] = array(
    'bundle' => 'grievance',
    'default_value' => array(
      0 => array(
        'value' => 'Unchanged',
      ),
    ),
    'deleted' => 0,
    'description' => 'Use this field to indicate that member data in the grievance system has changed from the Union\'s member management system, and that the new data needs to be uploaded back to the member management system.  "Unchanged" means that the data is the same in both places; "Pending" means that corrected data has been entered here and needs to be entered in the member management system.  After the information has been changed, it will be flagged as "Complete".',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 54,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 73,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 78,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_corrected',
    'label' => 'Data Corrected',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 47,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_data_alert'.
  $field_instances['node-grievance-field_grievance_data_alert'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Admin data never displayed',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 65,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 84,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 46,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 75,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_data_alert',
    'label' => 'Alert Data',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_date_1'.
  $field_instances['node-grievance-field_grievance_date_1'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => 'For discipline, the date of the employee’s conduct which led to discipline. For contract violation, the date of the employer’s action (if unknown, choose "Investigation").',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 8,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_date_1',
    'label' => 'Incident Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_date_2'.
  $field_instances['node-grievance-field_grievance_date_2'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => 'The date the employee received discipline.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 5,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 63,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 12,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_date_2',
    'label' => 'Discipline Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_daysoff'.
  $field_instances['node-grievance-field_grievance_daysoff'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => ', ',
        ),
        'type' => 'list_default',
        'weight' => 51,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 46,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 70,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => ', ',
        ),
        'type' => 'list_default',
        'weight' => 69,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_daysoff',
    'label' => 'Days Off',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_department'.
  $field_instances['node-grievance-field_grievance_department'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 47,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 16,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 25,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 44,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 65,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_department',
    'label' => 'Department',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_department_tid'.
  $field_instances['node-grievance-field_grievance_department_tid'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 48,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 68,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 66,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_department_tid',
    'label' => 'Department',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_document_types'.
  $field_instances['node-grievance-field_grievance_document_types'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 38,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => ', ',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 9,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 32,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 55,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_document_types',
    'label' => 'Standard Information Request',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => 'term_reference_1',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => 'grievance_information_request_types',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 53,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_ein'.
  $field_instances['node-grievance-field_grievance_ein'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the employee ID # as assigned by the company.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 52,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 64,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 79,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 70,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_ein',
    'label' => 'Employee ID #',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 45,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_email'.
  $field_instances['node-grievance-field_grievance_email'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 37,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 13,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 23,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 52,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_email',
    'label' => 'Email',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'email',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'email_textfield',
      'weight' => 30,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_external_id'.
  $field_instances['node-grievance-field_grievance_external_id'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_first_name'.
  $field_instances['node-grievance-field_grievance_first_name'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 34,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 31,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 42,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_first_name',
    'label' => 'First Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_gender'.
  $field_instances['node-grievance-field_grievance_gender'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 44,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 62,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_gender',
    'label' => 'Gender',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_hire_date'.
  $field_instances['node-grievance-field_grievance_hire_date'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 43,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 14,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 73,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 92,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 12,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 61,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_hire_date',
    'label' => 'Date of Hire',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '1900:+3',
      ),
      'type' => 'date_popup',
      'weight' => 36,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_id'.
  $field_instances['node-grievance-field_grievance_id'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 60,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 77,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_inforeq'.
  $field_instances['node-grievance-field_grievance_inforeq'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 37,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_plain',
        'weight' => 10,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_plain',
        'weight' => 33,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 50,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_inforeq',
    'label' => 'Additional Information Request',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 54,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_last_name'.
  $field_instances['node-grievance-field_grievance_last_name'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 35,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 28,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 16,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 32,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 46,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_last_name',
    'label' => 'Last Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 28,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_meeting_date'.
  $field_instances['node-grievance-field_grievance_meeting_date'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 77,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_meeting_date',
    'label' => 'Meeting Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_min'.
  $field_instances['node-grievance-field_grievance_min'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the union\'s ID # for this member.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 33,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 56,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 75,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 39,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_min',
    'label' => 'Member ID #',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 46,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_outcome'.
  $field_instances['node-grievance-field_grievance_outcome'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 70,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 52,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 46,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 57,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_outcome',
    'label' => 'Outcome',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 89,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_phone'.
  $field_instances['node-grievance-field_grievance_phone'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 36,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 12,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 48,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_phone',
    'label' => 'Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 29,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_remedy'.
  $field_instances['node-grievance-field_grievance_remedy'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 36,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 8,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 30,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 45,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_remedy',
    'label' => 'Remedy',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_remedy_other'.
  $field_instances['node-grievance-field_grievance_remedy_other'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 37,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 31,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 51,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_remedy_other',
    'label' => 'Remedy',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_assignee'.
  $field_instances['node-grievance-field_grievance_rep_assignee'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 23,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 29,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_assignee',
    'label' => 'Assignee',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 35,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_filed'.
  $field_instances['node-grievance-field_grievance_rep_filed'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 25,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 39,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 57,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 49,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 31,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_filed',
    'label' => 'Intake Person',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'lock_previous' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'apply_chosen' => '',
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_lead'.
  $field_instances['node-grievance-field_grievance_rep_lead'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 27,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 41,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 5,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 52,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 33,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_lead',
    'label' => 'Grievance Director',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_manager'.
  $field_instances['node-grievance-field_grievance_rep_manager'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 28,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 42,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 63,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 53,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 54,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 34,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_manager',
    'label' => 'Grievance Admin',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_organizer'.
  $field_instances['node-grievance-field_grievance_rep_organizer'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the organizer assigned to this grievance.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 26,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 40,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 4,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 51,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 20,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 32,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_organizer',
    'label' => 'Business Agent',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 39,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_rep_watching'.
  $field_instances['node-grievance-field_grievance_rep_watching'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 29,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 43,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 74,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 93,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 55,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 35,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_watching',
    'label' => 'Watching',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_an'.
  $field_instances['node-grievance-field_grievance_resproc_an'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 35,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 44,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 62,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 44,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_an',
    'label' => 'Arbitrator Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 82,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_cd'.
  $field_instances['node-grievance-field_grievance_resproc_cd'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 60,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_cd',
    'label' => 'Cancel By',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 88,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_ea'.
  $field_instances['node-grievance-field_grievance_resproc_ea'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 39,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 51,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 54,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_ea',
    'label' => 'Employer Attorney',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 85,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_er'.
  $field_instances['node-grievance-field_grievance_resproc_er'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 36,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 45,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 44,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 47,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_er',
    'label' => 'Employer Representative',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'available_options' => '',
        'available_options_php' => '',
        'markup_available_options_php' => '&lt;none&gt;',
        'other' => 'Other',
        'other_size' => 60,
        'other_title' => '',
        'other_unknown_defaults' => 'other',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 83,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_hd'.
  $field_instances['node-grievance-field_grievance_resproc_hd'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 59,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_hd',
    'label' => 'Hearing Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 87,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_lh'.
  $field_instances['node-grievance-field_grievance_resproc_lh'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '(in days)',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 34,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 71,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 41,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_lh',
    'label' => 'Length of Hearing',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 81,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_resproc_ur'.
  $field_instances['node-grievance-field_grievance_resproc_ur'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 37,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 50,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 45,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 49,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_resproc_ur',
    'label' => 'Union Representative',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 86,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_shift'.
  $field_instances['node-grievance-field_grievance_shift'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 50,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 48,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 67,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 68,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shift',
    'label' => 'Shift',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_shop'.
  $field_instances['node-grievance-field_grievance_shop'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 2,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 47,
      ),
      'grievance_form_226' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 7,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shop',
    'label' => 'Employer',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'deny',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 30,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_st_email'.
  $field_instances['node-grievance-field_grievance_st_email'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 56,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 71,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_st_email',
    'label' => 'Shop Steward Email',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 78,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_st_name'.
  $field_instances['node-grievance-field_grievance_st_name'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 55,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 72,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_st_name',
    'label' => 'Shop Steward Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 77,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_st_phone'.
  $field_instances['node-grievance-field_grievance_st_phone'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 57,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 48,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 73,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_st_phone',
    'label' => 'Shop Steward Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 79,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_st_selector'.
  $field_instances['node-grievance-field_grievance_st_selector'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 58,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 49,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_st_selector',
    'label' => 'Select a Shop Steward',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'markup',
      'settings' => array(),
      'type' => 'markup',
      'weight' => 76,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_state'.
  $field_instances['node-grievance-field_grievance_state'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 41,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 32,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 59,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_state',
    'label' => 'State or Province',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 2,
      ),
      'type' => 'text_textfield',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_status'.
  $field_instances['node-grievance-field_grievance_status'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 75,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 94,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 56,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 15,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_status',
    'label' => 'Status',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_status_date'.
  $field_instances['node-grievance-field_grievance_status_date'] = array(
    'bundle' => 'grievance',
    'deleted' => 0,
    'description' => 'Select the date that the status changed.  For example, if the status is "Step 1 Meeting", enter the date that the meeting occurred.  If no date is entered, today\'s date will be used.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 53,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 72,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 79,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_status_date',
    'label' => 'Status Change Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_supervisor_name'.
  $field_instances['node-grievance-field_grievance_supervisor_name'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 12,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_supervisor_name',
    'label' => 'Supervisor Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_supervisor_title'.
  $field_instances['node-grievance-field_grievance_supervisor_title'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_supervisor_title',
    'label' => 'Supervisor Title',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_supervisor_unit'.
  $field_instances['node-grievance-field_grievance_supervisor_unit'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 26,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 14,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 45,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_supervisor_unit',
    'label' => 'Supervisor Unit',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_type'.
  $field_instances['node-grievance-field_grievance_type'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 34,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 1,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 26,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 33,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 40,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_type',
    'label' => 'Grievance Complaint',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_type_other'.
  $field_instances['node-grievance-field_grievance_type_other'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 35,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 27,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 14,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 43,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_type_other',
    'label' => 'Grievance Complaint',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_violation'.
  $field_instances['node-grievance-field_grievance_violation'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the article of the contract that was violated, for example: Article ....',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 32,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 28,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 37,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_violation',
    'label' => 'Violation',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 39,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_work_status'.
  $field_instances['node-grievance-field_grievance_work_status'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 49,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 47,
      ),
      'grievance_gform' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 64,
      ),
      'grievance_gform_a' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 67,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_work_status',
    'label' => 'Work Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_zip'.
  $field_instances['node-grievance-field_grievance_zip'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 42,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 33,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 21,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'token' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 60,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_zip',
    'label' => 'Postal Code',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 10,
      ),
      'type' => 'text_textfield',
      'weight' => 35,
    ),
  );

  // Exported field_instance: 'node-grievance_attachment-field_grievance'.
  $field_instances['node-grievance_attachment-field_grievance'] = array(
    'bundle' => 'grievance_attachment',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance',
    'label' => 'Grievance',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_attachment-field_grievance_attachments'.
  $field_instances['node-grievance_attachment-field_grievance_attachments'] = array(
    'bundle' => 'grievance_attachment',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachment',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc png mov mp3 mp4 jpg jpeg gif docx xls xlsx rtf htm html',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_attachments',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_attachment-field_grievance_document_type'.
  $field_instances['node-grievance_attachment-field_grievance_document_type'] = array(
    'bundle' => 'grievance_attachment',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_document_type',
    'label' => 'Attachment Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => 'term_reference_1',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => 'grievance_attachment_types',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-grievance_attachment-field_grievance_label'.
  $field_instances['node-grievance_attachment-field_grievance_label'] = array(
    'bundle' => 'grievance_attachment',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_label',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-grievance_basic_page-body'.
  $field_instances['node-grievance_basic_page-body'] = array(
    'bundle' => 'grievance_basic_page',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_basic_page-field_grievance_attachments'.
  $field_instances['node-grievance_basic_page-field_grievance_attachments'] = array(
    'bundle' => 'grievance_basic_page',
    'deleted' => 0,
    'description' => 'Attachments will be listed at the bottom of the page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'txt pdf xls xlsx rtf doc docx',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_basic_page',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_address'.
  $field_instances['node-grievance_chapter-field_grievance_address'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_city'.
  $field_instances['node-grievance_chapter-field_grievance_city'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_city',
    'label' => 'City',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_co_fax'.
  $field_instances['node-grievance_chapter-field_grievance_co_fax'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_fax',
    'label' => 'Fax',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_phone'.
  $field_instances['node-grievance_chapter-field_grievance_phone'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_phone',
    'label' => 'Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_state'.
  $field_instances['node-grievance_chapter-field_grievance_state'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_state',
    'label' => 'State or Province',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-grievance_chapter-field_grievance_zip'.
  $field_instances['node-grievance_chapter-field_grievance_zip'] = array(
    'bundle' => 'grievance_chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_zip',
    'label' => 'Postal Code',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract-field_grievance_contract'.
  $field_instances['node-grievance_contract-field_grievance_contract'] = array(
    'bundle' => 'grievance_contract',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract',
    'label' => 'Contract',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc docx rtf',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_contract/contract/[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract-field_grievance_contract_tplt'.
  $field_instances['node-grievance_contract-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance_contract',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance_contract-field_grievance_valid'.
  $field_instances['node-grievance_contract-field_grievance_valid'] = array(
    'bundle' => 'grievance_contract',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_valid',
    'label' => 'Valid',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'blank',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-10:+10',
      ),
      'type' => 'date_popup',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_clause'.
  $field_instances['node-grievance_contract_clause-field_grievance_clause'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_clause',
    'label' => 'Full Text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_contract_section'.
  $field_instances['node-grievance_contract_clause-field_grievance_contract_section'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_section',
    'label' => 'Article',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_contract_tplt'.
  $field_instances['node-grievance_contract_clause-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_dummy'.
  $field_instances['node-grievance_contract_clause-field_grievance_dummy'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Is this a "dummy" or "placeholder" section?  This field should be left blank or set to "No" except for automatically-generated placeholder sections created when the article itself is meant to be selectable.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_dummy',
    'label' => 'Dummy?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_label'.
  $field_instances['node-grievance_contract_clause-field_grievance_label'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'E.g. 04.01 or 04.01.01 or 04.01.A',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_label',
    'label' => 'Section Number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_remedy'.
  $field_instances['node-grievance_contract_clause-field_grievance_remedy'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_remedy',
    'label' => 'Recommended Remedy',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_type'.
  $field_instances['node-grievance_contract_clause-field_grievance_type'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'When the grievance admin selects this clause, which "statement of grievance" should be selected?',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_type',
    'label' => 'Recommended Statement of Grievance',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_weight'.
  $field_instances['node-grievance_contract_clause-field_grievance_weight'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 20,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_weight',
    'label' => 'Weight',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievanct_cont_clse_tags'.
  $field_instances['node-grievance_contract_clause-field_grievanct_cont_clse_tags'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievanct_cont_clse_tags',
    'label' => 'Section Tags',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_sirius_page'.
  $field_instances['node-grievance_contract_clause-field_sirius_page'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_page',
    'label' => 'Page',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 3,
      ),
      'type' => 'text_textfield',
      'weight' => 31,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_alert_tid'.
  $field_instances['node-grievance_contract_rule-field_grievance_alert_tid'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_tid',
    'label' => 'Next Step',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_contract_tplt'.
  $field_instances['node-grievance_contract_rule-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_days'.
  $field_instances['node-grievance_contract_rule-field_grievance_days'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_days',
    'label' => 'Days',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_days_type'.
  $field_instances['node-grievance_contract_rule-field_grievance_days_type'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_days_type',
    'label' => 'Type of Days',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_from_status'.
  $field_instances['node-grievance_contract_rule-field_grievance_from_status'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_from_status',
    'label' => 'From Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_next_status'.
  $field_instances['node-grievance_contract_rule-field_grievance_next_status'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_next_status',
    'label' => 'Next Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_weight'.
  $field_instances['node-grievance_contract_rule-field_grievance_weight'] = array(
    'bundle' => 'grievance_contract_rule',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Used for sorting; generally set by drag-and-drop.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_weight',
    'label' => 'Weight',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 10,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_section-field_grievance_cont_sec_tags'.
  $field_instances['node-grievance_contract_section-field_grievance_cont_sec_tags'] = array(
    'bundle' => 'grievance_contract_section',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_cont_sec_tags',
    'label' => 'Article Tags',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_section-field_grievance_contract_tplt'.
  $field_instances['node-grievance_contract_section-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance_contract_section',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_section-field_grievance_section_number'.
  $field_instances['node-grievance_contract_section-field_grievance_section_number'] = array(
    'bundle' => 'grievance_contract_section',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_section_number',
    'label' => 'Article Number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 12,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_section-field_grievance_weight'.
  $field_instances['node-grievance_contract_section-field_grievance_weight'] = array(
    'bundle' => 'grievance_contract_section',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_weight',
    'label' => 'Weight',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_section-field_sirius_page'.
  $field_instances['node-grievance_contract_section-field_sirius_page'] = array(
    'bundle' => 'grievance_contract_section',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_page',
    'label' => 'Page',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 3,
      ),
      'type' => 'text_textfield',
      'weight' => 32,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_artsel'.
  $field_instances['node-grievance_contract_template-field_grievance_artsel'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Can the articles in this contract be selected?  If "Yes", then a "dummy" section will be created for each article.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_artsel',
    'label' => 'Articles Selectable?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_classifications'.
  $field_instances['node-grievance_contract_template-field_grievance_classifications'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_classifications',
    'label' => 'Possible Classifications',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 1,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_comments'.
  $field_instances['node-grievance_contract_template-field_grievance_comments'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_comments',
    'label' => 'Notes',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_cont_tplt_tags'.
  $field_instances['node-grievance_contract_template-field_grievance_cont_tplt_tags'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_cont_tplt_tags',
    'label' => 'Template Tags',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_contract'.
  $field_instances['node-grievance_contract_template-field_grievance_contract'] = array(
    'bundle' => 'grievance_contract_template',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract',
    'label' => 'Contract',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc rtf docx',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance/contracts/template/[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_departments'.
  $field_instances['node-grievance_contract_template-field_grievance_departments'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_departments',
    'label' => 'Possible Departments',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_document_types'.
  $field_instances['node-grievance_contract_template-field_grievance_document_types'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_document_types',
    'label' => 'Default Information Request',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_holidays'.
  $field_instances['node-grievance_contract_template-field_grievance_holidays'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_holidays',
    'label' => 'Holidays',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_grievance_pullclause'.
  $field_instances['node-grievance_contract_template-field_grievance_pullclause'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '(i.e. "... accordance with the provisions of [clause goes here] of our Collective Bargaining Agreement ...")',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_pullclause',
    'label' => 'Arbitration Clause',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_template-field_sirius_active'.
  $field_instances['node-grievance_contract_template-field_sirius_active'] = array(
    'bundle' => 'grievance_contract_template',
    'default_value' => array(
      0 => array(
        'value' => 'Yes',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_active',
    'label' => 'Active?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 32,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_bundle'.
  $field_instances['node-grievance_field_overrides-field_grievance_bundle'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Bundle to overried, e.g. "grievance"',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_bundle',
    'label' => 'Bundle',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_default'.
  $field_instances['node-grievance_field_overrides-field_grievance_default'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_default',
    'label' => 'Default Value',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_description'.
  $field_instances['node-grievance_field_overrides-field_grievance_description'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_description',
    'label' => 'Description',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_entity_type'.
  $field_instances['node-grievance_field_overrides-field_grievance_entity_type'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Entity type to override, almost always "node".',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_entity_type',
    'label' => 'Entity Type',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_field_name'.
  $field_instances['node-grievance_field_overrides-field_grievance_field_name'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Field name to override, e.g. "field_grievance_organizer"',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_field_name',
    'label' => 'Field Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_field_overrides-field_grievance_label'.
  $field_instances['node-grievance_field_overrides-field_grievance_label'] = array(
    'bundle' => 'grievance_field_overrides',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_label',
    'label' => 'Label',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-grievance_holiday-field_grievance_annual'.
  $field_instances['node-grievance_holiday-field_grievance_annual'] = array(
    'bundle' => 'grievance_holiday',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Does this holiday repeat every year on the same date?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_annual',
    'label' => 'Annual?',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'node-grievance_holiday-field_grievance_date'.
  $field_instances['node-grievance_holiday-field_grievance_date'] = array(
    'bundle' => 'grievance_holiday',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_date',
    'label' => 'Date',
    'required' => 1,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => -4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_irset-field_grievance_document_types'.
  $field_instances['node-grievance_irset-field_grievance_document_types'] = array(
    'bundle' => 'grievance_irset',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_document_types',
    'label' => 'Information Request',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => 'term_reference_1',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => 'grievance_information_request_types',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-grievance_letter_template-body'.
  $field_instances['node-grievance_letter_template-body'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_attachments'.
  $field_instances['node-grievance_letter_template-field_grievance_attachments'] = array(
    'bundle' => 'grievance_letter_template',
    'deleted' => 0,
    'description' => 'Use this field for a "letter" which has no customization, and which is simply meant to be downloaded.  No drop-in tokens are supported.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc png mov mp3 mp4 jpg jpeg gif docx xls xlsx rtf',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_letter_templates',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_css'.
  $field_instances['node-grievance_letter_template-field_grievance_css'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter CSS directives which should be included inline in the print version of this letter.  E.g. "#grievance-letter-body { font-size: 10pt; }"',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_css',
    'label' => 'CSS',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_emails'.
  $field_instances['node-grievance_letter_template-field_grievance_emails'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'These email addresses will be CC\'d on EVERY letter sent using this template.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_emails',
    'label' => 'CC Emails',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_images'.
  $field_instances['node-grievance_letter_template-field_grievance_images'] = array(
    'bundle' => 'grievance_letter_template',
    'deleted' => 0,
    'description' => 'Upload images to include in letter templates, such as a scanned signature.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_images',
    'label' => 'Images',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif pdf jpg jpeg',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_images',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_notify_body'.
  $field_instances['node-grievance_letter_template-field_grievance_notify_body'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Use this field to enter the default body of the email; it should generally be something like "Please refer to the attached document re. a grievance filed against your company."',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_notify_body',
    'label' => 'Body (Email)',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 10,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_notify_subject'.
  $field_instances['node-grievance_letter_template-field_grievance_notify_subject'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_notify_subject',
    'label' => 'Subject',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_roles'.
  $field_instances['node-grievance_letter_template-field_grievance_roles'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'role_field',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'role_field_formatter',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_roles',
    'label' => 'Roles',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_shortname'.
  $field_instances['node-grievance_letter_template-field_grievance_shortname'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Should contain only letters and numbers and underscore, used to generate filenames.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shortname',
    'label' => 'Short Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_grievance_update_rep'.
  $field_instances['node-grievance_letter_template-field_grievance_update_rep'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => array(
      0 => array(
        'value' => 'Yes',
      ),
    ),
    'deleted' => 0,
    'description' => 'If "Yes", then the default will be that any letter sent using this template will also set the grievance\'s "company representative" field to be the main recipient of the letter.  Generally this should be "Yes" for notifications to the company -- grievance filings and so forth -- and "No" for notifications to the grievant or letters that are meant to be sent internally.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_update_rep',
    'label' => 'Update Company Representative',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 11,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_letter_template-field_sirius_mustlog'.
  $field_instances['node-grievance_letter_template-field_sirius_mustlog'] = array(
    'bundle' => 'grievance_letter_template',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => 'If "Yes", then letters of this type MUST be logged.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_mustlog',
    'label' => 'Must Log?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance_log-body'.
  $field_instances['node-grievance_log-body'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Comments',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance'.
  $field_instances['node-grievance_log-field_grievance'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance',
    'label' => 'Grievance',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_alert'.
  $field_instances['node-grievance_log-field_grievance_alert'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert',
    'label' => 'Next Step',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_alert_date'.
  $field_instances['node-grievance_log-field_grievance_alert_date'] = array(
    'bundle' => 'grievance_log',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_date',
    'label' => 'Next Deadline',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_alert_tid'.
  $field_instances['node-grievance_log-field_grievance_alert_tid'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_alert_tid',
    'label' => 'Next Step',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_attachments'.
  $field_instances['node-grievance_log-field_grievance_attachments'] = array(
    'bundle' => 'grievance_log',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc png mov mp3 mp4 jpg jpeg gif docx xls xlsx',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_attachments',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_days'.
  $field_instances['node-grievance_log-field_grievance_days'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Number of days to extend the timelines',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 26,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_days',
    'label' => 'Number of Days',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_external_id'.
  $field_instances['node-grievance_log-field_grievance_external_id'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_log_tags'.
  $field_instances['node-grievance_log-field_grievance_log_tags'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_log_tags',
    'label' => 'Tags',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_log_type'.
  $field_instances['node-grievance_log-field_grievance_log_type'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_log_type',
    'label' => 'Grievance Log Type',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_status'.
  $field_instances['node-grievance_log-field_grievance_status'] = array(
    'bundle' => 'grievance_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_status_date'.
  $field_instances['node-grievance_log-field_grievance_status_date'] = array(
    'bundle' => 'grievance_log',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_status_date',
    'label' => 'Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-grievance_news-body'.
  $field_instances['node-grievance_news-body'] = array(
    'bundle' => 'grievance_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-grievance_news-field_grievance_attachments'.
  $field_instances['node-grievance_news-field_grievance_attachments'] = array(
    'bundle' => 'grievance_news',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'pdf txt doc docx xls xlsx rtf png ppt',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_news',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'node-grievance_settlement-field_grievance'.
  $field_instances['node-grievance_settlement-field_grievance'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance',
    'label' => 'Grievance',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-grievance_settlement-field_grievance_amt'.
  $field_instances['node-grievance_settlement-field_grievance_amt'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => ',',
          'field_delimiter' => '',
          'prefix_suffix' => 1,
          'scale' => 2,
          'thousand_separator' => ',',
        ),
        'type' => 'number_decimal',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_amt',
    'label' => 'Settlement Amount',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '$',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_settlement-field_grievance_amt_rcvd'.
  $field_instances['node-grievance_settlement-field_grievance_amt_rcvd'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => 1,
          'scale' => 2,
          'thousand_separator' => ',',
        ),
        'type' => 'number_decimal',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_amt_rcvd',
    'label' => 'Amount Received',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '$',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_settlement-field_grievance_checkno'.
  $field_instances['node-grievance_settlement-field_grievance_checkno'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_checkno',
    'label' => 'Check Number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_settlement-field_grievance_comments'.
  $field_instances['node-grievance_settlement-field_grievance_comments'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_comments',
    'label' => 'Comments',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_settlement-field_grievance_date_1'.
  $field_instances['node-grievance_settlement-field_grievance_date_1'] = array(
    'bundle' => 'grievance_settlement',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_date_1',
    'label' => 'Date Received',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_settlement-field_grievance_settlement_tags'.
  $field_instances['node-grievance_settlement-field_grievance_settlement_tags'] = array(
    'bundle' => 'grievance_settlement',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_settlement_tags',
    'label' => 'Settlement Tags',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_grievance_assignee_notes'.
  $field_instances['node-grievance_shop-field_grievance_assignee_notes'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_assignee_notes',
    'label' => 'Assignee Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_attachments'.
  $field_instances['node-grievance_shop-field_grievance_attachments'] = array(
    'bundle' => 'grievance_shop',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc docx xls xlsx rtf ppt zip',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_shop/attachments',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_chapter'.
  $field_instances['node-grievance_shop-field_grievance_chapter'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_chapter',
    'label' => 'Chapter',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_company'.
  $field_instances['node-grievance_shop-field_grievance_company'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the company that this shop is associated with.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_company',
    'label' => 'Company',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_contract'.
  $field_instances['node-grievance_shop-field_grievance_contract'] = array(
    'bundle' => 'grievance_shop',
    'deleted' => 0,
    'description' => 'Upload the current contract document for this shop.  The contract will be used by stewards to identify violations.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract',
    'label' => 'Contract',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'grievance_contract',
      'file_extensions' => 'pdf',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance/contracts/shop/[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'grievance_contract',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 10,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_grievance_contract_tplt'.
  $field_instances['node-grievance_shop-field_grievance_contract_tplt'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_external_id'.
  $field_instances['node-grievance_shop-field_grievance_external_id'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_grievance_rep_assignee'.
  $field_instances['node-grievance_shop-field_grievance_rep_assignee'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_assignee',
    'label' => 'Assignee',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_rep_filed'.
  $field_instances['node-grievance_shop-field_grievance_rep_filed'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 12,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_filed',
    'label' => 'Filed By',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_rep_lead'.
  $field_instances['node-grievance_shop-field_grievance_rep_lead'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 13,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_lead',
    'label' => 'Lead Organizer',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_rep_manager'.
  $field_instances['node-grievance_shop-field_grievance_rep_manager'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 14,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_manager',
    'label' => 'Grievance Admin',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 22,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_grievance_rep_organizer'.
  $field_instances['node-grievance_shop-field_grievance_rep_organizer'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_organizer',
    'label' => 'Organizer',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 19,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_grievance_rep_watching'.
  $field_instances['node-grievance_shop-field_grievance_rep_watching'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_watching',
    'label' => 'Watching',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_tags'.
  $field_instances['node-grievance_shop-field_grievance_tags'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_tags',
    'label' => 'Tags',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_sirius_active'.
  $field_instances['node-grievance_shop-field_sirius_active'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => array(
      0 => array(
        'value' => 'Yes',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 20,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_active',
    'label' => 'Active',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_sirius_dispatch_job_types'.
  $field_instances['node-grievance_shop-field_sirius_dispatch_job_types'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Indicates which job types this employer can create.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_types',
    'label' => 'Job Types',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 27,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop-field_sirius_dispatch_notesdef'.
  $field_instances['node-grievance_shop-field_sirius_dispatch_notesdef'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_notesdef',
    'label' => 'Default Dispatch Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 26,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_sirius_id'.
  $field_instances['node-grievance_shop-field_sirius_id'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_sirius_industry'.
  $field_instances['node-grievance_shop-field_sirius_industry'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 18,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_sirius_name_tts'.
  $field_instances['node-grievance_shop-field_sirius_name_tts'] = array(
    'bundle' => 'grievance_shop',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_name_tts',
    'label' => 'TtS Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_address'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_address'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_address_2'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_address_2'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_address_2',
    'label' => 'Address 2',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_city'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_city'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_city',
    'label' => 'City',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_email'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_email'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'email',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'email_plain',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_email',
    'label' => 'Email',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'email',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'email_textfield',
      'weight' => 24,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_fax'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_fax'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_fax',
    'label' => 'Fax',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 28,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_name'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_name'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_name',
    'label' => 'Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_phone'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_phone'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_phone',
    'label' => 'Office Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 25,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_phone_2'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_phone_2'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_phone_2',
    'label' => 'Personal Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 27,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_role'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_role'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the role of this representative, e.g. "Attorney" or "Manager".',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_role',
    'label' => 'Role',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_state'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_state'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_state',
    'label' => 'State or Province',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_co_zip'.
  $field_instances['node-grievance_shop_contact-field_grievance_co_zip'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_zip',
    'label' => 'Postal Code',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_company'.
  $field_instances['node-grievance_shop_contact-field_grievance_company'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If selected, this contact will appear for all shops associated with this company.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_company',
    'label' => 'Company',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 14,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_contact_types'.
  $field_instances['node-grievance_shop_contact-field_grievance_contact_types'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_contact_types',
    'label' => 'Contact Type',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_description'.
  $field_instances['node-grievance_shop_contact-field_grievance_description'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'E.g. "Step 1 Contact" or "Contact for Arbitration Only"',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_plain',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_description',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_external_id'.
  $field_instances['node-grievance_shop_contact-field_grievance_external_id'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-grievance_shop_contact-field_grievance_min'.
  $field_instances['node-grievance_shop_contact-field_grievance_min'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_min',
    'label' => 'Member ID #',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_shops'.
  $field_instances['node-grievance_shop_contact-field_grievance_shops'] = array(
    'bundle' => 'grievance_shop_contact',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 26,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shops',
    'label' => 'Shops',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-body'.
  $field_instances['node-sirius_bulk-body'] = array(
    'bundle' => 'sirius_bulk',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'HTML formatting is permitted, but all links should be absolute.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Email Body',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-field_sirius_bulk_medium'.
  $field_instances['node-sirius_bulk-field_sirius_bulk_medium'] = array(
    'bundle' => 'sirius_bulk',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_bulk_medium',
    'label' => 'Medium',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-field_sirius_bulk_status'.
  $field_instances['node-sirius_bulk-field_sirius_bulk_status'] = array(
    'bundle' => 'sirius_bulk',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_bulk_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-field_sirius_datetime'.
  $field_instances['node-sirius_bulk-field_sirius_datetime'] = array(
    'bundle' => 'sirius_bulk',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime',
    'label' => 'Send Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_bulk-field_sirius_datetime_completed'.
  $field_instances['node-sirius_bulk-field_sirius_datetime_completed'] = array(
    'bundle' => 'sirius_bulk',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_completed',
    'label' => 'Completed',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-field_sirius_sms'.
  $field_instances['node-sirius_bulk-field_sirius_sms'] = array(
    'bundle' => 'sirius_bulk',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_sms',
    'label' => 'SMS Body',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_bulk-field_sirius_voice'.
  $field_instances['node-sirius_bulk-field_sirius_voice'] = array(
    'bundle' => 'sirius_bulk',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_voice',
    'label' => 'Voice Body',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_aat'.
  $field_instances['node-sirius_dispatch-field_sirius_aat'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '"Access token" field, used to permit viewing of this node for unauthenticated users.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_aat',
    'label' => 'AAT',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_datetime'.
  $field_instances['node-sirius_dispatch-field_sirius_datetime'] = array(
    'bundle' => 'sirius_dispatch',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 5,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime',
    'label' => 'Notified',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch-field_sirius_datetime_completed'.
  $field_instances['node-sirius_dispatch-field_sirius_datetime_completed'] = array(
    'bundle' => 'sirius_dispatch',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 6,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_completed',
    'label' => 'Responded',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch-field_sirius_datetime_created'.
  $field_instances['node-sirius_dispatch-field_sirius_datetime_created'] = array(
    'bundle' => 'sirius_dispatch',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_created',
    'label' => 'Created',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_dispatch_cbn'.
  $field_instances['node-sirius_dispatch-field_sirius_dispatch_cbn'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_cbn',
    'label' => 'Call by Name?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_dispatch_job'.
  $field_instances['node-sirius_dispatch-field_sirius_dispatch_job'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 0,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job',
    'label' => 'Job',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch-field_sirius_dispatch_status'.
  $field_instances['node-sirius_dispatch-field_sirius_dispatch_status'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_id'.
  $field_instances['node-sirius_dispatch-field_sirius_id'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_payrate'.
  $field_instances['node-sirius_dispatch-field_sirius_payrate'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 7,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_payrate',
    'label' => 'Payrate',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch-field_sirius_worker'.
  $field_instances['node-sirius_dispatch-field_sirius_worker'] = array(
    'bundle' => 'sirius_dispatch',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 0,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 20,
      ),
      'teaser' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_worker',
    'label' => 'Worker',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility-field_sirius_address'.
  $field_instances['node-sirius_dispatch_facility-field_sirius_address'] = array(
    'bundle' => 'sirius_dispatch_facility',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'addressfield',
        'settings' => array(
          'field_delimiter' => '',
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 2,
      ),
      'sirius_worker' => array(
        'label' => 'hidden',
        'module' => 'addressfield',
        'settings' => array(
          'field_delimiter' => '',
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'addressfield',
      'settings' => array(
        'available_countries' => array(),
        'default_country' => 'US',
        'format_handlers' => array(
          'address' => 'address',
          'address-hide-country' => 'address-hide-country',
          'address-hide-postal-code' => 0,
          'address-hide-street' => 0,
          'address-hide-administrative-area' => 0,
          'address-hide-locality' => 0,
          'organisation' => 0,
          'name-full' => 0,
          'name-oneline' => 0,
          'address-optional' => 0,
        ),
      ),
      'type' => 'addressfield_standard',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility-field_sirius_address_geo'.
  $field_instances['node-sirius_dispatch_facility-field_sirius_address_geo'] = array(
    'bundle' => 'sirius_dispatch_facility',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'geofield_map',
        'settings' => array(
          'field_delimiter' => '',
          'geofield_map_baselayers_hybrid' => 1,
          'geofield_map_baselayers_map' => 1,
          'geofield_map_baselayers_physical' => 0,
          'geofield_map_baselayers_satellite' => 1,
          'geofield_map_center' => array(
            'geocode' => 'Find my location',
            'lat' => '',
            'lon' => '',
          ),
          'geofield_map_controltype' => 'default',
          'geofield_map_draggable' => 0,
          'geofield_map_height' => '300px',
          'geofield_map_maptype' => 'map',
          'geofield_map_max_zoom' => 0,
          'geofield_map_min_zoom' => 0,
          'geofield_map_mtc' => 'standard',
          'geofield_map_overview' => 0,
          'geofield_map_overview_opened' => 0,
          'geofield_map_pancontrol' => 1,
          'geofield_map_scale' => 0,
          'geofield_map_scrollwheel' => 0,
          'geofield_map_streetview_show' => 0,
          'geofield_map_width' => '300px',
          'geofield_map_zoom' => 14,
        ),
        'type' => 'geofield_map_map',
        'weight' => 3,
      ),
      'sirius_worker' => array(
        'label' => 'hidden',
        'module' => 'geofield_map',
        'settings' => array(
          'field_delimiter' => '',
          'geofield_map_baselayers_hybrid' => 1,
          'geofield_map_baselayers_map' => 1,
          'geofield_map_baselayers_physical' => 0,
          'geofield_map_baselayers_satellite' => 1,
          'geofield_map_center' => array(
            'geocode' => 'Find my location',
            'lat' => '',
            'lon' => '',
          ),
          'geofield_map_controltype' => 'default',
          'geofield_map_draggable' => 0,
          'geofield_map_height' => '300px',
          'geofield_map_maptype' => 'map',
          'geofield_map_max_zoom' => 0,
          'geofield_map_min_zoom' => 0,
          'geofield_map_mtc' => 'standard',
          'geofield_map_overview' => 0,
          'geofield_map_overview_opened' => 0,
          'geofield_map_pancontrol' => 1,
          'geofield_map_scale' => 0,
          'geofield_map_scrollwheel' => 0,
          'geofield_map_streetview_show' => 0,
          'geofield_map_width' => '300px',
          'geofield_map_zoom' => 8,
        ),
        'type' => 'geofield_map_map',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_address_geo',
    'label' => 'Address Geocode',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'geocoder',
      'settings' => array(
        'delta_handling' => 'default',
        'geocoder_field' => 'field_sirius_address',
        'geocoder_handler' => 'google',
        'handler_settings' => array(
          'google' => array(
            'all_results' => 0,
            'biasing' => array(
              'bounds' => '',
              'components' => '',
              'region' => '',
            ),
            'geometry_type' => 'point',
            'reject_results' => array(
              'APPROXIMATE' => 0,
              'GEOMETRIC_CENTER' => 0,
              'RANGE_INTERPOLATED' => 0,
              'ROOFTOP' => 0,
            ),
          ),
        ),
        'latlng_override' => 0,
      ),
      'type' => 'geocoder',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_facility-field_sirius_id'.
  $field_instances['node-sirius_dispatch_facility-field_sirius_id'] = array(
    'bundle' => 'sirius_dispatch_facility',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility-field_sirius_name_tts'.
  $field_instances['node-sirius_dispatch_facility-field_sirius_name_tts'] = array(
    'bundle' => 'sirius_dispatch_facility',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Name as it should be rendered by Text to Speech systems',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_name_tts',
    'label' => 'TtS Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility_hall-field_sirius_dispatch_facility'.
  $field_instances['node-sirius_dispatch_facility_hall-field_sirius_dispatch_facility'] = array(
    'bundle' => 'sirius_dispatch_facility_hall',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 0,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_facility',
    'label' => 'Facility',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility_hall-field_sirius_id'.
  $field_instances['node-sirius_dispatch_facility_hall-field_sirius_id'] = array(
    'bundle' => 'sirius_dispatch_facility_hall',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_facility_hall-field_sirius_name_short'.
  $field_instances['node-sirius_dispatch_facility_hall-field_sirius_name_short'] = array(
    'bundle' => 'sirius_dispatch_facility_hall',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_name_short',
    'label' => 'Abbreviation',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_grievance_departments'.
  $field_instances['node-sirius_dispatch_job-field_grievance_departments'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 10,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_departments',
    'label' => 'Departments',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_grievance_shop'.
  $field_instances['node-sirius_dispatch_job-field_grievance_shop'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shop',
    'label' => 'Employer',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_address_notes'.
  $field_instances['node-sirius_dispatch_job-field_sirius_address_notes'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_address_notes',
    'label' => 'Report Location',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 12,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_address_parking'.
  $field_instances['node-sirius_dispatch_job-field_sirius_address_parking'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_address_parking',
    'label' => 'Parking Instructions',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_batch_max'.
  $field_instances['node-sirius_dispatch_job-field_sirius_batch_max'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 20,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_batch_max',
    'label' => 'Batch Max',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_batch_min'.
  $field_instances['node-sirius_dispatch_job-field_sirius_batch_min'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 21,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_batch_min',
    'label' => 'Batch Min',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 28,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_batch_ratio'.
  $field_instances['node-sirius_dispatch_job-field_sirius_batch_ratio'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 22,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_batch_ratio',
    'label' => 'Batch Ratio',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 29,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_count'.
  $field_instances['node-sirius_dispatch_job-field_sirius_count'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 8,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_count',
    'label' => 'Worker Count',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_count_no'.
  $field_instances['node-sirius_dispatch_job-field_sirius_count_no'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 11,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 29,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_count_no',
    'label' => 'Declined Count',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 22,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_count_notified'.
  $field_instances['node-sirius_dispatch_job-field_sirius_count_notified'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 30,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_count_notified',
    'label' => 'Notified Count',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_count_yes'.
  $field_instances['node-sirius_dispatch_job-field_sirius_count_yes'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 10,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 28,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_count_yes',
    'label' => 'Accepted Count',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_datetime'.
  $field_instances['node-sirius_dispatch_job-field_sirius_datetime'] = array(
    'bundle' => 'sirius_dispatch_job',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 11,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime',
    'label' => 'Report Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - g:i:sa',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_datetime_completed'.
  $field_instances['node-sirius_dispatch_job-field_sirius_datetime_completed'] = array(
    'bundle' => 'sirius_dispatch_job',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 23,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 26,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 5,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_completed',
    'label' => 'Filled',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_datetime_created'.
  $field_instances['node-sirius_dispatch_job-field_sirius_datetime_created'] = array(
    'bundle' => 'sirius_dispatch_job',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 8,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'teaser' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_created',
    'label' => 'Created',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_eba'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_eba'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 16,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_eba',
    'label' => 'Include EBA Workers',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 24,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_eba_dates'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_eba_dates'] = array(
    'bundle' => 'sirius_dispatch_job',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only_sortable',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 17,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_eba_dates',
    'label' => 'EBA Dates',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'field_lock_multi_values' => array(
        'disable_reorder' => 1,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 25,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_facility'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_facility'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 13,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_facility',
    'label' => 'Facility',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 10,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_hall'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_hall'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 14,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 1,
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_hall',
    'label' => 'Hall',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 11,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_job_group'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_job_group'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 12,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_group',
    'label' => 'Group or Show',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 9,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_job_nfcns'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_job_nfcns'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_nfcns',
    'label' => 'Notifications',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 42,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_job_status'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_job_status'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_job_tags'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_job_tags'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 16,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_tags',
    'label' => 'Job Tags',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 14,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_dispatch_job_type'.
  $field_instances['node-sirius_dispatch_job-field_sirius_dispatch_job_type'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 7,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_job_type',
    'label' => 'Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_email'.
  $field_instances['node-sirius_dispatch_job-field_sirius_email'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_email',
    'label' => 'Notification Email',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 25,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_emails'.
  $field_instances['node-sirius_dispatch_job-field_sirius_emails'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_emails',
    'label' => 'Notification Emails',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_id'.
  $field_instances['node-sirius_dispatch_job-field_sirius_id'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'Call Number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_id2'.
  $field_instances['node-sirius_dispatch_job-field_sirius_id2'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id2',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_industry'.
  $field_instances['node-sirius_dispatch_job-field_sirius_industry'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 2,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_member_status'.
  $field_instances['node-sirius_dispatch_job-field_sirius_member_status'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 15,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_member_status',
    'label' => 'Member Status',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_notes'.
  $field_instances['node-sirius_dispatch_job-field_sirius_notes'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'sirius_worker' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sirius_notes',
    'label' => 'Notes',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_notify'.
  $field_instances['node-sirius_dispatch_job-field_sirius_notify'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 8,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_notify',
    'label' => 'Send Notifications?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 40,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job-field_sirius_representatives'.
  $field_instances['node-sirius_dispatch_job-field_sirius_representatives'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'sirius',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_user_worker',
        'weight' => 10,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_representatives',
    'label' => 'Representatives',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_skill'.
  $field_instances['node-sirius_dispatch_job-field_sirius_skill'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_skill',
    'label' => 'Skills',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_summary'.
  $field_instances['node-sirius_dispatch_job-field_sirius_summary'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_summary',
    'label' => 'Status Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 30,
    ),
  );

  // Exported field_instance: 'node-sirius_dispatch_job-field_sirius_timelimit'.
  $field_instances['node-sirius_dispatch_job-field_sirius_timelimit'] = array(
    'bundle' => 'sirius_dispatch_job',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 19,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_timelimit',
    'label' => 'Expire After',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => ' Minute| Minutes',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 26,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job_group-field_sirius_datetime'.
  $field_instances['node-sirius_dispatch_job_group-field_sirius_datetime'] = array(
    'bundle' => 'sirius_dispatch_job_group',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime',
    'label' => 'Active From',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job_group-field_sirius_datetime_completed'.
  $field_instances['node-sirius_dispatch_job_group-field_sirius_datetime_completed'] = array(
    'bundle' => 'sirius_dispatch_job_group',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_completed',
    'label' => 'Active Until',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_dispatch_job_group-field_sirius_industry'.
  $field_instances['node-sirius_dispatch_job_group-field_sirius_industry'] = array(
    'bundle' => 'sirius_dispatch_job_group',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_help-body'.
  $field_instances['node-sirius_help-body'] = array(
    'bundle' => 'sirius_help',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_help-field_sirius_paths'.
  $field_instances['node-sirius_help-field_sirius_paths'] = array(
    'bundle' => 'sirius_help',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Drupal paths on which to display the help text.  This can either be a path alias such as "my/page/here", or a native Drupal path such as "node/%/ac_questionnaire_questions".',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_paths',
    'label' => 'Paths',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-sirius_help-field_sirius_summary'.
  $field_instances['node-sirius_help-field_sirius_summary'] = array(
    'bundle' => 'sirius_help',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the help text as it should appear on the specified pages.  May contain HTML.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_summary',
    'label' => 'Summary',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-sirius_letterhead-field_sirius_attachments'.
  $field_instances['node-sirius_letterhead-field_sirius_attachments'] = array(
    'bundle' => 'sirius_letterhead',
    'deleted' => 0,
    'description' => 'Upload the letterhead in PDF format. 

If the file has a single page, then that will be used as the background for all pages in the input document. 

If the file has multiple pages, then each page in the input document will use the corresponding page in the letterhead as a background, with the last page in the letterhead repeated as many times as necessary if the input document is longer.  

(For example, if a two-page letterhead is uploaded, then the first page of the letterhead will be used as the first page of the generated document, and the second page will be repeated as many times as necessary.)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_attachments',
    'label' => 'PDF',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'pdf',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => FALSE,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'sirius_letterhead',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_letterhead-field_sirius_json'.
  $field_instances['node-sirius_letterhead-field_sirius_json'] = array(
    'bundle' => 'sirius_letterhead',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'sirius_jsonfield',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_jsonfield_formatter_full',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_json',
    'label' => 'JSON',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'sirius_jsonfield',
      'settings' => array(),
      'type' => 'sirius_jsonfield_widget',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_attachments'.
  $field_instances['node-sirius_log-field_sirius_attachments'] = array(
    'bundle' => 'sirius_log',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'file',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'file_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt pdf png gif jpg jpeg xls xlsx doc docx rtf zip',
      'filefield_paths' => array(
        'active_updating' => 1,
        'file_name' => array(
          'options' => array(
            'pathauto' => 0,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => 0,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'sirius_attachments',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_category'.
  $field_instances['node-sirius_log-field_sirius_category'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_category',
    'label' => 'Category',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_json'.
  $field_instances['node-sirius_log-field_sirius_json'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'sirius_jsonfield',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_jsonfield_formatter_full',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_json',
    'label' => 'JSON',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'sirius_jsonfield',
      'settings' => array(),
      'type' => 'sirius_jsonfield_widget_raw',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_log_handler'.
  $field_instances['node-sirius_log-field_sirius_log_handler'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => 0,
          'field_delimiter' => '',
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_log_handler',
    'label' => 'Handler',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_message'.
  $field_instances['node-sirius_log-field_sirius_message'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_message',
    'label' => 'Message',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_notes'.
  $field_instances['node-sirius_log-field_sirius_notes'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_notes',
    'label' => 'Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_summary'.
  $field_instances['node-sirius_log-field_sirius_summary'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_summary',
    'label' => 'Summary',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_log-field_sirius_type'.
  $field_instances['node-sirius_log-field_sirius_type'] = array(
    'bundle' => 'sirius_log',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_type',
    'label' => 'Type',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_news-body'.
  $field_instances['node-sirius_news-body'] = array(
    'bundle' => 'sirius_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-sirius_news-field_sirius_boolean'.
  $field_instances['node-sirius_news-field_sirius_boolean'] = array(
    'bundle' => 'sirius_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_boolean',
    'label' => 'Pop up',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_news-field_sirius_datetime_completed'.
  $field_instances['node-sirius_news-field_sirius_datetime_completed'] = array(
    'bundle' => 'sirius_news',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 18,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_datetime_completed',
    'label' => 'Popup Until',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_news-field_sirius_notify'.
  $field_instances['node-sirius_news-field_sirius_notify'] = array(
    'bundle' => 'sirius_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_notify',
    'label' => 'Send Notifications?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-sirius_news-field_sirius_roles'.
  $field_instances['node-sirius_news-field_sirius_roles'] = array(
    'bundle' => 'sirius_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'What roles should have access to this content',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_roles',
    'label' => 'Roles',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_grievance_departments'.
  $field_instances['node-sirius_worker-field_grievance_departments'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 37,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_departments',
    'label' => 'NOT Available for Departments',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_grievance_shop'.
  $field_instances['node-sirius_worker-field_grievance_shop'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 8,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_shop',
    'label' => 'Current Employer',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_aat'.
  $field_instances['node-sirius_worker-field_sirius_aat'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 32,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_aat',
    'label' => 'AAT',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_aat_required'.
  $field_instances['node-sirius_worker-field_sirius_aat_required'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 42,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_aat_required',
    'label' => 'Passcode required for phone access?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_address'.
  $field_instances['node-sirius_worker-field_sirius_address'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'addressfield',
        'settings' => array(
          'field_delimiter' => '',
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 20,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'module' => 'addressfield',
        'settings' => array(
          'field_delimiter' => '',
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'addressfield',
      'settings' => array(
        'available_countries' => array(
          'US' => 'US',
        ),
        'default_country' => 'US',
        'format_handlers' => array(
          'address' => 'address',
          'address-hide-country' => 'address-hide-country',
          'address-hide-postal-code' => 0,
          'address-hide-street' => 0,
          'address-hide-administrative-area' => 0,
          'address-hide-locality' => 0,
          'organisation' => 0,
          'name-full' => 0,
          'name-oneline' => 0,
          'address-optional' => 0,
        ),
      ),
      'type' => 'addressfield_standard',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_contact'.
  $field_instances['node-sirius_worker-field_sirius_contact'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 45,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_contact',
    'label' => 'Contact',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 48,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_asi'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_asi'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 32,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_asi',
    'label' => 'Auto Sign In?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 36,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_worker-field_sirius_dispatch_availdate'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_availdate'] = array(
    'bundle' => 'sirius_worker',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 7,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 28,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_availdate',
    'label' => 'Available Since',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_cbn'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_cbn'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 34,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 31,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_cbn',
    'label' => 'Call by Name Only?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_dncs'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_dncs'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 11,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_dncs',
    'label' => 'Employer-Initiated Do Not Call',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_dncs2'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_dncs2'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 39,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_dncs2',
    'label' => 'Worker-Initiated Do Not Call',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_eba'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_eba'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => 'Employed but Available',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 43,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_eba',
    'label' => 'EBA',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 46,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_worker-field_sirius_dispatch_eba_dates'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_eba_dates'] = array(
    'bundle' => 'sirius_worker',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only_sortable',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 44,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_eba_dates',
    'label' => 'EBA Dates',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 47,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_hfe'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_hfe'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 40,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 30,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_hfe',
    'label' => 'Hold for Employer',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'content_taxonomy_autocomplete_new_terms' => 'allow',
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 33,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_worker-field_sirius_dispatch_hfe_until'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_hfe_until'] = array(
    'bundle' => 'sirius_worker',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 41,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_hfe_until',
    'label' => 'Hold for Employer Until',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_medium'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_medium'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(
      0 => array(
        'value' => 'voice',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 31,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 33,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_medium',
    'label' => 'Notification Medium',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_dispatch_sib'.
  $field_instances['node-sirius_worker-field_sirius_dispatch_sib'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If set to "Yes", the worker will not be permitted to sign themselves in (set their dispatch status).',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 33,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_dispatch_sib',
    'label' => 'Sign-In Blocked?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_email'.
  $field_instances['node-sirius_worker-field_sirius_email'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_email',
    'label' => 'Email',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_gender'.
  $field_instances['node-sirius_worker-field_sirius_gender'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 7,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_gender',
    'label' => 'Gender',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_headshot'.
  $field_instances['node-sirius_worker-field_sirius_headshot'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'field_delimiter' => '',
          'image_link' => '',
          'image_style' => 'medium',
        ),
        'type' => 'image',
        'weight' => 33,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_headshot',
    'label' => 'Headshot',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => 0,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => '[node:nid]_[file:fid].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => 0,
            'slashes' => 0,
            'transliterate' => 0,
          ),
          'value' => 'sirius_headshot',
        ),
        'redirect' => FALSE,
        'retroactive_update' => 0,
      ),
      'filefield_paths_enabled' => 1,
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'imagefield_crop',
      'settings' => array(
        'collapsible' => 2,
        'croparea' => '500x500',
        'enforce_minimum' => TRUE,
        'enforce_ratio' => TRUE,
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
        'resolution' => '200x150',
      ),
      'type' => 'imagefield_crop_widget',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_id'.
  $field_instances['node-sirius_worker-field_sirius_id'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_id2'.
  $field_instances['node-sirius_worker-field_sirius_id2'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_id2',
    'label' => 'Titan ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_industry'.
  $field_instances['node-sirius_worker-field_sirius_industry'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 12,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_json'.
  $field_instances['node-sirius_worker-field_sirius_json'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'sirius_jsonfield',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_jsonfield_formatter_full',
        'weight' => 46,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_json',
    'label' => 'JSON',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'sirius_jsonfield',
      'settings' => array(),
      'type' => 'sirius_jsonfield_widget',
      'weight' => 50,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_member_status'.
  $field_instances['node-sirius_worker-field_sirius_member_status'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_member_status',
    'label' => 'Member Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'multiple_selects',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'multiple_selects',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_name'.
  $field_instances['node-sirius_worker-field_sirius_name'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'name',
        'settings' => array(
          'field_delimiter' => '',
          'format' => 'default',
          'markup' => 0,
          'multiple' => 'default',
          'multiple_and' => 'text',
          'multiple_delimiter' => ', ',
          'multiple_delimiter_precedes_last' => 'never',
          'multiple_el_al_first' => 1,
          'multiple_el_al_min' => 3,
          'output' => 'default',
        ),
        'type' => 'name_formatter',
        'weight' => 2,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'name',
        'settings' => array(
          'field_delimiter' => '',
          'format' => 'default',
          'markup' => 0,
          'multiple' => 'default',
          'multiple_and' => 'text',
          'multiple_delimiter' => ', ',
          'multiple_delimiter_precedes_last' => 'never',
          'multiple_el_al_first' => 1,
          'multiple_el_al_min' => 3,
          'output' => 'default',
        ),
        'type' => 'name_formatter',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'name',
        'settings' => array(
          'field_delimiter' => '',
          'format' => 'default',
          'markup' => 0,
          'multiple' => 'default',
          'multiple_and' => 'text',
          'multiple_delimiter' => ', ',
          'multiple_delimiter_precedes_last' => 'never',
          'multiple_el_al_first' => 1,
          'multiple_el_al_min' => 3,
          'output' => 'default',
        ),
        'type' => 'name_formatter',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_name',
    'label' => 'Name',
    'required' => 0,
    'settings' => array(
      'component_css' => '',
      'component_layout' => 'default',
      'components' => array(
        'credentials' => 0,
        'family' => 'family',
        'generational' => 0,
        'given' => 'given',
        'middle' => 'middle',
        'title' => 'title',
      ),
      'credentials_inline' => 0,
      'field_type' => array(
        'credentials' => 'text',
        'family' => 'text',
        'generational' => 'select',
        'given' => 'text',
        'middle' => 'text',
        'title' => 'select',
      ),
      'generational_field' => 'select',
      'inline_css' => array(
        'credentials' => '',
        'family' => '',
        'generational' => '',
        'given' => '',
        'middle' => '',
        'title' => '',
      ),
      'labels' => array(
        'credentials' => '',
        'family' => '',
        'generational' => '',
        'given' => '',
        'middle' => '',
        'title' => '',
      ),
      'minimum_components' => array(
        'credentials' => 0,
        'family' => 0,
        'generational' => 0,
        'given' => 0,
        'middle' => 0,
        'title' => 0,
      ),
      'override_format' => 'default',
      'show_component_required_marker' => 0,
      'size' => array(
        'credentials' => 35,
        'family' => 20,
        'generational' => 5,
        'given' => 20,
        'middle' => 20,
        'title' => 6,
      ),
      'title_display' => array(
        'credentials' => 'none',
        'family' => 'description',
        'generational' => 'description',
        'given' => 'description',
        'middle' => 'description',
        'title' => 'description',
      ),
      'title_field' => 'select',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'name',
      'settings' => array(),
      'type' => 'name_widget',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_phone'.
  $field_instances['node-sirius_worker-field_sirius_phone'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 16,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_phone',
    'label' => 'Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_phone_alt'.
  $field_instances['node-sirius_worker-field_sirius_phone_alt'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_phone_alt',
    'label' => 'Alt. Phone',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_phone_fax'.
  $field_instances['node-sirius_worker-field_sirius_phone_fax'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_phone_fax',
    'label' => 'Fax',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_skill'.
  $field_instances['node-sirius_worker-field_sirius_skill'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 9,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_skill',
    'label' => 'Skills',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'multiple_selects',
      'settings' => array(
        'cascading_selection' => 0,
        'checkall' => FALSE,
        'filter_view' => '',
        'leaves_only' => 0,
        'max_depth' => '',
        'parent_term_id' => '',
        'select_parents' => 0,
        'start_minimized' => 0,
        'token_display' => '',
        'track_list' => 0,
      ),
      'type' => 'multiple_selects',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_skill_expire'.
  $field_instances['node-sirius_worker-field_sirius_skill_expire'] = array(
    'bundle' => 'sirius_worker',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'field_delimiter' => '',
          'format_type' => 'date_only_sortable',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 10,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_skill_expire',
    'label' => 'Skill Expires',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'date_single_day' => array(),
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_skills_avail'.
  $field_instances['node-sirius_worker-field_sirius_skills_avail'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 35,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 26,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_skills_avail',
    'label' => 'Available For Skills',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 39,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_skills_availx'.
  $field_instances['node-sirius_worker-field_sirius_skills_availx'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => array(
      0 => array(
        'value' => 'Yes',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 36,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_skills_availx',
    'label' => 'Available for Unskilled Work',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_ssn'.
  $field_instances['node-sirius_worker-field_sirius_ssn'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'ddd-dd-dddd',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'sirius',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_ssn_formatter_conditional',
        'weight' => 4,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'module' => 'sirius',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_ssn_formatter_masked',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_ssn',
    'label' => 'SSN',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 11,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-sirius_worker-field_sirius_work_status'.
  $field_instances['node-sirius_worker-field_sirius_work_status'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'sirius_worker' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_work_status',
    'label' => 'Work Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 12,
    ),
  );

  // Exported field_instance:
  // 'node-sirius_worker-field_sirius_worker_dispstatus'.
  $field_instances['node-sirius_worker-field_sirius_worker_dispstatus'] = array(
    'bundle' => 'sirius_worker',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'sirius_worker' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_sirius_worker_dispstatus',
    'label' => 'Dispatch Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => 0,
      ),
      'type' => 'options_select',
      'weight' => 13,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_alert_types-field_grievance_actor'.
  $field_instances['taxonomy_term-grievance_alert_types-field_grievance_actor'] = array(
    'bundle' => 'grievance_alert_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_actor',
    'label' => 'Actor',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_alert_types-field_grievance_contract_tplts'.
  $field_instances['taxonomy_term-grievance_alert_types-field_grievance_contract_tplts'] = array(
    'bundle' => 'grievance_alert_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_contract_tplts',
    'label' => 'Contract Templates',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_alert_types-field_sirius_domain'.
  $field_instances['taxonomy_term-grievance_alert_types-field_sirius_domain'] = array(
    'bundle' => 'grievance_alert_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_domain',
    'label' => 'Domain',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_category-field_grievance_hidefields'.
  $field_instances['taxonomy_term-grievance_category-field_grievance_hidefields'] = array(
    'bundle' => 'grievance_category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'For grievances with this category, hide the fields listed below. Enter one per line, using the machine name of the field (i.e. field_grievance_remedy)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_hidefields',
    'label' => 'Hide Fields',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_contract_clause_tags-field_grievance_external_id'.
  $field_instances['taxonomy_term-grievance_contract_clause_tags-field_grievance_external_id'] = array(
    'bundle' => 'grievance_contract_clause_tags',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter an external identifier / code that can be used for importing, exporting, or synchronizing terms in this vocabulary.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_contract_section_tags-field_grievance_external_id'.
  $field_instances['taxonomy_term-grievance_contract_section_tags-field_grievance_external_id'] = array(
    'bundle' => 'grievance_contract_section_tags',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter an external identifier / code that can be used for importing, exporting, or synchronizing terms in this vocabulary.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_contract_section_tags-field_sirius_css_class'.
  $field_instances['taxonomy_term-grievance_contract_section_tags-field_sirius_css_class'] = array(
    'bundle' => 'grievance_contract_section_tags',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_css_class',
    'label' => 'CSS Class',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_contract_template_tags-field_grievance_external_id'.
  $field_instances['taxonomy_term-grievance_contract_template_tags-field_grievance_external_id'] = array(
    'bundle' => 'grievance_contract_template_tags',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter an external identifier / code that can be used for importing, exporting, or synchronizing terms in this vocabulary.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_external_id',
    'label' => 'External ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_department-field_sirius_dispatch_available'.
  $field_instances['taxonomy_term-grievance_department-field_sirius_dispatch_available'] = array(
    'bundle' => 'grievance_department',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If "Yes", this is a department that can be selected when creating a dispatch job.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_available',
    'label' => 'Use for Dispatch?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_department-field_sirius_id'.
  $field_instances['taxonomy_term-grievance_department-field_sirius_id'] = array(
    'bundle' => 'grievance_department',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_department-field_sirius_industry'.
  $field_instances['taxonomy_term-grievance_department-field_sirius_industry'] = array(
    'bundle' => 'grievance_department',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_document_types-field_grievance_can_attach'.
  $field_instances['taxonomy_term-grievance_document_types-field_grievance_can_attach'] = array(
    'bundle' => 'grievance_document_types',
    'default_value' => array(
      0 => array(
        'value' => 'Yes',
      ),
    ),
    'deleted' => 0,
    'description' => 'Is this an "attachment", meaning that it can be used as an attachment type for the grievance?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_can_attach',
    'label' => 'Attachment?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_document_types-field_grievance_can_ir'.
  $field_instances['taxonomy_term-grievance_document_types-field_grievance_can_ir'] = array(
    'bundle' => 'grievance_document_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Select "Yes" if this document type should appear as an "Information Request" option.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_can_ir',
    'label' => 'Information Request?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_document_types-field_grievance_emp_name'.
  $field_instances['taxonomy_term-grievance_document_types-field_grievance_emp_name'] = array(
    'bundle' => 'grievance_document_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If the company should see a longer or more descriptive name, enter it here.  (I.e. the name might be "Guest Complaints" and the employer name might be " Guest complaints (with complete mailing and/or email address and phone number)").  This longer name will be used on information requests and grievance forms sent to the company.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_emp_name',
    'label' => 'Employer Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
        'size' => 60,
      ),
      'type' => 'text_textarea',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_job_classification-field_grievance_departments'.
  $field_instances['taxonomy_term-grievance_job_classification-field_grievance_departments'] = array(
    'bundle' => 'grievance_job_classification',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_departments',
    'label' => 'Possible Departments',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_log_types-field_grievance_roles'.
  $field_instances['taxonomy_term-grievance_log_types-field_grievance_roles'] = array(
    'bundle' => 'grievance_log_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If only certain roles have access to edit log entries of this type, select the roles that should have access.  If no roles are checked, all roles will have access.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'role_field',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'role_field_formatter',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_roles',
    'label' => 'Roles',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_log_types-field_grievance_timeline_show'.
  $field_instances['taxonomy_term-grievance_log_types-field_grievance_timeline_show'] = array(
    'bundle' => 'grievance_log_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_timeline_show',
    'label' => 'Show on Timeline?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_contract_tplts'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_contract_tplts'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_contract_tplts',
    'label' => 'Contract Templates',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_notify_body'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_notify_body'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'When a notification is sent to the company, what should the body of the email be? ',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_notify_body',
    'label' => 'Notification Body',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_notify_subject'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_notify_subject'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'When a notification for this status is sent to the company, what subject should be used?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_notify_subject',
    'label' => 'Notification Subject',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_open'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_open'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Is this status considered "Un-Resolved"?  Only unresolved grievances are displayed on the "my grievances" list.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_open',
    'label' => 'Unresolved?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_resproc'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_resproc'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A grievance at this status should have available fields for the specified resolution procedure.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_resproc',
    'label' => 'Resolution Procedure',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_roles'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_roles'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Select the roles with permission to edit grievances at this step.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'role_field',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'role_field_formatter',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_roles',
    'label' => 'Roles',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_status-field_grievance_timeline_show'.
  $field_instances['taxonomy_term-grievance_status-field_grievance_timeline_show'] = array(
    'bundle' => 'grievance_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'When the grievance enters this status, should it be displayed on the timeline presented to the company?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_timeline_show',
    'label' => 'Show on Timeline?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-grievance_types-field_grievance_document_types'.
  $field_instances['taxonomy_term-grievance_types-field_grievance_document_types'] = array(
    'bundle' => 'grievance_types',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_document_types',
    'label' => 'Default Information Request',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_dispatch_job_type-field_sirius_json'.
  $field_instances['taxonomy_term-sirius_dispatch_job_type-field_sirius_json'] = array(
    'bundle' => 'sirius_dispatch_job_type',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'sirius_jsonfield',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'sirius_jsonfield_formatter_full',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_json',
    'label' => 'Dispatch Policy',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'sirius_jsonfield',
      'settings' => array(),
      'type' => 'sirius_jsonfield_widget',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_dispatch_job_type-field_sirius_name_display'.
  $field_instances['taxonomy_term-sirius_dispatch_job_type-field_sirius_name_display'] = array(
    'bundle' => 'sirius_dispatch_job_type',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_name_display',
    'label' => 'Display Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_industry-field_sirius_dispatch_3sen'.
  $field_instances['taxonomy_term-sirius_industry-field_sirius_dispatch_3sen'] = array(
    'bundle' => 'sirius_industry',
    'default_value' => array(),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_3sen',
    'label' => 'Threestrikes Enable',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_industry-field_sirius_dispatch_days'.
  $field_instances['taxonomy_term-sirius_industry-field_sirius_dispatch_days'] = array(
    'bundle' => 'sirius_industry',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_days',
    'label' => 'Days to Retain Position',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_industry-field_sirius_dispatch_notesdef'.
  $field_instances['taxonomy_term-sirius_industry-field_sirius_dispatch_notesdef'] = array(
    'bundle' => 'sirius_industry',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_notesdef',
    'label' => 'Default Dispatch Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'taxonomy_term-sirius_industry-field_sirius_id'.
  $field_instances['taxonomy_term-sirius_industry-field_sirius_id'] = array(
    'bundle' => 'sirius_industry',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_industry-field_sirius_name_short'.
  $field_instances['taxonomy_term-sirius_industry-field_sirius_name_short'] = array(
    'bundle' => 'sirius_industry',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_name_short',
    'label' => 'Abbreviation',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_grievance_weight'.
  $field_instances['taxonomy_term-sirius_member_status-field_grievance_weight'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_weight',
    'label' => 'Weight',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_active'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_active'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 11,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_active',
    'label' => 'Active?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 13,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_dispatch_job_types'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_dispatch_job_types'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 9,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_job_types',
    'label' => 'Job Types',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 11,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_dispatch_roles'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_dispatch_roles'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Users with these roles have permission to create dispatches jobs with this member status. These should be administrative roles only.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'role_field',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'role_field_formatter',
        'weight' => 8,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_roles',
    'label' => 'Dispatch Roles',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 10,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_id'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_id'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id',
    'label' => 'ListID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_industry'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_industry'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_name_short'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_name_short'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_name_short',
    'label' => 'Abbreviation',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_payrate'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_payrate'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 5,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_payrate',
    'label' => 'Unskilled Pay Rate',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_payrate_skilled'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_payrate_skilled'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_payrate_skilled',
    'label' => 'Skilled Payrate',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_member_status-field_sirius_roles'.
  $field_instances['taxonomy_term-sirius_member_status-field_sirius_roles'] = array(
    'bundle' => 'sirius_member_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Select the system roles that the user associated with workers with this member status should receive. For example, if the member status is "journeyman", and journeymen have a special role granting them additional permission, you might add that role here.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'role_field',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'role_field_formatter',
        'weight' => 7,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_roles',
    'label' => 'User Roles',
    'required' => 0,
    'settings' => array(
      'checkall' => array(
        'checkall' => 0,
        'checkall-check-all' => 1,
        'checkall-toggle' => 1,
        'checkall-uncheck-all' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'checkall' => FALSE,
      ),
      'type' => 'options_buttons',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'taxonomy_term-sirius_skill-field_grievance_days'.
  $field_instances['taxonomy_term-sirius_skill-field_grievance_days'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the default number of days before this skill expires. The default is for a skill to never expire.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'field_delimiter' => '',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 5,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_grievance_days',
    'label' => 'Default number of days',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'taxonomy_term-sirius_skill-field_sirius_badge'.
  $field_instances['taxonomy_term-sirius_skill-field_sirius_badge'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => array(
      0 => array(
        'value' => 'No',
      ),
    ),
    'deleted' => 0,
    'description' => 'Select "Yes" if this should be printed on the badge.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_badge',
    'label' => 'Badge?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'taxonomy_term-sirius_skill-field_sirius_id'.
  $field_instances['taxonomy_term-sirius_skill-field_sirius_id'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'taxonomy_term-sirius_skill-field_sirius_id2'.
  $field_instances['taxonomy_term-sirius_skill-field_sirius_id2'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id2',
    'label' => 'ID2',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_skill-field_sirius_industry'.
  $field_instances['taxonomy_term-sirius_skill-field_sirius_industry'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 6,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_industry',
    'label' => 'Industry',
    'required' => 0,
    'settings' => array(
      'field_lock_multi_values' => array(
        'disable_reorder' => 0,
        'hide_add_more' => 0,
        'hide_previous' => 0,
        'hide_remove' => 0,
        'lock_previous' => 0,
        'remove_header' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'view' => array(
      'args' => array(),
      'display_name' => '',
      'match_operator' => 'CONTAINS',
      'size' => '',
      'view_name' => '',
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'multiple_selects',
      'settings' => array(),
      'type' => 'multiple_selects',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_skill-field_sirius_name_short'.
  $field_instances['taxonomy_term-sirius_skill-field_sirius_name_short'] = array(
    'bundle' => 'sirius_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_name_short',
    'label' => 'Abbreviation',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_work_status-field_sirius_dispatch_available'.
  $field_instances['taxonomy_term-sirius_work_status-field_sirius_dispatch_available'] = array(
    'bundle' => 'sirius_work_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_dispatch_available',
    'label' => 'Available for Dispatch?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_work_status-field_sirius_id'.
  $field_instances['taxonomy_term-sirius_work_status-field_sirius_id'] = array(
    'bundle' => 'sirius_work_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_work_status-field_sirius_id2'.
  $field_instances['taxonomy_term-sirius_work_status-field_sirius_id2'] = array(
    'bundle' => 'sirius_work_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_id2',
    'label' => 'Titan ID',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_work_status-field_sirius_member_active'.
  $field_instances['taxonomy_term-sirius_work_status-field_sirius_member_active'] = array(
    'bundle' => 'sirius_work_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_member_active',
    'label' => 'Member?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'content_taxonomy_opt_groups' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-sirius_worker_dispatch_status-field_sirius_name_tts'.
  $field_instances['taxonomy_term-sirius_worker_dispatch_status-field_sirius_name_tts'] = array(
    'bundle' => 'sirius_worker_dispatch_status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_sirius_name_tts',
    'label' => 'TtS Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 32,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('"Access token" field, used to permit viewing of this node for unauthenticated users.');
  t('(i.e. "... accordance with the provisions of [clause goes here] of our Collective Bargaining Agreement ...")');
  t('(in days)');
  t('A grievance at this status should have available fields for the specified resolution procedure.');
  t('AAT');
  t('Abbreviation');
  t('Accepted Count');
  t('Active');
  t('Active From');
  t('Active Until');
  t('Active?');
  t('Actor');
  t('Additional Information Request');
  t('Address');
  t('Address 2');
  t('Address Geocode');
  t('Admin data never displayed');
  t('Alert Data');
  t('Alt. Phone');
  t('Amount Received');
  t('Annual?');
  t('Arbitration Clause');
  t('Arbitrator Name');
  t('Article');
  t('Article Number');
  t('Article Tags');
  t('Articles Selectable?');
  t('Assignee');
  t('Assignee Notes');
  t('Attachment');
  t('Attachment Type');
  t('Attachment?');
  t('Attachments');
  t('Attachments will be listed at the bottom of the page.');
  t('Auto Sign In?');
  t('Available For Skills');
  t('Available Since');
  t('Available for Dispatch?');
  t('Available for Unskilled Work');
  t('Badge?');
  t('Batch Max');
  t('Batch Min');
  t('Batch Ratio');
  t('Body');
  t('Body (Email)');
  t('Brought By');
  t('Building the Union with this case');
  t('Bundle');
  t('Bundle to overried, e.g. "grievance"');
  t('Business Agent');
  t('CBA Provision(s) Violated');
  t('CC Emails');
  t('CSS');
  t('CSS Class');
  t('Call Number');
  t('Call by Name Only?');
  t('Call by Name?');
  t('Can and should this case be settled?');
  t('Can the articles in this contract be selected?  If "Yes", then a "dummy" section will be created for each article.');
  t('Cancel By');
  t('Category');
  t('Chapter');
  t('Check Number');
  t('City');
  t('Classification');
  t('Comments');
  t('Company');
  t('Company Rep Address');
  t('Company Rep Address 2');
  t('Company Rep City');
  t('Company Rep Email');
  t('Company Rep Name');
  t('Company Rep Phone');
  t('Company Rep Postal Code');
  t('Company Rep State or Province');
  t('Company Rep Title');
  t('Completed');
  t('Contact');
  t('Contact Type');
  t('Contract');
  t('Contract Template');
  t('Contract Templates');
  t('Created');
  t('Current Employer');
  t('Data Corrected');
  t('Date');
  t('Date Received');
  t('Date of Hire');
  t('Days');
  t('Days Off');
  t('Days to Retain Position');
  t('Deadlines Waived');
  t('Deadlines have been waived as of this date.');
  t('Declined Count');
  t('Default Dispatch Notes');
  t('Default Information Request');
  t('Default Value');
  t('Default number of days');
  t('Department');
  t('Departments');
  t('Description');
  t('Discipline Date');
  t('Dispatch Policy');
  t('Dispatch Roles');
  t('Dispatch Status');
  t('Display Name');
  t('Does this holiday repeat every year on the same date?');
  t('Domain');
  t('Drupal paths on which to display the help text.  This can either be a path alias such as "my/page/here", or a native Drupal path such as "node/%/ac_questionnaire_questions".');
  t('Dummy?');
  t('E.g. "Step 1 Contact" or "Contact for Arbitration Only"');
  t('E.g. 04.01 or 04.01.01 or 04.01.A');
  t('EBA');
  t('EBA Dates');
  t('Email');
  t('Email Body');
  t('Employed but Available');
  t('Employee ID #');
  t('Employer');
  t('Employer Attorney');
  t('Employer Name');
  t('Employer Representative');
  t('Employer-Initiated Do Not Call');
  t('Enter CSS directives which should be included inline in the print version of this letter.  E.g. "#grievance-letter-body { font-size: 10pt; }"');
  t('Enter an external identifier / code that can be used for importing, exporting, or synchronizing terms in this vocabulary.');
  t('Enter the article of the contract that was violated, for example: Article ....');
  t('Enter the company that this shop is associated with.');
  t('Enter the default number of days before this skill expires. The default is for a skill to never expire.');
  t('Enter the employee ID # as assigned by the company.');
  t('Enter the help text as it should appear on the specified pages.  May contain HTML.');
  t('Enter the next deadline that the Union must meet -- i.e. "File Step 2" or "File for Arbitration".');
  t('Enter the organizer assigned to this grievance.');
  t('Enter the role of this representative, e.g. "Attorney" or "Manager".');
  t('Enter the union\'s ID # for this member.');
  t('Entity Type');
  t('Entity type to override, almost always "node".');
  t('Expire After');
  t('External ID');
  t('Facility');
  t('Fax');
  t('Field Name');
  t('Field name to override, e.g. "field_grievance_organizer"');
  t('Filed By');
  t('Filled');
  t('First Name');
  t('For discipline, the date of the employee’s conduct which led to discipline. For contract violation, the date of the employer’s action (if unknown, choose "Investigation").');
  t('For grievances with this category, hide the fields listed below. Enter one per line, using the machine name of the field (i.e. field_grievance_remedy)');
  t('From Status');
  t('Full Text');
  t('Gender');
  t('General Grievance?');
  t('Grievance');
  t('Grievance Admin');
  t('Grievance Complaint');
  t('Grievance Director');
  t('Grievance Document Type');
  t('Grievance Log Type');
  t('Group or Show');
  t('HTML formatting is permitted, but all links should be absolute.');
  t('Hall');
  t('Handler');
  t('Headshot');
  t('Hearing Date');
  t('Hide Fields');
  t('Hold for Employer');
  t('Hold for Employer Until');
  t('Holidays');
  t('ID');
  t('ID2');
  t('If "Yes", then letters of this type MUST be logged.');
  t('If "Yes", then the default will be that any letter sent using this template will also set the grievance\'s "company representative" field to be the main recipient of the letter.  Generally this should be "Yes" for notifications to the company -- grievance filings and so forth -- and "No" for notifications to the grievant or letters that are meant to be sent internally.');
  t('If "Yes", this is a department that can be selected when creating a dispatch job.');
  t('If only certain roles have access to edit log entries of this type, select the roles that should have access.  If no roles are checked, all roles will have access.');
  t('If selected, this contact will appear for all shops associated with this company.');
  t('If set to "Yes", the worker will not be permitted to sign themselves in (set their dispatch status).');
  t('If the company should see a longer or more descriptive name, enter it here.  (I.e. the name might be "Guest Complaints" and the employer name might be " Guest complaints (with complete mailing and/or email address and phone number)").  This longer name will be used on information requests and grievance forms sent to the company.');
  t('Images');
  t('Incident Date');
  t('Include EBA Workers');
  t('Indicates which job types this employer can create.');
  t('Industry');
  t('Information Request');
  t('Information Request?');
  t('Intake Person');
  t('Is this a "dummy" or "placeholder" section?  This field should be left blank or set to "No" except for automatically-generated placeholder sections created when the article itself is meant to be selectable.');
  t('Is this an "attachment", meaning that it can be used as an attachment type for the grievance?');
  t('Is this status considered "Un-Resolved"?  Only unresolved grievances are displayed on the "my grievances" list.');
  t('JSON');
  t('Job');
  t('Job Tags');
  t('Job Types');
  t('Label');
  t('Labor charges or other relevant shop issues');
  t('Last Name');
  t('Lead Organizer');
  t('Lead\'s Recommendaiton');
  t('Length of Hearing');
  t('ListID');
  t('Medium');
  t('Meeting Date');
  t('Member ID #');
  t('Member Status');
  t('Member?');
  t('Message');
  t('Must Log?');
  t('NOT Available for Departments');
  t('Name');
  t('Name as it should be rendered by Text to Speech systems');
  t('Next Deadline');
  t('Next Deadline Date');
  t('Next Status');
  t('Next Step');
  t('Next Step in the Grievance Procedure');
  t('Notes');
  t('Notification Body');
  t('Notification Email');
  t('Notification Emails');
  t('Notification Medium');
  t('Notification Subject');
  t('Notifications');
  t('Notified');
  t('Notified Count');
  t('Number of Days');
  t('Number of days to extend the timelines');
  t('Office Phone');
  t('Organizer');
  t('Outcome');
  t('PDF');
  t('Page');
  t('Parking Instructions');
  t('Passcode required for phone access?');
  t('Paths');
  t('Payrate');
  t('Personal Phone');
  t('Phone');
  t('Pop up');
  t('Popup Until');
  t('Possible Classifications');
  t('Possible Departments');
  t('Postal Code');
  t('Recommended Remedy');
  t('Recommended Statement of Grievance');
  t('Remedy');
  t('Report Date');
  t('Report Location');
  t('Representatives');
  t('Resolution Procedure');
  t('Responded');
  t('Role');
  t('Roles');
  t('SMS Body');
  t('SSN');
  t('Section Number');
  t('Section Tags');
  t('Select "Yes" if this document type should appear as an "Information Request" option.');
  t('Select "Yes" if this should be printed on the badge.');
  t('Select a Contact');
  t('Select a Shop Steward');
  t('Select the date that the status changed.  For example, if the status is "Step 1 Meeting", enter the date that the meeting occurred.  If no date is entered, today\'s date will be used.');
  t('Select the roles with permission to edit grievances at this step.');
  t('Select the system roles that the user associated with workers with this member status should receive. For example, if the member status is "journeyman", and journeymen have a special role granting them additional permission, you might add that role here.');
  t('Send Date');
  t('Send Notifications?');
  t('Settlement Amount');
  t('Settlement Tags');
  t('Shift');
  t('Shop Steward Email');
  t('Shop Steward Name');
  t('Shop Steward Phone');
  t('Shops');
  t('Short Name');
  t('Should contain only letters and numbers and underscore, used to generate filenames.');
  t('Show on Timeline?');
  t('Sign-In Blocked?');
  t('Skill Expires');
  t('Skilled Payrate');
  t('Skills');
  t('Standard Information Request');
  t('State or Province');
  t('Status');
  t('Status Change Date');
  t('Status Notes');
  t('Subject');
  t('Summary');
  t('Supervisor Name');
  t('Supervisor Title');
  t('Supervisor Unit');
  t('Tags');
  t('Template Tags');
  t('The date the employee received discipline.');
  t('These email addresses will be CC\'d on EVERY letter sent using this template.');
  t('Threestrikes Enable');
  t('Titan ID');
  t('TtS Name');
  t('Type');
  t('Type of Days');
  t('Union Attorney');
  t('Union Representative');
  t('Unresolved?');
  t('Unskilled Pay Rate');
  t('Update Company Representative');
  t('Upload images to include in letter templates, such as a scanned signature.');
  t('Upload the current contract document for this shop.  The contract will be used by stewards to identify violations.');
  t('Upload the letterhead in PDF format. 

If the file has a single page, then that will be used as the background for all pages in the input document. 

If the file has multiple pages, then each page in the input document will use the corresponding page in the letterhead as a background, with the last page in the letterhead repeated as many times as necessary if the input document is longer.  

(For example, if a two-page letterhead is uploaded, then the first page of the letterhead will be used as the first page of the generated document, and the second page will be repeated as many times as necessary.)');
  t('Use for Dispatch?');
  t('Use this field for a "letter" which has no customization, and which is simply meant to be downloaded.  No drop-in tokens are supported.');
  t('Use this field to enter the default body of the email; it should generally be something like "Please refer to the attached document re. a grievance filed against your company."');
  t('Use this field to indicate that member data in the grievance system has changed from the Union\'s member management system, and that the new data needs to be uploaded back to the member management system.  "Unchanged" means that the data is the same in both places; "Pending" means that corrected data has been entered here and needs to be entered in the member management system.  After the information has been changed, it will be flagged as "Complete".');
  t('Used for sorting; generally set by drag-and-drop.');
  t('User Roles');
  t('Users with these roles have permission to create dispatches jobs with this member status. These should be administrative roles only.');
  t('Valid');
  t('Violation');
  t('Voice Body');
  t('Watching');
  t('Weight');
  t('What roles should have access to this content');
  t('When a notification for this status is sent to the company, what subject should be used?');
  t('When a notification is sent to the company, what should the body of the email be? ');
  t('When the grievance admin selects this clause, which "statement of grievance" should be selected?');
  t('When the grievance enters this status, should it be displayed on the timeline presented to the company?');
  t('Work Status');
  t('Worker');
  t('Worker Count');
  t('Worker-Initiated Do Not Call');
  t('ddd-dd-dddd');

  return $field_instances;
}
