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
      'weight' => '2',
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
        5 => 'field_rooms',
        6 => 'union_information',
        7 => 'field_is_casino',
        8 => 'field_is_fs',
        9 => 'field_brand',
        10 => 'field_development_status',
        11 => 'field_owner_of_record',
        12 => 'field_convention_center',
        13 => 'field_notes',
        14 => 'group_union',
        15 => 'field_fs_dbe',
        16 => 'field_calc_convention_dist',
        17 => 'field_convention_center_attached',
        18 => 'field_fh_meeting_room_count',
        22 => 'field_fh_meeting_room_capacity',
        23 => 'group_amenities',
        24 => 'field_fh_meeting_room_largest',
        26 => 'field_fh_event_space',
        28 => 'field_fh_ballroom_count',
        30 => 'field_fh_ballroom_capacity',
        32 => 'field_fh_outdoor',
        34 => 'field_fh_outdoor_area',
        36 => 'field_labor_activity_index',
        38 => 'field_fh_parking',
        39 => 'field_fh_parking_capacity',
        41 => 'field_boycott_url',
        43 => 'field_csno_type',
        44 => 'field_fh_notes',
        46 => 'field_fh_nonunion',
        47 => 'group_airports',
        49 => 'field_csno_slots_count',
        50 => 'field_csno_slots_date',
        52 => 'field_fh_last_updated',
        54 => 'field_csno_tables_count',
        55 => 'field_fh_sources',
        57 => 'field_csno_tables_date',
        59 => 'field_csno_book',
        60 => 'field_csno_book_txt',
        61 => 'field_csno_horse',
        62 => 'field_csno_online_txt',
        63 => 'field_csno_online',
        64 => 'field_csno_igame',
        65 => 'field_csno_igame_txt',
        66 => 'group_convention_center',
        67 => 'field_csno_txt1',
        68 => 'field_csno_txt2',
        69 => 'field_csno_txt3',
        70 => 'field_csno_txt4',
        71 => 'group_casino',
        72 => 'group_fairhotel',
        73 => 'field_fs_concession_type',
        74 => 'field_airport',
        75 => 'field_calc_airport_dist',
        76 => 'field_airport_alt',
        77 => 'field_calc_airport_dist_alt',
        78 => 'group_restaurant',
        79 => 'field_csno_regulator',
        80 => 'field_csno_source',
      ),
      'right' => array(
        19 => 'field_address_geofield',
        20 => 'field_address',
        21 => 'field_address_canon',
        25 => 'field_address_canon_accuracy',
        27 => 'field_msa',
        29 => 'field_meg',
        31 => 'field_county',
        33 => 'field_neighborhood',
        35 => 'field_pin',
        37 => 'field_on_public_land',
        40 => 'field_phone',
        42 => 'field_fax',
        45 => 'field_url',
      ),
      'footer' => array(
        48 => 'changed_date',
        51 => 'field_calc_owner_last_updated',
        53 => 'field_calc_operator_last_updated',
        56 => 'field_attachments',
        58 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_niche' => 'left',
      'field_is_hotel' => 'left',
      'field_fs_segment' => 'left',
      'field_union_status' => 'left',
      'field_hotel_contract_covered' => 'left',
      'field_rooms' => 'left',
      'union_information' => 'left',
      'field_is_casino' => 'left',
      'field_is_fs' => 'left',
      'field_brand' => 'left',
      'field_development_status' => 'left',
      'field_owner_of_record' => 'left',
      'field_convention_center' => 'left',
      'field_notes' => 'left',
      'group_union' => 'left',
      'field_fs_dbe' => 'left',
      'field_calc_convention_dist' => 'left',
      'field_convention_center_attached' => 'left',
      'field_fh_meeting_room_count' => 'left',
      'field_address_geofield' => 'right',
      'field_address' => 'right',
      'field_address_canon' => 'right',
      'field_fh_meeting_room_capacity' => 'left',
      'group_amenities' => 'left',
      'field_fh_meeting_room_largest' => 'left',
      'field_address_canon_accuracy' => 'right',
      'field_fh_event_space' => 'left',
      'field_msa' => 'right',
      'field_fh_ballroom_count' => 'left',
      'field_meg' => 'right',
      'field_fh_ballroom_capacity' => 'left',
      'field_county' => 'right',
      'field_fh_outdoor' => 'left',
      'field_neighborhood' => 'right',
      'field_fh_outdoor_area' => 'left',
      'field_pin' => 'right',
      'field_labor_activity_index' => 'left',
      'field_on_public_land' => 'right',
      'field_fh_parking' => 'left',
      'field_fh_parking_capacity' => 'left',
      'field_phone' => 'right',
      'field_boycott_url' => 'left',
      'field_fax' => 'right',
      'field_csno_type' => 'left',
      'field_fh_notes' => 'left',
      'field_url' => 'right',
      'field_fh_nonunion' => 'left',
      'group_airports' => 'left',
      'changed_date' => 'footer',
      'field_csno_slots_count' => 'left',
      'field_csno_slots_date' => 'left',
      'field_calc_owner_last_updated' => 'footer',
      'field_fh_last_updated' => 'left',
      'field_calc_operator_last_updated' => 'footer',
      'field_csno_tables_count' => 'left',
      'field_fh_sources' => 'left',
      'field_attachments' => 'footer',
      'field_csno_tables_date' => 'left',
      'unite_relationships' => 'footer',
      'field_csno_book' => 'left',
      'field_csno_book_txt' => 'left',
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
