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
  if ($module == "strongarm" && $api == "strongarm") {
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
    'grievance_chapter' => array(
      'name' => t('Grievance Chapter'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Chapter Name'),
      'help' => '',
    ),
    'grievance_company' => array(
      'name' => t('Grievance Company'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Company Name'),
      'help' => '',
    ),
    'grievance_contract' => array(
      'name' => t('Grievance Contract'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_clause' => array(
      'name' => t('Grievance Contract Section'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Brief Statement'),
      'help' => '',
    ),
    'grievance_contract_rule' => array(
      'name' => t('Grievance Contract Rule'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_section' => array(
      'name' => t('Grievance Contract Article'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_template' => array(
      'name' => t('Grievance Contract Template'),
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
    'grievance_holiday' => array(
      'name' => t('Grievance Holiday'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_letter_template' => array(
      'name' => t('Grievance Letter Template'),
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
    'grievance_news' => array(
      'name' => t('Grievance News'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_settlement' => array(
      'name' => t('Grievance Settlement'),
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
    'grievance_shop_contact' => array(
      'name' => t('Grievance Shop Contact'),
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
