<?php

/**
 * @file
 * sirius_service_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sirius_service_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
