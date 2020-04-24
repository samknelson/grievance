<?php

//
//  $html .= 
//

function sirius_node_tab_event_participant_json_access($participant_node) {
	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	if (!$event_nid) { return FALSE; }
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_json_view_access($participant_node) { return sirius_node_tab_event_participant_json_access($participant_node); }
function sirius_node_tab_event_participant_json_edit_access($participant_node) { return sirius_node_tab_event_participant_json_access($participant_node); }

function sirius_node_tab_event_participant_json_backlinks($participant_node, $event_node) {
	return l('Back to participant list', "node/" . $event_node->nid . "/sirius_event_participants_current", array('attributes' => array('class' => 'tool-button')));
}

function sirius_node_tab_event_participant_json($participant_node) {
	drupal_goto(current_path() . '/view');
}

function sirius_node_tab_event_participant_json_view($participant_node) {
	drupal_add_css('sites/all/modules/_custom/sirius_event/css/event.css');

	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	$backlinks = sirius_node_tab_event_participant_json_backlinks($participant_node, $event_node);
	if (sirius_node_tab_event_phonebank_access($event_node)) {
		$backlinks .= l('Direct Call', "node/$event_nid/sirius_event_phonebank/call", array('query' => array('mode' => 'direct', 'return_to' => current_path(), 'phone' => $contact_node->field_sirius_phone['und'][0]['value']), 'attributes' => array('class' => 'tool-button')));
	}
	$html = '<div>' . $backlinks . '</div>';

	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');

	$html .= "<div class=\"sirius_event_survey_results\">";
	$html .= sirius_jsonfield_util_render_all_values($survey_form, $participant_results);
	$html .= '</div>';

	return $html;
}

function sirius_node_tab_event_participant_json_edit($participant_node) {
	return drupal_get_form('sirius_node_tab_event_participant_json_form', $participant_node);
}

function sirius_node_tab_event_participant_json_form($form, &$form_state, $participant_node) {
	$form_state['participant_nid'] = $participant_node->nid;
	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');
	sirius_jsonfield_util_set_defaults($survey_form, $participant_results);

	$form['#tree'] = TRUE;
	$form['survey'] = $survey_form;

	$backlinks = sirius_node_tab_event_participant_json_backlinks($participant_node, $event_node);
	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . $backlinks . '</div>' . 
		'<div class="sirius-form">',
		'#weight' => -9999,
	);

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 10000,
	);

	return $form;
}

function sirius_node_tab_event_participant_json_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	sirius_jsonfield_util_results_set($participant_json, 'survey', $form_state['values']['survey']);
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	sirius_jsonfield_log($participant_node, 'survey', 'Survey updated.');
	node_save($participant_node);
}