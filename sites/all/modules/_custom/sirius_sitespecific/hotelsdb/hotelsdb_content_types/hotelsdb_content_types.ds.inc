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
    'unite_relationships' => array(
      'weight' => '32',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'changed_date' => array(
      'weight' => '28',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
  );
  $export['node|property|default'] = $ds_fieldsetting;

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
        0 => 'field_fs_segment',
        1 => 'field_niche',
        2 => 'field_is_hotel',
        3 => 'field_is_casino',
        4 => 'field_rooms',
        5 => 'field_is_fs',
        6 => 'field_brand',
        7 => 'field_development_status',
        8 => 'field_owner_of_record',
        9 => 'field_notes',
        10 => 'field_convention_center',
        11 => 'group_contract',
        12 => 'group_amenities',
        13 => 'field_fs_dbe',
        14 => 'group_airports',
        15 => 'group_convention_center',
        16 => 'group_casino',
        17 => 'group_fairhotel',
        18 => 'field_calc_convention_dist',
        19 => 'group_restaurant',
        20 => 'field_convention_center_attached',
        23 => 'field_fh_meeting_room_count',
        27 => 'field_fh_meeting_room_capacity',
        29 => 'field_fh_meeting_room_largest',
        30 => 'field_fh_event_space',
        33 => 'field_fh_ballroom_count',
        34 => 'field_fh_ballroom_capacity',
        36 => 'field_fh_outdoor',
        38 => 'field_fh_outdoor_area',
        40 => 'field_labor_activity_index',
        41 => 'field_fh_parking',
        43 => 'field_fh_parking_capacity',
        45 => 'field_boycott_url',
        47 => 'field_fh_notes',
        48 => 'field_csno_type',
        50 => 'field_csno_slots_count',
        51 => 'field_fh_nonunion',
        54 => 'field_csno_slots_date',
        55 => 'field_fh_last_updated',
        57 => 'field_csno_tables_count',
        58 => 'field_fh_sources',
        59 => 'field_csno_tables_date',
        60 => 'field_csno_book',
        61 => 'field_csno_horse',
        62 => 'field_csno_regulator',
        63 => 'field_csno_online',
        64 => 'field_csno_source',
        65 => 'field_local',
        66 => 'field_union_status',
        67 => 'field_union_other',
        68 => 'field_hotel_contract_start_date',
        69 => 'field_hotel_contract_end_date',
        70 => 'field_hotel_contract_covered',
        71 => 'field_rest_contract_start_date',
        72 => 'field_rest_contract_end_date',
        73 => 'field_rest_contract_covered',
        74 => 'field_fs_concession_type',
        75 => 'field_airport',
        76 => 'field_calc_airport_dist',
        77 => 'field_airport_alt',
        78 => 'field_calc_airport_dist_alt',
      ),
      'right' => array(
        21 => 'field_address_geofield',
        22 => 'field_address',
        24 => 'field_address_canon',
        25 => 'field_address_canon_accuracy',
        26 => 'field_msa',
        28 => 'field_meg',
        31 => 'field_county',
        32 => 'field_neighborhood',
        35 => 'field_pin',
        37 => 'field_on_public_land',
        39 => 'field_phone',
        42 => 'field_fax',
        44 => 'field_url',
      ),
      'footer' => array(
        46 => 'changed_date',
        49 => 'field_calc_owner_last_updated',
        52 => 'field_calc_operator_last_updated',
        53 => 'field_attachments',
        56 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_fs_segment' => 'left',
      'field_niche' => 'left',
      'field_is_hotel' => 'left',
      'field_is_casino' => 'left',
      'field_rooms' => 'left',
      'field_is_fs' => 'left',
      'field_brand' => 'left',
      'field_development_status' => 'left',
      'field_owner_of_record' => 'left',
      'field_notes' => 'left',
      'field_convention_center' => 'left',
      'group_contract' => 'left',
      'group_amenities' => 'left',
      'field_fs_dbe' => 'left',
      'group_airports' => 'left',
      'group_convention_center' => 'left',
      'group_casino' => 'left',
      'group_fairhotel' => 'left',
      'field_calc_convention_dist' => 'left',
      'group_restaurant' => 'left',
      'field_convention_center_attached' => 'left',
      'field_address_geofield' => 'right',
      'field_address' => 'right',
      'field_fh_meeting_room_count' => 'left',
      'field_address_canon' => 'right',
      'field_address_canon_accuracy' => 'right',
      'field_msa' => 'right',
      'field_fh_meeting_room_capacity' => 'left',
      'field_meg' => 'right',
      'field_fh_meeting_room_largest' => 'left',
      'field_fh_event_space' => 'left',
      'field_county' => 'right',
      'field_neighborhood' => 'right',
      'field_fh_ballroom_count' => 'left',
      'field_fh_ballroom_capacity' => 'left',
      'field_pin' => 'right',
      'field_fh_outdoor' => 'left',
      'field_on_public_land' => 'right',
      'field_fh_outdoor_area' => 'left',
      'field_phone' => 'right',
      'field_labor_activity_index' => 'left',
      'field_fh_parking' => 'left',
      'field_fax' => 'right',
      'field_fh_parking_capacity' => 'left',
      'field_url' => 'right',
      'field_boycott_url' => 'left',
      'changed_date' => 'footer',
      'field_fh_notes' => 'left',
      'field_csno_type' => 'left',
      'field_calc_owner_last_updated' => 'footer',
      'field_csno_slots_count' => 'left',
      'field_fh_nonunion' => 'left',
      'field_calc_operator_last_updated' => 'footer',
      'field_attachments' => 'footer',
      'field_csno_slots_date' => 'left',
      'field_fh_last_updated' => 'left',
      'unite_relationships' => 'footer',
      'field_csno_tables_count' => 'left',
      'field_fh_sources' => 'left',
      'field_csno_tables_date' => 'left',
      'field_csno_book' => 'left',
      'field_csno_horse' => 'left',
      'field_csno_regulator' => 'left',
      'field_csno_online' => 'left',
      'field_csno_source' => 'left',
      'field_local' => 'left',
      'field_union_status' => 'left',
      'field_union_other' => 'left',
      'field_hotel_contract_start_date' => 'left',
      'field_hotel_contract_end_date' => 'left',
      'field_hotel_contract_covered' => 'left',
      'field_rest_contract_start_date' => 'left',
      'field_rest_contract_end_date' => 'left',
      'field_rest_contract_covered' => 'left',
      'field_fs_concession_type' => 'left',
      'field_airport' => 'left',
      'field_calc_airport_dist' => 'left',
      'field_airport_alt' => 'left',
      'field_calc_airport_dist_alt' => 'left',
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
