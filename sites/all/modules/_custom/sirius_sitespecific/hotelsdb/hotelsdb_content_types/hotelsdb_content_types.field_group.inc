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
  $field_group->identifier = 'group_airports|node|property|default';
  $field_group->group_name = 'group_airports';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Airports',
    'weight' => '10',
    'children' => array(
      0 => 'field_airport',
      1 => 'field_airport_alt',
      2 => 'field_calc_airport_dist',
      3 => 'field_calc_airport_dist_alt',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Airports',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-airports field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_airports|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_amenities|node|property|default';
  $field_group->group_name = 'group_amenities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Hotel Amenities',
    'weight' => '9',
    'children' => array(
      0 => 'field_rooms',
      1 => 'field_fh_meeting_room_count',
      2 => 'field_fh_meeting_room_capacity',
      3 => 'field_fh_event_space',
      4 => 'field_fh_ballroom_count',
      5 => 'field_fh_ballroom_capacity',
      6 => 'field_fh_outdoor',
      7 => 'field_fh_outdoor_area',
      8 => 'field_fh_parking',
      9 => 'field_fh_parking_capacity',
      10 => 'field_fh_meeting_room_largest',
      11 => 'field_niche',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Hotel Amenities',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-amenities field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_amenities|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_amenities|node|property|form';
  $field_group->group_name = 'group_amenities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Amenities',
    'weight' => '7',
    'children' => array(
      0 => 'field_rooms',
      1 => 'field_fh_meeting_room_count',
      2 => 'field_fh_meeting_room_capacity',
      3 => 'field_fh_event_space',
      4 => 'field_fh_ballroom_count',
      5 => 'field_fh_ballroom_capacity',
      6 => 'field_convention_center',
      7 => 'field_calc_convention_dist',
      8 => 'field_convention_center_attached',
      9 => 'field_fh_outdoor',
      10 => 'field_fh_outdoor_area',
      11 => 'field_fh_parking',
      12 => 'field_fh_parking_capacity',
      13 => 'field_fh_meeting_room_largest',
      14 => 'field_niche',
      15 => 'field_airport',
      16 => 'field_airport_alt',
      17 => 'field_calc_airport_dist',
      18 => 'field_calc_airport_dist_alt',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-amenities field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_amenities|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_casino|node|property|default';
  $field_group->group_name = 'group_casino';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Casino',
    'weight' => '12',
    'children' => array(
      0 => 'field_csno_slots_count',
      1 => 'field_csno_slots_date',
      2 => 'field_csno_tables_count',
      3 => 'field_csno_tables_date',
      4 => 'field_csno_source',
      5 => 'field_csno_regulator',
      6 => 'field_csno_book',
      7 => 'field_csno_horse',
      8 => 'field_csno_online',
      9 => 'field_csno_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Casino',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-casino field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_casino|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_casino|node|property|form';
  $field_group->group_name = 'group_casino';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Casino',
    'weight' => '8',
    'children' => array(
      0 => 'field_csno_slots_count',
      1 => 'field_csno_slots_date',
      2 => 'field_csno_tables_count',
      3 => 'field_csno_tables_date',
      4 => 'field_csno_source',
      5 => 'field_csno_regulator',
      6 => 'field_csno_book',
      7 => 'field_csno_horse',
      8 => 'field_csno_online',
      9 => 'field_csno_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-casino field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_casino|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contracts|node|property|form';
  $field_group->group_name = 'group_contracts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contracts',
    'weight' => '4',
    'children' => array(
      0 => 'field_union_status',
      1 => 'field_local',
      2 => 'field_hotel_contract_start_date',
      3 => 'field_hotel_contract_end_date',
      4 => 'field_hotel_contract_covered',
      5 => 'field_rest_contract_start_date',
      6 => 'field_rest_contract_end_date',
      7 => 'field_rest_contract_covered',
      8 => 'field_union_other',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_contracts|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contract|node|property|default';
  $field_group->group_name = 'group_contract';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contract',
    'weight' => '8',
    'children' => array(
      0 => 'field_union_status',
      1 => 'field_local',
      2 => 'field_hotel_contract_start_date',
      3 => 'field_hotel_contract_end_date',
      4 => 'field_hotel_contract_covered',
      5 => 'field_rest_contract_start_date',
      6 => 'field_rest_contract_end_date',
      7 => 'field_rest_contract_covered',
      8 => 'field_union_other',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Contract',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-contract field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_contract|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_convention_center|node|property|default';
  $field_group->group_name = 'group_convention_center';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Convention Center',
    'weight' => '11',
    'children' => array(
      0 => 'field_convention_center',
      1 => 'field_calc_convention_dist',
      2 => 'field_convention_center_attached',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Convention Center',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-convention-center field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_convention_center|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_fairhotel|node|property|default';
  $field_group->group_name = 'group_fairhotel';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'FairHotels',
    'weight' => '13',
    'children' => array(
      0 => 'field_boycott_url',
      1 => 'field_labor_activity_index',
      2 => 'field_fh_notes',
      3 => 'field_fh_nonunion',
      4 => 'field_fh_last_updated',
      5 => 'field_fh_sources',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'FairHotels',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-fairhotel field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_fairhotel|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_fairhotel|node|property|form';
  $field_group->group_name = 'group_fairhotel';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'FairHotels',
    'weight' => '9',
    'children' => array(
      0 => 'field_boycott_url',
      1 => 'field_labor_activity_index',
      2 => 'field_fh_notes',
      3 => 'field_fh_nonunion',
      4 => 'field_fh_last_updated',
      5 => 'field_fh_sources',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-fairhotel field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_fairhotel|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_fs|node|property|form';
  $field_group->group_name = 'group_fs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Food Service',
    'weight' => '10',
    'children' => array(
      0 => 'field_fs_segment',
      1 => 'field_fs_dbe',
      2 => 'field_fs_concession_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-fs field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_fs|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lg|node|property_actor|default';
  $field_group->group_name = 'group_lg';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property_actor';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Loans and Grants',
    'weight' => '6',
    'children' => array(
      0 => 'field_lg_balance',
      1 => 'field_lg_servicer_commentary',
      2 => 'field_lg_date_due',
      3 => 'field_lg_date_issued',
      4 => 'field_lg_date_balance',
      5 => 'field_lg_date_servicer_comment',
      6 => 'field_lg_sources',
      7 => 'field_lg_date_sources',
      8 => 'field_lg_master_servicer',
      9 => 'field_lg_special_servicer',
      10 => 'field_lg_multiparty',
      11 => 'field_lg_amount',
      12 => 'field_lg_id',
      13 => 'field_lg_status',
      14 => 'field_lg_type',
      15 => 'field_lg_portfolio',
      16 => 'field_lg_amortization',
      17 => 'field_lg_employees_claimed',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Loans and Grants',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-lg field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_lg|node|property_actor|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lg|node|property_actor|form';
  $field_group->group_name = 'group_lg';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property_actor';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Loans and Grants',
    'weight' => '20',
    'children' => array(
      0 => 'field_lg_balance',
      1 => 'field_lg_servicer_commentary',
      2 => 'field_lg_date_due',
      3 => 'field_lg_date_issued',
      4 => 'field_lg_date_balance',
      5 => 'field_lg_date_servicer_comment',
      6 => 'field_lg_sources',
      7 => 'field_lg_date_sources',
      8 => 'field_lg_master_servicer',
      9 => 'field_lg_special_servicer',
      10 => 'field_lg_multiparty',
      11 => 'field_lg_amount',
      12 => 'field_lg_id',
      13 => 'field_lg_status',
      14 => 'field_lg_type',
      15 => 'field_lg_portfolio',
      16 => 'field_lg_amortization',
      17 => 'field_lg_employees_claimed',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Loans and Grants',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-lg field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_lg|node|property_actor|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|property|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '1',
    'children' => array(
      0 => 'field_address',
      1 => 'field_address_geofield',
      2 => 'field_phone',
      3 => 'field_fax',
      4 => 'field_url',
      5 => 'field_msa',
      6 => 'field_neighborhood',
      7 => 'field_county',
      8 => 'field_meg',
      9 => 'field_on_public_land',
      10 => 'field_pin',
      11 => 'field_address_canon',
      12 => 'field_address_canon_accuracy',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-location field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_location|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ownership|node|property_actor|default';
  $field_group->group_name = 'group_ownership';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property_actor';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Ownership',
    'weight' => '5',
    'children' => array(
      0 => 'field_pension',
      1 => 'field_percent_owned',
      2 => 'field_amount_owned',
      3 => 'field_owned_raw',
      4 => 'field_sales_price',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Ownership',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-ownership field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_ownership|node|property_actor|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ownership|node|property_actor|form';
  $field_group->group_name = 'group_ownership';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property_actor';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Ownership',
    'weight' => '10',
    'children' => array(
      0 => 'field_pension',
      1 => 'field_percent_owned',
      2 => 'field_amount_owned',
      3 => 'field_owned_raw',
      4 => 'field_sales_price',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Ownership',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-ownership field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_ownership|node|property_actor|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|property|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '0',
    'children' => array(
      0 => 'field_brand',
      1 => 'field_development_status',
      2 => 'field_is_casino',
      3 => 'field_notes',
      4 => 'field_legacy_id',
      5 => 'field_owner_of_record',
      6 => 'field_is_fs',
      7 => 'field_is_hotel',
      8 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_property|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_restaurant|node|property|default';
  $field_group->group_name = 'group_restaurant';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Food Service',
    'weight' => '14',
    'children' => array(
      0 => 'field_fs_segment',
      1 => 'field_fs_dbe',
      2 => 'field_fs_concession_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Food Service',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-restaurant field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_restaurant|node|property|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_restaurant|node|property|form';
  $field_group->group_name = 'group_restaurant';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'property';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Restaurant',
    'weight' => '10',
    'children' => array(
      0 => 'field_rest_cuisine',
      1 => 'field_rest_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-restaurant field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_restaurant|node|property|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
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

  // Translatables
  // Included for use with string extractors like potx.
  t('Airports');
  t('Amenities');
  t('Casino');
  t('Contract');
  t('Contracts');
  t('Convention Center');
  t('FairHotels');
  t('Food Service');
  t('Hotel Amenities');
  t('Loans and Grants');
  t('Location');
  t('Ownership');
  t('Property');
  t('Restaurant');
  t('Tabs');

  return $field_groups;
}
