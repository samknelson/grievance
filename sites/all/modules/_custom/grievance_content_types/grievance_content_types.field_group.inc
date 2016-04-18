<?php
/**
 * @file
 * grievance_content_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function grievance_content_types_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_arbitration|node|grievance|default';
  $field_group->group_name = 'group_grievance_arbitration';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Arbitration',
    'weight' => '12',
    'children' => array(
      0 => 'field_grievance_arbitration_a',
      1 => 'field_grievance_arbitration_b',
      2 => 'field_grievance_arbitration_c',
      3 => 'field_grievance_arbitration_d',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Arbitration',
      'instance_settings' => array(
        'classes' => 'group-grievance-arbitration field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_arbitration|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_arbitration|node|grievance|form';
  $field_group->group_name = 'group_grievance_arbitration';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Arbitration',
    'weight' => '8',
    'children' => array(
      0 => 'field_grievance_arbitration_a',
      1 => 'field_grievance_arbitration_b',
      2 => 'field_grievance_arbitration_c',
      3 => 'field_grievance_arbitration_d',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Arbitration',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Use this form to manage information about the grievance\'s arbitration procedure.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_arbitration|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_a|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Representatives',
    'weight' => '3',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_grievance_co_name',
      2 => 'field_grievance_co_email',
      3 => 'field_grievance_co_phone',
      4 => 'field_grievance_co_address',
      5 => 'field_grievance_co_address_2',
      6 => 'field_grievance_co_city',
      7 => 'field_grievance_co_state',
      8 => 'field_grievance_co_zip',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Company Representatives',
      'instance_settings' => array(
        'classes' => 'group-grievance-a field-group-fieldset',
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'id' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_a|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_b|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Union Representatives',
    'weight' => '2',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
      4 => 'field_grievance_rep_watching',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Union Representatives',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-b field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_b|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_address|node|grievance_shop_contact|default';
  $field_group->group_name = 'group_grievance_co_address';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Address',
    'weight' => '2',
    'children' => array(
      0 => 'field_grievance_co_address',
      1 => 'field_grievance_co_address_2',
      2 => 'field_grievance_co_city',
      3 => 'field_grievance_co_state',
      4 => 'field_grievance_co_zip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Address',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-co-address field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_co_address|node|grievance_shop_contact|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_address|node|grievance_shop_contact|form';
  $field_group->group_name = 'group_grievance_co_address';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance_co';
  $field_group->data = array(
    'label' => 'Address',
    'weight' => '8',
    'children' => array(
      0 => 'field_grievance_co_address',
      1 => 'field_grievance_co_address_2',
      2 => 'field_grievance_co_city',
      3 => 'field_grievance_co_state',
      4 => 'field_grievance_co_zip',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-grievance-co-address field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_co_address|node|grievance_shop_contact|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_email|node|grievance_shop_contact|default';
  $field_group->group_name = 'group_grievance_co_email';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Email and Phone',
    'weight' => '1',
    'children' => array(
      0 => 'field_grievance_co_phone',
      1 => 'field_grievance_co_email',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Email and Phone',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-co-email field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_co_email|node|grievance_shop_contact|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_email|node|grievance_shop_contact|form';
  $field_group->group_name = 'group_grievance_co_email';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance_co';
  $field_group->data = array(
    'label' => 'Phone and Email',
    'weight' => '9',
    'children' => array(
      0 => 'field_grievance_co_phone',
      1 => 'field_grievance_co_email',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Phone and Email',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-co-email field-group-accordion-item',
        'description' => 'The email field will be used to send notifications to this representative.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_co_email|node|grievance_shop_contact|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_nameandrole|node|grievance_shop_contact|default';
  $field_group->group_name = 'group_grievance_co_nameandrole';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Name and Role',
    'weight' => '0',
    'children' => array(
      0 => 'field_grievance_co_name',
      1 => 'field_grievance_co_role',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Name and Role',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-co-nameandrole field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_co_nameandrole|node|grievance_shop_contact|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_nameandrole|node|grievance_shop_contact|form';
  $field_group->group_name = 'group_grievance_co_nameandrole';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance_co';
  $field_group->data = array(
    'label' => 'Name and Role',
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_co_name',
      1 => 'field_grievance_co_role',
      2 => 'title',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-grievance-co-nameandrole field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_co_nameandrole|node|grievance_shop_contact|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_shops|node|grievance_shop_contact|default';
  $field_group->group_name = 'group_grievance_co_shops';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Shops and Statuses',
    'weight' => '3',
    'children' => array(
      0 => 'field_grievance_shops',
      1 => 'field_grievance_statuses',
      2 => 'field_grievance_company',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Shops and Statuses',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-co-shops field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_co_shops|node|grievance_shop_contact|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co_shops|node|grievance_shop_contact|form';
  $field_group->group_name = 'group_grievance_co_shops';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance_co';
  $field_group->data = array(
    'label' => 'Shops and Statuses',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_shops',
      1 => 'field_grievance_statuses',
      2 => 'field_grievance_company',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Shops and Statuses',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-co-shops field-group-accordion-item',
        'description' => 'This contact will appear as an option for any of the shops selected.  If you select statuses, then the contact will also appear as an option for <strong>every</strong> grievance with that status.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_co_shops|node|grievance_shop_contact|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_comments|node|grievance|default';
  $field_group->group_name = 'group_grievance_comments';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Comments and Attachments',
    'weight' => '11',
    'children' => array(
      0 => 'field_grievance_comments',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Comments and Attachments',
      'instance_settings' => array(
        'classes' => 'group-grievance-comments field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_comments|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_comments|node|grievance|form';
  $field_group->group_name = 'group_grievance_comments';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Comments',
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_comments',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Comments',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_comments|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_company|node|grievance|default';
  $field_group->group_name = 'group_grievance_company';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Represenatives',
    'weight' => '8',
    'children' => array(
      0 => 'field_grievance_co_name',
      1 => 'field_grievance_co_email',
      2 => 'field_grievance_co_phone',
      3 => 'field_grievance_co_address',
      4 => 'field_grievance_co_address_2',
      5 => 'field_grievance_co_city',
      6 => 'field_grievance_co_state',
      7 => 'field_grievance_co_zip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Company Represenatives',
      'instance_settings' => array(
        'classes' => 'group-grievance-company field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_company|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_company|node|grievance|form';
  $field_group->group_name = 'group_grievance_company';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Company Represenatives',
    'weight' => '2',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_grievance_co_name',
      2 => 'field_grievance_co_email',
      3 => 'field_grievance_co_phone',
      4 => 'field_grievance_co_address',
      5 => 'field_grievance_co_address_2',
      6 => 'field_grievance_co_city',
      7 => 'field_grievance_co_state',
      8 => 'field_grievance_co_zip',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Company Represenatives',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Enter information about the company representative.  This representative will receive notification when the grievance enters specific statuses, such as Step 1, Step 2, or Arbitration.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_company|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_co|node|grievance_shop_contact|form';
  $field_group->group_name = 'group_grievance_co';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop_contact';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Representative',
    'weight' => '0',
    'children' => array(
      0 => 'group_grievance_co_nameandrole',
      1 => 'group_grievance_co_email',
      2 => 'group_grievance_co_address',
      3 => 'group_grievance_co_shops',
    ),
    'format_type' => 'accordion',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'effect' => 'none',
        'classes' => '',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_grievance_co|node|grievance_shop_contact|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_c|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employee',
    'weight' => '4',
    'children' => array(
      0 => 'field_grievance_department',
      1 => 'field_grievance_phone',
      2 => 'field_grievance_first_name',
      3 => 'field_grievance_last_name',
      4 => 'field_grievance_hire_date',
      5 => 'field_grievance_address',
      6 => 'field_grievance_address_2',
      7 => 'field_grievance_city',
      8 => 'field_grievance_state',
      9 => 'field_grievance_zip',
      10 => 'field_grievance_email',
      11 => 'field_grievance_classification',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Employee',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-c field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_c|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_d|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_d';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supervisor',
    'weight' => '5',
    'children' => array(
      0 => 'field_grievance_supervisor_name',
      1 => 'field_grievance_supervisor_title',
      2 => 'field_grievance_supervisor_unit',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Supervisor',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-d field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_d|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_employee|node|grievance|default';
  $field_group->group_name = 'group_grievance_employee';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employee',
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_department',
      1 => 'field_grievance_phone',
      2 => 'field_grievance_first_name',
      3 => 'field_grievance_last_name',
      4 => 'field_grievance_hire_date',
      5 => 'field_grievance_address',
      6 => 'field_grievance_address_2',
      7 => 'field_grievance_city',
      8 => 'field_grievance_state',
      9 => 'field_grievance_zip',
      10 => 'field_grievance_email',
      11 => 'field_grievance_classification',
      12 => 'field_grievance_ein',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Employee',
      'instance_settings' => array(
        'classes' => 'group-grievance-employee field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_employee|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_employee|node|grievance|form';
  $field_group->group_name = 'group_grievance_employee';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Employee',
    'weight' => '4',
    'children' => array(
      0 => 'field_grievance_department',
      1 => 'field_grievance_phone',
      2 => 'field_grievance_first_name',
      3 => 'field_grievance_last_name',
      4 => 'field_grievance_hire_date',
      5 => 'field_grievance_address',
      6 => 'field_grievance_address_2',
      7 => 'field_grievance_city',
      8 => 'field_grievance_state',
      9 => 'field_grievance_zip',
      10 => 'field_grievance_email',
      11 => 'field_grievance_classification',
      12 => 'field_grievance_ein',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_employee|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_e|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_e';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Grievance',
    'weight' => '6',
    'children' => array(
      0 => 'field_grievance_type',
      1 => 'field_grievance_remedy_other',
      2 => 'field_grievance_remedy',
      3 => 'field_grievance_type_other',
      4 => 'field_grievance_violation',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Grievance',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-e field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_e|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_form|node|grievance|default';
  $field_group->group_name = 'group_grievance_form';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Grievance Form',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_type',
      1 => 'field_grievance_remedy_other',
      2 => 'field_grievance_remedy',
      3 => 'field_grievance_type_other',
      4 => 'field_grievance_violation',
      5 => 'field_grievance_inforeq',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Grievance Form',
      'instance_settings' => array(
        'classes' => 'group-grievance-form field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_form|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_form|node|grievance|form';
  $field_group->group_name = 'group_grievance_form';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Grievance Form',
    'weight' => '6',
    'children' => array(
      0 => 'field_grievance_type',
      1 => 'field_grievance_date_1',
      2 => 'field_grievance_remedy_other',
      3 => 'field_grievance_remedy',
      4 => 'field_grievance_type_other',
      5 => 'field_grievance_violation',
      6 => 'field_grievance_inforeq',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_form|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_f|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_f';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Arbitration Comments',
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_arbitration_a',
      1 => 'field_grievance_arbitration_b',
      2 => 'field_grievance_arbitration_c',
      3 => 'field_grievance_arbitration_d',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Arbitration Comments',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-f field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_f|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_g|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_g';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Attachments',
    'weight' => '8',
    'children' => array(
      0 => 'field_grievance_attachments',
      1 => 'field_grievance_attachments_a',
      2 => 'field_grievance_attachments_b',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Attachments',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-g field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_g|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_h|node|grievance|grievance_arbitrationform';
  $field_group->group_name = 'group_grievance_h';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_arbitrationform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Timeline',
    'weight' => '9',
    'children' => array(
      0 => 'field_grievance_date_1',
      1 => 'field_grievance_date_2',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Timeline',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-h field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_h|node|grievance|grievance_arbitrationform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_notifications|node|grievance_shop|default';
  $field_group->group_name = 'group_grievance_notifications';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '2',
    'children' => array(),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-grievance-notifications field-group-fieldset',
      ),
    ),
  );
  $field_groups['group_grievance_notifications|node|grievance_shop|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_notify|taxonomy_term|grievance_status|form';
  $field_group->group_name = 'group_grievance_notify';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'grievance_status';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '5',
    'children' => array(
      0 => 'field_grievance_notify',
      1 => 'field_grievance_notify_subject',
      2 => 'field_grievance_notify_body',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Notifications',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-notify field-group-fieldset',
        'description' => 'Use the fields below to define how notifications are sent to the company when a grievance is moved to this status.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_grievance_notify|taxonomy_term|grievance_status|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_status|node|grievance|form';
  $field_group->group_name = 'group_grievance_status';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Status',
    'weight' => '1',
    'children' => array(
      0 => 'field_grievance_status',
      1 => 'field_grievance_alert_date',
      2 => 'field_grievance_alert',
      3 => 'field_grievance_alert_waived',
      4 => 'field_grievance_alert_tid',
      5 => 'title',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Status',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_status|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_supervisor|node|grievance|default';
  $field_group->group_name = 'group_grievance_supervisor';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supervisor',
    'weight' => '9',
    'children' => array(
      0 => 'field_grievance_supervisor_name',
      1 => 'field_grievance_supervisor_title',
      2 => 'field_grievance_supervisor_unit',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Supervisor',
      'instance_settings' => array(
        'classes' => 'group-grievance-supervisor field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_supervisor|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_supervisor|node|grievance|form';
  $field_group->group_name = 'group_grievance_supervisor';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Supervisor',
    'weight' => '5',
    'children' => array(
      0 => 'field_grievance_supervisor_name',
      1 => 'field_grievance_supervisor_title',
      2 => 'field_grievance_supervisor_unit',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Supervisor',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Enter information about the employer\'s supervisor.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_supervisor|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_timeline|node|grievance|default';
  $field_group->group_name = 'group_grievance_timeline';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Timeline',
    'weight' => '13',
    'children' => array(
      0 => 'field_grievance_date_1',
      1 => 'grievance_timeline',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Timeline',
      'instance_settings' => array(
        'classes' => 'group-grievance-timeline field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_timeline|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_union_rep|node|grievance|default';
  $field_group->group_name = 'group_grievance_union_rep';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Union Representatives',
    'weight' => '6',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
      4 => 'field_grievance_rep_watching',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Union Representatives',
      'instance_settings' => array(
        'classes' => 'group-grievance-union-rep field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_union_rep|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_union_rep|node|grievance|form';
  $field_group->group_name = 'group_grievance_union_rep';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Union Representatives',
    'weight' => '3',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
      4 => 'field_grievance_rep_watching',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Union Representatives',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Enter information about the union representatives.  These representatives will receive alerts each time the grievance is updated, and will also receive alerts when the deadline (under "Status") is about to expire.',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_union_rep|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_union|node|grievance|grievance_gform';
  $field_group->group_name = 'group_grievance_union';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_gform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Union Representatives',
    'weight' => '43',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Union Representatives',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-union field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_union|node|grievance|grievance_gform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance|node|grievance|form';
  $field_group->group_name = 'group_grievance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Grievance',
    'weight' => '0',
    'children' => array(
      0 => 'group_grievance_employee',
      1 => 'group_grievance_comments',
      2 => 'group_grievance_supervisor',
      3 => 'group_grievance_form',
      4 => 'group_grievance_union_rep',
      5 => 'group_grievance_company',
      6 => 'group_grievance_arbitration',
      7 => 'group_grievance_status',
    ),
    'format_type' => 'accordion',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'effect' => 'none',
        'classes' => 'group-grievance field-group-accordion',
      ),
    ),
  );
  $field_groups['group_grievance|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notifications|node|grievance_shop|form';
  $field_group->group_name = 'group_notifications';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '3',
    'children' => array(
      0 => 'field_grievance_first_name',
      1 => 'field_grievance_last_name',
      2 => 'field_grievance_email',
      3 => 'field_grievance_address',
      4 => 'field_grievance_address_2',
      5 => 'field_grievance_city',
      6 => 'field_grievance_state',
      7 => 'field_grievance_zip',
      8 => 'field_grievance_phone',
      9 => 'field_grievance_co_name',
      10 => 'field_grievance_co_phone',
      11 => 'field_grievance_co_email',
      12 => 'field_grievance_co_address',
      13 => 'field_grievance_co_address_2',
      14 => 'field_grievance_co_city',
      15 => 'field_grievance_co_state',
      16 => 'field_grievance_co_zip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Notifications',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-notifications field-group-fieldset',
        'description' => 'When a grievance is filed or moved to step 2, an email should be sent to this contact.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_notifications|node|grievance_shop|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Address');
  t('Arbitration');
  t('Arbitration Comments');
  t('Attachments');
  t('Comments');
  t('Comments and Attachments');
  t('Company Represenatives');
  t('Company Representative');
  t('Company Representatives');
  t('Email and Phone');
  t('Employee');
  t('Grievance');
  t('Grievance Form');
  t('Name and Role');
  t('Notifications');
  t('Phone and Email');
  t('Shops and Statuses');
  t('Status');
  t('Supervisor');
  t('Timeline');
  t('Union Representatives');

  return $field_groups;
}
