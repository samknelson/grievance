<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_twilio_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_log']['sirius_twilio_calllog'] = array(
    'title' => 'Call Log',
    'description' => '',
    'callback' => 'sirius_node_tab_calllog',
    'permissions' => array('sirius log view'),
  );

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_twilio_menu() {
  $items = array();

  $items['sirius/twilio/test'] = array(
    'title' => 'Sirius: Twilio: Test',
    'description' => '',
    'page callback' => 'sirius_twilio_page_test',
    'page arguments' => array(),
    'access arguments' => array('sirius twilio admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/incoming/sms'] = array(
    'title' => 'Sirius: Twilio: Incoming: SMS',
    'description' => '',
    'page callback' => 'sirius_twilio_incoming_sms',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/incoming/voice'] = array(
    'title' => 'Sirius: Twilio: Incoming: Voice',
    'description' => '',
    'page callback' => 'sirius_twilio_incoming_voice',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/browserphone/twiml'] = array(
    'title' => 'TwiML BrowserPhone Server',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_twilio_browserphone_twiml_dial',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/browserphone/twiml/omg'] = array(
    'page callback' => 'sirius_twilio_browserphone_twiml_omg',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/browserphone/twiml/omg/play'] = array(
    'page callback' => 'sirius_twilio_browserphone_twiml_omg_play',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/browserphone/twiml/omg/stream'] = array(
    'page callback' => 'sirius_twilio_browserphone_twiml_omg_stream',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/phone'] = array(
    'title' => 'Sirius: Twilio: Phone',
    'description' => '',
    'page callback' => 'sirius_twilio_page_phone',
    'page arguments' => array(),
    'access arguments' => array('sirius twilio admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/twilio'] = array(
    'title' => 'Twilio Configuration ',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_twilio_config'),
    'type' => MENU_NORMAL_ITEM,
  );



  return $items;
}
