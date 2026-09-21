<?php

/**
 * @file
 * hotelsdb_content_types.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function hotelsdb_content_types_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|property|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'property';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'union_information' => array(
      'weight' => '10',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'unite_relationships' => array(
      'weight' => '34',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'changed_date' => array(
      'weight' => '30',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
  );
  $export['node|property|default'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function hotelsdb_content_types_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'union_information';
  $ds_field->label = 'Union Information';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'code' => array(
      'value' => '[Replace me]',
      'format' => 'ds_code',
    ),
    'use_token' => 0,
  );
  $export['union_information'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function hotelsdb_content_types_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|actor|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'actor';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_title_canon',
        1 => 'field_actor_type',
        2 => 'field_public',
        3 => 'field_address',
      ),
      'right' => array(
        4 => 'field_phone',
        5 => 'field_fax',
        6 => 'field_url',
        7 => 'field_notes',
        8 => 'field_attachments',
      ),
      'footer' => array(
        9 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_title_canon' => 'left',
      'field_actor_type' => 'left',
      'field_public' => 'left',
      'field_address' => 'left',
      'field_phone' => 'right',
      'field_fax' => 'right',
      'field_url' => 'right',
      'field_notes' => 'right',
      'field_attachments' => 'right',
      'unite_relationships' => 'footer',
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
  );
  $export['node|actor|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|actor|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'actor';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_phone',
        1 => 'field_fax',
        2 => 'field_url',
        3 => 'field_notes',
        4 => 'field_address',
        5 => 'field_address_geofield',
      ),
      'right' => array(
        6 => 'property_list',
        7 => 'field_relationships',
      ),
    ),
    'fields' => array(
      'field_phone' => 'left',
      'field_fax' => 'left',
      'field_url' => 'left',
      'field_notes' => 'left',
      'field_address' => 'left',
      'field_address_geofield' => 'left',
      'property_list' => 'right',
      'field_relationships' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|actor|teaser'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|hdb_airport|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'hdb_airport';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_sirius_name_short',
        1 => 'field_address',
        2 => 'field_address_canon',
      ),
      'right' => array(
        3 => 'field_address_geofield',
      ),
    ),
    'fields' => array(
      'field_sirius_name_short' => 'left',
      'field_address' => 'left',
      'field_address_canon' => 'left',
      'field_address_geofield' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
  );
  $export['node|hdb_airport|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|hdb_convention_center|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'hdb_convention_center';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_sirius_name_short',
        1 => 'field_address',
        2 => 'field_address_canon',
        4 => 'field_fh_booth_capacity',
        5 => 'field_notes',
      ),
      'right' => array(
        3 => 'field_address_geofield',
      ),
    ),
    'fields' => array(
      'field_sirius_name_short' => 'left',
      'field_address' => 'left',
      'field_address_canon' => 'left',
      'field_address_geofield' => 'right',
      'field_fh_booth_capacity' => 'left',
      'field_notes' => 'left',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
  );
  $export['node|hdb_convention_center|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|property|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'property';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_niche',
        1 => 'field_is_hotel',
        2 => 'field_fs_segment',
        3 => 'field_union_status',
        4 => 'field_hotel_contract_covered',
        5 => 'field_is_casino',
        6 => 'field_hotel_contract_covered_by',
        7 => 'field_rooms',
        8 => 'field_is_fs',
        9 => 'field_brand',
        10 => 'field_development_status',
        11 => 'field_owner_of_record',
        12 => 'field_convention_center',
        13 => 'field_notes',
        14 => 'union_information',
        15 => 'field_fs_dbe',
        16 => 'group_union',
        17 => 'field_calc_convention_dist',
        18 => 'field_convention_center_attached',
        20 => 'field_fh_meeting_room_count',
        22 => 'field_fh_meeting_room_capacity',
        25 => 'field_fh_meeting_room_largest',
        26 => 'group_amenities',
        28 => 'field_fh_event_space',
        29 => 'field_fh_ballroom_count',
        31 => 'field_fh_ballroom_capacity',
        33 => 'field_fh_outdoor',
        35 => 'field_fh_outdoor_area',
        37 => 'field_labor_activity_index',
        38 => 'field_fh_parking',
        41 => 'field_fh_parking_capacity',
        42 => 'field_boycott_url',
        44 => 'field_csno_type',
        45 => 'field_fh_notes',
        48 => 'field_fh_nonunion',
        49 => 'field_csno_slots_count',
        50 => 'group_airports',
        52 => 'field_csno_slots_date',
        53 => 'field_fh_last_updated',
        54 => 'field_csno_tables_count',
        55 => 'field_fh_sources',
        58 => 'field_csno_tables_date',
        59 => 'field_csno_book_txt',
        61 => 'field_csno_book',
        62 => 'field_csno_horse',
        63 => 'field_csno_online_txt',
        64 => 'field_csno_online',
        65 => 'field_csno_igame',
        66 => 'field_csno_igame_txt',
        67 => 'group_convention_center',
        68 => 'field_csno_txt1',
        69 => 'field_csno_txt2',
        70 => 'field_csno_txt3',
        71 => 'field_csno_txt4',
        72 => 'group_casino',
        73 => 'group_fairhotel',
        74 => 'field_fs_concession_type',
        75 => 'field_airport',
        76 => 'field_calc_airport_dist',
        77 => 'field_airport_alt',
        78 => 'field_calc_airport_dist_alt',
        79 => 'group_restaurant',
        80 => 'field_csno_regulator',
        81 => 'field_csno_source',
      ),
      'right' => array(
        19 => 'field_address_geofield',
        21 => 'field_address',
        23 => 'field_address_canon',
        24 => 'field_address_canon_accuracy',
        27 => 'field_msa',
        30 => 'field_meg',
        32 => 'field_county',
        34 => 'field_neighborhood',
        36 => 'field_pin',
        39 => 'field_on_public_land',
        40 => 'field_phone',
        43 => 'field_fax',
        46 => 'field_url',
      ),
      'footer' => array(
        47 => 'changed_date',
        51 => 'field_calc_owner_last_updated',
        56 => 'field_calc_operator_last_updated',
        57 => 'field_attachments',
        60 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_niche' => 'left',
      'field_is_hotel' => 'left',
      'field_fs_segment' => 'left',
      'field_union_status' => 'left',
      'field_hotel_contract_covered' => 'left',
      'field_is_casino' => 'left',
      'field_hotel_contract_covered_by' => 'left',
      'field_rooms' => 'left',
      'field_is_fs' => 'left',
      'field_brand' => 'left',
      'field_development_status' => 'left',
      'field_owner_of_record' => 'left',
      'field_convention_center' => 'left',
      'field_notes' => 'left',
      'union_information' => 'left',
      'field_fs_dbe' => 'left',
      'group_union' => 'left',
      'field_calc_convention_dist' => 'left',
      'field_convention_center_attached' => 'left',
      'field_address_geofield' => 'right',
      'field_fh_meeting_room_count' => 'left',
      'field_address' => 'right',
      'field_fh_meeting_room_capacity' => 'left',
      'field_address_canon' => 'right',
      'field_address_canon_accuracy' => 'right',
      'field_fh_meeting_room_largest' => 'left',
      'group_amenities' => 'left',
      'field_msa' => 'right',
      'field_fh_event_space' => 'left',
      'field_fh_ballroom_count' => 'left',
      'field_meg' => 'right',
      'field_fh_ballroom_capacity' => 'left',
      'field_county' => 'right',
      'field_fh_outdoor' => 'left',
      'field_neighborhood' => 'right',
      'field_fh_outdoor_area' => 'left',
      'field_pin' => 'right',
      'field_labor_activity_index' => 'left',
      'field_fh_parking' => 'left',
      'field_on_public_land' => 'right',
      'field_phone' => 'right',
      'field_fh_parking_capacity' => 'left',
      'field_boycott_url' => 'left',
      'field_fax' => 'right',
      'field_csno_type' => 'left',
      'field_fh_notes' => 'left',
      'field_url' => 'right',
      'changed_date' => 'footer',
      'field_fh_nonunion' => 'left',
      'field_csno_slots_count' => 'left',
      'group_airports' => 'left',
      'field_calc_owner_last_updated' => 'footer',
      'field_csno_slots_date' => 'left',
      'field_fh_last_updated' => 'left',
      'field_csno_tables_count' => 'left',
      'field_fh_sources' => 'left',
      'field_calc_operator_last_updated' => 'footer',
      'field_attachments' => 'footer',
      'field_csno_tables_date' => 'left',
      'field_csno_book_txt' => 'left',
      'unite_relationships' => 'footer',
      'field_csno_book' => 'left',
      'field_csno_horse' => 'left',
      'field_csno_online_txt' => 'left',
      'field_csno_online' => 'left',
      'field_csno_igame' => 'left',
      'field_csno_igame_txt' => 'left',
      'group_convention_center' => 'left',
      'field_csno_txt1' => 'left',
      'field_csno_txt2' => 'left',
      'field_csno_txt3' => 'left',
      'field_csno_txt4' => 'left',
      'group_casino' => 'left',
      'group_fairhotel' => 'left',
      'field_fs_concession_type' => 'left',
      'field_airport' => 'left',
      'field_calc_airport_dist' => 'left',
      'field_airport_alt' => 'left',
      'field_calc_airport_dist_alt' => 'left',
      'group_restaurant' => 'left',
      'field_csno_regulator' => 'left',
      'field_csno_source' => 'left',
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
  );
  $export['node|property|default'] = $ds_layout;

  return $export;
}
