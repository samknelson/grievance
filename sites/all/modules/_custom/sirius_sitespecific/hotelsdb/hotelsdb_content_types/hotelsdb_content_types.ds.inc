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
        0 => 'field_is_hotel',
        1 => 'field_fs_segment',
        2 => 'field_niche',
        3 => 'field_is_casino',
        4 => 'field_rooms',
        5 => 'field_is_fs',
        6 => 'field_brand',
        7 => 'field_development_status',
        8 => 'field_hotel_contract_covered',
        9 => 'field_union_status',
        10 => 'field_owner_of_record',
        11 => 'field_notes',
        12 => 'field_convention_center',
        13 => 'group_amenities',
        14 => 'field_fs_dbe',
        15 => 'group_airports',
        16 => 'group_convention_center',
        17 => 'group_casino',
        18 => 'group_fairhotel',
        19 => 'field_calc_convention_dist',
        20 => 'group_restaurant',
        22 => 'field_convention_center_attached',
        25 => 'field_fh_meeting_room_count',
        28 => 'field_fh_meeting_room_capacity',
        30 => 'field_fh_meeting_room_largest',
        32 => 'field_fh_event_space',
        33 => 'field_fh_ballroom_count',
        35 => 'field_fh_ballroom_capacity',
        37 => 'field_fh_outdoor',
        40 => 'field_fh_outdoor_area',
        41 => 'field_labor_activity_index',
        43 => 'field_fh_parking',
        45 => 'field_fh_parking_capacity',
        47 => 'field_boycott_url',
        49 => 'field_fh_notes',
        50 => 'field_csno_type',
        52 => 'field_fh_nonunion',
        53 => 'field_csno_slots_count',
        54 => 'field_csno_slots_date',
        56 => 'field_fh_last_updated',
        58 => 'field_fh_sources',
        59 => 'field_csno_tables_count',
        60 => 'field_csno_tables_date',
        61 => 'field_csno_book',
        62 => 'field_csno_book_txt',
        63 => 'field_csno_horse',
        64 => 'field_csno_online',
        65 => 'field_csno_online_txt',
        66 => 'field_csno_igame',
        67 => 'field_csno_igame_txt',
        68 => 'field_csno_txt1',
        69 => 'field_csno_txt2',
        70 => 'field_csno_txt3',
        71 => 'field_csno_txt4',
        72 => 'field_fs_concession_type',
        73 => 'field_airport',
        74 => 'field_calc_airport_dist',
        75 => 'field_airport_alt',
        76 => 'field_calc_airport_dist_alt',
        77 => 'field_csno_regulator',
        78 => 'field_csno_source',
      ),
      'right' => array(
        21 => 'field_address_geofield',
        23 => 'field_address',
        24 => 'field_address_canon',
        26 => 'field_address_canon_accuracy',
        27 => 'field_msa',
        29 => 'field_meg',
        31 => 'field_county',
        34 => 'field_neighborhood',
        36 => 'field_pin',
        38 => 'field_on_public_land',
        39 => 'field_phone',
        42 => 'field_fax',
        44 => 'field_url',
      ),
      'footer' => array(
        46 => 'changed_date',
        48 => 'field_calc_owner_last_updated',
        51 => 'field_calc_operator_last_updated',
        55 => 'field_attachments',
        57 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_is_hotel' => 'left',
      'field_fs_segment' => 'left',
      'field_niche' => 'left',
      'field_is_casino' => 'left',
      'field_rooms' => 'left',
      'field_is_fs' => 'left',
      'field_brand' => 'left',
      'field_development_status' => 'left',
      'field_hotel_contract_covered' => 'left',
      'field_union_status' => 'left',
      'field_owner_of_record' => 'left',
      'field_notes' => 'left',
      'field_convention_center' => 'left',
      'group_amenities' => 'left',
      'field_fs_dbe' => 'left',
      'group_airports' => 'left',
      'group_convention_center' => 'left',
      'group_casino' => 'left',
      'group_fairhotel' => 'left',
      'field_calc_convention_dist' => 'left',
      'group_restaurant' => 'left',
      'field_address_geofield' => 'right',
      'field_convention_center_attached' => 'left',
      'field_address' => 'right',
      'field_address_canon' => 'right',
      'field_fh_meeting_room_count' => 'left',
      'field_address_canon_accuracy' => 'right',
      'field_msa' => 'right',
      'field_fh_meeting_room_capacity' => 'left',
      'field_meg' => 'right',
      'field_fh_meeting_room_largest' => 'left',
      'field_county' => 'right',
      'field_fh_event_space' => 'left',
      'field_fh_ballroom_count' => 'left',
      'field_neighborhood' => 'right',
      'field_fh_ballroom_capacity' => 'left',
      'field_pin' => 'right',
      'field_fh_outdoor' => 'left',
      'field_on_public_land' => 'right',
      'field_phone' => 'right',
      'field_fh_outdoor_area' => 'left',
      'field_labor_activity_index' => 'left',
      'field_fax' => 'right',
      'field_fh_parking' => 'left',
      'field_url' => 'right',
      'field_fh_parking_capacity' => 'left',
      'changed_date' => 'footer',
      'field_boycott_url' => 'left',
      'field_calc_owner_last_updated' => 'footer',
      'field_fh_notes' => 'left',
      'field_csno_type' => 'left',
      'field_calc_operator_last_updated' => 'footer',
      'field_fh_nonunion' => 'left',
      'field_csno_slots_count' => 'left',
      'field_csno_slots_date' => 'left',
      'field_attachments' => 'footer',
      'field_fh_last_updated' => 'left',
      'unite_relationships' => 'footer',
      'field_fh_sources' => 'left',
      'field_csno_tables_count' => 'left',
      'field_csno_tables_date' => 'left',
      'field_csno_book' => 'left',
      'field_csno_book_txt' => 'left',
      'field_csno_horse' => 'left',
      'field_csno_online' => 'left',
      'field_csno_online_txt' => 'left',
      'field_csno_igame' => 'left',
      'field_csno_igame_txt' => 'left',
      'field_csno_txt1' => 'left',
      'field_csno_txt2' => 'left',
      'field_csno_txt3' => 'left',
      'field_csno_txt4' => 'left',
      'field_fs_concession_type' => 'left',
      'field_airport' => 'left',
      'field_calc_airport_dist' => 'left',
      'field_airport_alt' => 'left',
      'field_calc_airport_dist_alt' => 'left',
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
