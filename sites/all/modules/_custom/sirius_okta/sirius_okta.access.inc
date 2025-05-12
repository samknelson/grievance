<?php

class Sirius_Okta_Access extends Sirius_Access {
  public $type = 'okta';
  public $name = 'Okta';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $user_full = user_load($user->uid);

    $actions = array(
      // No parameters
      'staff',
      'user_sync',
    );

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius okta admin')) { 
      $is_admin = $is_staff = $is_worker = TRUE;
    }

    // Admins can do pretty much anything.
    if ($is_admin) { return TRUE; }

    return FALSE;
  }
}
