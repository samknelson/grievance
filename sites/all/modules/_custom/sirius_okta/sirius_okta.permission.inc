<?php

/***
 * Implements hook_permission
 */

function sirius_okta_permission() {
  return array(
    'sirius okta admin' => array(
      'title' => t('Sirius Okta Admin'), 
      'description' => t('Permission to configure Okta functionality.'),
    ),
  );
}
