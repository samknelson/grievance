<?php
/**
 * @file
 * grievance_content_types.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function grievance_content_types_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function grievance_content_types_node_info() {
  $items = array(
    'grievance' => array(
      'name' => t('Grievance'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_attachment' => array(
      'name' => t('Grievance Attachment'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_basic_page' => array(
      'name' => t('Grievance Basic Page'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_field_overrides' => array(
      'name' => t('Grievance Field Overrides'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_log' => array(
      'name' => t('Grievance Log'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_shop' => array(
      'name' => t('Grievance Shop'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Shop Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
