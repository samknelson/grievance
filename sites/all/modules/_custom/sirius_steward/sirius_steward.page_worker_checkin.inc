<?php

function sirius_steward_node_tab_worker_checkin_access($worker_node) {
	// Do I have superpowers?
	if (user_access('sirius admin') || user_access('sirius domain admin')) { return TRUE; }

	// Can I check in *anybody*?
	if (user_access('sirius steward checkin other')) { return TRUE; }

	// Can I check in myself
	if (user_access('sirius steward checkin self')) {
		$my_worker_node = sirius_worker_get();
		if ($my_worker_node && ($my_worker_node->nid == $worker_node->nid)) { return TRUE; }
	}

	// Oh well
	return FALSE;
}

function sirius_steward_node_tab_worker_checkin($worker_node) {
	$current_checkins = sirius_steward_checkin_search(array('worker_nid' => $worker_node->nid, 'status' => 'in'));

	if ($current_checkins) {
		$current_checkin_nid = reset($current_checkins);
		$current_checkin_node = node_load($current_checkin_nid);
		return drupal_get_form('sirius_steward_node_tab_worker_checkin_out_form', $worker_node, $current_checkin_node);
	} else {
		return drupal_get_form('sirius_steward_node_tab_worker_checkin_in_form', $worker_node);
	}
}

// ****************************************************************************************
// * Check IN
// *****************************************************************************************/

function sirius_steward_node_tab_worker_checkin_in_form($form, &$form_state, $worker_node) {
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
	$disclaimer_nid = sirius_domain_variable_get('sirius_steward_checkin_disclaimer');
	if (!$disclaimer_nid) { 
		return sirius_form_error(
			"Please select a disclaimer for steward checkin: " . 
			l('Configuration', 'sirius/steward/config', array('query' => array('destination' => current_path())))
		);
	}
	$disclaimer_node = node_load($disclaimer_nid);
	$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
	$form['checkin']['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json);

	$form['checkin']['customfield'] = sirius_steward_checkin_customfield_form($checkin_node, sirius_domain_variable_get('sirius_steward_checkin_in_customfield'));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check Me In'),
	);

	return $form;
}

function sirius_steward_node_tab_worker_checkin_in_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$facility_nid = $form_state['values']['checkin']['facility'];
	$json = $form_state['values']['checkin'];

	$result = sirius_steward_checkin_in($facility_nid, $worker_nid, $json);
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

function sirius_steward_node_tab_worker_checkin_out_form($form, &$form_state, $worker_node, $checkin_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form_state['checkin_nid'] = $checkin_node->nid;
	$form['#tree'] = TRUE;

	// Get the current location
	$form['checkin']['geolocation'] = sirius_map_geolocation_render_form();

	$form['checkin']['render'] = sirius_steward_checkin_render_form($checkin_node, array(
			'hide_map' => TRUE, 
			'hide_disclaimer' => TRUE,
			'hide_customfield' => TRUE,
		)
	);

	$form['checkin']['customfield'] = sirius_steward_checkin_customfield_form($checkin_node, sirius_domain_variable_get('sirius_steward_checkin_out_customfield'));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check Me Out'),
	);

	return $form;
}

function sirius_steward_node_tab_worker_checkin_out_form_submit($form, &$form_state) {
	$checkin_nid = $form_state['checkin_nid'];
	$checkin_node = node_load($checkin_nid);
	$json = $form_state['values']['checkin'];

	$result = sirius_steward_checkin_out($checkin_node, $json);
	if (!$result['success']) {
		$form_state['rebuild'] = TRUE;
		drupal_set_message("Checkin failed: " . $result['msg'], 'error');
		return;
	}

	drupal_set_message("You have been checked out.");
}

/**
 * Utility function to add a customfield form
 */

function sirius_steward_checkin_customfield_form($entity_node, $customfield_set_nid) {
	if (!$customfield_set_nid) { return array(); }

  $customfield_set_node = node_load($customfield_set_nid);
  $customfield_set_json = sirius_json_get($customfield_set_node);
  $customfield_form = sirius_jsonfield_util_form_get($customfield_set_json, 'default');
  if (!$customfield_form) { 
  	return sirius_form_error("The form for this custom field set has not been configured. It can be set up " . l('here.', 'sirius/customfield/definition/list'));
  }

  $which = 'customfield_' . $customfield_set_nid;

  $entity_json = sirius_json_get($checkin_node);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
  sirius_jsonfield_util_set_defaults($customfield_form, $entity_results);
  $customfield_form['!intro'] = array(
    '#markup' => '<div class="sirius-form">',
    '#weight' => -9999,
  );
  $customfield_form['!outro'] = array(
    '#markup' => '</div>',
    '#weight' => 9999,
  );

  return array($which => $customfield_form);
}