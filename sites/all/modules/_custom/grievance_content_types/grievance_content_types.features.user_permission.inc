<?php

/**
 * @file
 * grievance_content_types.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function grievance_content_types_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create field_grievance_shops'.
  $permissions['create field_grievance_shops'] = array(
    'name' => 'create field_grievance_shops',
    'roles' => array(
      'admin' => 'admin',
      'systems admin' => 'systems admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_shops'.
  $permissions['edit field_grievance_shops'] = array(
    'name' => 'edit field_grievance_shops',
    'roles' => array(
      'admin' => 'admin',
      'systems admin' => 'systems admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_shops'.
  $permissions['edit own field_grievance_shops'] = array(
    'name' => 'edit own field_grievance_shops',
    'roles' => array(
      'admin' => 'admin',
      'systems admin' => 'systems admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_shops'.
  $permissions['view field_grievance_shops'] = array(
    'name' => 'view field_grievance_shops',
    'roles' => array(
      'admin' => 'admin',
      'systems admin' => 'systems admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_shops'.
  $permissions['view own field_grievance_shops'] = array(
    'name' => 'view own field_grievance_shops',
    'roles' => array(
      'admin' => 'admin',
      'systems admin' => 'systems admin',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
