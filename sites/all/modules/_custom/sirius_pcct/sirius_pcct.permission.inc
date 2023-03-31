<?php

/***
 * Implements hook_permission
 */

function sirius_pcct_permission() {
  return array(
    'sirius pcct admin' => array(
      'title' => t('Sirius PCCT Admin'), 
      'description' => t('Permission to administer Personal Cost Comparison Tool (PCCT) services.'),
    ),
  );
}
