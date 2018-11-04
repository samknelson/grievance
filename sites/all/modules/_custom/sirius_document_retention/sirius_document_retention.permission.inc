<?php

/***
 * Implements hook_permission
 */

function sirius_document_retention_permission() {
  return array(
    'sirius document retention admin' => array(
      'title' => t('sirius document retention admin'), 
      'description' => t('Administer document retention policies'),
    ),
  );
}
