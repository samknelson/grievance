<?php

function sirius_smf_feed_ehrs_upload_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form = sirius_feed_wizard_upload_form($form, $form_state, $info, $data, $wizard_node, $current_step);

	// We don't use the source field
	$form['source']['#type'] = 'value';
	$form['source']['#value'] = $form['source']['#default_value'];
	
	return sirius_sms_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_upload_form_validate($form, &$form_state) {
	return sirius_feed_wizard_upload_form_validate($form, $form_state);
}

function sirius_smf_feed_ehrs_upload_form_submit($form, &$form_state) {
	return sirius_feed_wizard_upload_form_submit($form, $form_state);
}