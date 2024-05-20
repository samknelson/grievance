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
  );
}
