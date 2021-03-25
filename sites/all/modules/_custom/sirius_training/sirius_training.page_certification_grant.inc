<?php

function sirius_training_node_tab_certification_grant_access($certification_node) {
	// Only log entries of type certification
	if ($certification_node->field_sirius_category['und'][0]['value'] != 'worker:certification') { return FALSE; }

	// Admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Author can access, but only in "signed" mode.
	global $user;
	$status = $certification_node->field_sirius_type['und'][0]['value'];
	if ($user->uid == $certification_node->uid && $status == 'signed') {
		return TRUE;
	}

	// Oh well
	return FALSE;
}

function sirius_training_node_tab_certification_grant($certification_node) {
	return drupal_get_form('sirius_training_node_tab_certification_grant_form', $certification_node);
}

function sirius_training_node_tab_certification_grant_form($form, &$form_state, $certification_node) {
	sirius_esignature_init();
	$form_state['certification_nid'] = $certification_node->nid;
	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_event_participant') {
			$participant_node = $handler_node;
			$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
		} else if ($handler_node->type == 'sirius_json_definition') {
			$certification_definition_node = $handler_node;
		}
	}

	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$survey_form = sirius_jsonfield_util_form_get($json, 'certification');
	$results = sirius_jsonfield_util_results_get($json, 'certification');
	sirius_jsonfield_util_set_defaults($survey_form, $results);

	$form['#tree'] = TRUE;

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . sirius_training_node_tab_certification_backlinks($certification_node) . '<br /><br />' . '</div><div class="sirius-form">',
	);
	
	$form['status_intro'] = array('#markup' => '<hr><h2>' . t('Certification Status') . '</h2>');

	$form['certification_status'] = array(
		'#type' => 'select',
		'#title' => t('Set the certification status to:'),
		'#options' => sirius_training_certification_status_options(),
		'#default_value' => $certification_node->field_sirius_type['und'][0]['value'],
		'#required' => TRUE,
	);

	$worker_node = sirius_worker_get_by_contact_nid($participant_node->field_sirius_contact['und'][0]['target_id']);
	if ($worker_node && $certification_definition_node) {
		$certification_definition_json = sirius_jsonfield_util_node_json_get($certification_definition_node);
		if ($certification_definition_json['certification']['skills']) {
			$skill_terms = taxonomy_term_load_multiple(array_values($certification_definition_json['certification']['skills']));
			$html = '<hr><h2>' . t('Skills') . '</h2>';

			$rows = array();
			$header = array(t('Skill'), t('Expires'));

			if ($worker_node->field_sirius_skill['und']) {
				foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
					$skill_tid = $tmp['tid'];
					if (!in_array($skill_tid, array_keys($skill_terms))) { continue; }
					$skill_term = $skill_terms[$skill_tid];

					$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
					if (!$expires_ts || $expires_ts > time() + 60*60*24*365*50) {
						$expires = sirius_domain_variable_get('sirius_worker_skill_never_expires_text', 'Never Expires');
					} else {
						$expires = t('Expires: @date', array('@date' => date('m/d/Y', $expires_ts)));
						$expires .= ' (' . format_interval($expires_ts - time(), 2) . t(' remaining') . ')';
					}

					$row = array();
					$row[] = $skill_term->name;
					$row[] = $expires;
					$rows[] = $row;
				}
			}

			if ($rows) {
				$html .= 'The worker currently has the following skills associated with this certification:';
				$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
			} else {
				$html .= 'The worker does not currently have any of the skills associated with this certification.';
			}

			$form['skills_intro'] = array('#markup' => $html);




			foreach ($skill_terms as $skill_tid => $skill_term) {
				$form['skills'][$skill_tid] = array(
					'#type' => 'select',
					'#title' => $skill_term->name,
					'#options' => array('grant' => t('Grant or Renew'), 'skip' => t('Skip')),
				);
			}
		}
	}

	if ($participant_node) {
		$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
		$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
		$pstatus_options = sirius_event_participant_role_status_tids($prole_tid);

		$html = '<hr><h2>' . t('Course Status') . '</h2>';
		$html .= "The attendance record for this participant is below.";
		$html .= sirius_training_node_tab_certification_grant_get_hours_display_this($event_node, $participant_node);
		$form['course_intro'] = array('#markup' => $html);

		$form['participant_status'] = array(
			'#type' => 'select',
			'#title' => t('Based on the participant\'s attendance and success in the class, set their course status to:'),
			'#options' => $pstatus_options,
			'#default_value' => $pstatus_tid,
			'#required' => TRUE,
		);
	}


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

function sirius_training_node_tab_certification_grant_form_submit($form, &$form_state) {
	$certification_node = node_load($form_state['certification_nid']);
	$form_state['rebuild'] = TRUE;
	$result = sirius_training_certification_grant($certification_node, $form_state['values']);
	if ($result['success']) { 
		// drupal_set_message($result['msg']);
	} else { 
		drupal_set_message($result['msg'], 'warning');
	}
	drupal_set_message(sirius_minilog_render_html());
}

function sirius_training_node_tab_certification_grant_submit($form, &$form_state) {
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

/**
 * @todo: Refactor this code! It's copy-pasted from the sirius_training.page_worker_classes.
 */

function sirius_training_node_tab_certification_grant_get_hours_display_this($event_node, $participant_node) {
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date', 'Y-m-d');
	$rows = array();
	$hours = array();
	foreach ($dates as $delta => $date) {
		$which = 'attendance_' . $delta;
		$duration_default = strtotime($event_node->field_sirius_daterepeat['und'][$delta]['value2']) - strtotime($event_node->field_sirius_daterepeat['und'][$delta]['value']);
		$duration = $participant_json[$which]['data']['hours'];
		if (!isset($duration)) {
			$duration_render = sirius_event_duration_format_hours($duration_default) . ' (est.)';
			$total_hours['total'] += $duration_default;
			$total_hours['est'] += $duration_default;
		} else {
			$duration_render = sirius_event_duration_format_hours($duration);
			$total_hours['total'] += $duration;
			$total_hours['confirmed'] += $duration;
		}
		$esignature = sirius_jsonfield_util_esignature_get($participant_json, $which, NULL);
		$row = array();
		$row[] = $date;
		$row[] = $duration_render;
		$row[] = sirius_esignature_render($esignature, '', $participant_node->nid, $which);
		$row[] = sirius_text_to_html($participant_json[$which]['data']['comments']);
		$rows[] = $row;
	}

	$total_render = sirius_event_duration_format_hours($total_hours['total']);
	if ($total_hours['est'] > 0) { $total_render .= " (" . sirius_event_duration_format_hours($total_hours['est']) . ' est.)'; }
	$row = array();
	$row[] = '<strong>' . t('Total') . '</strong>';
	$row[] = $total_render;
	$row[] = '';
	$row[] = '';
	$rows[] = $row;

	$header = array('Date', 'Hours', 'Signature', 'Notes');

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}

function sirius_training_node_tab_certification_grant_get_hours_display_all($this_event_node, $this_participant_node) {
	// Get the event type
	$event_type_tid = $this_event_node->field_sirius_event_type['und'][0]['tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);

	$all_events = sirius_event_participants_by_contact($this_participant_node->field_sirius_contact['und'][0]['target_id'], array($event_type_tid));
	return "This class: " . $this_event_node->title . " / all classes of type " . $event_type_term->name;
}
