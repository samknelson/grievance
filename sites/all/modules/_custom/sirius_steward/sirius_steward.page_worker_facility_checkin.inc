<?php

function sirius_steward_node_tab_worker_facility_checkin_access($worker_node) {
	// Do I have superpowers?
	if (user_access('sirius admin') || user_access('sirius domain admin')) { return TRUE; }

	// Can I check in *anybody*?
	if (user_access('sirius steward facility checkin other')) { return TRUE; }

	// Can I check in myself
	if (user_access('sirius steward facility checkin self')) {
		$my_worker_node = sirius_worker_get();
		if ($my_worker_node && ($my_worker_node->nid == $worker_node->nid)) { return TRUE; }
	}

	// Oh well
	return FALSE;
}

function sirius_steward_node_tab_worker_facility_checkin($worker_node) {
	$current_checkins = sirius_steward_facility_checkin_search(array('worker_nid' => $worker_node->nid, 'status' => 'in'));

	if ($current_checkins) {
		$current_checkin_nid = reset($current_checkins);
		$current_checkin_node = node_load($current_checkin_nid);
		return drupal_get_form('sirius_steward_node_tab_worker_facility_checkin_out_form', $worker_node, $current_checkin_node);
	} else {
		return drupal_get_form('sirius_steward_node_tab_worker_facility_checkin_in_form', $worker_node);
	}
}

// ****************************************************************************************
// * Check IN
// *****************************************************************************************/

function sirius_steward_node_tab_worker_facility_checkin_in_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	// Must have a dispatch
	$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
	if (!$dispatch_node) {
		return sirius_form_error(t('You are not on a current dispatch. Check in is not available.'));
	}


	// Get the current location
	$form['checkin']['geolocation'] = sirius_map_geolocation_render_form();

	// Facility
	$facilities = sirius_dispatch_facility_dropdown();
	if (!$facilities) { return sirius_form_error("No facilities have been defined. Checkin is not possible."); }
	$form['checkin']['facility'] = array(
		'#type' => 'select',
		'#title' => t('Facility'),
		'#options' => $facilities,
		'#required' => TRUE,
	);

	// Render the disclaimer
	$disclaimer_nid = sirius_domain_variable_get('sirius_steward_facility_checkin_disclaimer');
	if (!$disclaimer_nid) { 
		return sirius_form_error(
			"Please select a disclaimer for steward checkin: " . 
			l('Configuration', 'sirius/steward/config', array('query' => array('destination' => current_path())))
		);
	}
	$disclaimer_node = node_load($disclaimer_nid);
	$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];

	/*
	$form['checkin']['disclaimer'] = array(
		'#type' => 'fieldset',
		'#states' => array('visible' => array(":input[name=\"checkin[facility]\"" => array('value' => 2402816))),
		'#title' => 'Whatever',
	);
	*/
	
	$form['checkin']['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json);
	$form['checkin']['disclaimer']['#states'] = array(
		
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check Me In'),
	);

	return $form;
}

function sirius_steward_node_tab_worker_facility_checkin_in_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$facility_nid = $form_state['values']['checkin']['facility'];
	$json = $form_state['values']['checkin'];

	$result = sirius_steward_facility_checkin_in($facility_nid, $worker_nid, $json);
	if (!$result['success']) {
		$form_state['rebuild'] = TRUE;
		drupal_set_message("Checkin failed: " . $result['msg'], 'error');
		return;
	}

	drupal_set_message("You have been checked in to the facility.");
}

// ****************************************************************************************
// * Check OUT
// *****************************************************************************************/

function sirius_steward_node_tab_worker_facility_checkin_out_form($form, &$form_state, $worker_node, $checkin_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form_state['checkin_nid'] = $checkin_node->nid;
	$form['#tree'] = TRUE;

	// Get the current location
	$form['checkin']['geolocation'] = sirius_map_geolocation_render_form();

	$form['checkin']['render'] = sirius_steward_facility_checkin_render_form($checkin_node, array('hide_map' => TRUE, 'hide_disclaimer' => TRUE));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check Me Out'),
	);

	return $form;
}

function sirius_steward_node_tab_worker_facility_checkin_out_form_submit($form, &$form_state) {
	$checkin_nid = $form_state['checkin_nid'];
	$checkin_node = node_load($checkin_nid);
	$json = $form_state['values']['checkin'];

	$result = sirius_steward_facility_checkin_out($checkin_node, $json);
	if (!$result['success']) {
		$form_state['rebuild'] = TRUE;
		drupal_set_message("Checkin failed: " . $result['msg'], 'error');
		return;
	}

	drupal_set_message("You have been checked out.");
}