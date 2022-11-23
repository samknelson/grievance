<?php

/***
 * Implements hook_sirius_wizard_info() 
 */

function sirius_employer_sirius_wizard_info() {
  sirius_require('sirius_employer', 'sirius_employer.feed_employer.inc');

  $items = array();

  // $items += sirius_ledger_feed_charges_info();
  $items += sirius_employer_feed_dispatch_check_info();
  // $items += sirius_employer_feed_employer_info();

  return $items;
}