<?php

/***
 * Implements hook_permission
 */

function sirius_worker_rating_permission() {
  return array(
    'sirius worker rating worker' => array(
      'title' => t('Sirius Worker rating Worker'), 
      'description' => t('Use the time-off sick system as a worker'),
    ),
    'sirius worker rating admin' => array(
      'title' => t('Sirius Worker rating Admin'), 
      'description' => t('Administer the time-off sick system'),
    ),
  );
}
