<?php
/**
 * @file
 * grievance_help_text.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function grievance_help_text_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "custom_help_text" && $api == "default_custom_help_text") {
    return array("version" => "1");
  }
}
