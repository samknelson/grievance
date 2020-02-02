<?php

/**
 * @file
 * grievance_content_types.default_field_validation_rules.inc
 */

/**
 * Implements hook_default_field_validation_rule().
 */
function grievance_content_types_default_field_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Sirius Unique ID';
  $rule->name = 'sirius_worker_id_unique';
  $rule->field_name = 'field_sirius_id';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'sirius_worker';
  $rule->validator = 'field_validation_unique_validator';
  $rule->settings = array(
    'data' => 'bundle',
    'per_user' => 0,
    'ignore_empty' => 1,
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
      12 => 0,
      16 => 0,
      20 => 0,
      23 => 0,
      18 => 0,
      24 => 0,
      21 => 0,
      25 => 0,
      14 => 0,
      22 => 0,
      15 => 0,
      17 => 0,
      19 => 0,
      26 => 0,
      27 => 0,
      29 => 0,
    ),
    'errors' => 0,
    'condition' => 0,
    'condition_wrapper' => array(
      'condition_field' => '',
      'condition_operator' => 'equals',
      'condition_value' => '',
    ),
  );
  $rule->error_message = 'ID must be unique';
  $export['sirius_worker_id_unique'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Sirius Worker Unique ID2';
  $rule->name = 'sirius_worker_unique_id2';
  $rule->field_name = 'field_sirius_id2';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'sirius_worker';
  $rule->validator = 'field_validation_unique_validator';
  $rule->settings = array(
    'data' => 'global',
    'per_user' => 0,
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
      12 => 0,
      16 => 0,
      20 => 0,
      23 => 0,
      18 => 0,
      24 => 0,
      21 => 0,
      25 => 0,
      14 => 0,
      22 => 0,
      15 => 0,
      17 => 0,
      19 => 0,
    ),
    'errors' => 0,
    'condition' => 0,
    'condition_wrapper' => array(
      'condition_field' => '',
      'condition_operator' => 'equals',
      'condition_value' => '',
    ),
  );
  $rule->error_message = 'Value must be unique';
  $export['sirius_worker_unique_id2'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Sirius Worker Unique SSN';
  $rule->name = 'sirius_worker_unique_ssn';
  $rule->field_name = 'field_sirius_ssn';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'sirius_worker';
  $rule->validator = 'field_validation_unique_validator';
  $rule->settings = array(
    'data' => 'bundle',
    'per_user' => 0,
    'ignore_empty' => 1,
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
      12 => 0,
      16 => 0,
      20 => 0,
      23 => 0,
      18 => 0,
      24 => 0,
      21 => 0,
      25 => 0,
      14 => 0,
      22 => 0,
      15 => 0,
      17 => 0,
      19 => 0,
      26 => 0,
      27 => 0,
      29 => 0,
    ),
    'errors' => 0,
    'condition' => 0,
    'condition_wrapper' => array(
      'condition_field' => 'sirius_worker_unique_ssn ',
      'condition_operator' => 'not_equals',
      'condition_value' => '',
    ),
  );
  $rule->error_message = 'SSN must be unique';
  $export['sirius_worker_unique_ssn'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'SSN';
  $rule->name = 'ssn';
  $rule->field_name = 'field_sirius_ssn';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'sirius_worker';
  $rule->validator = 'field_validation_pattern_validator';
  $rule->settings = array(
    'pattern' => '999-99-9999',
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
      12 => 0,
      16 => 0,
      18 => 0,
      14 => 0,
      15 => 0,
      17 => 0,
    ),
    'errors' => 0,
    'condition' => 0,
    'condition_wrapper' => array(
      'condition_field' => '',
      'condition_operator' => 'equals',
      'condition_value' => '',
    ),
  );
  $rule->error_message = 'Please enter a valid SSN';
  $export['ssn'] = $rule;

  return $export;
}
