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
  $field_group->identifier = 'group_grieance_company|node|grievance|grievance_gform';
  $field_group->group_name = 'group_grieance_company';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_gform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Representatives',
    'weight' => '82',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Company Representatives',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grieance-company field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grieance_company|node|grievance|grievance_gform'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_226_1|node|grievance|grievance_form_226';
  $field_group->group_name = 'group_grievance_226_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_form_226';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Contact',
    'weight' => '1',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_grievance_co_name',
      2 => 'field_grievance_co_role',
      3 => 'grievance_company_rep_ad_combine',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Company Contact',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-226-1 field-group-div',
        'description' => 'Grievance to:',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_226_1|node|grievance|grievance_form_226'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_226_2|node|grievance|grievance_form_226';
  $field_group->group_name = 'group_grievance_226_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_form_226';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employee Contact',
    'weight' => '2',
    'children' => array(
      0 => 'field_grievance_rep_organizer',
      1 => 'field_grievance_rep_lead',
      2 => 'field_grievance_class_tid',
      3 => 'grievant_name',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Employee Contact',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-226-2 field-group-div',
        'description' => 'Grievance on behalf of:',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_226_2|node|grievance|grievance_form_226'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_226_3|node|grievance|grievance_form_226';
  $field_group->group_name = 'group_grievance_226_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_form_226';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Information Request',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_inforeq',
      1 => 'field_grievance_document_types',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Information Request',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-226-3 field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_226_3|node|grievance|grievance_form_226'] = $field_group;

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
    'weight' => '18',
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
    'weight' => '10',
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
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_assignment|node|grievance|form';
  $field_group->group_name = 'group_grievance_assignment';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Assignment',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_rep_assignee',
      1 => 'field_grievance_assignee_notes',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-grievance-assignment field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_assignment|node|grievance|form'] = $field_group;

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
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_classaction|node|grievance|form';
  $field_group->group_name = 'group_grievance_classaction';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'General Grievance',
    'weight' => '6',
    'children' => array(
      0 => 'field_grievance_broughtby',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'General Grievance',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-classaction field-group-accordion-item',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_classaction|node|grievance|form'] = $field_group;

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
    'weight' => '25',
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
      2 => 'field_grievance_co_fax',
      3 => 'field_grievance_co_phone_2',
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
    'weight' => '26',
    'children' => array(
      0 => 'field_grievance_co_phone',
      1 => 'field_grievance_co_email',
      2 => 'field_grievance_co_fax',
      3 => 'field_grievance_co_phone_2',
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
      2 => 'field_grievance_min',
      3 => 'field_grievance_contact_types',
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
    'weight' => '24',
    'children' => array(
      0 => 'field_grievance_co_name',
      1 => 'field_grievance_co_role',
      2 => 'field_grievance_min',
      3 => 'field_grievance_contact_types',
      4 => 'field_grievance_external_id',
      5 => 'title',
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
    'label' => 'Shops and Company',
    'weight' => '3',
    'children' => array(
      0 => 'field_grievance_shops',
      1 => 'field_grievance_company',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Shops and Company',
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
    'label' => 'Shops and Company',
    'weight' => '27',
    'children' => array(
      0 => 'field_grievance_shops',
      1 => 'field_grievance_company',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Shops and Company',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-co-shops field-group-accordion-item',
        'description' => 'This contact will appear as an option for any of the shops selected.  If you select a company, it will appear as an option for all shops in that company.',
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
    'label' => 'Comments',
    'weight' => '17',
    'children' => array(
      0 => 'field_grievance_comments',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Comments',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-comments field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
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
    'weight' => '6',
    'children' => array(
      0 => 'field_grievance_comments',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Comments',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Use the space below to enter:
<ul>
<li>Agency Charges (NLRB, EEOC, Etc.)</li>
<li>Related Grievances</li>
<li>Prior Arbitration Awards</li>
<li>Other</li>
</ul>',
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
    'label' => 'Company Rep',
    'weight' => '13',
    'children' => array(
      0 => 'field_grievance_co_name',
      1 => 'field_grievance_co_email',
      2 => 'field_grievance_co_phone',
      3 => 'field_grievance_co_address',
      4 => 'field_grievance_co_address_2',
      5 => 'field_grievance_co_city',
      6 => 'field_grievance_co_state',
      7 => 'field_grievance_co_zip',
      8 => 'field_grievance_co_role',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Company Rep',
      'instance_settings' => array(
        'id' => '',
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
    'label' => 'Company Representatives',
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
      8 => 'field_grievance_contact_selector',
      9 => 'field_grievance_co_role',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Company Representatives',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Enter information about the company representative who should be listed on the grievance form as the main recipient of the grievance.',
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
  $field_group->identifier = 'group_grievance_default_ureps|node|grievance_shop|form';
  $field_group->group_name = 'group_grievance_default_ureps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_sirius_details';
  $field_group->data = array(
    'label' => 'Grievance',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_assignee_notes',
      1 => 'field_grievance_rep_assignee',
      2 => 'field_grievance_rep_filed',
      3 => 'field_grievance_rep_lead',
      4 => 'field_grievance_rep_manager',
      5 => 'field_grievance_rep_organizer',
      6 => 'field_grievance_rep_watching',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Grievance',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_default_ureps|node|grievance_shop|form'] = $field_group;

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
    'weight' => '12',
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
      13 => 'field_grievance_min',
      14 => 'field_grievance_class_tid',
      15 => 'field_grievance_department_tid',
      16 => 'field_grievance_daysoff',
      17 => 'field_grievance_shift',
      18 => 'field_grievance_work_status',
      19 => 'field_grievance_gender',
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
    'weight' => '2',
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
      13 => 'field_grievance_min',
      14 => 'field_grievance_corrected',
      15 => 'field_grievance_class_tid',
      16 => 'field_grievance_department_tid',
      17 => 'field_grievance_daysoff',
      18 => 'field_grievance_shift',
      19 => 'field_grievance_work_status',
      20 => 'field_grievance_gender',
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
  $field_group->identifier = 'group_grievance_employee|node|grievance|grievance_gform';
  $field_group->group_name = 'group_grievance_employee';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_gform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employee',
    'weight' => '81',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Employee',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-employee field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_employee|node|grievance|grievance_gform'] = $field_group;

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
    'weight' => '15',
    'children' => array(
      0 => 'field_grievance_type',
      1 => 'field_grievance_remedy_other',
      2 => 'field_grievance_remedy',
      3 => 'field_grievance_type_other',
      4 => 'field_grievance_violation',
      5 => 'field_grievance_clauseref',
      6 => 'field_grievance_broughtby',
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
    'weight' => '5',
    'children' => array(
      0 => 'field_grievance_type',
      1 => 'field_grievance_date_1',
      2 => 'field_grievance_remedy_other',
      3 => 'field_grievance_remedy',
      4 => 'field_grievance_type_other',
      5 => 'field_grievance_violation',
      6 => 'field_grievance_clauseref',
      7 => 'field_grievance_date_2',
      8 => 'field_grievance_category',
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
  $field_group->identifier = 'group_grievance_inforeq|node|grievance|default';
  $field_group->group_name = 'group_grievance_inforeq';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Information Request',
    'weight' => '16',
    'children' => array(
      0 => 'field_grievance_inforeq',
      1 => 'field_grievance_document_types',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Information Request',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-inforeq field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_inforeq|node|grievance|default'] = $field_group;

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
    'weight' => '5',
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
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_notify_subject',
      1 => 'field_grievance_notify_body',
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
  $field_group->identifier = 'group_grievance_resproc|node|grievance|default';
  $field_group->group_name = 'group_grievance_resproc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Resolution Procedure',
    'weight' => '20',
    'children' => array(
      0 => 'field_grievance_resproc_an',
      1 => 'field_grievance_resproc_lh',
      2 => 'field_grievance_resproc_er',
      3 => 'field_grievance_outcome',
      4 => 'field_grievance_resproc_ea',
      5 => 'field_grievance_resproc_ur',
      6 => 'field_field_grievance_resproc_ua',
      7 => 'field_grievance_resproc_hd',
      8 => 'field_grievance_resproc_cd',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Resolution Procedure',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-resproc field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_resproc|node|grievance|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_resproc|node|grievance|form';
  $field_group->group_name = 'group_grievance_resproc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Resolution Procedure',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_arbitration_a',
      1 => 'field_grievance_arbitration_b',
      2 => 'field_grievance_arbitration_c',
      3 => 'field_grievance_arbitration_d',
      4 => 'field_grievance_resproc_an',
      5 => 'field_grievance_resproc_lh',
      6 => 'field_grievance_resproc_er',
      7 => 'field_grievance_outcome',
      8 => 'field_grievance_resproc_ea',
      9 => 'field_grievance_resproc_ur',
      10 => 'field_field_grievance_resproc_ua',
      11 => 'field_grievance_resproc_hd',
      12 => 'field_grievance_resproc_cd',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Resolution Procedure',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-grievance-resproc field-group-accordion-item',
        'description' => '...',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_resproc|node|grievance|form'] = $field_group;

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
      1 => 'field_grievance_shop',
      2 => 'field_grievance_alert_date',
      3 => 'field_grievance_alert',
      4 => 'field_grievance_alert_waived',
      5 => 'field_grievance_alert_tid',
      6 => 'field_grievance_id',
      7 => 'field_grievance_meeting_date',
      8 => 'field_grievance_contract_tplt',
      9 => 'field_grievance_status_date',
      10 => 'field_grievance_classaction',
      11 => 'field_grievance_broughtby',
      12 => 'field_grievance_external_id',
      13 => 'title',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Status',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_grievance_status|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_stewards|node|grievance|form';
  $field_group->group_name = 'group_grievance_stewards';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Shop Stewards',
    'weight' => '9',
    'children' => array(
      0 => 'field_grievance_st_email',
      1 => 'field_grievance_st_name',
      2 => 'field_grievance_st_phone',
      3 => 'field_grievance_st_selector',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-grievance-stewards field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_grievance_stewards|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_grievance_st|node|grievance|default';
  $field_group->group_name = 'group_grievance_st';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Shop Steward',
    'weight' => '11',
    'children' => array(
      0 => 'field_grievance_st_email',
      1 => 'field_grievance_st_name',
      2 => 'field_grievance_st_phone',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Shop Steward',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-st field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_st|node|grievance|default'] = $field_group;

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
    'weight' => '14',
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
    'weight' => '3',
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
  $field_group->identifier = 'group_grievance_supervisor|node|grievance|grievance_gform';
  $field_group->group_name = 'group_grievance_supervisor';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'grievance_gform';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supervisor',
    'weight' => '83',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Supervisor',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-grievance-supervisor field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_grievance_supervisor|node|grievance|grievance_gform'] = $field_group;

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
    'weight' => '19',
    'children' => array(
      0 => 'grievance_timeline_short',
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
    'label' => 'Union Rep',
    'weight' => '10',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
      4 => 'field_grievance_rep_watching',
      5 => 'field_grievance_assignee_notes',
      6 => 'field_grievance_rep_assignee',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Union Rep',
      'instance_settings' => array(
        'id' => '',
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
    'weight' => '4',
    'children' => array(
      0 => 'field_grievance_rep_filed',
      1 => 'field_grievance_rep_organizer',
      2 => 'field_grievance_rep_lead',
      3 => 'field_grievance_rep_manager',
      4 => 'field_grievance_rep_watching',
      5 => 'field_grievance_assignee_notes',
      6 => 'field_grievance_rep_assignee',
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
    'weight' => '80',
    'children' => array(
      0 => 'field_grievance_rep_organizer',
      1 => 'field_grievance_rep_lead',
      2 => 'field_grievance_rep_manager',
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
      0 => 'group_grievance_comments',
      1 => 'group_grievance_company',
      2 => 'group_grievance_employee',
      3 => 'group_grievance_form',
      4 => 'group_grievance_resproc',
      5 => 'group_grievance_status',
      6 => 'group_grievance_stewards',
      7 => 'group_grievance_supervisor',
      8 => 'group_grievance_union_rep',
      9 => 'group_inforeq',
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
  $field_group->identifier = 'group_inforeq|node|grievance|form';
  $field_group->group_name = 'group_inforeq';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_grievance';
  $field_group->data = array(
    'label' => 'Information Request',
    'weight' => '7',
    'children' => array(
      0 => 'field_grievance_inforeq',
      1 => 'field_grievance_document_types',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-inforeq field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_inforeq|node|grievance|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
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

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_contract|node|grievance_shop|form';
  $field_group->group_name = 'group_sirius_contract';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_sirius_details';
  $field_group->data = array(
    'label' => 'Contract',
    'weight' => '9',
    'children' => array(
      0 => 'field_grievance_contract',
      1 => 'field_grievance_contract_tplt',
      2 => 'field_grievance_attachments',
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
  $field_groups['group_sirius_contract|node|grievance_shop|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_details|node|grievance_shop|form';
  $field_group->group_name = 'group_sirius_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '2',
    'children' => array(
      0 => 'group_grievance_default_ureps',
      1 => 'group_sirius_contract',
      2 => 'group_sirius_identity',
      3 => 'group_sirius_dispatch',
    ),
    'format_type' => 'accordion',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'effect' => 'none',
        'classes' => 'group-sirius-details field-group-accordion',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_sirius_details|node|grievance_shop|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_dispatch_details|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_dispatch_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispatch Details',
    'weight' => '4',
    'children' => array(
      0 => 'field_sirius_datetime_completed',
      1 => 'field_sirius_timelimit',
      2 => 'field_sirius_member_status',
      3 => 'field_sirius_summary',
      4 => 'field_sirius_dispatch_eba',
      5 => 'field_sirius_dispatch_eba_dates',
      6 => 'field_sirius_batch_max',
      7 => 'field_sirius_batch_min',
      8 => 'field_sirius_batch_ratio',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispatch Details',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-dispatch-details field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_dispatch_details|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_dispatch_details|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_dispatch_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispatch Details',
    'weight' => '5',
    'children' => array(
      0 => 'field_sirius_count_yes',
      1 => 'field_sirius_count_no',
      2 => 'field_sirius_count_notified',
      3 => 'field_sirius_timelimit',
      4 => 'field_sirius_member_status',
      5 => 'field_sirius_summary',
      6 => 'field_sirius_dispatch_eba',
      7 => 'field_sirius_dispatch_eba_dates',
      8 => 'field_sirius_batch_max',
      9 => 'field_sirius_batch_min',
      10 => 'field_sirius_batch_ratio',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispatch Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-dispatch-details field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_dispatch_details|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_dispatch|node|grievance_shop|form';
  $field_group->group_name = 'group_sirius_dispatch';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_sirius_details';
  $field_group->data = array(
    'label' => 'Dispatch',
    'weight' => '11',
    'children' => array(
      0 => 'field_sirius_dispatch_notesdef',
      1 => 'field_sirius_dispatch_job_types',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sirius-dispatch field-group-accordion-item',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_sirius_dispatch|node|grievance_shop|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_dispatch|taxonomy_term|sirius_industry|form';
  $field_group->group_name = 'group_sirius_dispatch';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'sirius_industry';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispatch',
    'weight' => '3',
    'children' => array(
      0 => 'field_sirius_dispatch_3sen',
      1 => 'field_sirius_dispatch_days',
      2 => 'field_sirius_dispatch_notesdef',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispatch',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-dispatch field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_dispatch|taxonomy_term|sirius_industry|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_employer_info|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_employer_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employer Info',
    'weight' => '0',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_sirius_id',
      2 => 'field_sirius_industry',
      3 => 'field_sirius_id2',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Employer Info',
      'instance_settings' => array(
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_employer_info|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_employer_info|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_employer_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employer Info',
    'weight' => '1',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_sirius_id',
      2 => 'field_sirius_industry',
      3 => 'field_sirius_id2',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Employer Info',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_employer_info|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_employer_notes|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_employer_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employer Notes',
    'weight' => '3',
    'children' => array(
      0 => 'field_sirius_address_parking',
      1 => 'field_sirius_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Employer Notes',
      'instance_settings' => array(
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_employer_notes|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_employer_notes|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_employer_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Employer Notes',
    'weight' => '4',
    'children' => array(
      0 => 'field_sirius_address_parking',
      1 => 'field_sirius_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Employer Notes',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-employer-notes field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_employer_notes|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_identity|node|grievance_shop|form';
  $field_group->group_name = 'group_sirius_identity';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'grievance_shop';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_sirius_details';
  $field_group->data = array(
    'label' => 'Identity',
    'weight' => '8',
    'children' => array(
      0 => 'field_grievance_company',
      1 => 'field_grievance_tags',
      2 => 'field_grievance_external_id',
      3 => 'field_grievance_chapter',
      4 => 'field_sirius_industry',
      5 => 'field_sirius_active',
      6 => 'field_sirius_name_tts',
      7 => 'field_sirius_id',
      8 => 'title',
    ),
    'format_type' => 'accordion-item',
    'format_settings' => array(
      'label' => 'Identity',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-sirius-identity field-group-accordion-item',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_sirius_identity|node|grievance_shop|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_job_details|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_job_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_sirius_datetime',
      1 => 'field_sirius_dispatch_hall',
      2 => 'field_sirius_dispatch_facility',
      3 => 'field_sirius_address_notes',
      4 => 'field_sirius_skill',
      5 => 'field_sirius_dispatch_job_group',
      6 => 'field_grievance_departments',
      7 => 'field_sirius_dispatch_job_tags',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Job Details',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-job-details field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_job_details|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_job_details|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_job_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job Details',
    'weight' => '3',
    'children' => array(
      0 => 'field_sirius_datetime',
      1 => 'field_sirius_count',
      2 => 'field_sirius_dispatch_hall',
      3 => 'field_sirius_dispatch_facility',
      4 => 'field_sirius_address_notes',
      5 => 'field_sirius_skill',
      6 => 'field_sirius_dispatch_job_group',
      7 => 'field_grievance_departments',
      8 => 'field_sirius_dispatch_job_tags',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Job Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-job-details field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_job_details|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_job_info|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_job_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job Info',
    'weight' => '1',
    'children' => array(
      0 => 'field_sirius_datetime_created',
      1 => 'field_sirius_dispatch_job_type',
      2 => 'field_sirius_dispatch_job_status',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Job Info',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-job-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_job_info|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_job_info|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_job_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job Info',
    'weight' => '2',
    'children' => array(
      0 => 'field_sirius_datetime_created',
      1 => 'field_sirius_datetime_completed',
      2 => 'field_sirius_dispatch_job_type',
      3 => 'field_sirius_dispatch_job_status',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Job Info',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_job_info|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_notify|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_notify';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notification Settings',
    'weight' => '5',
    'children' => array(
      0 => 'field_sirius_dispatch_job_nfcns',
      1 => 'field_sirius_notify',
      2 => 'field_sirius_emails',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Notification Settings',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-notify field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_notify|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_notify|node|sirius_dispatch_job|form';
  $field_group->group_name = 'group_sirius_notify';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '6',
    'children' => array(
      0 => 'field_sirius_dispatch_job_nfcns',
      1 => 'field_sirius_notify',
      2 => 'field_sirius_emails',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sirius-notify field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_sirius_notify|node|sirius_dispatch_job|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_contact|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact',
    'weight' => '3',
    'children' => array(
      0 => 'field_sirius_phone',
      1 => 'field_sirius_address',
      2 => 'field_sirius_phone_alt',
      3 => 'field_sirius_email',
      4 => 'field_sirius_phone_fax',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Contact',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-contact field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_contact|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_contact|node|sirius_worker|form';
  $field_group->group_name = 'group_sirius_worker_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact',
    'weight' => '3',
    'children' => array(
      0 => 'field_sirius_phone',
      1 => 'field_sirius_address',
      2 => 'field_sirius_phone_alt',
      3 => 'field_sirius_email',
      4 => 'field_sirius_headshot',
      5 => 'field_sirius_phone_fax',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Contact',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-worker-contact field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_contact|node|sirius_worker|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_contact|node|sirius_worker|sirius_worker';
  $field_group->group_name = 'group_sirius_worker_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'sirius_worker';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact',
    'weight' => '5',
    'children' => array(
      0 => 'field_sirius_phone',
      1 => 'field_sirius_address',
      2 => 'field_sirius_phone_alt',
      3 => 'field_sirius_email',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sirius-worker-contact field-group-fieldset',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_sirius_worker_contact|node|sirius_worker|sirius_worker'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_counts|node|sirius_dispatch_job|default';
  $field_group->group_name = 'group_sirius_worker_counts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_dispatch_job';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Worker Counts',
    'weight' => '9',
    'children' => array(
      0 => 'field_sirius_count',
      1 => 'field_sirius_count_yes',
      2 => 'field_sirius_count_no',
      3 => 'field_sirius_count_notified',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Worker Counts',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-counts field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_counts|node|sirius_dispatch_job|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_holds|node|sirius_worker|form';
  $field_group->group_name = 'group_sirius_worker_holds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_sirius_worker_preferences';
  $field_group->data = array(
    'label' => 'Holds',
    'weight' => '45',
    'children' => array(
      0 => 'field_sirius_dispatch_hfe',
      1 => 'field_sirius_dispatch_hfe_until',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Holds',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-worker-holds field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_holds|node|sirius_worker|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_identity|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_identity';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Identity',
    'weight' => '1',
    'children' => array(
      0 => 'field_sirius_id',
      1 => 'field_sirius_name',
      2 => 'field_sirius_ssn',
      3 => 'field_sirius_aat',
      4 => 'field_sirius_id2',
      5 => 'field_sirius_gender',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Identity',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-identity field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_identity|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_identity|node|sirius_worker|form';
  $field_group->group_name = 'group_sirius_worker_identity';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Identity',
    'weight' => '2',
    'children' => array(
      0 => 'field_sirius_id',
      1 => 'field_sirius_name',
      2 => 'field_sirius_ssn',
      3 => 'field_sirius_aat',
      4 => 'field_sirius_id2',
      5 => 'field_sirius_gender',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Identity',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-worker-identity field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_identity|node|sirius_worker|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_identity|node|sirius_worker|sirius_worker';
  $field_group->group_name = 'group_sirius_worker_identity';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'sirius_worker';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Identity',
    'weight' => '0',
    'children' => array(
      0 => 'field_sirius_id',
      1 => 'field_sirius_name',
      2 => 'field_sirius_ssn',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sirius-worker-identity field-group-fieldset',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_sirius_worker_identity|node|sirius_worker|sirius_worker'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_preferences|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_preferences';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispatch Preferences',
    'weight' => '4',
    'children' => array(
      0 => 'field_sirius_skills_avail',
      1 => 'field_sirius_dispatch_dncs2',
      2 => 'field_sirius_dispatch_asi',
      3 => 'field_sirius_dispatch_medium',
      4 => 'field_sirius_dispatch_cbn',
      5 => 'field_sirius_dispatch_hfe',
      6 => 'field_sirius_dispatch_hfe_until',
      7 => 'field_sirius_aat_required',
      8 => 'field_grievance_departments',
      9 => 'field_sirius_skills_availx',
      10 => 'field_sirius_dispatch_eba',
      11 => 'field_sirius_dispatch_eba_dates',
      12 => 'field_sirius_dispatch_sib',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispatch Preferences',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-preferences field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_preferences|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_preferences|node|sirius_worker|form';
  $field_group->group_name = 'group_sirius_worker_preferences';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispatch Preferences',
    'weight' => '5',
    'children' => array(
      0 => 'field_sirius_skills_avail',
      1 => 'field_sirius_dispatch_dncs2',
      2 => 'field_sirius_dispatch_asi',
      3 => 'field_sirius_dispatch_medium',
      4 => 'field_sirius_dispatch_cbn',
      5 => 'field_sirius_aat_required',
      6 => 'field_grievance_departments',
      7 => 'field_sirius_skills_availx',
      8 => 'field_sirius_dispatch_eba',
      9 => 'field_sirius_dispatch_eba_dates',
      10 => 'field_sirius_dispatch_sib',
      11 => 'group_sirius_worker_holds',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispatch Preferences',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-worker-preferences field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_preferences|node|sirius_worker|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_skill_exp|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_skill_exp';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skill Expiration',
    'weight' => '46',
    'children' => array(
      0 => 'field_sirius_skill_expire',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Skill Expiration',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-skill-exp field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_skill_exp|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_skills|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '46',
    'children' => array(
      0 => 'field_sirius_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Skills',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-skills field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_skills|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_status|node|sirius_worker|default';
  $field_group->group_name = 'group_sirius_worker_status';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Status',
    'weight' => '2',
    'children' => array(
      0 => 'field_grievance_shop',
      1 => 'field_sirius_member_status',
      2 => 'field_sirius_work_status',
      3 => 'field_sirius_dispatch_dncs',
      4 => 'field_sirius_worker_dispstatus',
      5 => 'field_sirius_dispatch_availdate',
      6 => 'field_sirius_industry',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Status',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-sirius-worker-status field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_status|node|sirius_worker|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sirius_worker_status|node|sirius_worker|form';
  $field_group->group_name = 'group_sirius_worker_status';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sirius_worker';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Status',
    'weight' => '4',
    'children' => array(
      0 => 'field_sirius_skill',
      1 => 'field_grievance_shop',
      2 => 'field_sirius_member_status',
      3 => 'field_sirius_work_status',
      4 => 'field_sirius_dispatch_dncs',
      5 => 'field_sirius_worker_dispstatus',
      6 => 'field_sirius_dispatch_availdate',
      7 => 'field_sirius_industry',
      8 => 'field_sirius_skill_expire',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Status',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sirius-worker-status field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sirius_worker_status|node|sirius_worker|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Address');
  t('Arbitration');
  t('Arbitration Comments');
  t('Assignment');
  t('Attachments');
  t('Comments');
  t('Company Contact');
  t('Company Rep');
  t('Company Representative');
  t('Company Representatives');
  t('Contact');
  t('Contract');
  t('Details');
  t('Dispatch');
  t('Dispatch Details');
  t('Dispatch Preferences');
  t('Email and Phone');
  t('Employee');
  t('Employee Contact');
  t('Employer Info');
  t('Employer Notes');
  t('General Grievance');
  t('Grievance');
  t('Grievance Form');
  t('Holds');
  t('Identity');
  t('Information Request');
  t('Job Details');
  t('Job Info');
  t('Name and Role');
  t('Notification Settings');
  t('Notifications');
  t('Phone and Email');
  t('Resolution Procedure');
  t('Shop Steward');
  t('Shop Stewards');
  t('Shops and Company');
  t('Skill Expiration');
  t('Skills');
  t('Status');
  t('Supervisor');
  t('Timeline');
  t('Union Rep');
  t('Union Representatives');
  t('Worker Counts');

  return $field_groups;
}
