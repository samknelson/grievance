<?php

function sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#survey_wrap", render($form['survey']));
  $commands[] = ajax_command_replace("#pstatus_wrap", render($form['pstatus']));
  $commands[] = ajax_command_replace("#intro_wrap", render($form['intro']));
  $commands[] = ajax_command_replace("#submit_wrap", render($form['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#submit_wrap", render($form['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax_close($form, $form_state) {
	ctools_include('ajax'); 
	ctools_add_js('ajax-responder');
	$return_to = $form_state['values']['return_to'];
	$commands = array();
	$commands[] = ctools_ajax_command_redirect($return_to);
	return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax_next($form, $form_state) {
	ctools_include('ajax'); 
	ctools_add_js('ajax-responder');
	$return_to = $form_state['values']['return_to'];
	$mode = $form_state['values']['mode'];
	$commands = array();
	$commands[] = ctools_ajax_command_redirect($return_to, 0, array('query' => array('mode' => $mode)));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form($form, &$form_state, $event_nid, $phone, $mode) {
	/////////////////////////////////////////////////////////////////////
	// Setup
	/////////////////////////////////////////////////////////////////////

	global $user;

	drupal_add_js(drupal_get_path('module', 'sirius_event') . '/js/sirius_event.phonebank.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/sirius_event.phonebank.css', 'file');

	$form_state['event_nid'] = $event_nid;
	$form_state['phone'] = $phone;

	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_tid_to_call = $pstatus_tid = $event_json['phonebank']['pstatus_tid'];
	$participant_nodes = sirius_event_phonebank_participants_by_phone($event_node, $prole_tid, $phone);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { drupal_set_message("No survey!"); return array(); }
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$return_to = $_REQUEST['return_to'];
	if (!$return_to) { $return_to = current_path(); }
	$form['return_to'] = array('#type' => 'hidden', '#value' => $return_to);
	$form['mode'] = array('#type' => 'hidden', '#value' => $mode);

	// The participant NID may change throughout the life of this function. We start with the value in "survey_participant_nid"
	$participant_nid = $form_state['values']['survey_participant_nid'];
	if (!$participant_nid) { $participant_nid = array_keys($participant_nodes)[0]; }
	$form['survey_participant_nid'] = array('#type' => 'hidden', '#value' => $participant_nid);

	$participant_node = $participant_nodes[$participant_nid];
	$participant_json = 	sirius_jsonfield_util_node_json_get($participant_node);
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');

	/////////////////////////////////////////////////////////////////////
	// Processing 
	/////////////////////////////////////////////////////////////////////

	$trigger_callback = $form_state['triggering_element']['#ajax']['callback'];
	if ($participant_node && in_array($trigger_callback, array(
			'sirius_node_tab_event_phonebank_call_form_submit_ajax', 
			'sirius_node_tab_event_phonebank_call_form_submit_ajax_close',
			'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient',
			'sirius_node_tab_event_phonebank_call_form_submit_ajax_next')
			)
		) {
		$participant_nid = $form_state['values']['participant_nid'];
		sirius_jsonfield_util_results_set($participant_json, 'survey', $form_state['values']['survey']);

		// Set the caller;
		$old_caller = $participant_json['phonebank']['caller'];
		$new_caller_uid = $form_state['values']['caller'];
		if ($new_caller_uid && ($new_caller_uid != $old_caller['uid'])) {
			$new_caller_user = user_load($new_caller_uid);
			$new_caller = array(
				'uid' => $new_caller_uid,
				'name' => $new_caller_user->name,
				'mail' => $new_caller_user->mail,
				'phone' => $new_caller_user->field_sirius_phone['und'][0]['value'],
			);
			$participant_json['phonebank']['caller'] = $new_caller;
		}
		if (!$participant_json['phonebank']['first_call']) {
			$participant_json['phonebank']['first_call'] = array(
				'ts' => time(),
				'date' => date('l, F d Y - g:ia'),
			);
		}
		$participant_json['phonebank']['last_call'] = array(
			'ts' => time(),
			'date' => date('l, F d Y - g:ia'),
		);
		$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $form_state['values']['pstatus'];

		sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
		node_save($participant_node);

		// Set linked fields, formerly known as "candidate-issue"
		$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');
		$count = sirius_jsonfield_link_set($survey_form, $form_state['values']['survey'], $contact_results);
		if ($count) {
			sirius_jsonfield_util_results_set($contact_json, 'default', $contact_results);
			sirius_jsonfield_util_node_json_set($contact_node, $contact_json);
			node_save($contact_node);
		}

		// Sometimes we want to set the pstatus of other participants as well
		$set_all_pstatus_tid = $form_state['values']['set_all_pstatus_tid'];
		if ($set_all_pstatus_tid) {
			$pstatuses = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
			if ($pstatuses[$set_all_pstatus_tid]) {
				foreach ($participant_nodes as $other_participant_nid => $other_participant_node) {
					if ($other_participant_node->field_sirius_event_pstatus['und'][0]['tid'] == $set_all_pstatus_tid) { continue; }
					$other_participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $set_all_pstatus_tid;
					node_save($other_participant_node);
					drupal_set_message("Saved with $set_all_pstatus_tid: " . $other_participant_node->title . ": " . $other_participant_node->nid);
				}
			} else {
				drupal_set_message("Unable to set statuses to $set_all_pstatus_tid: that is not an approved status.", 'warning');
			} 
		}
	}

	if ($trigger_callback == 'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient') {
		unset($form_state['values']['survey']);
		unset($form_state['input']['survey']);

		unset($form_state['values']['pstatus']);
		unset($form_state['input']['pstatus']);

		unset($form_state['values']['caller']);
		unset($form_state['input']['caller']);

		// The participant NID may change throughout the life of this function. We start with the value in "survey_participant_nid"
		$participant_nid = $form_state['values']['participant_nid'];
		if (!$participant_nid) { $participant_nid = array_keys($participant_nodes)[0]; }
		$form['survey_participant_nid']['#value'] = $participant_nid;

		$participant_node = $participant_nodes[$participant_nid];
		$participant_json = 	sirius_jsonfield_util_node_json_get($participant_node);
		$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
		$contact_node = node_load($contact_nid);
		$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
		$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');
	}

	/////////////////////////////////////////////////////////////////////
	// Form
	/////////////////////////////////////////////////////////////////////

	sirius_event_phonebank_prep_survey($survey_form, $participant_results, $event_json);
	$pstatuses = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));

	$form['#tree'] = TRUE;

	$form['prefix'] = array('#markup' => '<div class="sirius-form" id="sirius_event_phonebank_survey"><table><tr>');
	$form['prefix_col_1'] = array('#markup' => '<td valign="top" style="vertical-align: top;">');
	$form['survey'] = $survey_form;
	$form['survey']['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap">', '#weight' => -9999);
	$form['survey']['survey_wrap_close'] = array('#markup' => '</div>', '#weight' => 9999);
	$form['suffix_col_1'] = array('#markup' => '</td>');

	$form['prefix_col_2'] = array('#markup' => '<td width="35%" valign="top" style="vertical-align: top; border-left: 1px solid #333333; padding-left: 10px;">');
	$browserphone_params = array();
	$browserphone_params['default_phone'] = $phone;
	$browserphone_params['callerid_nids'] = sirius_fieldvals($survey_node, 'field_sirius_callerids', 'target_id');
	$browserphone_params['omg_nid'] = $survey_node->nid;
	if ($event_json['phonebank']['conference'] == 'Yes') { 
		$browserphone_params['conference_number'] = $event_json['phonebank']['conference_number'];
	}
	if ($survey_json['omg']['pstatus']) {
		$browserphone_params['omg']['callback_js'] = 'sirius_event_phonebank_set_all_pstatus';
		$browserphone_params['omg']['callback_js_args'] = array($survey_json['omg']['pstatus']);
	}
	$form['phone_interface_html'] = array('#markup' => sirius_twilio_browserphone_render($browserphone_params));

	$participant_options = array();
	$submit_disabled = FALSE;
	$count = 0;
	foreach ($participant_nodes as $participant_nid_alt => $participant_node_alt) {
		++$count;
		$contact_nid_alt = $participant_node_alt->field_sirius_contact['und'][0]['target_id'];
		$contact_node_alt = node_load($contact_nid_alt);
		$participant_options[$participant_nid_alt] = $contact_node_alt->title . '<br />';
		if ($participant_node_alt != $participant_node && $participant_node_alt->field_sirius_event_pstatus['und'][0]['tid'] == $pstatus_tid_to_call) { $submit_disabled = TRUE; }
		$submit_disabled_msg .= "Set to $submit_disabled because $participant_nid_alt had status " . $participant_node_alt->field_sirius_event_pstatus['und'][0]['tid'] . " vs $pstatus_tid_to_call<br>"; 
	}
	// drupal_set_message($submit_disabled_msg);

	if (count($participant_options) > 1) {
		$form['participant_nid'] = array(
			'#type' => 'radios',
			// '#title' => 'Participant',
			'#options' => $participant_options,
			'#default_value' => $participant_nid,
			'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient'),
			// '#prefix' => '<div class="container-inline">',
			// '#suffix' => '</div>',
			'#suffix' => '<hr>',
		);
	} else {
		$form['participant_nid'] = array(
			'#type' => 'value',
			'#value' => array_keys($participant_options)[0],
		);
	}

	$intro_html = '<div class="sirius_form_intro">';
	if ($contact_node) {
		$contact_link = '';
		if (drupal_valid_path('node/' . $contact_node->nid)) { 
			$contact_link = l($contact_node->title, 'node/' . $contact_node->nid, array('attributes' => array('target' => '_blank')));
		} else {
			$contact_link = $contact_node->title;
		}
		$intro_html .= sirius_fakerender_field('Speaking with:', $contact_link);
		$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');
		$contact_definition_json = sirius_contact_definition();
		$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

		$display_fields = sirius_contact_definition_display_get($contact_definition_json, 'phonebank-default');
		sirius_jsonfield_util_add_node_fields($contact_node, $contact_definition_form, $contact_results);
		$intro_html .= sirius_jsonfield_util_render_values($contact_definition_form, $contact_results, $display_fields, array('show_title' => TRUE));
	}
	$intro_html .= sirius_fakerender_field('Phone:', $phone);
	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => '<div id="intro_wrap">' . $intro_html . '</div>');

	if ($participant_nid) {
		$form['pstatus'] = array(
			'#type' => 'radios',
			'#options' => $pstatuses,
			'#title' => 'Status',
			'#default_value' => $participant_node->field_sirius_event_pstatus['und'][0]['tid'],
			'#prefix' => '<div id="pstatus_wrap">',
			'#suffix' => '</div>',
			/*
			'#ajax' => array(
				'callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax',
				// 'event' => 'blur'
			),
			*/
		);
		if ($event_json['phonebank']['autosave']['field'] == 'Yes') {
			$form['pstatus']['#ajax'] = array(
				'callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax',
			);
		}

		$caller_uid = $participant_json['phonebank']['caller']['uid'];
		global $user;
		$my_uid = $user->uid;
		if (!$caller_uid || ($caller_uid == $my_uid)) {
			$form['caller'] = array(
				'#type' => 'hidden',
				'#name' => 'caller',
				'#value' => $my_uid,
			);
		} else {
			$caller_user = user_load($caller_uid);
			$form['caller'] = array(
				'#type' => 'select',
				'#options' => array(
					$user->uid => $user->name,
					$caller_uid => $caller_user->name,
				),
				'#title' => 'Called by',
				'#prefix' => '<div id="caller_wrap">',
				'#suffix' => '</div>',
				'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax'),
				'#default_value' => $caller_uid,
			);
			if ($event_json['phonebank']['autosave']['field'] == 'Yes') {
				$form['caller']['#ajax'] = array(
					'callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax',
					'event' => 'blur'
				);
			}
		}
	}

	$form['set_all_pstatus_tid'] = array(
		'#type' => 'hidden',
		'#attributes' => array('id' => 'set_all_pstatus_tid'),
		// '#type' => 'select',
		// '#options' => array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)))
	);

	$form['set_all_pstatus_submit'] = array(
		'#id' => 'set_all_pstatus_submit',
		'#type' => 'button',
		'#value' => 'Set All Pstatus',
		'#attributes' => array('hidden' => TRUE),
		'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax_next'),
	);

	$form['save'] = array(
		'#type' => 'button',
		'#value' => t('Save'),
    '#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax'),
	);

	$form['save_close'] = array(
		'#type' => 'button',
		'#value' => t('Close'),
    '#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax_close'),
	);

	if ($mode == 'live') {
		$class = 'disable_if_pstatus_' . $pstatus_tid_to_call;
		if ($submit_disabled) { $class .= ' disable_always'; }
		$form['submit'] = array(
			'#type' => 'button',
			'#value' => t('Next'),
			// '#disabled' => $submit_disabled,
			'#prefix' => '<span id="submit_wrap">',
			'#suffix' => '</span>',
			'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax_next'),
			'#attributes' => array('class' => array('class' => $class)),
		);
	}

	$form['suffix_col_2'] = array('#markup' => '</td');
	$form['suffix'] = array('#markup' => '</tr></table></div>');

	return $form;
}

function sirius_event_phonebank_prep_survey(&$form, $defaults, $settings) {
	if (!is_array($form)) { return; }
	foreach ($form as $key => $value) {
		if (!is_array($form[$key])) { continue; }
		
		$type = $form[$key]['#type'];
		if ($type) {
			if ($type == 'select' || $type == 'textfield' || $type == 'textarea' || $type == 'date_popup' || $type == 'radios' || $type == 'checkboxes')  {
				// unset($form[$key]['#markup']);
				// $form[$key]['#attributes']['class'][] = 'sirius_quicksave';

				if ($settings['phonebank']['autosave']['field'] == 'Yes') {
					$form[$key]['#ajax'] = array(
						'callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax',
						'event' => 'blur'
					);
				}

				if (isset($defaults[$key])) {
					$form[$key]['#default_value'] = $defaults[$key];
					if ($type == 'date_popup' && is_array($defaults[$key])) { 
						$form[$key]['#default_value'] = date('Y-m-d H:i:s', strtotime($defaults[$key]['date'] . ' ' . $defaults[$key]['time']));
						$form[$key]['#attributes']['autocomplete'] = 'off';
					}
					else { $form[$key]['#default_value'] = $defaults[$key]; }
				}
				if ($form[$key]['#required']) {
					unset($form[$key]['#required']);
					$form[$key]['#title'] = '<strong>[' . t('Required') . ']</strong> ' . $form[$key]['#title'];
					$form[$key]['#attributes']['class'][] = 'sirius_phonebank_field_required'; 
				}
			} else if ($type == 'fieldset') {
				sirius_event_phonebank_prep_survey($form[$key], $defaults[$key], $settings);
			}
		} else {
			sirius_event_phonebank_prep_survey($form[$key], $defaults[$key], $settings);
		}
	}
}
