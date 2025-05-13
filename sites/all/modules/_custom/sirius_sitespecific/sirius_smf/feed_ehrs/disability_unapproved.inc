<?php

function sirius_smf_feed_ehrs_disability_unapproved_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_nid'] = $feed_node->nid;
	$form_state['current_step'] = $current_step;

	$which = preg_replace('/disability_unapproved_/', '', $current_step);
	if (!($which > 0)) { return sirius_form_error("Don't know which new worker."); }
	$row = $data['records']['rows'][$which-1];
	if (!$row) { return sirius_form_error("New worker not found."); }

	$html = '<h2>' . t('Confirm Un-Approved Disability') . '</h2>';
	$html .= '<p>The worker below was not on disability or FMLA last month. In order to enter them as disabled, please upload the FMLA denial document.</p>';


	$html .= sirius_fakerender_field($fields[$pk]['title'], $row[$pk]);
	$html .= sirius_fakerender_field('First Name', $row['name_first']);
	$html .= sirius_fakerender_field('Last Name', $row['name_last']);
	$html .= '<br />';

	$form['intro'] = array('#markup' => $html);

	$form['file'] = array(
		'#type' => 'file',
		'#title' => t('FMLA Denial'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm Disability',
	);

	return sirius_sms_feed_ehrs_form_prep($form, $form_state, $info, $data, $feed_node, $current_step);
}

function sirius_smf_feed_ehrs_disability_unapproved_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = node_load($form_state['feed_nid']);
	$current_step = $form_state['current_step'];

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Un-Approved Disability Confirmed.");
}