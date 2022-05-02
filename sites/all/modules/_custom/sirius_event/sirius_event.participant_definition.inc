<?php

function sirius_event_page_participant_definitions() {
	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	return views_embed_view('sirius_json_definitions', 'block_1', $domain_nid, $type);
}

// kludge
function sirius_node_tab_event_participant_definition_edit_access($definition_node) { 
	$type = $definition_node->field_sirius_type['und'][0]['value'];

	$no_form_types = array(
		'sirius_ledger:policy', 
		'sirius:disclaimer', 
		'sirius_event:catalog'
	);
	if (in_array($type, $no_form_types)) { return FALSE; }
	return TRUE; 
}

function sirius_node_tab_event_participant_definition_preview_access($definition_node) { return sirius_node_tab_event_participant_definition_edit_access($definition_node); }
function sirius_node_tab_event_participant_definition_signatures_access($definition_node) { return sirius_node_tab_event_participant_definition_edit_access($definition_node); }
function sirius_node_tab_event_participant_definition_exim_access($definition_node) { return sirius_node_tab_event_participant_definition_edit_access($definition_node); }
function sirius_node_tab_event_participant_definition_preferences_access($definition_node) { return sirius_node_tab_event_participant_definition_edit_access($definition_node); }

function sirius_node_tab_event_participant_definition_edit($definition_node) {
	$contact_json = sirius_contact_definition();
	$contact_form = sirius_jsonfield_util_form_get($contact_json, 'default');
	return drupal_get_form('sirius_json_definition_edit_form', $definition_node->nid, $contact_form);
}

function sirius_node_tab_event_participant_definition_preview($definition_node) {
	return drupal_get_form('sirius_json_definition_preview_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_exim($definition_node) {
	return drupal_get_form('sirius_json_definition_exim_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_signatures($definition_node) {
	return drupal_get_form('sirius_json_definition_signatures_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_preferences($definition_node) {
	return drupal_get_form('sirius_json_definition_preferences_form', $definition_node->nid);
}




function sirius_event_instance_participant_definition_select($event_node) {
	return drupal_get_form('sirius_event_instance_participant_definition_select_form', $event_node);
}

function sirius_event_instance_participant_definition_select_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	$options = array(NULL => t('-- Select --'));
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', $domain_nid, $type);
	if ($avail) {
		foreach ($avail as $hr) {
			$options[$hr->nid] = $hr->node_title;
		}
	}

	$surveys = array('default');

	/*
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	if (sirius_event_rule_widget_exists($event_type_json['register']['rules'], 'intake')) { $surveys[] = 'intake'; }
	*/
	
	$form['#tree'] = TRUE;

	foreach ($surveys as $which) {
		$survey_nid = sirius_event_survey_get_nid($event_node, $which);
		$html = '<div class="sirius_form_intro">';
		$html .= '<h2>' . sirius_event_survey_which_name($which) . '</h2>';
		if ($survey_nid) {
			$survey_node = node_load($survey_nid);
			$html .= 'The current survey is: <strong>' . $survey_node->title . '</strong>. ';
			$paths = array(
				"node/$survey_nid/sirius_event_participant_definition_form/preview" => 'Preview', 
				"node/$survey_nid/sirius_event_participant_definition_form" => 'Edit Fields', 
			);
			foreach ($paths as $path => $title) {
				if (!drupal_valid_path($path)) { continue; }
				$html .= l($title, $path, array('attributes' => array('class' => array('tool-button')))) . ' ';
			}
		}
		$html .= '</div>';

		$form['survey'][$which] = array(
			'#type' => 'select',
			'#title' => 'Survey',
			'#options' => $options,
			'#default_value' => $survey_nid,
			'#prefix' => $html,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	return $form;
}

function sirius_event_instance_participant_definition_select_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$event_json['event']['survey'] = $form_state['values']['survey'];
	sirius_jsonfield_util_node_json_set($event_node, $event_json);
	node_save($event_node);
}
