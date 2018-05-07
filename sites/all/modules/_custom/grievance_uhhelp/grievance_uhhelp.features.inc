<?php
/**
 * @file
 * grievance_uhhelp.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function grievance_uhhelp_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
