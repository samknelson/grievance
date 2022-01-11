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
      'weight' => '33',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'unite_relationships' => array(
      'weight' => '46',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'changed_date' => array(
      'weight' => '38',
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
        23 => 'field_airport',
        24 => 'field_neighborhood',
        25 => 'field_owner_of_record',
        26 => 'field_pin',
        27 => 'field_county',
        28 => 'field_on_public_land',
        29 => 'field_local',
        30 => 'field_labor_activity_index',
        31 => 'field_boycott_url',
        32 => 'field_notes',
        33 => 'ds_flag_bookmarks',
      ),
      'right' => array(
        34 => 'field_address_geofield',
        35 => 'field_url',
        36 => 'field_phone',
        37 => 'field_fax',
      ),
      'footer' => array(
        38 => 'changed_date',
        39 => 'field_hotel_contract_start_date',
        40 => 'field_hotel_contract_end_date',
        41 => 'field_hotel_contract_covered',
        42 => 'field_rest_contract_start_date',
        43 => 'field_rest_contract_end_date',
        44 => 'field_rest_contract_covered',
        45 => 'field_attachments',
        46 => 'unite_relationships',
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
      'field_airport' => 'left',
      'field_neighborhood' => 'left',
      'field_owner_of_record' => 'left',
      'field_pin' => 'left',
      'field_county' => 'left',
      'field_on_public_land' => 'left',
      'field_local' => 'left',
      'field_labor_activity_index' => 'left',
      'field_boycott_url' => 'left',
      'field_notes' => 'left',
      'ds_flag_bookmarks' => 'left',
      'field_address_geofield' => 'right',
      'field_url' => 'right',
      'field_phone' => 'right',
      'field_fax' => 'right',
      'changed_date' => 'footer',
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
