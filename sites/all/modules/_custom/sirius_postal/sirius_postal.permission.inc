<?php

/***
 * Implements hook_permission
 */

function sirius_postal_permission() {
  return array(
    'sirius postal admin' => array(
      'title' => t('Sirius Postal Admin'), 
      'description' => t('Administer all aspects of the postal module, including configuring postal services'),
    ),
    'sirius postal send' => array(
      'title' => t('Sirius Postal Send'), 
      'description' => t('Permission to send a postal letter'),
    ),
  );
}

class Sirius_Postal_Access extends Sirius_Access {
  public $type = 'postal';
  public $name = 'Postal Mail';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'admin',
    );

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius dispatch admin')) {
      $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius dispatch staff')) {
      $is_staff = TRUE;
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    return FALSE;
  }
}
