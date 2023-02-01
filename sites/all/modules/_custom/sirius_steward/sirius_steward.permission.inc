<?php

/***
 * Implements hook_permission
 */

function sirius_steward_permission() {
  return array(
    // Workers who have this permission are stewards
    'sirius steward' => array(
      'title' => t('Sirius Steward Worker'), 
      'description' => t('Permission to operate as a steward.'),
    ),

    // Permission to check in or view checkin records
    'sirius steward checkin self' => array(
      'title' => t('Sirius Steward Worker Checkin Self'), 
      'description' => t('Permission to check yourself in or out of a facility'),
    ),
    'sirius steward checkin other' => array(
      'title' => t('Sirius Steward Checkin Other'), 
      'description' => t('Permission to check any steward in or out of a facility'),
    ),
    'sirius steward checkin view map' => array(
      'title' => t('Sirius Steward Checkin View Map'), 
      'description' => t('Permission to view the the map of a steward\'s checkin location'),
    ),

    // Permission to edit or view schedules
    'sirius steward sched worker' => array(
      'title' => t('Sirius Steward Sched Worker'),
      'description' => t('Permission to operate as a scheduling steward, including the ability to set schedules for your employer'),
    ),
    'sirius steward sched employer' => array(
      'title' => t('Sirius Steward Sched Employer'),
      'description' => t('Permission to operate as an employer with access to view stewards schedules'),
    ),

    // Permission to administer stewards
    'sirius steward admin' => array(
      'title' => t('Sirius Steward Admin'), 
      'description' => t('Permission to administer stewards and steward settings.'),
    ),
  );
}
