<?php
/**
 * @file
 * grievance_settings.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function grievance_settings_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'table_trash_decorations';
  $strongarm->value = array(
    1 => array(
      'decoration-params' => array(
        'search-box' => 1,
        'column-reorder' => 1,
        'export-buttons' => 1,
        'retrieve-data' => 0,
        'pager-style' => '',
        'page-height' => '',
        'dont-sort-columns' => '',
        'x-scroll' => '',
        'fixed-left-columns' => '',
        'fixed-header' => 0,
        'responsive' => array(
          'responsive-expand-col' => '',
          'responsive-collapse-cols-phone' => '',
          'responsive-collapse-cols-tablet' => '',
        ),
      ),
      'pages-and-selector' => array(
        'include-pages' => 'grievance/admin/shops
grievance/admin/shops/companies
grievance/admin/shops/contacts
admin/people
grievances/mine
grievances/mine/mine-all
grievances/mine/reminder-30
grievances/mine/reminder-60
grievances/mine/reminder-90
grievances/mine/reminder-no-first-call',
        'exclude-pages' => 'admin/reports/status
admin/modules*',
        'table-selector' => '',
      ),
    ),
    2 => array(
      'decoration-params' => array(
        'search-box' => 0,
        'column-reorder' => 1,
        'export-buttons' => 1,
        'retrieve-data' => 0,
        'pager-style' => '',
        'page-height' => '',
        'dont-sort-columns' => '',
        'x-scroll' => '',
        'fixed-left-columns' => '',
        'fixed-header' => 0,
        'responsive' => array(
          'responsive-expand-col' => '',
          'responsive-collapse-cols-phone' => '',
          'responsive-collapse-cols-tablet' => '',
        ),
      ),
      'pages-and-selector' => array(
        'include-pages' => 'grievances/all
grievance/report/uh226-report-summary
',
        'exclude-pages' => 'admin/reports/status
admin/modules*',
        'table-selector' => '',
      ),
    ),
  );
  $export['table_trash_decorations'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'table_trash_global_settings';
  $strongarm->value = array(
    'responsive' => array(
      'responsive-breakpoint-phone' => '',
      'responsive-breakpoint-tablet' => '',
    ),
    'use-bug-fixed-library' => 1,
    'use-datatables-css' => 1,
    'use-module-css' => 1,
  );
  $export['table_trash_global_settings'] = $strongarm;

  return $export;
}
