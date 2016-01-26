<?php
/**
 * @file
 * grievance_pages.features.inc
 */

/**
 * Implements hook_node_info().
 */
function grievance_pages_node_info() {
  $items = array(
    'grievance_basic_page' => array(
      'name' => t('Grievance Basic Page'),
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
