<?php

/***
 * Implements hook_permission
 */

function sirius_office_permission() {
  return array(
    'sirius office iprestrict admin' => array(
      'title' => t('Sirius Office IPRestrict Admin'), 
      'description' => t('Permission to administer the IPRestrict policy'),
    ),
    'sirius office iprestrict remote' => array(
      'title' => t('Sirius Office IPRestrict Remote'), 
      'description' => t('Permission to override the IPRestriction policy and sign in remotely, regardless of other settings.'),
    ),
  );
}
