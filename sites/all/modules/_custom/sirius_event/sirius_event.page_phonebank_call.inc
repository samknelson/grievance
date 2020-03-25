<?php

function sirius_node_tab_event_phonebank_call_access($event_node) {
	if (!sirius_node_tab_event_phonebank_access($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$caller_prole_tids = array_values($type_json['phonebank']['caller_prole_tids']);
	if (!in_array($prole_tid, $caller_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_phonebank_call($event_node) {
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_tid = $json['phonebank']['pstatus_tid'];
	if (!$prole_tid || !$pstatus_tid) {
		return 'Please go to the settings page and select the role and status to call.';
	}


	$mode = $_REQUEST['mode'];
	if (!$mode) {
		// Release locks on this page, because we're not in the caller interface
		global $user;
		$lock_type = sirius_event_phonebank_lock_type($event_node);
		sirius_lock_release_by_uid($lock_type, $user->uid);

		$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);
		$html .= $json['phonebank']['caller_intro'];
		$html .= '<br /><br />';
		$html .= l(t('Dry Run'), current_path(),  array('query' => array('mode' => 'test'), 'attributes' => array('class' => array('tool-button'))));
		if ($json['phonebank']['active'] == 'Yes') { 
			$html .= ' / ';
			$html .= l(t('Start Making Calls'), current_path(),  array('query' => array('mode' => 'live'), 'attributes' => array('class' => array('tool-button'))));
		}
		return $html;
	}

	// Get the participant
	if ($mode == 'live' && $json['phonebank']['active'] == 'Yes') {
		$phone = sirius_event_phonebank_next_number($event_node, $prole_tid, $pstatus_tid);
		if (!$phone) { return 'There are no more calls to make!'; }
	} 
	return drupal_get_form('sirius_node_tab_event_phonebank_call_form', $event_node->nid, $phone);
}

function sirius_node_tab_event_phonebank_call_form($form, &$form_state, $event_nid, $phone) {
	$event_node = node_load($event_nid);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_tid = $json['phonebank']['pstatus_tid'];
	$participant_nodes = sirius_event_phonebank_participants_by_phone($event_node, $prole_tid, $phone);

	// If we changed participant, throw away our survey values.
	if ($form_state['triggering_element']['#ajax']['callback'] == 'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient') {
		unset($form_state['values']['survey']);
		unset($form_state['input']['survey']);
	}

	//drupal_add_js(drupal_get_path('module', 'sirius_event') . '/js/sirius_event.phonebank.js', 'file');
	//drupal_add_js(array('sirius_event_phonebank' => array('participant_nid' => $participant_nid)), array('type' => 'setting'));
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/sirius_event.phonebank.css', 'file');

	$form_state['event_nid'] = $event_nid;
	$form_state['phone'] = $phone;

	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { drupal_set_message("No survey!"); return array(); }
	$survey_form = json_decode($survey_node->field_sirius_json['und'][0]['value'], TRUE)['form']['default'];

	$participant_nid = $form_state['values']['participant_nid'];
	if (!$participant_nid) { 
		$participant_nid = array_keys($participant_nodes)[0];
	}

	$participant_node = $participant_nodes[$participant_nid];
	$participant_prole = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$participant_json = json_decode($participant_node->field_sirius_json['und'][0]['value'], TRUE);
	$event_node = node_load($event_nid);
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	sirius_event_phonebank_prep_survey($survey_form, $participant_json['survey']);
	$pstatuses = sirius_event_participant_role_status_tids($participant_prole);

	$form['#tree'] = TRUE;
	$form['close1'] = array('#markup' => '<div id="phonebank_wrapper">' . l('Close', current_path(), array('attributes' => array('class' => array('tool-button', 'phonebank-close-button')))));
	$form['prefix'] = array('#markup' => '<div class="sirius-form" id="sirius_event_phonebank_survey"><table><tr>');
	$form['prefix_col_1'] = array('#markup' => '<td valign="top" style="vertical-align: top;">');
	$form['survey'] = $survey_form;

	$form['survey']['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap">', '#weight' => -9999);
	$form['survey']['survey_wrap_close'] = array('#markup' => '</div>', '#weight' => 9999);
	$form['suffix_col_1'] = array('#markup' => '</td>');

	$form['prefix_col_2'] = array('#markup' => '<td width="35%" valign="top" style="vertical-align: top; border-left: 1px solid #333333; padding-left: 10px;">');
	$intro_html = '<div class="sirius_form_intro">';
	if ($contact_node) {
		$intro_html .= sirius_fakerender_field('Speaking with:', l($contact_node->title, 'node/' . $participant_nid, array('attributes' => array('target' => '_blank'))));
		$contact_json = json_decode($contact_node->field_sirius_json['und'][0]['value'], TRUE);
		$contact_definition = sirius_contact_definition();

		$display_fields = sirius_contact_definition_display_get($contact_definition, 'phonebank-default');
		sirius_jsonfield_util_add_node_fields($contact_node, $contact_definition['form']['default'], $contact_json['survey']);
		$intro_html .= sirius_jsonfield_util_render_values($contact_definition['form']['default'], $contact_json['survey'], $display_fields, array('show_title' => TRUE));
	}
	$intro_html .= sirius_fakerender_field('Phone:', $phone);
	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => $intro_html);


	$participant_options = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
		$contact_node = node_load($contact_nid);
		$participant_options[$participant_nid] = $contact_node->title;
	}

	if (count($participant_options) > 1) {
		$form['participant_nid'] = array(
			'#type' => 'select',
			'#title' => 'Participant',
			'#options' => $participant_options,
			'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient'),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
	} else {
		$form['participant_nid'] = array(
			'#type' => 'value',
			'#value' => array_keys($participant_options)[0],
		);
	}

	$browserphone_params = array();
	$browserphone_params['default_phone'] = $phone;
	$browserphone_params['callerid_nids'] = sirius_fieldvals($survey_node, 'field_sirius_callerids', 'target_id');
	$browserphone_params['omg_nid'] = $survey_node->nid;
	$form['phone_interface_html'] = array('#markup' => sirius_twilio_browserphone_render($browserphone_params));

	if ($participant_nid) {
		$form['pstatus'] = array(
			'#type' => 'radios',
			'#options' => $pstatuses,
			'#title' => 'Status',
			'#default_value' => $participant_node->field_sirius_event_pstatus['und'][0]['tid'],
			'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax'),
		);
	}

	$form['submit_next'] = array(
		'#type' => 'submit',
		'#value' => t('Save and Next'),
	);

	$form['suffix_col_2'] = array('#markup' => '</td');
	$form['suffix'] = array('#markup' => '</tr></table></div>');

	$form['close2'] = array('#markup' => l('Close', current_path(), array('attributes' => array('class' => array('tool-button', 'phonebank-close-button')))) . '</div>');

	return $form;
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#survey_wrap", render($form['survey']));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax($form, $form_state) {
	global $user;

	$participant_nid = $form_state['values']['participant_nid'];
	if (!$participant_nid) { return; }
	$participant_node = node_load($participant_nid);
	$json = json_decode($participant_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['survey'] = $form_state['values']['survey'];
	$participant_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	sirius_jsonfield_log($participant_node, 'survey', 'Survey submitted.');	
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $form_state['values']['pstatus'];
	node_save($participant_node);

	// Set linked fields, formerly known as "candidate-issue"
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	$survey_json = json_decode($survey_node->field_sirius_json['und'][0]['value'], TRUE);
	$contact_json = json_decode($contact_node->field_sirius_json['und'][0]['value'], TRUE);
	$count = sirius_jsonfield_link_set($survey_json, $json['survey'], $contact_json);
	if ($count) {
		$contact_node->field_sirius_json['und'][0]['value'] = json_encode($contact_json);
		node_save($contact_node);
	}

	return array('#type' => 'ajax', '#commands' => array());
}

function sirius_node_tab_event_phonebank_call_form_submit($form, &$form_state) {
	global $user;
	$lock_type = sirius_event_phonebank_lock_type($event_node);
	sirius_lock_release_by_uid($lock_type, $user->uid);
}

function sirius_event_phonebank_prep_survey(&$form, $defaults) {
	if (!is_array($form)) { return; }
	foreach ($form as $key => $value) {
		if (!is_array($form[$key])) { continue; }
		
		$type = $form[$key]['#type'];
		if ($type) {
			if ($type == 'select' || $type == 'textfield' || $type == 'textarea' || $type == 'date_popup' || $type == 'radios' || $type == 'checkboxes')  {
				// unset($form[$key]['#markup']);
				$form[$key]['#ajax'] = array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax');
				if (isset($defaults[$key])) {
					$form[$key]['#default_value'] = $defaults[$key];
					if ($type == 'date_popup') { $form[$key]['#default_value'] = date('Y-m-d H:i:s', strtotime($defaults[$key]['date'] . ' ' . $defaults[$key]['time'])); }
					else { $form[$key]['#default_value'] = $defaults[$key]; }
				}
			}
		} else {
			sirius_event_phonebank_prep_survey($form[$key], $defaults[$key]);
		}
	}
}
