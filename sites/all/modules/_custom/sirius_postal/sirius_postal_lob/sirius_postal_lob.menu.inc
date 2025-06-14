<?php

/**
 * Implements hook_menu
 */

function sirius_postal_lob_menu() {
  $items = array();

  $items['sirius/postal/lob/webhook'] = array(
    'title' => 'Sirius: Twilio: Status Callback: SMS',
    'description' => '',
    'page callback' => 'sirius_postal_lob_webhook',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );


  $items['sirius/postal/lob/body/%'] = array(
    'title' => 'Sirius: Postal: Lob: Letter Body',
    'description' => '',
    'page callback' => 'sirius_postal_lob_page_body',
    'file' => 'sirius_postal_lob.page_body.inc',
    'page arguments' => array(4),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  return $items;
}