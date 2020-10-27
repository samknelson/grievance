<?php

function sirius_event_register_user_self_form($form, &$form_state, $event_node, $rule) {
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
  if (sirius_node_tab_event_bullpen_view_access($event_node)) {
		$html .= sirius_node_tab_event_bullpen_view($event_node);
  } else {
    $renderable = node_view($event_node, 'full');
    $html .= drupal_render($renderable);
  }
  $html .= '</div>';

  $form['#tree'] = TRUE;
	$form['intro'] = array('#markup' => $html);

	$domain_nid = sirius_domain_nid();
	$pstatus_tids = sirius_event_participant_role_status_tids($prole_tid);

	foreach ($rule['register_as'] as $key) {
		list ($prole_tid, $pstatus_tid) = explode('|', $key);
		$prole_term = taxonomy_term_load($prole_tid);
		$pstatus_term = taxonomy_term_load($pstatus_tid);
		$register_as_options[$key] = $prole_term->name . ': ' . $pstatus_term->name;
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

	if ($result['success']) { 
		$confirmation_text = $widget_settings['confirmation_text'];
		if (!$confirmation_text) { $confirmation_text = $result['msg']; }
		drupal_set_message($confirmation_text);
	} else { 
		drupal_set_message($result['msg'], 'error'); 
		return;
	}
}
