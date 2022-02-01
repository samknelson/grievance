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
    'ds_flag_bookmarks' => array(
      'weight' => '36',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'unite_relationships' => array(
      'weight' => '51',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'changed_date' => array(
      'weight' => '41',
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
  $ds_layout->id = 'node|property|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'property';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_fh_meeting_room_count',
        1 => 'field_fh_meeting_room_capacity',
        2 => 'field_fh_event_space',
        3 => 'field_fh_ballroom_count',
        4 => 'field_fh_ballroom_capacity',
        5 => 'field_is_casino',
        6 => 'field_csno_slots_count',
        7 => 'field_csno_slots_date',
        8 => 'field_csno_tables_count',
        9 => 'field_csno_tables_date',
        10 => 'field_csno_source',
        11 => 'field_csno_regulator',
        12 => 'field_csno_book',
        13 => 'field_csno_horse',
        14 => 'field_csno_online',
      ),
      'left' => array(
        15 => 'field_union_status',
        16 => 'field_rooms',
        17 => 'field_brand',
        18 => 'field_development_status',
        19 => 'field_address',
        20 => 'field_address_canon',
        21 => 'field_msa',
        22 => 'field_meg',
        23 => 'field_neighborhood',
        24 => 'field_owner_of_record',
        25 => 'field_pin',
        26 => 'field_county',
        27 => 'field_on_public_land',
        28 => 'field_local',
        29 => 'field_airport',
        30 => 'field_calc_airport_dist',
        31 => 'field_airport_alt',
        32 => 'field_calc_airport_dist_alt',
        33 => 'field_labor_activity_index',
        34 => 'field_boycott_url',
        35 => 'field_notes',
        36 => 'ds_flag_bookmarks',
      ),
      'right' => array(
        37 => 'field_address_geofield',
        38 => 'field_url',
        39 => 'field_phone',
        40 => 'field_fax',
      ),
      'footer' => array(
        41 => 'changed_date',
        42 => 'field_calc_owner_last_updated',
        43 => 'field_calc_operator_last_updated',
        44 => 'field_hotel_contract_start_date',
        45 => 'field_hotel_contract_end_date',
        46 => 'field_hotel_contract_covered',
        47 => 'field_rest_contract_start_date',
        48 => 'field_rest_contract_end_date',
        49 => 'field_rest_contract_covered',
        50 => 'field_attachments',
        51 => 'unite_relationships',
      ),
    ),
    'fields' => array(
      'field_fh_meeting_room_count' => 'header',
      'field_fh_meeting_room_capacity' => 'header',
      'field_fh_event_space' => 'header',
      'field_fh_ballroom_count' => 'header',
      'field_fh_ballroom_capacity' => 'header',
      'field_is_casino' => 'header',
      'field_csno_slots_count' => 'header',
      'field_csno_slots_date' => 'header',
      'field_csno_tables_count' => 'header',
      'field_csno_tables_date' => 'header',
      'field_csno_source' => 'header',
      'field_csno_regulator' => 'header',
      'field_csno_book' => 'header',
      'field_csno_horse' => 'header',
      'field_csno_online' => 'header',
      'field_union_status' => 'left',
      'field_rooms' => 'left',
      'field_brand' => 'left',
      'field_development_status' => 'left',
      'field_address' => 'left',
      'field_address_canon' => 'left',
      'field_msa' => 'left',
      'field_meg' => 'left',
      'field_neighborhood' => 'left',
      'field_owner_of_record' => 'left',
      'field_pin' => 'left',
      'field_county' => 'left',
      'field_on_public_land' => 'left',
      'field_local' => 'left',
      'field_airport' => 'left',
      'field_calc_airport_dist' => 'left',
      'field_airport_alt' => 'left',
      'field_calc_airport_dist_alt' => 'left',
      'field_labor_activity_index' => 'left',
      'field_boycott_url' => 'left',
      'field_notes' => 'left',
      'ds_flag_bookmarks' => 'left',
      'field_address_geofield' => 'right',
      'field_url' => 'right',
      'field_phone' => 'right',
      'field_fax' => 'right',
      'changed_date' => 'footer',
      'field_calc_owner_last_updated' => 'footer',
      'field_calc_operator_last_updated' => 'footer',
      'field_hotel_contract_start_date' => 'footer',
      'field_hotel_contract_end_date' => 'footer',
      'field_hotel_contract_covered' => 'footer',
      'field_rest_contract_start_date' => 'footer',
      'field_rest_contract_end_date' => 'footer',
      'field_rest_contract_covered' => 'footer',
      'field_attachments' => 'footer',
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
  $export['node|property|default'] = $ds_layout;

  return $export;
}
