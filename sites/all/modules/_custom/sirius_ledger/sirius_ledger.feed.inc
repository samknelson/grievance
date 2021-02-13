<?php

/***
 * Implements hook_sirius_wizard_info() 
 */

function sirius_ledger_sirius_wizard_info() {
  $items = array();

  // $items += sirius_ledger_feed_charges_info();
  $items += sirius_ledger_feed_allocate_info();

  return $items;
}