<?php

function sirius_trust_election_wizard_post($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_post_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_post_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$intro_text = "Click below to post this election wizard. This will create or update any dependent and relationship records, and will create an active election for the worker.";

	$form['#tree'] = TRUE;

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '</div>');

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_post_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	// Push to update the remote version, if necessary
	if (!sirius_proxy_active()) {
		$wizard_rnode = $wizard_node;
	} else {
		$result = sirius_proxy_call('sirius_trust_worker_election_wizard_push_remote', $wizard_node);
		if (!$result['success']) {
			drupal_set_message($result['msg'], 'warning');
			return;
		}
		$data['push']['server_nid'] = $result['wizard_node']->nid;
		$data['push']['ts'] = time();
		$wizard_rnode = $result['wizard_node'];
	}

	$result = sirius_proxy_call('sirius_trust_worker_election_post_from_wizard', $wizard_rnode);

	if ($result['success']) { 
		$wizard_node->field_sirius_type['und'][0]['value'] = 'posted';
		sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	} else {
		drupal_set_message($result['msg'], 'warning');
	}
}
