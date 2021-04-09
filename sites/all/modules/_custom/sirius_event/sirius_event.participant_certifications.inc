<?php

function sirius_event_page_participant_certifications($event_node, $rule) {
	return drupal_get_form('sirius_event_page_participant_certifications_form', $event_node, $rule);
}

function sirius_event_page_participant_certifications_form($form, &$form_state, $event_node, $rule) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['rule'] = $rule;

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	if (!$event_type_json['class']['certifications']['available']) { drupal_set_message('No certifications have been configured for this event type.', 'error'); return;; }
	$certification_definition_nodes = node_load_multiple(array_values($event_type_json['class']['certifications']['available']));
	if (!$certification_definition_nodes) { drupal_set_message('No certifications found for this event type.', 'error'); }

	// Get the participants
	$participant_nids = sirius_event_participant_list_by_rule($event_node->nid, $rule);
	$participant_nodes = node_load_multiple($participant_nids);

	$header = array('Name', 'Badge',);

	foreach ($certification_definition_nodes as $certification_definition_nid => $certification_definition_node) {
		$header[] = $certification_definition_node->title;
		$header[] = t('Tools');
	}

	$rows = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$worker_node = sirius_worker_get_by_contact_nid($participant_node->field_sirius_contact['und'][0]['target_id']);
		if (!$worker_node) { continue; }
		$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
		$esignature = sirius_jsonfield_util_esignature_get($participant_json, $which);
		$row = array();
		$row[] = $worker_node->title;
		$row[] = $worker_node->field_sirius_id['und'][0]['value'];

		// Load all the certifications for this participant
		$certification_nodes = sirius_training_certifications_fetch_by_participant($participant_nid);
	  $certification_nodes_by_definition = array();
	  foreach ($certification_nodes as $certification_nid => $certification_node) {
	  	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	  	foreach ($handler_nodes as $handler_nid => $handler_node) {
	  		if ($handler_node->type == 'sirius_json_definition') {
	  			$certification_nodes_by_definition[$handler_node->nid][] = $certification_node;
	  		}
	  	}
	  }

		foreach ($certification_definition_nodes as $certification_definition_nid => $certification_definition_node) {
			$paths = array();
			if ($certification_nodes_by_definition[$certification_definition_nid]) {
				$certification_node = $certification_nodes_by_definition[$certification_definition_nid][0];
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_view'] = 'View';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_edit'] = 'Edit';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_signatures'] = 'Signatures';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_grant'] = 'Grant';
				$title = $certification_node->field_sirius_type['und'][0]['value'];
			} else {
				$certification_node = '';
				$title = '[incomplete]';
				$paths['sirius/training/certification/' . $worker_node->nid . '/' . $certification_definition_nid . '/' . $participant_node->nid] = 'Add';
			}

			$tools = '';
			foreach ($paths as $path => $tool_title) {
				if (drupal_valid_path($path)) {
					$tools .= l($tool_title, $path, array('attributes' => array('class' => 'tool-button')));
				}
			}

			$row[] = $title;
			$row[] = $tools;
		}

		$options[$participant_nid] = $row;
	}

	$form['participant_nids'] = array(
		'#type' => 'tableselect',
		'#title' => 'Participants',
		'#header' => $header,
		'#options' => $options,
		'#required' => TRUE,
	);


	$confirm_html = '<p>';
	$confirm_html .= t('When the button below is clicked, the following actions will be taken on all the selected participants:');
	$confirm_html .= '<ul>';
	$confirm_html .= '<li>' . t('All participants will be set to the status for granted certifications (usually "Passed")') . '</li>';
	$confirm_html .= '<li>' . t('All certifications for each selected participant will be granted') . '</li>';
	$confirm_html .= '<li>' . t('All the skill associated with the granted certifications will be added or renewed') . '</li>';
	$confirm_html .= '</ul>';

	$form['#tree'] = TRUE;
	$form['confirm'] = array(
		'#type' => 'checkbox', 
		'#required' => TRUE,
		'#prefix' => $confirm_html,
		'#title' => t('Yes, perform all these actions on the selected participants.'),
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_page_participant_certifications_form_submit($form, &$form_state) {
	$participant_nids = array_values($form_state['values']['participant_nids']);

	foreach ($participant_nids as $participant_nid) {
		if (!$participant_nid) { continue; }
		$certification_nodes = sirius_training_certifications_fetch_by_participant($participant_nid);
		foreach ($certification_nodes as $certification_nid => $certification_node) {
			$result = sirius_training_certification_grant($certification_node);
			if ($result['success']) { 
				// drupal_set_message($result['msg']);
			} else { 
				drupal_set_message($result['msg'], 'warning');
			}
		}
	}

	drupal_set_message(sirius_minilog_render_html());
}
