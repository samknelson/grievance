<?php

function sirius_steward_node_tab_log_checkin_access($checkin_node) {
	if ($checkin_node->field_sirius_category['und'][0]['value'] != 'steward:checkin') { return FALSE; }
	return TRUE;
}

function sirius_steward_node_tab_log_checkin_form($form, &$form_state, $checkin_node) {
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

function sirius_steward_node_tab_log_checkin_form_submit($form, &$form_state) {
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
