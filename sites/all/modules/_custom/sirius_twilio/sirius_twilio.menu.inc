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

  $node_tabs['sirius_log']['sirius_twilio_recording'] = array(
    'title' => 'Recording',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_recording',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_recording.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_log']['sirius_twilio_recording/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_recording',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_recording.inc',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_log']['sirius_twilio_recording/comment'] = array(
    'title' => 'Comment',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_recording_comment',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_recording_comment.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_twilio_recording/worker'] = array(
    'title' => 'Worker',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_recording_worker',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_recording_worker.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_log']['sirius_twilio_call'] = array(
    'title' => 'Call',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_call',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_call.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_log']['sirius_twilio_call/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_call',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_call.inc',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_log']['sirius_twilio_call/recording'] = array(
    'title' => 'Recording',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_call_recording',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_call_recording.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_twilio_call/worker'] = array(
    'title' => 'Worker',
    'description' => '',
    'callback' => 'sirius_twilio_node_tab_call_worker',
    'permissions' => array('sirius log view'),
    'file' => 'sirius_twilio.page_call_worker.inc',
    'weight' => 3,
  );

  sirius_node_tab_set_module('sirius_twilio', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_twilio_menu() {
  $items = array();

  $items['sirius/twilio/incoming/sms'] = array(
    'title' => 'Sirius: Twilio: Incoming: SMS',
    'description' => '',
    'page callback' => 'sirius_twilio_incoming_sms',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/incoming/voice'] = array(
    'title' => 'Sirius: Twilio: Incoming: Voice',
    'description' => '',
    'page callback' => 'sirius_twilio_incoming_voice',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/outgoing/voice/stream'] = array(
    'title' => 'Sirius: Twilio: Outgoing: Stream',
    'description' => '',
    'page callback' => 'sirius_twilio_outgoing_voice_stream',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/browserphone/twiml'] = array(
    'title' => 'TwiML BrowserPhone Server',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_twilio_browserphone_twiml_dial',
    'page arguments' => array(),
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/browserphone/twiml/callended'] = array(
    'title' => 'TwiML BrowserPhone Server',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_twilio_browserphone_twiml_callended',
    'page arguments' => array(),
    'type' => MENU_CALLBACK,
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

  $items['sirius/twilio/browserphone/twiml/conference'] = array(
    'page callback' => 'sirius_twilio_browserphone_twiml_conference',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/twilio/browserphone/twiml/echo'] = array(
    'title' => 'TwiML BrowserPhone Server',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_twilio_browserphone_twiml_echo',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/browserphone/twiml/conference/add'] = array(
    'title' => 'TwiML BrowserPhone Server',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_twilio_browserphone_twiml_conference_add',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/twilio/phone'] = array(
    'title' => 'Sirius: Twilio: Phone',
    'description' => '',
    'page callback' => 'sirius_twilio_page_phone',
    'page arguments' => array(),
    'access arguments' => array('sirius twilio admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/phone/twilio'] = array(
    'title' => 'Twilio Settings',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_twilio_config'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/phone/twilio/global'] = array(
    'title' => 'Global Settings',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_twilio_config'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/phone/twilio/amd'] = array(
    'title' => 'Answering Machine Detection',
    'description' => '',
    'access arguments' => array('sirius domain admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_twilio_config_amd'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/phone/twilio/test'] = array(
    'title' => 'Test',
    'description' => '',
    'page callback' => 'sirius_twilio_page_test',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/twilio/recordings'] = array(
    'title' => 'Twilio Recordings',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recordings',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recordings.inc',
  );

  $items['sirius/twilio/recordings/remote'] = array(
    'title' => 'Remote',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recordings',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recordings.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/twilio/recordings/local'] = array(
    'title' => 'Local',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recordings_local',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recordings_local.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/twilio/recordings/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recordings_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recordings_settings.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/twilio/recordings/scan'] = array(
    'title' => 'Scan',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recordings_scan',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recordings_scan.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 5,
  );

  $items['sirius/twilio/recording/stream/%'] = array(
    'title' => '',
    'description' => '',
    'page callback' => 'sirius_twilio_page_recording_stream',
    'page arguments' => array(4),
    'access arguments' => array('sirius domain admin'),
    'file' => 'sirius_twilio.page_recording_stream.inc',
    'type' => MENU_CALLBACK,
  );

  return $items;
}
