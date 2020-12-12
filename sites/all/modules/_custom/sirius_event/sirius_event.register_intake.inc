<?php

function sirius_event_register_intake_form($form, &$form_state, $event_node, $rule) {
	/////////////////////////////////////////////////////////////////////
	// Setup
	/////////////////////////////////////////////////////////////////////

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	global $user;
	$event_nid = $event_node->nid;

	$form_state['event_nid'] = $event_nid;
	$form_state['rule'] = $rule;

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$survey_node = sirius_event_survey_get($event_node, 'intake');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	/////////////////////////////////////////////////////////////////////
	// Form
	/////////////////////////////////////////////////////////////////////

	$prep_settings = $event_json;
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($survey_form, array(), $prep_settings);

	$form['#tree'] = TRUE;

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');
	$form['intro'] = array('#markup' => '<h2>New Client Intake</h2>');

	$form['name_first'] = array(
		'#type' => 'textfield',
		'#title' => '<span class="title_wrap">First Name</span>',
		'#required' => TRUE,
	);

	$form['name_last'] = array(
		'#type' => 'textfield',
		'#title' => '<span class="title_wrap">Last Name</span>',
		'#required' => TRUE,
	);

	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => '<span class="title_wrap">Phone Number</span>',
		'#required' => TRUE,
	);

	$form['survey'] = $survey_form;
	$form['save'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);
	$form['survey_wrap_close'] = array('#markup' => '</div>');


	return $form;
}


function sirius_event_register_intake_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$rule = $form_state['rule'];
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node, 'intake');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	// Create a contact
	global $user;
	$domain_nid = sirius_domain_nid();
	$contact_node = new stdClass();
	$contact_node->type = 'sirius_contact';
	$contact_node->no_recurse = TRUE;
	$contact_node->language = $user->language;
	$contact_node->uid = $user->uid;
	$contact_node->status = 1;
	$contact_node->promote = 0;
	$contact_node->comment = 0;
	$contact_node->field_sirius_name['und'][0]['given'] = $form_state['values']['name_first'];
	$contact_node->field_sirius_name['und'][0]['family'] = $form_state['values']['name_last'];
	$contact_node->field_sirius_phone['und'][0]['value'] = $form_state['values']['phone'];
	if ($domain_nid) { $contact_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	$contact_json = array();
	sirius_jsonfield_util_results_set($contact_json, 'default', $form_state['values']['survey']);
	sirius_jsonfield_util_node_json_set($contact_node, $contact_json);
	sirius_jsonfield_log($contact_node, 'survey', 'Survey updated.');
  node_object_prepare($contact_node);
  $contact_node = node_submit($contact_node);
  node_save($contact_node);

  // Register
  $participant_json = array();
	sirius_jsonfield_util_results_set($participant_json, 'survey', $form_state['values']['survey']);
	sirius_jsonfield_util_results_set($participant_json, 'survey_intake', $form_state['values']['survey']);
  list ($prole_tid, $pstatus_tid) = explode('|', array_keys($rule['register_as'])[0]);
	$result = sirius_event_participant_register($event_node->nid, $contact_node->nid, $prole_tid, $pstatus_tid, $participant_json);
	$participant_nid = $result['nid'];

  drupal_set_message("Thank you for completing the intake form.");
  drupal_set_message("Admin notes: " . l("Contact #" . $contact_node->nid, 'node/' . $contact_node->nid) . ', ' . l("Participant #$participant_nid", 'node/' . $participant_nid));
  drupal_goto('node/' . $event_node->nid . '/sirius_event_info');

  $form_state['rebuild'] = TRUE;
}































































































function sirius_event_register_intake_config_form($form, &$form_state, $event_type_term, $rule_type, $delta, $widget) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$form_state['rule_type'] = $rule_type;
	$form_state['delta'] = $delta;
	$form_state['widget'] = $widget;

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
	$contact_paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));
	$contact_core_paths = array_reverse(array_keys(sirius_jsonfield_util_list_node_fields('sirius_contact')));
	foreach ($contact_core_paths as $path) { array_unshift($contact_paths, $path); }

	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	$surveys = array(NULL => t('-- Select --'));
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', $domain_nid, $type);
	if ($avail) {
		foreach ($avail as $hr) {
			$surveys[$hr->nid] = $hr->node_title;
		}
	}

	// @todo
	$survey_nid = $event_type_json['widget_settings'][$rule_type][$delta][$widget]['survey'];
	if ($survey_nid) {
		$survey_node = node_load($survey_nid);
		$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
		$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');
		$survey_paths = array_values(sirius_jsonfield_util_form_list_paths($survey_form, TRUE));
	}

	$form['#tree'] = TRUE;
	$form['settings']['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Widget Name',
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['name'],
	);

	$weight = 0;
	$options = array();
	foreach ($contact_paths as $path) {
		$options["$weight$path"] = $path;
		++$weight;
	}
	$form['settings']['contact_fields'] = array(
		'#type' => 'select',
		'#multiple' => TRUE,
		'#title' => 'Contact Fields',
		'#options' => $options,
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['contact_fields'],
		'#size' => 10,
	);

	$form['settings']['survey'] = array(
		'#type' => 'select',
		'#title' => 'Survey',
		'#options' => $surveys,
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['survey'],
	);

	if ($survey_nid) {
		$weight = 0;
		$options = array();
		foreach ($survey_paths as $path) {
			$options["$weight$path"] = $path;
			++$weight;
		}
		$form['settings']['participant_fields'] = array(
			'#type' => 'select',
			'#multiple' => TRUE,
			'#title' => 'Participant Fields',
			'#options' => $options,
			'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['participant_fields'],
			'#size' => 10,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_register_intake_config_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$rule_type = $form_state['rule_type'];
	$delta = $form_state['delta'];
	$widget = $form_state['widget'];

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$event_type_json['widget_settings'][$rule_type][$delta][$widget] = $form_state['values']['settings'];
	sirius_jsonfield_util_term_json_set($event_type_term, $event_type_json);
	taxonomy_term_save($event_type_term);
}
