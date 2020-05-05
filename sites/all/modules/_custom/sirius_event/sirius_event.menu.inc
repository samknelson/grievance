<?php

function sirius_event_node_tabs() {
	$node_tabs = array();

  $node_tabs['sirius_event_participant']['sirius_event_participant_json'] = array(
    'title' => 'Survey',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_view',
    'permissions' => array('sirius'),
    'weight' => 1,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_json/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_view',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_json/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_edit',
    'permissions' => array('sirius'),
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_node_tab_event_info',
    'permissions' => array('sirius'),
    'weight' => 1,
  );

  $node_tabs['sirius_event']['sirius_event_info/schedule'] = array(
    'title' => 'Schedule',
    'description' => '',
    'callback' => 'sirius_node_tab_event_schedule',
    'permissions' => array('sirius'),
    'weight' => 1,
  );

  $node_tabs['sirius_event']['sirius_event_info/body'] = array(
    'title' => 'Description',
    'description' => '',
    'callback' => 'sirius_node_tab_event_body',
    'permissions' => array('sirius'),
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_participants_current'] = array(
    'title' => 'Participants',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participants_current',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_participants_register'] = array(
    'title' => 'Register',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participants_register',
    'permissions' => array('sirius'),
    'weight' => 4,
  );

  $node_tabs['sirius_event']['sirius_event_instance_participant_definition'] = array(
    'title' => 'Survey',
    'description' => '',
    'callback' => 'sirius_event_instance_participant_definition',
    'permissions' => array('sirius event admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_event']['sirius_event_instance_participant_definition/select'] = array(
    'title' => 'Select',
    'description' => '',
    'callback' => 'sirius_event_instance_participant_definition_select',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_event']['sirius_event_instance_participant_definition/preview'] = array(
    'title' => 'Preview',
    'description' => '',
    'callback' => 'sirius_event_instance_participant_definition_preview',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank'] = array(
    'title' => 'Phonebank',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_call',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/call'] = array(
    'title' => 'Call',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_call',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/status'] = array(
    'title' => 'Status',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_status',
    'permissions' => array('sirius'),
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/lock'] = array(
    'title' => 'Locks',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_lock',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_settings',
    'permissions' => array('sirius'),
    'weight' => 4,
  );

  $node_tabs['sirius_event']['sirius_event_class'] = array(
    'title' => 'Class',
    'description' => '',
    'callback' => 'sirius_node_tab_event_class_settings',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_class/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_class_settings',
    'permissions' => array('sirius'),
    'weight' => 1,
    // 'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event']['sirius_event_bullpen_self'] = array(
    'title' => 'Self Sign-in',
    'description' => '',
    'callback' => 'sirius_node_tab_event_bullpen_self',
    'permissions' => array(),
    'weight' => 2,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_comments'] = array(
    'title' => 'Comments',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_comments',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_json_log'] = array(
    'title' => 'Change Log',
    'description' => '',
    'callback' => 'sirius_jsonfield_log_render',
    'permissions' => array('sirius event admin'),
    'weight' => 3,
  );

  /*
  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_settings',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
  );
  */

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form'] = array(
    'title' => 'Form',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_edit',
    'permissions' => array('sirius domain admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_edit',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form/preview'] = array(
    'title' => 'Preview',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_preview',
    'permissions' => array('sirius domain admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form/signatures'] = array(
    'title' => 'Signatures',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_signatures',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form/exim'] = array(
    'title' => 'Export / Import',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_exim',
    'permissions' => array('sirius domain admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_audio'] = array(
    'title' => 'Audio',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_audio',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_callerid'] = array(
    'title' => 'Caller ID',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_callerid',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
  );

	return $node_tabs;
}

function sirius_event_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_event_type']['sirius_event_type_policy'] = array(
    'title' => 'Event Access',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/create'] = array(
    'title' => 'Creation',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_create',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/register'] = array(
    'title' => 'Registration',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_register',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/participants'] = array(
    'title' => 'Participants',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_participants',
    'permissions' => array('sirius event admin'),
    'weight' => 3,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_view',
    'permissions' => array('sirius event admin'),
    'weight' => 4,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_edit',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_policy/materials'] = array(
    'title' => 'Materials',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy_materials',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_schedule'] = array(
    'title' => 'Schedule',
    'description' => '',
    'callback' => 'sirius_event_term_tab_schedule',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_widgets'] = array(
    'title' => 'Widgets',
    'description' => '',
    'callback' => 'sirius_event_term_tab_widgets',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
  );


  $term_tabs['sirius_event_type']['sirius_event_type_phonebank'] = array(
    'title' => 'Phonebank',
    'description' => '',
    'callback' => 'sirius_event_term_tab_phonebank',
    'permissions' => array('sirius event admin'),
    'weight' => 4,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_bullpen'] = array(
    'title' => 'Bullpen',
    'description' => '',
    'callback' => 'sirius_event_term_tab_bullpen',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_class'] = array(
    'title' => 'Class',
    'description' => '',
    'callback' => 'sirius_event_term_tab_class',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
  );

  return $term_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_event_menu() {
  $items = array();

  $items['sirius/event'] = array(
    'title' => 'Events',
    'description' => '',
    'page callback' => 'sirius_event_page_landing',
    'page arguments' => array(),
    'access callback' => 'sirius_event_page_landing_access',
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/list'] = array(
    'title' => 'Event List',
    'description' => '',
    'page callback' => 'sirius_event_page_list',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/bullpens'] = array(
    'title' => 'Event List',
    'description' => '',
    'page callback' => 'sirius_event_page_list_bullpens',
    'page arguments' => array(),
    'access callback' => array('sirius_event_page_list_bullpens_access'),
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/classes'] = array(
    'title' => 'Class List',
    'description' => '',
    'page callback' => 'sirius_event_page_list_classes',
    'page arguments' => array(),
    'access callback' => array('sirius_event_page_list_classes_access'),
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/list/%'] = array(
    'title' => 'Event List',
    'description' => '',
    'page callback' => 'sirius_event_page_list_type',
    'page arguments' => array(3),
    'access callback' => array('sirius_event_page_list_access'),
    'access arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/list/%/upcoming'] = array(
    'title' => 'Upcoming',
    'description' => '',
    'page callback' => 'sirius_event_page_list_type_upcoming',
    'page arguments' => array(3),
    'access callback' => array('sirius_event_page_list_access'),
    'access arguments' => array(3),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['sirius/event/list/%/all'] = array(
    'title' => 'All',
    'description' => '',
    'page callback' => 'sirius_event_page_list_type_all',
    'page arguments' => array(3),
    'access callback' => array('sirius_event_page_list_access'),
    'access arguments' => array(3),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/event/create/%'] = array(
    'title' => 'Create',
    'description' => '',
    'page callback' => 'sirius_event_page_create',
    'page arguments' => array(3),
    'access callback' => 'sirius_event_page_create_access',
    'access arguments' => array(3),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );
  $items['sirius/event/participant_definitions'] = array(
    'title' => 'Surveys',
    'description' => '',
    'page callback' => 'sirius_event_page_participant_definitions',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/event/contact/search/%/%'] = array(
    'page callback' => 'sirius_event_contact_search',
    'page arguments' => array(4,5),
    'access callback' => array('sirius_event_page_list_access'),
    'access arguments' => array(4),
    'type' => MENU_CALLBACK,
  );

  $items['sirius/ajax/bullpen-quicksearch'] = array(
    'page callback' => 'grievance_quicksearch_autocomplete',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_CALLBACK,
  );

  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_event_modal_paths() {
  $paths = array();

  /*
  $paths['grievance/modal/%node/log-add'] = array(
    'style' => 'grievance-modal-style',
  );
  */

  return $paths;
}

function sirius_event_modal_styles() {
  $styles = array();

  /*
  $styles['grievance-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 500,
      'height' => 350,
    ),
  );
  */

  return $styles;
}



