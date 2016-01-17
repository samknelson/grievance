<?php
/**
 * @file
 * grievance_help_text.default_custom_help_text.inc
 */

/**
 * Implements hook_default_custom_help_text().
 */
function grievance_help_text_default_custom_help_text() {
  $export = array();

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '1';
  $custom_help_text->path = 'admin/structure/custom_help_text';
  $custom_help_text->help = 'Use this page to manage help text across the site.';
  $custom_help_text->weight = 0;
  $export['1'] = $custom_help_text;

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '2';
  $custom_help_text->path = 'grievance/list';
  $custom_help_text->help = 'The list below shows the most recent grievances filed.  Use the form below to search for a specific grievance.  Or, <a href="/node/add/grievance"><strong>click here to create and file a new grievance.</strong></a>';
  $custom_help_text->weight = 0;
  $export['2'] = $custom_help_text;

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '3';
  $custom_help_text->path = 'grievance/report/bytype';
  $custom_help_text->help = 'The report below shows the number of grievances filed by type and status.  Use the links across the top to see a more detailed breakdown by either status or type.';
  $custom_help_text->weight = 0;
  $export['3'] = $custom_help_text;

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '4';
  $custom_help_text->path = 'node/*/statuslog';
  $custom_help_text->help = 'This log shows contains an entry for each update made to the grievance, as well as manual log entries and notifications.';
  $custom_help_text->weight = 0;
  $export['4'] = $custom_help_text;

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '5';
  $custom_help_text->path = 'grievance/admin/shops';
  $custom_help_text->help = 'The list below contains all the shops configured in the system.  In order to have a grievance filed against it, a shop must be configured.  Shops should have a contact entered, to receive notifications; they should also have their contracts uploaded, so that stewards can easily identify violations.';
  $custom_help_text->weight = 0;
  $export['5'] = $custom_help_text;

  $custom_help_text = new stdClass();
  $custom_help_text->disabled = FALSE; /* Edit this to true to make a default custom_help_text disabled initially */
  $custom_help_text->api_version = 1;
  $custom_help_text->hid = '6';
  $custom_help_text->path = 'admin/config/system/flood-control';
  $custom_help_text->help = '"Flood Control" is used to prevent users from breaking into the system by repeatedly trying to guess passwords.  Users and internet addresses which have too many incorrect passwords in a short period of time (a "flood") are blocked.

Use the settings below to control when a "flood" is occurring.';
  $custom_help_text->weight = 0;
  $export['6'] = $custom_help_text;

  return $export;
}
