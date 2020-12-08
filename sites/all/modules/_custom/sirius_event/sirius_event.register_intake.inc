<?php

function sirius_event_register_intake_form($form, &$form_state, $event_node, $rule) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['rule'] = $rule;
	$form_state['participant_role_tid'] = $participant_role_tid;
	$widget_settings = $event_type_json['widget_settings']['register'][$rule['delta']][$widget];

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$widget = 'intake';
	$widget_settings = $event_type_json['widget_settings']['register'][$rule['delta']][$widget];

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Register',
	);

	return $form;
}

function sirius_event_register_intake_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);

	$contact_nid = $form_state['values']['contact_nid'];
	$contact_node = node_load($contact_nid);

	$register_as = $form_state['values']['register_as'];
	list ($prole_tid, $pstatus_tid) = explode('|', $register_as);
	$prole_term = taxonomy_term_load($prole_tid);
	$pstatus_term = taxonomy_term_load($pstatus_tid);

  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  global $base_url;
  $url = $base_url . '/node/' . $event_nid . '/sirius_event_info';

	$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
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
