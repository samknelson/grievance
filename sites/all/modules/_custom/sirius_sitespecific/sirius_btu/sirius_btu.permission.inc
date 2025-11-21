<?php

/***
 * Implements hook_permission
 */

function sirius_btu_permission() {
  return array(
    'sirius btu bps' => array(
      'title' => t('Sirius BTU BPS'), 
      'description' => t('Operate as a "super-employer", BPS, with access to *all* employers.'),
    ),
  );
}
