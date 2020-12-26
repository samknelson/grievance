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

	// Get the list of participants
	$participant_nids = sirius_event_participant_list_by_rule($event_node->nid, $rule, 3000);
	if (!$participant_nids) {
		drupal_set_message("No participants found for this event.", 'warning');
		return $html;
	}

  // Create settings for the map.
  $map_features = array();

  $pstatus_terms = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full');
  $pstatus_makers = array();
  foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
  	$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
  	$pstatus_markers[$pstatus_tid] = $pstatus_json['map']['icon'];
  }

	// Fetch all the relevant data about our participants
	$sql = "select contact_node.title, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "node.nid as participant_nid, ";
	$sql .= "field_sirius_event_pstatus_tid, ";
	$sql .= "field_sirius_address_thoroughfare, ";
	$sql .= "field_sirius_address_premise, ";
	$sql .= "field_sirius_address_geo_lat, ";
	$sql .= "field_sirius_address_geo_lon ";
	$sql .= " from node ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
	$sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = contact_node.nid ";
	$sql .= "where node.nid in (:participant_nids) ";
	$sql .= "and field_sirius_address_geo_lat is not null ";
	$stmt = sirius_sql_query($sql, array(':participant_nids' => $participant_nids));
	while ($hr = $stmt->fetchAssoc()) {
		$pstatus_tid = $hr['field_sirius_event_pstatus_tid'];

		$map_feature = array(
	    'type' => 'point',
	    'lat' => $hr['field_sirius_address_geo_lat'],
	    'lon' => $hr['field_sirius_address_geo_lon'],
	    'popup' => sirius_map_popup_html($event_node->nid, $hr['contact_nid'], $hr['title'], $hr['field_sirius_address_thoroughfare'], $hr['field_sirius_address_premise'], $hr['participant_nid'], $pstatus_terms[$pstatus_tid]->name),
	    'leaflet_id' => 'participant_' . $hr['participant_nid'],
	    'sirius' => array('marker' => array('id' => $hr['participant_nid'])),
		);

		if ($pstatus_markers[$pstatus_tid]) {
			$map_feature['icon'] = array('iconUrl' => '/sites/all/modules/_custom/sirius/images/map/' . $pstatus_markers[$pstatus_tid]);
    }
		$map_features[] = $map_feature;
	}
	if (!$map_features) {
		drupal_set_message("None of the participants for this event have been geocoded.", 'warning');
		return $html;
	}

	// Get map implementation provided by http://drupal.org/project/leaflet_googlemaps.
	$map = sirius_map_leaflet_create();
	// $map['center'] = array('lon' => 0, 'lat' => 0);
  $map_build = leaflet_build_map($map, $map_features, '600px');
	$html .= render($map_build);
	$html .= l('Legend', 'sirius/modal/event/map-legend/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));
	$html .= '<a href="/sirius/map" id="sirius_map_poll" class="tool-button">Refresh Now</a>';
	$html .= '<div id="sirius_map_poll_message"></div>';

	return $html;
}




function sirius_event_map_ajax_poll_access($event_nid) {
	$event_node = node_load($event_nid);
	return sirius_node_tab_event_phonebank_call_access($event_node);
}

function sirius_event_map_ajax_poll($event_nid) {
	// Setup
	$event_node = node_load($event_nid);
	$cutoff = time() - 60*60;
  $pstatus_terms = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full');
  $pstatus_icons = array();
  foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
  	$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
  	$pstatus_icons[$pstatus_tid] = $pstatus_json['map']['icon'];
  }
	$icon_names = sirius_map_icons();

	// @todo: We really ought to be passing in the rule here, since this will include things like volunteers and other participants that we might not have access to.
	$participant_nids = sirius_event_participant_list_by_rule($event_node->nid, $rule, 3000);
	$sql = "select contact_node.title, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "node.nid as participant_nid, ";
	$sql .= "field_sirius_event_pstatus_tid, ";
	$sql .= "field_sirius_address_thoroughfare, ";
	$sql .= "field_sirius_address_premise, ";
	$sql .= "field_sirius_address_geo_lat, ";
	$sql .= "field_sirius_address_geo_lon ";
	$sql .= " from node ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
	$sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = contact_node.nid ";
	$sql .= "where node.nid in (:participant_nids) ";
	$sql .= "and field_sirius_address_geo_lat is not null ";
	$sql .= "and node.changed > :cutoff ";
	$sql_args = array();
	$sql_args[':cutoff'] = $cutoff;
	$sql_args[':participant_nids'] = $participant_nids;
	$stmt = sirius_sql_query($sql, $sql_args);

	$records = array();
	while ($hr = $stmt->fetchAssoc()) {
		$pstatus_tid = $hr['field_sirius_event_pstatus_tid'];

		$icon_name = $icon_names[$pstatus_icons[$pstatus_tid]];
		if (!$icon_name) { $icon_name = 'Blue'; }
		$record = array(
			'participant_nid' => $hr['participant_nid'],
			'icon_name' => $icon_name,
	    'popup_html' => sirius_map_popup_html($event_node->nid, $hr['contact_nid'], $hr['title'], $hr['field_sirius_address_thoroughfare'], $hr['field_sirius_address_premise'], $hr['participant_nid'], $pstatus_terms[$pstatus_tid]->name),
		);
		$records[] = $record;
	}
	drupal_json_output(array('updates' => $records));
	exit();
}

function sirius_event_map_modal_access($event_nid, $participant_nid) {
	$event_node = node_load($event_nid);
	$participant_node = node_load($participant_nid);
	if ($participant_node->field_sirius_event['und'][0]['target_id'] != $event_nid) { return FALSE; }
	return sirius_node_tab_event_phonebank_call_access($event_node);
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
	    'popup_html' => sirius_map_popup_html($event_nid, $contact_node->nid, $contact_node->title, $contact_node->field_sirius_address['und'][0]['thoroughfare'], $contact_node->field_sirius_address['und'][0]['premise'], $participant_nid, $pstatus_term->name),
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
	return sirius_node_tab_event_phonebank_call_access($event_node);
}

function sirius_event_map_modal_legend($event_nid) {
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
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
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
	$pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));

	$rows = array();
	foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
		$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
		$icon = $pstatus_json['map']['icon'];
		if (!$icon) { $icon = sirius_map_icon_default(); }

		$row = array();
		$row[] = sirius_map_icon_render($icon);
		$row[] = $pstatus_term->name;
		$row[] = $pstatus_term->description;
		$rows[] = $row;
	}
	$header = array('Marker', 'Status', 'Description');
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	return $html;
}
