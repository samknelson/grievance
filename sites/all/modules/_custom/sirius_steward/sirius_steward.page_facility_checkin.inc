<?php

function sirius_steward_node_tab_facility_checkin($facility_node) {
	$checkin_nids = sirius_steward_checkin_search(array('facility_nid' => $facility_node->nid, 'status' => 'in'));

	if ($checkin_nids) {
		$checkin_nid = reset($checkin_nids);
		$checkin_node = node_load($checkin_nid);
		return drupal_get_form('sirius_steward_node_tab_facility_checkin_form', $checkin_node);
	} else {
		return '<p>' . t('There is no steward currently checked in to this facility.') . '</p>';
	}
}

function sirius_steward_node_tab_facility_checkin_form($form, &$form_state, $checkin_node) {
	$form['#tree'] = TRUE;
	$form_state['checkin_nid'] = $checkin_node->nid;
	$form['checkin']['geolocation'] = sirius_map_geolocation_render_form();
	$form['checkin']['render'] = sirius_steward_checkin_render_form($checkin_node);
	if ($checkin_node->field_sirius_type['und'][0]['value'] == 'in') {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Check Out'),
		);
	}
	return $form;
}

function sirius_steward_node_tab_facility_checkin_form_submit($form, &$form_state) {
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
