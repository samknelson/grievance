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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => -4,
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
        'weight' => 18,
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
      'weight' => 16,
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
        'weight' => 41,
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
        'weight' => 19,
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
      'weight' => 17,
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
        'weight' => 30,
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
        'weight' => 62,
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
      'weight' => 36,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 1,
        ),
        'type' => 'date_default',
        'weight' => 7,
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
        'weight' => 63,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 38,
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
        'weight' => 6,
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
        'weight' => 52,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 37,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
        ),
        'type' => 'date_default',
        'weight' => 8,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
        ),
        'type' => 'date_default',
        'weight' => 3,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 40,
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
        'weight' => 64,
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
      'weight' => 50,
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
        'weight' => 61,
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
      'weight' => 51,
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
        'weight' => 3,
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
        'weight' => 60,
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
      'weight' => 52,
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
        'weight' => 4,
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
        'weight' => 59,
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
      'weight' => 54,
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
        'label' => 'above',
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
      'weight' => 62,
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
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
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
        'module' => 'taxonomy',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 42,
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
      'settings' => array(),
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
        'weight' => 42,
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
        'weight' => 20,
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
      'weight' => 18,
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
        'weight' => 46,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_class_tid',
    'label' => 'Classification',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 22,
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
        'weight' => 49,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 33,
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
        'weight' => 47,
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
        'weight' => 25,
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
      'weight' => 23,
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
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
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
        'module' => 'entityreference',
        'settings' => array(
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
          'field_delimiter' => '',
          'link' => 0,
        ),
        'type' => 'entityreference_label',
        'weight' => 30,
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
      'weight' => 38,
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
        'weight' => 10,
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
      'weight' => 53,
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
        'weight' => 11,
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
      'weight' => 54,
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
        'weight' => 12,
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
      'weight' => 55,
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
        'weight' => 6,
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
        'weight' => 9,
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
        'weight' => 7,
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
        'weight' => 4,
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
      'weight' => 56,
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
        'weight' => 5,
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
      'weight' => 57,
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
        'weight' => 65,
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
        'weight' => 32,
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
        'weight' => 50,
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
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 2,
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
      'settings' => array(),
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
        'weight' => 54,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'node-grievance-field_grievance_data_alert'.
  $field_instances['node-grievance-field_grievance_data_alert'] = array(
    'bundle' => 'grievance',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 31,
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
        'weight' => 58,
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
      'weight' => 2,
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
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 33,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 0,
      ),
      'grievance_form_226' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 4,
      ),
      'grievance_gform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 0,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 34,
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
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
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
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 5,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 35,
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
        'weight' => 46,
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
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 28,
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
        'weight' => 48,
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
        'weight' => 26,
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
      'weight' => 25,
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
        'weight' => 49,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 24,
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
        'weight' => 33,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_document_types',
    'label' => 'Information Request',
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
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 8,
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
        'weight' => 53,
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
        'weight' => 53,
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
      'weight' => 29,
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
        'type' => 'email_default',
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
        'weight' => 24,
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
      'weight' => 15,
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
        'weight' => 16,
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
      'weight' => 11,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 45,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'weight' => 66,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '1900:+3',
      ),
      'type' => 'date_popup',
      'weight' => 21,
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
        'weight' => 51,
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
      'weight' => 41,
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
        'weight' => 34,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_inforeq',
    'label' => 'Information Request',
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
      'weight' => 9,
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
        'weight' => 17,
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
      'weight' => 12,
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
        'weight' => 33,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 39,
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
      'weight' => 30,
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
        'weight' => 52,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 60,
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
        'weight' => 23,
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
      'weight' => 14,
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
        'weight' => 35,
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
        'weight' => 31,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_remedy',
    'label' => 'Remedy',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
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
        'weight' => 36,
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
        'weight' => 32,
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
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 61,
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
        'settings' => array(),
        'type' => 'hidden',
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_filed',
    'label' => 'Filed By',
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 63,
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
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 41,
      ),
      'grievance_form_226' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 62,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_lead',
    'label' => 'Lead Organizer',
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 65,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_manager',
    'label' => 'Grievance Admin',
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 66,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_organizer',
    'label' => 'Organizer',
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 64,
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
        'weight' => 67,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_rep_watching',
    'label' => 'Watching',
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 67,
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
      'weight' => 56,
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
        'weight' => 38,
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
      'weight' => 58,
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
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 57,
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
      'weight' => 55,
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
      'weight' => 59,
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
        'weight' => 48,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 27,
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
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
      'grievance_arbitrationform' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
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
          'field_delimiter' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 8,
      ),
      'grievance_gform_a' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 30,
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
        'weight' => 43,
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
        'weight' => 21,
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
      'weight' => 19,
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
        'weight' => 5,
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
        'weight' => 68,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 34,
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
        'weight' => 53,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 35,
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
        'weight' => 13,
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
        'weight' => 14,
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
        'weight' => 15,
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
        'weight' => 31,
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
        'weight' => 27,
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_type',
    'label' => 'Grievance Complaint',
    'required' => 0,
    'settings' => array(
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
        'weight' => 32,
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
        'weight' => 28,
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
      'weight' => 37,
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
        'weight' => 33,
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
        'weight' => 29,
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
        'weight' => 47,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 26,
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
        'weight' => 44,
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
        'weight' => 22,
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
      'weight' => 20,
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
    'label' => 'Attachment',
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
      'weight' => 2,
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
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
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
      'settings' => array(),
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
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
      'settings' => array(),
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
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_clause-field_grievance_label'.
  $field_instances['node-grievance_contract_clause-field_grievance_label'] = array(
    'bundle' => 'grievance_contract_clause',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'E.g. 4.01 or A. II.',
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
      'settings' => array(),
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
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
      'settings' => array(),
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
      'settings' => array(),
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
      'settings' => array(),
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
  // 'node-grievance_contract_rule-field_grievance_notify_body'.
  $field_instances['node-grievance_contract_rule-field_grievance_notify_body'] = array(
    'bundle' => 'grievance_contract_rule',
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
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_contract_rule-field_grievance_notify_subject'.
  $field_instances['node-grievance_contract_rule-field_grievance_notify_subject'] = array(
    'bundle' => 'grievance_contract_rule',
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
      'weight' => 7,
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
      'settings' => array(),
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
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
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
      'weight' => 2,
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
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 6,
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
      'weight' => 3,
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
      'settings' => array(),
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => -4,
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
        'weight' => 5,
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
      'weight' => 3,
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
        'weight' => 19,
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
      'weight' => 6,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 8,
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
    'field_name' => 'field_grievance_alert_tid',
    'label' => 'Next Step',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 7,
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
      'weight' => 9,
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
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_log_type',
    'label' => 'Grievance Log Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
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
    'field_name' => 'field_grievance_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-grievance_log-field_grievance_status_date'.
  $field_instances['node-grievance_log-field_grievance_status_date'] = array(
    'bundle' => 'grievance_log',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 5,
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
          'field_delimiter' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-grievance_shop-field_grievance_attachments'.
  $field_instances['node-grievance_shop-field_grievance_attachments'] = array(
    'bundle' => 'grievance_shop',
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
      'weight' => 5,
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 0,
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
      'weight' => 3,
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
    'field_name' => 'field_grievance_contract_tplt',
    'label' => 'Contract Template',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
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
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 6,
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
        'weight' => 4,
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
        'weight' => 5,
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
        'weight' => 6,
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
        'weight' => 2,
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
      'weight' => 4,
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
        'weight' => 0,
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
      'weight' => 2,
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
        'weight' => 3,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_co_phone',
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
        'weight' => 1,
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
      'weight' => 3,
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
        'weight' => 7,
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
      'weight' => 8,
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
        'weight' => 22,
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 14,
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
        'weight' => 20,
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
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 15,
    ),
  );

  // Exported field_instance:
  // 'node-grievance_shop_contact-field_grievance_statuses'.
  $field_instances['node-grievance_shop_contact-field_grievance_statuses'] = array(
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
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_grievance_statuses',
    'label' => 'Statuses',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 16,
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
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 0,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 0,
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
        'label' => 'above',
        'module' => 'list',
        'settings' => array(
          'field_delimiter' => '',
        ),
        'type' => 'list_default',
        'weight' => 0,
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
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
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
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
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
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 2,
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
      'settings' => array(),
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
      'settings' => array(),
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
      'settings' => array(),
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
      'settings' => array(),
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
      'settings' => array(),
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
    'field_name' => 'field_grievance_document_types',
    'label' => 'Default Information Request',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('(in days)');
  t('A grievance at this status should have available fields for the specified resolution procedure.');
  t('Actor');
  t('Address');
  t('Address 2');
  t('Alert Data');
  t('Amount Received');
  t('Annual?');
  t('Arbitrator Name');
  t('Article');
  t('Article Number');
  t('Assignee');
  t('Assignee Notes');
  t('Attachment');
  t('Attachment Type');
  t('Attachments');
  t('Body');
  t('Brought By');
  t('Building the Union with this case');
  t('Bundle');
  t('Bundle to overried, e.g. "grievance"');
  t('CBA Provision(s) Violated');
  t('Can and should this case be settled?');
  t('Category');
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
  t('Contract');
  t('Contract Template');
  t('Contract Templates');
  t('Data Corrected');
  t('Date');
  t('Date Received');
  t('Date of Hire');
  t('Days');
  t('Days Off');
  t('Deadlines Waived');
  t('Deadlines have been waived as of this date.');
  t('Default Information Request');
  t('Default Value');
  t('Department');
  t('Description');
  t('Discipline Date');
  t('Does this holiday repeat every year on the same date?');
  t('E.g. 4.01 or A. II.');
  t('Email');
  t('Employee ID #');
  t('Employer');
  t('Employer Attorney');
  t('Employer Name');
  t('Employer Representative');
  t('Enter the article of the contract that was violated, for example: Article ....');
  t('Enter the company that this shop is associated with.');
  t('Enter the employee ID # as assigned by the company.');
  t('Enter the next deadline that the Union must meet -- i.e. "File Step 2" or "File for Arbitration".');
  t('Enter the organizer assigned to this grievance.');
  t('Enter the role of this representative, e.g. "Attorney" or "Manager".');
  t('Enter the union\'s ID # for this member.');
  t('Entity Type');
  t('Entity type to override, almost always "node".');
  t('Field Name');
  t('Field name to override, e.g. "field_grievance_organizer"');
  t('Filed By');
  t('First Name');
  t('For discipline, the date of the employee’s conduct which led to discipline. For contract violation, the date of the employer’s action (if unknown, choose "Investigation").');
  t('From Status');
  t('Full Text');
  t('General Grievance?');
  t('Grievance');
  t('Grievance Admin');
  t('Grievance Complaint');
  t('Grievance Document Type');
  t('Grievance Log Type');
  t('Holidays');
  t('ID');
  t('If selected, this contact will appear for all shops associated with this company.');
  t('If the company should see a longer or more descriptive name, enter it here.  (I.e. the name might be "Guest Complaints" and the employer name might be " Guest complaints (with complete mailing and/or email address and phone number)").  This longer name will be used on information requests and grievance forms sent to the company.');
  t('Incident Date');
  t('Information Request');
  t('Information Request?');
  t('Is this status considered "Un-Resolved"?  Only unresolved grievances are displayed on the "my grievances" list.');
  t('Label');
  t('Labor charges or other relevant shop issues');
  t('Last Name');
  t('Lead Organizer');
  t('Lead\'s Recommendaiton');
  t('Length of Hearing');
  t('Meeting Date');
  t('Member ID #');
  t('Name');
  t('Next Deadline');
  t('Next Deadline Date');
  t('Next Status');
  t('Next Step');
  t('Next Step in the Grievance Procedure');
  t('Notes');
  t('Notification Body');
  t('Notification Subject');
  t('Organizer');
  t('Outcome');
  t('Phone');
  t('Possible Classifications');
  t('Possible Departments');
  t('Postal Code');
  t('Recommended Remedy');
  t('Recommended Statement of Grievance');
  t('Remedy');
  t('Resolution Procedure');
  t('Role');
  t('Roles');
  t('Section Number');
  t('Select "Yes" if this document type should appear as an "Information Request" option.');
  t('Select a Contact');
  t('Select the date that the status changed.  For example, if the status is "Step 1 Meeting", enter the date that the meeting occurred.  If no date is entered, today\'s date will be used.');
  t('Select the roles with permission to edit grievances at this step.');
  t('Settlement Amount');
  t('Shift');
  t('Shops');
  t('Show on Timeline?');
  t('State or Province');
  t('Status');
  t('Status Change Date');
  t('Statuses');
  t('Supervisor Name');
  t('Supervisor Title');
  t('Supervisor Unit');
  t('Tags');
  t('The date the employee received discipline.');
  t('Type of Days');
  t('Union Representative');
  t('Unresolved?');
  t('Upload the current contract document for this shop.  The contract will be used by stewards to identify violations.');
  t('Use this field to indicate that member data in the grievance system has changed from the Union\'s member management system, and that the new data needs to be uploaded back to the member management system.  "Unchanged" means that the data is the same in both places; "Pending" means that corrected data has been entered here and needs to be entered in the member management system.  After the information has been changed, it will be flagged as "Complete".');
  t('Valid');
  t('Violation');
  t('Watching');
  t('When a notification for this status is sent to the company, what subject should be used?');
  t('When a notification is sent to the company, what should the body of the email be? ');
  t('When the grievance admin selects this clause, which "statement of grievance" should be selected?');
  t('When the grievance enters this status, should it be displayed on the timeline presented to the company?');
  t('Work Status');

  return $field_instances;
}
