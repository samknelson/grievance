<?php

/***
 * Implements hook_permission
 */

function sirius_teamsters631_permission() {
  return array(
    'sirius teamsters631 admin' => array(
      'title' => t('Sirius Teamsters631 Admin'), 
      'description' => t('Permission to administer the Teamsters 631 module'),
    ),
  );
}
