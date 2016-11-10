<?php
/**
 * @file
 * grievance_roles_and_permissions.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function grievance_roles_and_permissions_user_default_roles() {
  $roles = array();

  // Exported role: grievance admin.
  $roles['grievance admin'] = array(
    'name' => 'grievance admin',
    'weight' => 6,
  );

  // Exported role: grievance staff.
  $roles['grievance staff'] = array(
    'name' => 'grievance staff',
    'weight' => 5,
  );

  // Exported role: grievance user.
  $roles['grievance user'] = array(
    'name' => 'grievance user',
    'weight' => 3,
  );

  // Exported role: grievance view.
  $roles['grievance view'] = array(
    'name' => 'grievance view',
    'weight' => 4,
  );

  return $roles;
}
