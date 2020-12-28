<?php

function sirius_node_tab_event_map_view_access($event_node) {
	if (!sirius_node_tab_event_map_access($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];

	$active = sirius_map_active($event_node);
	if ($active['success']) {
		// Any caller can access
		$permitted_prole_tids = array_values($type_json['map']['caller_prole_tids']);
	} else {
		// Only admins can access
		$permitted_prole_tids = array_values($type_json['map']['admin_prole_tids']);
	}

	return in_array($prole_tid, $permitted_prole_tids);
}

function sirius_node_tab_event_map_view($event_node) {
	$active = sirius_map_active($event_node);
	if (!$active['success']) {
		$html = '<div><em>' . t('Map is closed. Admin access only is allowed.') . '</em></div>';
	}
	
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));
	$js_settings = array();
	$js_settings['current_location'] = array('show' => TRUE);
	$js_settings['event_nid'] = $event_node->nid;
	$js_settings['poll'] = array('active' => TRUE, 'timeout' => 1000 * 60 * 3);
	drupal_add_js(array('sirius_map' => $js_settings), 'setting');

	global $user;
	$event_nid = $event_node->nid;
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node, 'map');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$map_features = sirius_event_map_list($event_node);
	if (!$map_features) {
		drupal_set_message("No geocoded participants for this event have been found.", 'warning');
		return $html;
	}

	// Get map implementation provided by http://drupal.org/project/leaflet_googlemaps.
	$map = sirius_map_leaflet_create();
	// $map['center'] = array('lon' => 0, 'lat' => 0);
  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '600px'));
	$html .= render($map_build);
	$html .= l('Legend', 'sirius/modal/event/map-legend/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));
	$html .= '<a href="/sirius/map" id="sirius_map_poll" class="tool-button">Refresh Now</a>';
	$html .= '<div id="sirius_map_poll_message"></div>';

	return $html;
}

function sirius_event_map_ajax_poll_access($event_nid) {
	$event_node = node_load($event_nid);
	return sirius_node_tab_event_map_access($event_node);
}

function sirius_event_map_ajax_poll($event_nid) {
	// Setup
	$event_node = node_load($event_nid);
	$cutoff_ts = time() - 60*60;
	$map_features = sirius_event_map_list($event_node, $cutoff_ts);

	drupal_json_output(array('updates' => $map_features));
	exit();
}

function sirius_event_map_modal_access($event_nid, $participant_nid) {
	$event_node = node_load($event_nid);
	$participant_node = node_load($participant_nid);
	if ($participant_node->field_sirius_event['und'][0]['target_id'] != $event_nid) { return FALSE; }
	return sirius_node_tab_event_map_access($event_node);
}

function sirius_event_map_modal($event_nid, $participant_nid) {
	return drupal_get_form('sirius_event_map_modal_form', $event_nid, $participant_nid);
}

function sirius_event_map_modal_form($form, &$form_state, $event_nid, $participant_nid) {
	$event_node = node_load($event_nid);
	$participant_node = node_load($participant_nid);
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

    // Find the icon
    $pstatus_term = taxonomy_term_load($pstatus_tid);
  	$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
  	$icon = $pstatus_json['map']['icon']; 
  	$markers = sirius_map_icons();
  	$icon_name = $markers[$icon];
  	if (!$icon_name) { $icon_name = 'Blue'; }

  	// Update the map
    $commands[] = array(
	    'command' => 'sirius_command_map_update',
	    'participant_nid' => $participant_nid,
	    'icon_name' => $icon_name,
	    'popup' => sirius_map_popup_html($event_nid, $contact_node->nid, $contact_node->title, $contact_node->field_sirius_address['und'][0]['thoroughfare'], $contact_node->field_sirius_address['und'][0]['premise'], $participant_nid, $pstatus_term->name),
    );

    // Show any messages
    $commands[] = ajax_command_replace('#sirius_map_poll_message', 
      '<div id="sirius_map_poll_message">' .
      // '<div id="messages">' . 
      theme_status_messages(array()) .
      // '</div>' .
      '</div>'
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
	$form_state['participant_nid'] = $participant_nid;

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


	return $form;
}

function sirius_event_map_modal_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;

	$event_node = node_load($form_state['event_nid']);
	$participant_node = node_load($form_state['participant_nid']);

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

	drupal_set_message("Saved: " . $contact_node->title);
}

function sirius_map_popup_html($event_nid, $contact_nid, $contact_name, $thoroughfare, $premise, $participant_nid, $status_name) {
	$popup_html = $contact_name;
	$popup_html .= '<br />' . $thoroughfare;
	if ($premise) { $popup_html .= ', ' . $premise; }
	$popup_html .= '<br /><strong>' . $status_name . '</strong>';
	// $popup_html .= '<br />' . l('Contact', 'node/' . $contact_nid . '/edit') . ' / ' . l('Participant', 'node/' . $participant_nid . '/edit');
	$popup_html .= '<br />' . l("Survey", 'sirius/modal/event/map/' . $event_nid . '/' . $participant_nid, array('attributes' => array('class' => array('tool-button', 'modal_link'))));
	return $popup_html;
}


function sirius_event_map_modal_legend_access($event_nid) {
	$event_node = node_load($event_nid);
	return sirius_node_tab_event_map_access($event_node);
}

function sirius_event_map_modal_legend($event_nid) {
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['map']['prole_tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
	$pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));
	$html = $event_json['map']['caller_intro'];
	$html .= sirius_event_map_legend_render($event_nid);
	return $html;
}

function sirius_event_map_legend_render($event_nid) {
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['map']['prole_tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
	$pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));

	$counts = sirius_event_participant_summary($event_nid, $prole_tid);

	$rows = array();
	foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
		$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
		$icon = $pstatus_json['map']['icon'];
		if (!$icon) { $icon = sirius_map_icon_default(); }

		$row = array();
		$row[] = sirius_map_icon_render($icon);
		$row[] = $pstatus_term->name;
		$row[] = number_format($counts[$pstatus_tid], 0);
		$row[] = $pstatus_term->description;
		$rows[] = $row;
	}
	$header = array('Marker', 'Status', 'Count', 'Description');
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	return $html;
}

