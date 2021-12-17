<?php

/**
 * @file
 * hotelsdb_content_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function hotelsdb_content_types_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contracts|node|property|form';
  $field_group->group_name = 'group_contracts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Contracts',
    'weight' => '28',
    'children' => array(
      0 => 'field_hotel_contract_start_date',
      1 => 'field_hotel_contract_end_date',
      2 => 'field_hotel_contract_covered',
      3 => 'field_rest_contract_start_date',
      4 => 'field_rest_contract_end_date',
      5 => 'field_rest_contract_covered',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-contracts field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_contracts|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_left|field_collection_item|field_relationships|form';
  $field_group->group_name = 'group_left';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_relationships';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '3',
    'children' => array(
      0 => 'field_percent_owned',
      1 => 'field_amount_owned',
      2 => 'field_pension',
      3 => 'field_owned_raw',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => '',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-left field-group-fieldset group-relationships-fc',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_left|field_collection_item|field_relationships|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_middle|field_collection_item|field_relationships|form';
  $field_group->group_name = 'group_middle';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_relationships';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '4',
    'children' => array(
      0 => 'field_start_date',
      1 => 'field_end_date',
      2 => 'field_sales_price',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => '',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-middle field-group-fieldset group-relationships-fc',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_middle|field_collection_item|field_relationships|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|property|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '27',
    'children' => array(
      0 => 'field_address',
      1 => 'field_address_geofield',
      2 => 'field_phone',
      3 => 'field_fax',
      4 => 'field_url',
      5 => 'field_boycott_url',
      6 => 'field_msa',
      7 => 'field_neighborhood',
      8 => 'field_county',
      9 => 'field_rooms',
      10 => 'field_union_status',
      11 => 'field_labor_activity_index',
      12 => 'field_local',
      13 => 'field_brand',
      14 => 'field_meg',
      15 => 'field_development_status',
      16 => 'field_on_public_land',
      17 => 'field_notes',
      18 => 'field_legacy_id',
      19 => 'field_airport',
      20 => 'field_pin',
      21 => 'field_owner_of_record',
      22 => 'field_attachments',
      23 => 'title',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-property field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_property|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_right|field_collection_item|field_relationships|form';
  $field_group->group_name = 'group_right';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_relationships';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '5',
    'children' => array(
      0 => 'field_notes',
      1 => 'field_legacy_id',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => '',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-right field-group-fieldset group-relationships-fc',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_right|field_collection_item|field_relationships|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|property|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '0',
    'children' => array(
      0 => 'group_property',
      1 => 'group_contracts',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-tabs field-group-htabs',
      ),
    ),
  );
  $field_groups['group_tabs|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_top|field_collection_item|field_relationships|form';
  $field_group->group_name = 'group_top';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_relationships';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '0',
    'children' => array(
      0 => 'field_relationship_type',
      1 => 'field_actor',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => '',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-top field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_top|field_collection_item|field_relationships|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Contracts');
  t('Property');
  t('Tabs');

  return $field_groups;
}
