<?php

/**
 * Implements hook_permission().
 */

function sirius_tfa_permission() {
  $items = array();

  $items['sirius tfa setup own'] = array(
    'title' => t('Set up own TFA'),
    'description' => t('Allow users to set up TFA for their account. Users with "administer users" permission can edit other account\'s TFA.'),
  );

  $items['sirius tfa admin'] = array(
    'title' => t('Administer TFA'),
    'description' => t('Administer TFA settings.'),
  );

  return $items;
}
