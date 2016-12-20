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

  // Exported permission: 'create field_grievance_rep_lead'.
  $permissions['create field_grievance_rep_lead'] = array(
    'name' => 'create field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_grievance_rep_organizer'.
  $permissions['create field_grievance_rep_organizer'] = array(
    'name' => 'create field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_grievance_shops'.
  $permissions['create field_grievance_shops'] = array(
    'name' => 'create field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_rep_lead'.
  $permissions['edit field_grievance_rep_lead'] = array(
    'name' => 'edit field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_rep_organizer'.
  $permissions['edit field_grievance_rep_organizer'] = array(
    'name' => 'edit field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_shops'.
  $permissions['edit field_grievance_shops'] = array(
    'name' => 'edit field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_rep_lead'.
  $permissions['edit own field_grievance_rep_lead'] = array(
    'name' => 'edit own field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_rep_organizer'.
  $permissions['edit own field_grievance_rep_organizer'] = array(
    'name' => 'edit own field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_shops'.
  $permissions['edit own field_grievance_shops'] = array(
    'name' => 'edit own field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_rep_lead'.
  $permissions['view field_grievance_rep_lead'] = array(
    'name' => 'view field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_rep_organizer'.
  $permissions['view field_grievance_rep_organizer'] = array(
    'name' => 'view field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_shops'.
  $permissions['view field_grievance_shops'] = array(
    'name' => 'view field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_rep_lead'.
  $permissions['view own field_grievance_rep_lead'] = array(
    'name' => 'view own field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_rep_organizer'.
  $permissions['view own field_grievance_rep_organizer'] = array(
    'name' => 'view own field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_shops'.
  $permissions['view own field_grievance_shops'] = array(
    'name' => 'view own field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
