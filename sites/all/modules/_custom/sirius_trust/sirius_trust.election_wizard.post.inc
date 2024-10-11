<?php

function sirius_trust_election_wizard_post($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_post_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_post_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$form['#tree'] = TRUE;

	$steps = sirius_trust_election_wizard_steps($data, $wizard_node);
	foreach ($steps as $step) {
		if ($step['file']) { sirius_require($step['module'], $step['file']); }
	}

	$form['status'] = array('#markup' => sirius_trust_election_wizard_status_render($info, $data, $wizard_node, $current_step));

	$form['name'] = array('#markup' => sirius_trust_election_wizard_name_render($info, $data, $wizard_node, $current_step));
	$form['addr'] = array('#markup' => sirius_trust_election_wizard_addr_render($info, $data, $wizard_node, $current_step));
	$form['phone'] = array('#markup' => sirius_trust_election_wizard_phone_render($info, $data, $wizard_node, $current_step));
	$form['email'] = array('#markup' => sirius_trust_election_wizard_email_render($info, $data, $wizard_node, $current_step));
	$form['benefits'] = array('#markup' => sirius_trust_election_wizard_benefits_render($info, $data, $wizard_node, $current_step));

	$form['review'] = array(
		'#markup' => '<p><em>Under development - render the rest of the sections, PDF download</em></p>',
	);

	$access = Sirius_Trust_Access::getInstance();
	$access_context = array('election_wizard_node' => $wizard_node, 'current_step' => $current_step);

	if ($access->access('election_wizard_edit', $access_context)) {
		$form['post'] = array(
			'#markup' => '<p><strong>Click below to post this election wizard. This will create or update any dependent and relationship records, and will create an active election for the worker.</strong></p>',
		);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_post_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	// sirius_require('sirius_trust', 'sirius_trust.changequeue.inc');
	// $result = SiriusResult::create(sirius_trust_election_wizard_name_post($info, $data, $wizard_node, $current_step));
	// if (!$result->success()) { return $result->drupalSetMessage(); }

	drupal_set_message("Under Development", 'warning');
	return;

	$wizard_node->field_sirius_type['und'][0]['value'] = 'posted';
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}

function sirius_trust_election_wizard_status_render($info, $data, $wizard_node, $current_step) {
	$status = sirius_trust_election_wizard_get_status($wizard_node);
	$html .= '<div class="sirius_wizard_render_section sirius_callout_basic">';
	$html .= '<strong>';
	if ($status == 'draft') {
		$html .= 'This is a DRAFT election. It can still be edited.';
	} else if ($status == 'submitted') {
		$html .= 'This is a SUBMITTED election. It is locked and can only be edited by staff. It has not yet been posted.';
	} else if ($status == 'posted') {
		$html .= 'This is a POSTED election. It has been posted to the member\'s record, and cannot be changed. To make any changes, a new election must be created.';
	}
	$html .= '</strong>';
	$html .= '</div>';
	return $html;
}