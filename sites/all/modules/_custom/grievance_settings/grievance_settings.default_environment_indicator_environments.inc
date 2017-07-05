<?php
/**
 * @file
 * grievance_settings.default_environment_indicator_environments.inc
 */

/**
 * Implements hook_default_environment_indicator_environment().
 */
function grievance_settings_default_environment_indicator_environment() {
  $export = array();

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'grievance_development';
  $environment->name = 'Grievance Development';
  $environment->regexurl = 'clevernamehere';
  $environment->settings = array(
    'color' => '#0013db',
    'text_color' => '#ffffff',
    'weight' => '',
    'position' => 'top',
    'fixed' => 1,
  );
  $export['grievance_development'] = $environment;

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'grievance_stage';
  $environment->name = 'Grievance Staging';
  $environment->regexurl = 'stage';
  $environment->settings = array(
    'color' => '#5fff2e',
    'text_color' => '#ffffff',
    'weight' => '',
    'position' => 'top',
    'fixed' => 1,
  );
  $export['grievance_stage'] = $environment;

  return $export;
}
