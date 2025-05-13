<?php

/***
 * Implements hook_permission
 */

function sirius_worker_tos_permission() {
  return array(
    'sirius worker tos worker' => array(
      'title' => t('Sirius Worker TOS Worker'), 
      'description' => t('Use the time-off sick system as a worker'),
    ),
    'sirius worker tos employer' => array(
      'title' => t('Sirius Worker TOS Employer'), 
      'description' => t('Use the time-off sick system as an employer'),
    ),
    'sirius worker tos admin' => array(
      'title' => t('Sirius Worker TOS Admin'), 
      'description' => t('Administer the time-off sick system'),
    ),
  );
}
