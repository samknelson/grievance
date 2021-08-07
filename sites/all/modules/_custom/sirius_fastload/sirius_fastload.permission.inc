<?php

/***
 * Implements hook_permission
 */

function sirius_fastload_permission() {
  return array(
    'sirius fastload admin' => array(
      'title' => t('Sirius Fastload Admin'), 
      'description' => t('Administer the FastLoad Tool'),
    ),
  );
}
