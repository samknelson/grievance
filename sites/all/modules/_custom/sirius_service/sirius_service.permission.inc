<?php

/***
 * Implements hook_permission
 */

function sirius_service_permission() {
  return array(
    'sirius service admin' => array(
      'title' => t('Sirius Service Admin'), 
      'description' => t('Administer all aspects of the services module, including configuring as a proxy or remote server.'),
    ),
  );
}
