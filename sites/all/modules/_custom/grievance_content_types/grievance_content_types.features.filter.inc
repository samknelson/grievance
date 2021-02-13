<?php

/**
 * @file
 * grievance_content_types.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function grievance_content_types_filter_default_formats() {
  $formats = array();

  // Exported format: Sirius Form Editor.
  $formats['sirius_form_editor'] = array(
    'format' => 'sirius_form_editor',
    'name' => 'Sirius Form Editor',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(),
  );

  return $formats;
}
