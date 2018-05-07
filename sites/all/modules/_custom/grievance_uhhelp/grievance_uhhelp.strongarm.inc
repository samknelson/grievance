<?php
/**
 * @file
 * grievance_uhhelp.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function grievance_uhhelp_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_basic_page';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_basic_page'] = $strongarm;

  return $export;
}
