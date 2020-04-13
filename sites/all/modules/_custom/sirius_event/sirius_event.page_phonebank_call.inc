<?php

function sirius_node_tab_event_phonebank_call_access($event_node) {
	if (!sirius_node_tab_event_phonebank_access($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$json = sirius_jsonfield_util_node_json_get($event_node);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$caller_prole_tids = array_values($type_json['phonebank']['caller_prole_tids']);
	if (!in_array($prole_tid, $caller_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_phonebank_call($event_node) {
	$active = sirius_phonebank_active($event_node);
	$json = sirius_jsonfield_util_node_json_get($event_node);
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

		if (!$active) {
		  if ($event_node->field_sirius_daterepeat['und']) {
		    $ts_now = time();
				$opens_in = 999999999;
		    foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
		      $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
		      $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
		      if ($start_ts <= 1 || $end_ts <= 1) { continue; }
		      if ($start_ts >= $ts_now) {
		      	$range = $start_ts - $ts_now;
		      	if ($range && $range < $opens_in) { $opens_in = $range; }
		      }
		    }
		    if ($opens_in > 1 && $opens_in < 999999999) {
		    	drupal_set_message(t('Opening in: @interval', array('@interval' => format_interval($opens_in))));
		    }
		  }
		}


		$json = sirius_jsonfield_util_node_json_get($event_node);
		$html .= $json['phonebank']['caller_intro'];
		$html .= '<br />';
		$html .= sirius_fakerender_field('Current Time:', date('l, F d Y - g:ia'));
		$html .= '<br />';
		$html .= sirius_dispatch_view_render_field($event_node, 'field_sirius_daterepeat', 'Phone Bank Open');
		$html .= '<br />';

		$html .= l(t('Dry Run'), current_path(),  array('query' => array('mode' => 'test'), 'attributes' => array('class' => array('tool-button'))));
		if ($active) { 
			$html .= l(t('Start Making Calls'), current_path(),  array('query' => array('mode' => 'live'), 'attributes' => array('class' => array('tool-button'))));
		}
		return $html;
	}

	// Get the participant
	if ($mode == 'live' && $active == 'Yes') {
		$phone = sirius_event_phonebank_next_number($event_node, $prole_tid, $pstatus_tid);
		if (!$phone) { return 'There are no more calls to make!'; }
	} 
	return drupal_get_form('sirius_node_tab_event_phonebank_call_form', $event_node->nid, $phone);
}

function sirius_node_tab_event_phonebank_call_form($form, &$form_state, $event_nid, $phone) {
	$event_node = node_load($event_nid);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_tid = $json['phonebank']['pstatus_tid'];
	$participant_nodes = sirius_event_phonebank_participants_by_phone($event_node, $prole_tid, $phone);

	//drupal_add_js(drupal_get_path('module', 'sirius_event') . '/js/sirius_event.phonebank.js', 'file');
	//drupal_add_js(array('sirius_event_phonebank' => array('participant_nid' => $participant_nid)), array('type' => 'setting'));
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/sirius_event.phonebank.css', 'file');

	$form_state['event_nid'] = $event_nid;
	$form_state['phone'] = $phone;

	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { drupal_set_message("No survey!"); return array(); }
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_nid = $form_state['values']['participant_nid'];
	if (!$participant_nid) { 
		$participant_nid = array_keys($participant_nodes)[0];
	}

	$participant_node = $participant_nodes[$participant_nid];
	$participant_prole = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$participant_json = 	sirius_jsonfield_util_node_json_get($participant_node);
	$event_node = node_load($event_nid);
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');
	sirius_event_phonebank_prep_survey($survey_form, $participant_results);
	$pstatuses = sirius_event_participant_role_status_tids($participant_prole);

	// If we changed participant, throw away our survey values.
	if ($form_state['triggering_element']['#ajax']['callback'] == 'sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient') {
		unset($form_state['values']['survey']);
		unset($form_state['input']['survey']);

		unset($form_state['values']['pstatus']);
		unset($form_state['input']['pstatus']);

		unset($form_state['values']['caller']);
		unset($form_state['input']['caller']);
	}
	if ($form_state['triggering_element']['#ajax']['callback'] == 'sirius_node_tab_event_phonebank_call_form_submit_ajax') {
		// A bit of a kludge. sirius_node_tab_event_phonebank_call_form_submit_ajax() saves the participant with a new pstatus. 
		// But it happens after the form is built, and we need this value to decide whether to enable the submit button.
		// (A better model would probably be to make the work happen here, rather than in the callback...)
		if ($form_state['input']['pstatus']) { $participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $form_state['input']['pstatus']; }
	}

	$form['#tree'] = TRUE;
	$form['close1'] = array('#markup' => '<div id="phonebank_wrapper">' . l('Close', current_path(), array('attributes' => array('class' => array('tool-button', 'phonebank-close-button')))));
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
	$form['phone_interface_html'] = array('#markup' => sirius_twilio_browserphone_render($browserphone_params));

	$participant_options = array();
	$submit_disabled = FALSE;
	// $submit_disabled_msg = '';
	$count = 0;
	foreach ($participant_nodes as $participant_nid_alt => $participant_node_alt) {
		++$count;
		$contact_nid_alt = $participant_node_alt->field_sirius_contact['und'][0]['target_id'];
		$contact_node_alt = node_load($contact_nid_alt);
		$participant_options[$participant_nid_alt] = $contact_node_alt->title . '<br />';
		if ($participant_node_alt->field_sirius_event_pstatus['und'][0]['tid'] == $pstatus_tid) { $submit_disabled = TRUE; }
		// $submit_disabled_msg .= "Set to $submit_disabled because $participant_nid_alt had status " . $participant_node_alt->field_sirius_event_pstatus['und'][0]['tid'] . " vs $pstatus_tid_to_call<br>"; 
	}

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
		$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
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
			'#ajax' => array('callback' => 'sirius_node_tab_event_phonebank_call_form_submit_ajax'),
			'#prefix' => '<div id="pstatus_wrap">',
			'#suffix' => '</div>',
		);

		$caller_uid = $participant_json['phonebank']['caller']['uid'];
		global $user;
		$my_uid = $user->uid;
		if (!$caller_uid || ($caller_uid == $my_uid)) {
			$form['caller'] = array(
				'#type' => 'hidden',
				'#name' => 'caller',
				'#value' => $caller_uid,
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
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save and Next'),
		'#disabled' => $submit_disabled,
		'#prefix' => '<div id="submit_wrap">',
		'#suffix' => '</div>',
	);

	$form['suffix_col_2'] = array('#markup' => '</td');
	$form['suffix'] = array('#markup' => '</tr></table></div>');

	$form['close2'] = array('#markup' => l('Close', current_path(), array('attributes' => array('class' => array('tool-button', 'phonebank-close-button')))) . '</div>');

	return $form;
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax_change_recipient($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#survey_wrap", render($form['survey']));
  $commands[] = ajax_command_replace("#pstatus_wrap", render($form['pstatus']));
  $commands[] = ajax_command_replace("#intro_wrap", render($form['intro']));
  $commands[] = ajax_command_replace("#submit_wrap", render($form['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function sirius_node_tab_event_phonebank_call_form_submit_ajax($form, $form_state) {
	global $user;

	$participant_nid = $form_state['values']['participant_nid'];
	if (!$participant_nid) { return; }
	$participant_node = node_load($participant_nid);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
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
	// sirius_jsonfield_log($participant_node, 'survey', 'Survey submitted.');
	node_save($participant_node);

	// Set linked fields, formerly known as "candidate-issue"
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);

	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');

	$count = sirius_jsonfield_link_set($survey_form, $form_state['values']['survey'], $contact_results);
	if ($count) {
		sirius_jsonfield_util_results_set($contact_json, 'default', $contact_results);
		sirius_jsonfield_util_node_json_set($contact_node, $contact_json);
		node_save($contact_node);
	}

	$commands = array();
  $commands[] = ajax_command_replace("#submit_wrap", render($form['submit']));

	return array('#type' => 'ajax', '#commands' => $commands);
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
