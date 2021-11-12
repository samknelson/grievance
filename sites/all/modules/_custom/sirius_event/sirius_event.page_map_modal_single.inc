<?php

function sirius_event_map_modal_form_single($form, &$form_state, $event_nid, $marker_id) {
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_map_modal_single');

	$event_node = node_load($event_nid);
	$participant_node = node_load($marker_id);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	drupal_set_title($contact_node->title);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];

  // dismiss if the form was submitted via a modal
  if($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();

    // Dismiss the modal
    $commands[] = ctools_modal_command_dismiss();

    // Find the pin
    $pstatus_term = taxonomy_term_load($pstatus_tid);
  	$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
  	$pin = sirius_map_pin($pstatus_json['map']['pin']);

  	// Update the map
    $commands[] = array(
	    'command' => 'sirius_command_map_poll',
    );

    // Show any messages
    $commands[] = ajax_command_replace(
    	'#sirius_map_poll_message', 
      '<div id="sirius_map_poll_message">' . theme_status_messages(array()) . '</div>'
    );

    print ajax_render($commands);
    drupal_exit();
  }

	/////////////////////////////////////////////////////////////////////
	// Setup
	/////////////////////////////////////////////////////////////////////

	global $user;
	$event_node = node_load($event_nid);

	$form_state['event_nid'] = $event_nid;
	$form_state['marker_id'] = $marker_id;

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
	$display_fields = sirius_contact_definition_display_get($contact_definition_json, 'phonebank-default');
	$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');

	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));

	/////////////////////////////////////////////////////////////////////
	// Form
	/////////////////////////////////////////////////////////////////////

	$prep_settings = $event_json;
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($survey_form, $participant_results, $prep_settings);

	$form['#tree'] = TRUE;

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$contact_link = $contact_node->title;
	if (drupal_valid_path('node/' . $contact_node->nid)) { 
		$contact_link .= ' [' . l('Contact', 'node/' . $contact_node->nid, array('attributes' => array('target' => '_blank'))) . ']';
	} 
	if (drupal_valid_path('node/' . $participant_node->nid)) { 
		$contact_link .= ' [' .l('Participant', 'node/' . $participant_node->nid, array('attributes' => array('target' => '_blank'))) . ']';
	} 
	$intro_html .= sirius_fakerender_field('Speaking with:', $contact_link);
	sirius_jsonfield_util_add_node_fields($contact_node, $contact_definition_form, $contact_results);
	$intro_html .= sirius_jsonfield_util_render_values($contact_definition_form, $contact_results, $display_fields, array('show_title' => TRUE));
	$intro_html .= '<hr>';
	$form['intro'] = array('#markup' => $intro_html);

	$form['survey'] = $survey_form;

	$form['pstatus'] = array(
		'#type' => 'radios',
		'#options' => $pstatus_options,
		'#title' => 'Status',
		'#default_value' => $pstatus_tid,
	);

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
			'#title' => 'Visited by',
			'#prefix' => '<div id="caller_wrap">',
			'#suffix' => '</div>',
			'#default_value' => $caller_uid,
		);
	}

	$form['save'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);
	$form['survey_wrap_close'] = array('#markup' => '</div>');

	if ($_REQUEST['all_participant_nids']) {
		$form['all_participant_nids'] = array('#type' => 'value', '#value' => $_REQUEST['all_participant_nids']);
	}

	return $form;
}

function sirius_event_map_modal_form_single_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;

	$event_node = node_load($form_state['event_nid']);
	$participant_node = node_load($form_state['marker_id']);

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

  $participant_json = sirius_jsonfield_util_node_json_get($participant_node);

  // Set the results
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


	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $form_state['values']['pstatus'];
	node_save($participant_node);

	// Set linked fields, formerly known as "candidate-issue"
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');
	$count = sirius_jsonfield_link_set($survey_form, $form_state['values']['survey'], $contact_results);
	if ($count) {
		sirius_jsonfield_util_results_set($contact_json, 'default', $contact_results);
		sirius_jsonfield_util_node_json_set($contact_node, $contact_json);
		node_save($contact_node);
	}

	// Update the timestamp of all participant nids, in case there are multiple residents
	if ($form_state['values']['all_participant_nids']) {
		$all_participant_nids = explode(',', $form_state['values']['all_participant_nids']);
		$sql = "update node set changed = :ts where nid in (:all_participant_nids)";
		$stmt = sirius_sql_query($sql, array(':ts' => time(), ':all_participant_nids' => $all_participant_nids));
	}

	drupal_set_message("Saved: " . $contact_node->title);
}


