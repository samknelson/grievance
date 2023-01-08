<?php

/***
 * Implements hook_permission
 */

function sirius_steward_permission() {
  return array(
    'sirius steward' => array(
      'title' => t('Sirius Steward'), 
      'description' => t('Permission to operate as a steward.'),
    ),
    'sirius steward facility checkin self' => array(
      'title' => t('Sirius Steward Facility Checkin Self'), 
      'description' => t('Permission to check yourself in or out of a facility'),
    ),
    'sirius steward facility checkin other' => array(
      'title' => t('Sirius Steward Facility Checkin Other'), 
      'description' => t('Permission to check any steward in or out of a facility'),
    ),
    'sirius steward facility checkin view current' => array(
      'title' => t('Sirius Steward Facility Checkin View Current'), 
      'description' => t('Permission to view the current checked-in stewards at a facility'),
    ),
    'sirius steward facility checkin view history' => array(
      'title' => t('Sirius Steward Facility Checkin View History'), 
      'description' => t('Permission to view the the check-in history of stewards at a facility'),
    ),
    'sirius steward admin' => array(
      'title' => t('Sirius Steward Admin'), 
      'description' => t('Permission to administer stewards and steward settings.'),
    ),
  );
}
