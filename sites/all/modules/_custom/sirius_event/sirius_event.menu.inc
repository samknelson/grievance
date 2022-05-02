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

  $node_tabs['sirius_event_participant']['sirius_event_participant_intake'] = array(
    'title' => 'Intake',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_view_intake',
    'permissions' => array('sirius'),
    'weight' => 6,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_intake/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_view_intake',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_intake/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_json_edit_intake',
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

  $node_tabs['sirius_event']['sirius_event_info/info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_node_tab_event_info',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event']['sirius_event_info/schedule'] = array(
    'title' => 'Schedule',
    'description' => '',
    'callback' => 'sirius_node_tab_event_schedule',
    'permissions' => array('sirius'),
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_info/body'] = array(
    'title' => 'Description',
    'description' => '',
    'callback' => 'sirius_node_tab_event_body',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_info/survey'] = array(
    'title' => 'Survey',
    'description' => '',
    'callback' => 'sirius_event_instance_participant_definition_select',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
  );

  $node_tabs['sirius_event']['sirius_event_info/class'] = array(
    'title' => 'Class Listing',
    'description' => '',
    'callback' => 'sirius_node_tab_event_class_settings',
    'permissions' => array('sirius'),
    'weight' => 5,
  );

  $node_tabs['sirius_event']['sirius_event_participants_current'] = array(
    'title' => 'Current Participants',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participants_current',
    'permissions' => array('sirius'),
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_participants_register'] = array(
    'title' => 'Add New Participants',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participants_register',
    'permissions' => array('sirius'),
    'weight' => 4,
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

  $node_tabs['sirius_event']['sirius_event_phonebank/prescan'] = array(
    'title' => 'Pre-Scan',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_prescan',
    'permissions' => array('sirius'),
    'weight' => 4,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_settings',
    'permissions' => array('sirius'),
    'weight' => 5,
  );

  $node_tabs['sirius_event']['sirius_event_phonebank/network_test'] = array(
    'title' => 'Network Test',
    'description' => '',
    'callback' => 'sirius_node_tab_event_phonebank_network_test',
    'permissions' => array('sirius'),
    'weight' => 6,
  );

  $node_tabs['sirius_event']['sirius_event_intake'] = array(
    'title' => 'Intake',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_applications',
    'permissions' => array('sirius'),
    'weight' => 5,
    'file' => 'sirius_event.page_intake_applications.inc',
  );

  $node_tabs['sirius_event']['sirius_event_intake/applications'] = array(
    'title' => 'Applications',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_applications',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_applications.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_event']['sirius_event_intake/info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_event']['sirius_event_intake/status'] = array(
    'title' => 'Status',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_status',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_status.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_event']['sirius_event_intake/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_settings',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_settings.inc',
    'weight' => 10,
  );

  // $map_files = array('sirius_event.page_map.inc','sirius_event.page_map_list.inc','sirius_event.page_map_view.inc','sirius_event.page_map_settings.inc');

  $node_tabs['sirius_event']['sirius_event_map'] = array(
    'title' => 'Map',
    'description' => '',
    'callback' => 'sirius_node_tab_event_map',
    'permissions' => array('sirius'),
    'weight' => 5,
    'file' => $map_files,
  );

  $node_tabs['sirius_event']['sirius_event_map/info'] = array(
    'title' => 'Info',
    'callback' => 'sirius_node_tab_event_map',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => $map_files,
  );

  $node_tabs['sirius_event']['sirius_event_map/view'] = array(
    'title' => 'View',
    'callback' => 'sirius_node_tab_event_map_view',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $map_files,
  );

  $node_tabs['sirius_event']['sirius_event_map/list'] = array(
    'title' => 'List',
    'callback' => 'sirius_node_tab_event_map_list',
    'permissions' => array('sirius'),
    'weight' => 3,
    'file' => $map_files,
  );

  $node_tabs['sirius_event']['sirius_event_map/settings'] = array(
    'title' => 'Settings',
    'callback' => 'sirius_node_tab_event_map_settings',
    'permissions' => array('sirius'),
    'weight' => 4,
    'file' => $map_files,
  );

  $node_tabs['sirius_event']['sirius_event_bullpen_self'] = array(
    'title' => 'Self Sign-in',
    'description' => '',
    'callback' => 'sirius_node_tab_event_bullpen_self',
    'permissions' => array(),
    'weight' => 2,
  );

  $disclaimer_files = array('sirius_event.page_disclaimer.inc','sirius_event.page_disclaimer_list.inc','sirius_event.page_disclaimer_settings.inc');

  $node_tabs['sirius_event']['sirius_event_disclaimer'] = array(
    'title' => 'Disclaimer',
    'description' => '',
    'callback' => 'sirius_node_tab_event_disclaimer_view',
    'permissions' => array('sirius'),
    'weight' => 5,
    'file' => $disclaimer_files,
  );

  $node_tabs['sirius_event']['sirius_event_disclaimer/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_node_tab_event_disclaimer_view',
    'permissions' => array('sirius'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => $disclaimer_files,
  );

  $node_tabs['sirius_event']['sirius_event_disclaimer/list_yes'] = array(
    'title' => 'List - Accepted',
    'description' => '',
    'callback' => 'sirius_node_tab_event_disclaimer_list',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $disclaimer_files,
  );

  $node_tabs['sirius_event']['sirius_event_disclaimer/list_no'] = array(
    'title' => 'List - Not Accepted',
    'description' => '',
    'callback' => 'sirius_node_tab_event_disclaimer_list',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $disclaimer_files,
  );

  $node_tabs['sirius_event']['sirius_event_disclaimer/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_disclaimer_settings',
    'permissions' => array('sirius'),
    'weight' => 10,
    'file' => $disclaimer_files,
  );

  $section_files = array(
    'sirius_event.page_section.inc', 
    'sirius_event.page_section_assign.inc',
    'sirius_event.page_section_manage.inc',
    // 'sirius_event.page_section_sort.inc',
    'sirius_event.page_section_settings.inc', 
  );

  $node_tabs['sirius_event']['sirius_event_section'] = array(
    'title' => 'Sections',
    'callback' => 'sirius_node_tab_event_section',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $section_files,
    'title callback' => 'sirius_event_section_alias',
    'title arguments' => array(1),
  );

  $node_tabs['sirius_event']['sirius_event_section/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_node_tab_event_section',
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => $section_files,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_event']['sirius_event_section/assign'] = array(
    'title' => 'Assign',
    'description' => '',
    'callback' => 'sirius_node_tab_event_section_assign',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $section_files,
  );

  $node_tabs['sirius_event']['sirius_event_section/manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_event_section_manage',
    'permissions' => array('sirius'),
    'weight' => 2,
    'file' => $section_files,
  );

  /*
  $node_tabs['sirius_event']['sirius_event_section/sort'] = array(
    'title' => 'Sort',
    'callback' => 'sirius_node_tab_event_section_sort',
    'permissions' => array('sirius'),
    'weight' => 3,
    'file' => $section_files,
  );
  */

  $node_tabs['sirius_event']['sirius_event_section/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_section_settings',
    'permissions' => array('sirius'),
    'weight' => 10,
    'file' => $section_files,
  );


  $node_tabs['sirius_event']['sirius_event_info/clone'] = array(
    'title' => 'Clone',
    'description' => '',
    'callback' => 'sirius_node_tab_event_clone',
    'permissions' => array('sirius domain admin'),
    'weight' => 10,
  );

  $node_tabs['sirius_event']['sirius_event_info/exim'] = array(
    'title' => 'Export / Import',
    'description' => '',
    'callback' => 'sirius_node_tab_event_exim',
    'permissions' => array('sirius domain admin'),
    'weight' => 11,
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_status'] = array(
    'title' => 'Status',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_status',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
    'file' => 'sirius_event.page_participant_status.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_comments'] = array(
    'title' => 'Comments',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_comments',
    'permissions' => array('sirius event admin'),
    'weight' => 3,
    'file' => 'sirius_event.page_participant_comments.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_disclaimer'] = array(
    'title' => 'Disclaimer',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_disclaimer',
    'permissions' => array('sirius event admin'),
    'weight' => 4,
    'file' => 'sirius_event.page_participant_disclaimer.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_disclaimer/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_disclaimer',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_event.page_participant_disclaimer.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_disclaimer/revoke'] = array(
    'title' => 'Revoke',
    'description' => '',
    'callback' => 'sirius_event_node_tab_participant_disclaimer_revoke',
    'permissions' => array('sirius event admin'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_event.page_participant_disclaimer.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_event_participant_json_log'] = array(
    'title' => 'Change Log',
    'description' => '',
    'callback' => 'sirius_jsonfield_node_tab_entity_sublog',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
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

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_form/preferences'] = array(
    'title' => 'Preferences',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_preferences',
    'permissions' => array('sirius domain admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_audio'] = array(
    'title' => 'Outgoing Message',
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

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_followup'] = array(
    'title' => 'Follow-Up',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_followup',
    'permissions' => array('sirius domain admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_followup/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_followup',
    'permissions' => array('sirius domain admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_event.page_participant_definition_followup.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_followup/sms'] = array(
    'title' => 'SMS',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_followup_sms',
    'permissions' => array('sirius domain admin'),
    'weight' => 2,
    'file' => 'sirius_event.page_participant_definition_followup_items.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_event_participant_definition_followup/email'] = array(
    'title' => 'Email',
    'description' => '',
    'callback' => 'sirius_node_tab_event_participant_definition_followup_email',
    'permissions' => array('sirius domain admin'),
    'weight' => 3,
    'file' => 'sirius_event.page_participant_definition_followup_items.inc',
  );

  $node_tabs['sirius_contact']['sirius_contact_events'] = array(
    'title' => 'Events',
    'description' => '',
    'callback' => 'sirius_event_node_tab_contact_events',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_contact_events.inc',
  );

  $node_tabs['sirius_contact']['sirius_contact_events/list'] = array(
    'title' => 'Events',
    'description' => '',
    'callback' => 'sirius_event_node_tab_contact_events',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_contact_events.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_contact']['sirius_contact_events/interestlist'] = array(
    'title' => 'Interest Lists',
    'description' => '',
    'callback' => 'sirius_event_node_tab_contact_interestlists',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_contact_interestlists.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_events'] = array(
    'title' => 'Events',
    'description' => '',
    'callback' => 'sirius_event_node_tab_worker_events',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_worker_events.inc',
  );

  $node_tabs['sirius_worker']['sirius_worker_events/list'] = array(
    'title' => 'Events',
    'description' => '',
    'callback' => 'sirius_event_node_tab_worker_events',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_worker_events.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_worker_events/interestlist'] = array(
    'title' => 'Interest Lists',
    'description' => '',
    'callback' => 'sirius_event_node_tab_worker_interestlists',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_worker_interestlists.inc',
    'weight' => 2,
  );

  /*
  $node_tabs['sirius_log']['sirius_event_intake_application'] = array(
    'title' => 'Application',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application.inc',
    'weight' => 1,
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_disclaimers'] = array(
    'title' => 'Disclaimers',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_disclaimers',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_disclaimers.inc',
    'weight' => 1,
  );
  */

  $node_tabs['sirius_log']['sirius_event_intake_application_wizard'] = array(
    'title' => 'Wizard',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_wizard',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_wizard.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage.inc',
    'weight' => 4,
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_manage/status'] = array(
    'title' => 'Status',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage.inc',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_manage/ms'] = array(
    'title' => 'Industries',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage_ms',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage_ms.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_manage/skills-unverified'] = array(
    'title' => 'Unverified Skills',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage_skill',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage_skill.inc',
    'weight' => 3,
    'callback arguments' => array('unverified'),
  );

  $node_tabs['sirius_log']['sirius_event_intake_application_manage/skills-verified'] = array(
    'title' => 'Verified Skills',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage_skill',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage_skill.inc',
    'weight' => 4,
    'callback arguments' => array('verified'),
  );


  $node_tabs['sirius_json_definition']['sirius_event_catalog_view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_event_node_tab_catalog_view',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_catalog_view.inc',
    'weight' => 11,
    'type' => MENU_CALLBACK,
  );

  $node_tabs['sirius_json_definition']['sirius_event_catalog_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_event_node_tab_catalog_settings',
    'permissions' => array('sirius event admin'),
    'file' => 'sirius_event.page_catalog_settings.inc',
    'weight' => 12,
  );

  sirius_node_tab_set_module('sirius_event', $node_tabs);

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
    'file' => 'sirius_event.term_tab_create.inc'
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

  $term_tabs['sirius_event_type']['sirius_event_type_map'] = array(
    'title' => 'Map',
    'description' => '',
    'callback' => 'sirius_event_term_tab_map',
    'permissions' => array('sirius event admin'),
    'weight' => 4,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_section'] = array(
    'title' => 'Sections',
    'description' => '',
    'callback' => 'sirius_event_term_tab_section',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
    'file' => 'sirius_event.term_tab_section.inc',
  );

  $term_tabs['sirius_event_type']['sirius_event_type_disclaimer'] = array(
    'title' => 'Disclaimer',
    'description' => '',
    'callback' => 'sirius_event_term_tab_disclaimer',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
    'file' => 'sirius_event.term_tab_disclaimer.inc',
  );

  $term_tabs['sirius_event_type']['sirius_event_type_bullpen'] = array(
    'title' => 'Bullpen',
    'description' => '',
    'callback' => 'sirius_event_term_tab_bullpen',
    'permissions' => array('sirius event admin'),
    'weight' => 5,
    'file' => 'sirius_event.term_tab_bullpen.inc',
  );

  $term_tabs['sirius_event_type']['sirius_event_type_class'] = array(
    'title' => 'Class',
    'description' => '',
    'callback' => 'sirius_event_term_tab_class',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_intake'] = array(
    'title' => 'Intake',
    'description' => '',
    'callback' => 'sirius_event_term_tab_intake',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
    'file' => array('sirius_event.term_tab_intake.inc'),
  );

  $term_tabs['sirius_event_type']['sirius_event_type_interestlist'] = array(
    'title' => 'Interest List',
    'description' => '',
    'callback' => 'sirius_event_term_tab_interestlist_self',
    'permissions' => array('sirius'),
    'weight' => 6,
    'file' => 'sirius_event.term_tab_interestlist_self.inc',
  );

  $term_tabs['sirius_event_type']['sirius_event_type_interestlist/self'] = array(
    'title' => 'Sign Up Self',
    'description' => '',
    'callback' => 'sirius_event_term_tab_interestlist_self',
    'permissions' => array('sirius'),
    'weight' => 6,
    'file' => 'sirius_event.term_tab_interestlist_self.inc',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_interestlist/worker'] = array(
    'title' => 'Sign Up Worker',
    'description' => '',
    'callback' => 'sirius_event_term_tab_interestlist_worker',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
    'file' => 'sirius_event.term_tab_interestlist_worker.inc',
    'weight' => 2,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_interestlist/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_event_term_tab_interestlist_list',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
    'file' => 'sirius_event.term_tab_interestlist_list.inc',
    'weight' => 3,
  );

  $term_tabs['sirius_event_type']['sirius_event_type_interestlist/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_event_term_tab_interestlist_settings',
    'permissions' => array('sirius event admin'),
    'weight' => 10,
    'file' => 'sirius_event.term_tab_interestlist_settings.inc',
  );

  $term_tabs['sirius_event_participant_status']['sirius_event_pstatus'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_event_term_tab_pstatus',
    'permissions' => array('sirius event admin'),
    'weight' => 6,
  );

  sirius_node_tab_set_module('sirius_event', $term_tabs);
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

  $items['sirius/twilio/network-test'] = array(
    'title' => 'Twilio Network Test',
    'page callback' => 'sirius_node_tab_event_phonebank_network_test',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/ajax/phonebank-quicksave'] = array(
    'page callback' => 'sirius_event_phonebank_quicksave',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );

  $items['sirius/modal/event/map/%/%'] = array(
    'title'            => 'Participant Modal',
    'page callback'    => 'sirius_event_map_modal',
    'page arguments'   => array(4,5),
    'access callback'   => 'sirius_event_map_modal_access',
    'access arguments' => array(4,5),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/modal/event/map-legend/%'] = array(
    'title'            => 'Legend',
    'page callback'    => 'sirius_event_map_modal_legend',
    'page arguments'   => array(4),
    'access callback'   => 'sirius_event_map_modal_legend_access',
    'access arguments' => array(4),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/modal/event/followup/%/%/%'] = array(
    'title'            => 'Follow Up',
    'page callback'    => 'sirius_event_followup_modal',
    'page arguments'   => array(4,5,6),
    'access callback'   => 'sirius_event_followup_modal_access',
    'access arguments' => array(4,5,6),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/ajax/event/map/poll/%'] = array(
    'page callback'    => 'sirius_event_map_ajax_poll',
    'page arguments'   => array(5),
    'access callback'   => 'sirius_event_map_ajax_poll_access',
    'access arguments' => array(5),
    'type'             => MENU_CALLBACK,
    'weight' => 20,
  );


  $items['sirius/event/interestlist/settings'] = array(
    'title' => 'Interest List Settings',
    'description' => '',
    'access arguments' => array('sirius event admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_event_interestlist_settings_form'),
  );

  $items['sirius/event/interestlists'] = array(
    'title' => 'Interest Lists',
    'description' => '',
    'access arguments' => array('sirius'),
    'page callback' => 'sirius_event_page_interestlists',
    'page arguments' => array(),
    'file' => 'sirius_event.page_interestlists.inc',
  );

  $items['sirius/event/duplicate_participants'] = array(
    'title' => 'Duplicate Participants',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'sirius_event_page_duplicate_participants',
    'page arguments' => array(),
    'file' => 'sirius_event.page_duplicate_participants.inc',
  );

  $items['intake'] = array(
    'title' => 'Intake',
    'access callback' => TRUE,
    'page callback' => 'sirius_event_page_intake_anonymous_landing',
    'page arguments' => array(),
    'file' => 'sirius_event.page_intake_anonymous_landing.inc',
  );

  $items['intake/%'] = array(
    'title' => 'Intake',
    'access callback' => TRUE,
    'page callback' => 'sirius_event_page_intake_anonymous',
    'page arguments' => array(1),
    'file' => 'sirius_event.page_intake_anonymous.inc',
  );

  $items['intake/%/wizard'] = array(
    'title' => 'Intake',
    'access callback' => TRUE,
    'page callback' => 'sirius_event_page_intake_anonymous_wizard',
    'page arguments' => array(1),
    'file' => 'sirius_event.page_intake_anonymous_wizard.inc',
  );

  $items['sirius/event/catalog/list'] = array(
    'title' => 'Catalogs',
    'access arguments' => array('sirius'),
    'page callback' => 'sirius_event_page_catalog_list',
    'page arguments' => array('sirius event admin'),
    'file' => 'sirius_event.page_catalog_list.inc',
  );

  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_event_modal_paths() {
  $paths = array();

  $paths['sirius/modal/event/map/%/%'] = array('style' => 'sirius-event-modal-style');
  $paths['sirius/modal/event/map-legend/%'] = array('style' => 'sirius-event-modal-style');
  $paths['sirius/modal/event/followup/%/%/%'] = array('style' => 'sirius-event-modal-style');

  return $paths;
}

function sirius_event_modal_styles() {
  $styles = array();

  $styles['sirius-event-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 1000,
      'height' => 800,
    ),
  );

  return $styles;
}



