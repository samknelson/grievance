<?php

function sirius_event_register_user_self_form($form, &$form_state, $event_node, $rule) {
	drupal_add_css('sites/all/modules/_custom/sirius_event/css/event.css');

	$form_state['event_nid'] = $event_node->nid;
	$form_state['rule'] = $rule;
	$form_state['participant_role_tid'] = $participant_role_tid;

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$widget = 'user_self';
	$widget_settings = $event_type_json['widget_settings']['register'][$rule['delta']][$widget];

	// Already registered?
	$contact_nid = 	sirius_contact_nid_get_from_user();
	$participant_node = sirius_event_participant_get($event_node->nid, $contact_nid);

	$html = '<div class="sirius_form_intro">';
	$html .= sirius_node_tab_event_info($event_node); 
  $html .= '</div>';

  $form['#tree'] = TRUE;
	$form['intro'] = array('#markup' => $html);

	$domain_nid = sirius_domain_nid();
	$pstatus_tids = sirius_event_participant_role_status_tids($prole_tid);

	$prole_tids = array();
	foreach ($rule['register_as'] as $key) {
		list ($prole_tid, $pstatus_tid) = explode('|', $key);
		$prole_term = taxonomy_term_load($prole_tid);
		$pstatus_term = taxonomy_term_load($pstatus_tid);
		$register_as_options[$key] = $prole_term->name . ': ' . $pstatus_term->name;
		$prole_tids[$prole_tid] = TRUE;
	}

	if ($participant_node) {
		$form['participant_intro'] = array('#markup' => '<strong>' . t('You are registered for this event.') . '</strong>');
	} else {
		$form['register_as'] = array(
			'#title' => 'Register as',
			'#type' => 'select',
			'#options' => $register_as_options,
		);
		if (count($register_as_options) == 1) {
			$form['register_as']['#type'] = 'hidden';
			$form['register_as']['#value'] = array_keys($register_as_options)[0];
		}

		if (sirius_event_disclaimer_enabled($event_node)) {
			$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
			$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);
			if ($disclaimer_nodes) {
				if ($event_disclaimer_json['prole_tids'] && array_keys($prole_tids) && array_intersect(array_keys($prole_tids), array_filter(array_values($event_disclaimer_json['prole_tids'])))) {
					foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
						$disclaimer_json = sirius_jsonfield_util_node_json_get($disclaimer_node)['disclaimer'];
						$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json);			
					}
				}
			}
		}

		if (sirius_event_section_enabled($event_node)) {
			$counts = sirius_event_section_counts($event_node);
			$options_attributes = array();
			$options = sirius_event_section_options($event_node, $counts, $options_attributes);

			$form['#tree'] = TRUE;

			$form['section']['uuid'] = array(
				'#type' => 'select',
				'#options' => $options,
				'#title' => t('Section'),
				'#default_value' => $participant_json['section']['uuid'],
				'#options_attributes' => $options_attributes,
			);

			foreach ($options as $uuid => $title) {
				$section = $event_json['section_list'][$uuid];
				if (!$section || !$section['comments']) { continue; }
				$form['section'][$uuid]['comments'] = array(
					'#type' => 'item',
					'#markup' => '<strong>' . sirius_event_section_title($section) . ':</strong> ' . $section['comments'],
					'#states' => array('visible' => array(":input[name=\"section[uuid]\"]" => array('value' => $uuid))),
				);
			}

		}

		$form['participant']['comments'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
		);

		$submit_name = $widget_settings['name'];
		if (!$submit_name) { $submit_name = 'Register'; }
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => $submit_name,
		);
	}

	return $form;
}

function sirius_event_register_user_self_form_submit($form, &$form_state) {
	$rule = $form_state['rule'];
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$widget_settings = $event_type_json['widget_settings']['register'][$rule['delta']]['user_self'];

	$contact_nid = 	sirius_contact_nid_get_from_user();
	if (!$contact_nid) {
		drupal_set_message("No contact associated with this user", 'error');
		return;
	}

	$register_as = $form_state['values']['register_as'];
	list ($prole_tid, $pstatus_tid) = explode('|', $register_as);
	$prole_term = taxonomy_term_load($prole_tid);
	$pstatus_term = taxonomy_term_load($pstatus_tid);

	$result = sirius_event_participant_register(
		$event_nid, 
		$contact_nid, 
		$prole_tid, 
		$pstatus_tid,
		$form_state['values']['participant']
	);

	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error'); 
		return;
	}

	if ($form_state['values']['disclaimer']) {
		$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
		$participant_node = node_load($result['nid']);
		sirius_disclaimer_accept($participant_node, $disclaimer_nodes, array());
	}

	if ($form_state['values']['section']['uuid']) {
		$result = sirius_event_section_assign($participant_node->nid, $form_state['values']['section']['uuid']);
		if (!$result['success']) { drupal_set_message($result['msg'], 'warning'); }
	}


	$confirmation_text = $widget_settings['confirmation_text'];
	if (!$confirmation_text) { $confirmation_text = $result['msg']; }
	drupal_set_message($confirmation_text);
	drupal_goto('node/' . $event_node->nid . '/sirius_event_info');
}


function sirius_event_register_user_self_config_form($form, &$form_state, $event_type_term, $rule_type, $delta, $widget) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$form_state['rule_type'] = $rule_type;
	$form_state['delta'] = $delta;
	$form_state['widget'] = $widget;

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$form['#tree'] = TRUE;
	$form['settings']['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Widget Name',
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['name'],
	);

	$form['settings']['confirmation_text'] = array(
		'#type' => 'textarea',
		'#title' => 'Confirmation Text',
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['confirmation_text'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_register_user_self_config_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$rule_type = $form_state['rule_type'];
	$delta = $form_state['delta'];
	$widget = $form_state['widget'];
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$event_type_json['widget_settings'][$rule_type][$delta][$widget] = $form_state['values']['settings'];
	sirius_jsonfield_util_term_json_set($event_type_term, $event_type_json);
	taxonomy_term_save($event_type_term);
}
