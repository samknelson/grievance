<?php

function sirius_event_page_participant_definitions() {
	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	return views_embed_view('sirius_json_definitions', 'block_1', $domain_nid, $type);
}

// kludge
function sirius_node_tab_event_participant_definition_edit_access($definition_node) { return ($definition_node->field_sirius_type['und'][0]['value'] != 'sirius_ledger:policy'); }
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

function sirius_event_instance_participant_definition($event_node) {
	drupal_goto(current_path() . '/select');
}

function sirius_event_instance_participant_definition_select($event_node) {
	return drupal_get_form('sirius_event_instance_participant_definition_select_form', $event_node);
}

function sirius_event_instance_participant_definition_select_form($form, &$form_state, $event_node) {
	$form_state['event_node'] = $event_node;
	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	$options = array(NULL => t('-- Select --'));
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', $domain_nid, $type);
	if ($avail) {
		foreach ($avail as $hr) {
			$options[$hr->nid] = $hr->node_title;
		}
	}

	$default_json_definition_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	if ($default_json_definition_nid) {
		$default_json_definition_node = node_load($default_json_definition_nid);
		$html = '<div class="sirius_form_intro">';
		$html .= 'The current survey is: <strong>' . $default_json_definition_node->title . '</strong>. ';
		$paths = array(
			"node/$default_json_definition_nid/sirius_event_participant_definition_edit" => 'Edit Fields', 
			"node/$default_json_definition_nid/sirius_event_participant_definition_preview" => 'Preview', 
		);
		foreach ($paths as $path => $title) {
			if (!drupal_valid_path($path)) { continue; }
			$html .= l($title, $path, array('attributes' => array('class' => array('tool-button')))) . ' ';
		}
		$html .= '</div>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['field_sirius_json_definition'] = array(
		'#type' => 'select',
		'#title' => 'Survey',
		'#options' => $options,
		'#default_value' => $event_node->field_sirius_json_definition['und'][0]['target_id'],
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	return $form;
}

function sirius_event_instance_participant_definition_select_form_submit($form, &$form_state) {
	$event_node = $form_state['event_node'];
	if ($form_state['values']['field_sirius_json_definition']) {
		$event_node->field_sirius_json_definition['und'][0]['target_id'] = $form_state['values']['field_sirius_json_definition'];
	} else {
		unset($event_node->field_sirius_json_definition['und']);
	}
	node_save($event_node);
}

function sirius_event_instance_participant_definition_preview($event_node) {
	$definition_node = node_load($event_node->field_sirius_json_definition['und'][0]['target_id']);
	if (!$definition_node) {
		drupal_set_message('There is no survey selected for this event.', 'warning');
		return '';
	}

	return drupal_get_form('sirius_json_definition_preview_form', $definition_node->nid);
}

