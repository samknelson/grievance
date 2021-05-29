<?php

/***
 * Implements hook_permission
 */

function sirius_dashboard_permission() {
  return array(
    'sirius dashboard admin' => array(
      'title' => t('Sirius Dashboard Admin'), 
      'description' => t('Configure dashboard plugins'),
    ),
    'sirius dashboard user' => array(
      'title' => t('Sirius Dashboard User'), 
      'description' => t('Use dashboard elements'),
    ),
  );
}
