<?php

function sirius_event_map_modal_form_multiple($form, &$form_state, $event_nid, $marker_id) {
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_map_modal_multiple');

	$participant_nids = explode('_', $marker_id);
	$participant_nodes = node_load_multiple($participant_nids);

	$event_node = node_load($event_nid);
	drupal_set_title('Multiple Participants');

  // dismiss if the form was submitted via a modal
  if ($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();

    // Dismiss the modal
    $commands[] = ctools_modal_command_dismiss();

  	// Update the map
    $commands[] = array(
	    'command' => 'sirius_command_map_poll',
    );

    // Show any messages
    $commands[] = ajax_command_replace('#sirius_map_poll_message', 
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

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	$prole_tid = $participant_nodes[$participant_nids[0]]->field_sirius_event_prole['und'][0]['tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));

	/////////////////////////////////////////////////////////////////////
	// Form
	/////////////////////////////////////////////////////////////////////

	$prep_settings = $event_json;
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($survey_form, $participant_results, $prep_settings);

	$form['#tree'] = TRUE;
	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$participant_options = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_title = sirius_node_title($participant_node->field_sirius_contact['und'][0]['target_id']) . ' (' . $pstatus_options[$participant_node->field_sirius_event_pstatus['und'][0]['tid']] . ')';

		$participant_options[$participant_nid] = $contact_title;
	}

	$form['participant_nids'] = array(
		'#title' => 'Record Responses For:',
		'#type' => 'checkboxes',
		'#options' => $participant_options,
		'#suffix' => '<hr>',
		'#required' => TRUE,
	);

	$form['survey'] = $survey_form;

	$form['pstatus'] = array(
		'#type' => 'radios',
		'#options' => $pstatus_options,
		'#title' => 'Status',
		'#default_value' => $pstatus_tid,
		'#required' => TRUE,
	);

	global $user;
	$form['caller'] = array(
		'#type' => 'hidden',
		'#name' => 'caller',
		'#value' => $my_uid,
	);

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

/*
function sirius_event_map_modal_form_submit_multiple_validate($form, &$form_state) {
	$participant_nids = array_filter(array_values($form_state['values']['participant_nids']));
	if (!$participant_nids) { 
		form_set_error('participant_nids', 'Please select at least one resident.');
	}
}
*/


function sirius_event_map_modal_form_multiple_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;

	$event_node = node_load($form_state['event_nid']);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_nids = array_filter(array_values($form_state['values']['participant_nids']));
	$participant_nodes = node_load_multiple($participant_nids);

	foreach ($participant_nodes as $participant_nid => $participant_node) {

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

		drupal_set_message("Saved: " . $contact_node->title);
	}

	// Update the timestamp of all participant nids, in case there are multiple residents
	if ($form_state['values']['all_participant_nids']) {
		$all_participant_nids = explode(',', $form_state['values']['all_participant_nids']);
		$sql = "update node set changed = :ts where nid in (:all_participant_nids)";
		$stmt = sirius_sql_query($sql, array(':ts' => time(), ':all_participant_nids' => $all_participant_nids));
	}
}

