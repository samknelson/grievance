<?php
/**
 * @file
 * sirius_document_retention.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sirius_document_retention_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sirius_document_retention_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function sirius_document_retention_node_info() {
  $items = array(
    'sirius_document_retention_log' => array(
      'name' => t('Sirius Document Retention Log'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
