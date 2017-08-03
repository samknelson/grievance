<?php
/**
 * @file
 * grievance_uhstandards.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function grievance_uhstandards_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'My First Contract Template Tag',
    'description' => '',
    'format' => 'full_html',
    'weight' => 0,
    'uuid' => '48991447-da8f-44e6-ae49-fd89bb2f5a9a',
    'hweight' => 0,
    'hdepth' => 0,
    'vocabulary_machine_name' => 'grievance_contract_template_tags',
    'field_grievance_external_id' => array(
      'und' => array(
        0 => array(
          'value' => 'first-contract-template-tag',
          'format' => NULL,
        ),
      ),
    ),
  );
  return $terms;
}
