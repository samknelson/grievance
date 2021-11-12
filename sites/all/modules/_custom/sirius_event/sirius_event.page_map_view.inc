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
  sirius_event_disclaimer_redirect($event_node);
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/map.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));

	global $user;
	$event_nid = $event_node->nid;
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node, 'map');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');
	$prole_tid = $type_json['map']['prole_tid'];

	$active = sirius_map_active($event_node);
	if (!$active['success']) {
		$html = '<div><em>' . t('Map is closed. Admin access only is allowed.') . '</em></div>';
	}
	
	$js_settings = array();
	$js_settings['current_location'] = array(
		'show' => TRUE,
		'pin_key' => sirius_domain_variable_get('sirius_map_current_location_pin', 'dot-blue'),
	);
	$js_settings['pins'] = sirius_map_pins();

	if (sirius_domain_component_enabled('mapping:checkin') && ($event_json['map']['checkin']['active'] == 'Yes')) {
		$js_settings['current_location']['checkin'] = array(
			'active' => TRUE,
			'timeout' => 1000 * 60,
		);
	}
	$js_settings['event_nid'] = $event_node->nid;
	$js_settings['poll'] = array('active' => TRUE, 'timeout' => 1000 * 60 * 3);
	drupal_add_js(array('sirius_map' => $js_settings), 'setting');

	$pstatus_tids = $_REQUEST['pstatus'];
	if ($pstatus_tids) { $pstatus_tids = array_filter(array_values($pstatus_tids)); }

	$map_features = sirius_event_map_list($event_node, array('show_pstatus_tids' => $pstatus_tids));

	if (!$map_features) {
		drupal_set_message("No geocoded participants for this event have been found.", 'warning');
		return $html;
	}

	// Filters
	$filter_form = drupal_get_form('sirius_event_map_filters_form', $event_nid, $prole_tid);
	$html .= drupal_render($filter_form);

	// Get map implementation provided by http://drupal.org/project/leaflet_googlemaps.
	$map = sirius_map_leaflet_create();
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
	$query_str = preg_replace('/^\?/', '', $_REQUEST['query']);
	$query = array();
	parse_str($query_str, $query);

	$pstatus_tids = array();
	if ($query['pstatus']) {
		$pstatus_tids = array_filter(array_values($query['pstatus']));
	}

	// Setup
	$event_node = node_load($event_nid);
	$cutoff_ts = time() - 60*60;

	// Updates
	$updates = sirius_event_map_list(
		$event_node, 
		array(
			'cutoff_ts' => $cutoff_ts,
			'disable_group_by_address' => $_REQUEST['disable_group_by_address'],
			'show_pstatus_tids' => $pstatus_tids,
		)
	);

	// Removals
	$removals = sirius_event_map_removals(
		$event_node, 
		array(
			'cutoff_ts' => $cutoff_ts,
			'disable_group_by_address' => $_REQUEST['disable_group_by_address'],
			'show_pstatus_tids' => $pstatus_tids,
		)
	);

	drupal_json_output(
		array(
			'updates' => $updates,
			'removals' => $removals,
			'debug' => array(),
		)
	);
	exit();
}

function sirius_event_map_filters_form($form, &$form_state, $event_nid, $prole_tid) {
	$form['#method'] = 'get';
	$form['filters'] = array(
		'#type' => 'fieldset', 
		'#title' => 'Filters',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#prefix' => '<div id="sirius_map_filters">',
		'#suffix' => '</div>',
	);

	$pstatus_terms = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full');
	$counts = sirius_event_participant_summary($event_nid, $prole_tid);
	if (!$counts) { return array(); }

	$options = array();
	foreach ($counts as $pstatus_tid => $count) {
		$pstatus_term = $pstatus_terms[$pstatus_tid];
		$pstatus_json = sirius_json_get($pstatus_term);
		$pin = sirius_map_pin($pstatus_json['map']['pin']);
		$pin['title'] = $pstatus_term->name;
		$options[$pstatus_tid] = '<div class="wrap"><div class="image">' . sirius_map_pin_render($pin, array('scale' => 0.5)) . '</div><div class="label">' . $pin['title'] . '</div></div>';
	}

	$form['filters']['pstatus'] = array(
		'#type' => 'checkboxes',
		// '#title' => 'Pin',
		'#options' => $options,
		'#prefix' => '<div class="sirius_map_pin_picker sirius_map_pin_picker_small">',
		'#suffix' => '</div><br clear="all" />',
		'#default_value' => array_combine(array_keys($options), array_keys($options)),
	);
	if ($_REQUEST['pstatus']) {
		$form['filters']['pstatus']['#default_value'] = array_filter(array_values($_REQUEST['pstatus']));
	}

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_map_popup_html($event_nid, $contact_names, $thoroughfare, $premise, $marker_id, $pstatus_names) {
	$participant_nids = explode('_', $marker_id);

	if (count($participant_nids) == 1) {
		$popup_html .= $thoroughfare;
		if ($premise) { $popup_html .= ', ' . $premise; }
		$popup_html .= '<br /><strong>' . $pstatus_names[0] . '</strong>';
		$popup_html .= '<br />' . l($contact_names[0], 'sirius/modal/event/map/' . $event_nid . '/' . $marker_id, array('attributes' => array('class' => array('tool-button', 'modal_link'))));
		return $popup_html;
	} else {
		$popup_html .= $thoroughfare;
		if ($premise) { $popup_html .= ', ' . $premise; }
		$popup_html .= ' / <strong>' . $pstatus_names[0] . '</strong>';

		foreach ($participant_nids as $delta => $participant_nid) {
			$popup_html .= '<br />' . l(
				$contact_names[$delta] . ' (' . $pstatus_names[$delta] . ')', 
				'sirius/modal/event/map/' . $event_nid . '/' . $participant_nid, 
				array(
					'query' => array('all_participant_nids' => join(',', $participant_nids)), 
					'attributes' => array('class' => array('tool-button', 'modal_link'))
				)
			);
		}
		$popup_html .= '<br />' . l(
			'ALL RESIDENTS', 
			'sirius/modal/event/map/' . $event_nid . '/' . $marker_id, 
			array(
				'query' => array('all_participant_nids' => join(',', $participant_nids)), 
				'attributes' => array('class' => array('tool-button', 'modal_link'))
			)
		);
		return $popup_html;

	}
}
