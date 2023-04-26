<?php

function sirius_steward_node_tab_worker_checkin_list_access($worker_node) {
	// At the moment, anybody can see the checkin list.
	return TRUE;
}

function sirius_steward_node_tab_worker_checkin_list_form($form, &$form_state, $worker_node) {
	// We want a list of facilities with a currently checked-in steward.
	// @todo: I'm doing this by brute-forcing loading all current active records. It should really
	//        be a db query, like "sirius_steward_checkin_facility_list".
	$all_facilities = sirius_dispatch_facility_dropdown();
	$all_checkin_nids = sirius_steward_checkin_search(array('status' => 'in'));
	$all_checkin_nodes = node_load_multiple($all_checkin_nids);
	$facilities = array();
	foreach ($all_checkin_nodes as $checkin_node) {
		$json = sirius_json_get($checkin_node);
		$facility_nid = $json['in']['facility'];
		if (!$facility_nid) { continue; }
		$facilities[$facility_nid] = $all_facilities[$facility_nid];
	}
	if (!$facilities) { 
		drupal_set_message('There are no stewards currently checked in.', 'warning');
		return array();
	}

	$form['facility'] = array(
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $facilities,
		'#title' => t('Facility'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['values']['facility']) {
		// Get the checkin records
		$checkin_nids = sirius_steward_checkin_search(
			array(
				'facility_nid' => $form_state['values']['facility'],
				'status' => 'in',
			)
		);
		if (!$checkin_nids) { 
			// Should never happen, because we're restricting the list to those with checked-in stewards.
			$html = '<p><em>' . t('There are no checked-in stewards at that facility.') . '</em></p>';
		} else {
			$params = array();
			$params['hide_status'] = TRUE;
			$params['hide_in'] = TRUE;
			$params['hide_out'] = TRUE;
			if (!user_access('sirius steward admin')) { $params['hide_tools'] = TRUE; }
			$html = sirius_steward_checkin_render_list($checkin_nids, $params);
		}

		$form['results'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_steward_node_tab_worker_checkin_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}