<?php

function sirius_training_node_tab_certification_edit_access($certification_node) {
	// Only log entries of type certification
	if ($certification_node->field_sirius_category['und'][0]['value'] != 'worker:certification') { return FALSE; }

	// Admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Author can access, but only in draft mode.
	global $user;
	$status = $certification_node->field_sirius_type['und'][0]['value'];
	if ($user->uid == $certification_node->uid && $status == 'draft') {
		return TRUE;
	}

	// Oh well
	return FALSE;
}

function sirius_training_node_tab_certification_edit($certification_node) {
	return drupal_get_form('sirius_training_node_tab_certification_edit_form', $certification_node);
}

function sirius_training_node_tab_certification_edit_form($form, &$form_state, $certification_node) {
	$form_state['certification_nid'] = $certification_node->nid;

	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$survey_form = sirius_jsonfield_util_form_get($json, 'certification');
	$results = sirius_jsonfield_util_results_get($json, 'certification');
	sirius_jsonfield_util_set_defaults($survey_form, $results);

	$form['#tree'] = TRUE;
	$form['survey'] = $survey_form;

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . sirius_training_node_tab_certification_backlinks($certification_node) . '<br /><br />' . '</div><div class="sirius-form">',
		'#weight' => -10000,
	);
	
	if (user_access('sirius training admin')) {
		$form['status'] = array(
			'#type' => 'select',
			'#title' => t('Status'),
			'#options' => sirius_training_certification_status_options(),
			'#default_value' => $certification_node->field_sirius_type['und'][0]['value'],
			'#required' => TRUE,
			'#weight' => -9999,
		);
	}

	$intro = sirius_domain_variable_get('sirius_training_certifications_form_intro', '');
	if ($intro && $intro['value']) {
		$form['intro'] = array(
			'#markup' => $intro['value'],
			'#weight' => -9998,
		);
	}

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
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

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_training_node_tab_certification_edit_form_submit($form, &$form_state) {
	$certification_nid = $form_state['certification_nid'];
	$certification_node = node_load($certification_nid);

	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$original_results = sirius_jsonfield_util_results_get($json, 'certification');
	sirius_jsonfield_util_results_set($json, 'certification', $form_state['values']['survey']);
	sirius_jsonfield_util_node_json_set($certification_node, $json);
	sirius_jsonfield_log($certification_node, 'edit', 'Edited', array('original' => $original_results));
	if ($form_state['values']['status']) {
		$certification_node->field_sirius_type['und'][0]['value'] = $form_state['values']['status'];
	}
	node_save($certification_node);
	drupal_set_message("Certification saved.");
}

function sirius_training_node_tab_certification_backlinks($certification_node) {
	$html = '';
	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_worker') {
			if (drupal_valid_path("node/$handler_nid/sirius_worker_tc/certifications")) {
				$html .= l('Back to Worker Certifications', "node/$handler_nid/sirius_worker_tc/certifications", array('attributes' => array('class' => 'tool-button'), 'query' => array('status' => $status)));
			}
		}
		else if ($handler_node->type == 'sirius_event_participant') {
			$event_nid = $handler_node->field_sirius_event['und'][0]['target_id'];
			if (drupal_valid_path("node/$event_nid/sirius_event_participants_current")) {
				$html .= l('Back to Participant List', "node/$event_nid/sirius_event_participants_current", array('attributes' => array('class' => 'tool-button')));
			}
		}
	}
	return $html;	
}