<?php

function sirius_steward_page_checkin_list() {
	return drupal_get_form('sirius_steward_page_checkin_list_form');
}

function sirius_steward_page_checkin_list_form($form, &$form_state) {
	$checkin_nids = sirius_steward_checkin_search(array('status' => 'in'));
	
	$params = array();
	$params['hide_status'] = TRUE;
	$params['hide_out'] = TRUE;
	$html = sirius_steward_checkin_render_list($checkin_nids, $params);
	$form['list'] = array('#markup' => $html);

	return $form;
}


function sirius_steward_page_checkin_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}