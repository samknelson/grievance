<?php

function sirius_node_tab_event_participant_json_access($participant_node, $which = 'default') {
	// Participant nodes don't have domains, but in order to see this page the participant and the event must both have
	// the same domain, and we must be in it.
	$domain_nid = sirius_domain_nid();

	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	if (!$event_node) { return FALSE; }
	if ($domain_nid != $event_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	if (!$contact_node) { return FALSE; }
	if ($domain_nid != $contact_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	// The survey should exist in the event
	if (!sirius_event_survey_get_nid($event_node, $which)) { return FALSE; }

	// Must have access to any participant rule that matches this participant's prole.
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$all_rules = $json['participants']['rules'];
	if (user_access('sirius_event_admin')) {
		$rules = $all_rules;
	} else {
		$rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE);
	}

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
	$found = FALSE;
	foreach ($rules as $rule) {
		if ($rule['participants']) {
			foreach ($rule['participants'] as $key) {
				list ($probe_prole_tid, $probe_pstatus_tid) = explode('|', $key);
				if ( ($probe_prole_tid == $prole_tid) && ($probe_pstatus_tid = $pstatus_tid) ) { $found = TRUE; break; }
			}
		}
		if ($found) { break; }
	}

	if (!$found) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_participant_json_view_access($participant_node, $which = 'default') { return sirius_node_tab_event_participant_json_access($participant_node, $which); }
function sirius_node_tab_event_participant_json_edit_access($participant_node, $which = 'default') { return sirius_node_tab_event_participant_json_access($participant_node, $which); }
function sirius_node_tab_event_participant_json_view_intake_access($participant_node, $which = 'intake') { return sirius_node_tab_event_participant_json_access($participant_node, $which); }
function sirius_node_tab_event_participant_json_edit_intake_access($participant_node, $which = 'intake') { return sirius_node_tab_event_participant_json_access($participant_node, $which); }
function sirius_node_tab_event_participant_json_view_intake($participant_node, $which = 'intake') { return sirius_node_tab_event_participant_json_view($participant_node, $which); }
function sirius_node_tab_event_participant_json_edit_intake($participant_node, $which = 'intake') { return sirius_node_tab_event_participant_json_edit($participant_node, $which); }

function sirius_node_tab_event_participant_json_backlinks($participant_node, $event_node) {
	return l('Back to participant list', "node/" . $event_node->nid . "/sirius_event_participants_current", array('attributes' => array('class' => 'tool-button')));
}


function sirius_node_tab_event_participant_json_view($participant_node, $which = 'default') {
	drupal_add_css('sites/all/modules/_custom/sirius_event/css/event.css');

	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	$backlinks = sirius_node_tab_event_participant_json_backlinks($participant_node, $event_node);
	if (sirius_node_tab_event_phonebank_access($event_node)) {
		$backlinks .= l('Direct Call', "node/$event_nid/sirius_event_phonebank/call", array('query' => array('mode' => 'direct', 'return_to' => current_path(), 'phone' => $contact_node->field_sirius_phone['und'][0]['value']), 'attributes' => array('class' => 'tool-button')));
	}
	$html = '<div>' . $backlinks . '</div>';

	$survey_nid = sirius_event_survey_get_nid($event_node, $which);
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, sirius_event_survey_which_results($which));
	$html .= "<div class=\"sirius_event_survey_results\">";
	$html .= sirius_jsonfield_util_render_all_values($survey_form, $participant_results);
	$html .= '</div>';

	return $html;
}

function sirius_node_tab_event_participant_json_edit($participant_node, $which = 'default') {
	return drupal_get_form('sirius_node_tab_event_participant_json_form', $participant_node, $which);
}

function sirius_node_tab_event_participant_json_form($form, &$form_state, $participant_node, $which = 'default') {
	$form_state['participant_nid'] = $participant_node->nid;
	$form_state['which'] = $which;

	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	$survey_nid = sirius_event_survey_get_nid($event_node, $which);
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, sirius_event_survey_which_results($which));
	sirius_jsonfield_util_set_defaults($survey_form, $participant_results);

	$form['#tree'] = TRUE;
	$form[sirius_event_survey_which_results($which)] = $survey_form;

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
	$which = $form_state['which'];
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	sirius_jsonfield_util_results_set($participant_json, sirius_event_survey_which_results($which), $form_state['values'][sirius_event_survey_which_results($which)]);
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	sirius_jsonfield_log($participant_node, sirius_event_survey_which_results($which), 'Survey updated.');
	node_save($participant_node);
}