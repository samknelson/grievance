<?php


function sirius_trust_election_wizard_push($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_push_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_push_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policy_rnode = $result['data'];
	$policy_json = sirius_json_get($policy_rnode);

	$intro_text = $policy_json['elections']['push']['body'];
	if (!$intro_text) {
		$intro_text = "Click 'Submit' below to submit your application. After your application is submitted, it will be locked and no additional changes will be permitted.";
	}

	$form['#tree'] = TRUE;

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '</div>');

	$form['review'] = array('#markup' => '<p><em>Review to come</em></p>');

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_push_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	if (!sirius_proxy_active()) {
		$result = array('success' => TRUE, 'msg' => 'Push not necessary, running locally.');
	} else {
		$result = sirius_proxy_call('sirius_trust_worker_election_wizard_push_remote', $wizard_node);
		if ($result['wizard_node']) {
			$data['push']['server_nid'] = $result['wizard_node']->nid;
			$data['push']['ts'] = time();
		}
	}

	if ($result['success']) {
		$wizard_node->field_sirius_type['und'][0]['value'] = 'submitted';
		sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	} else {
		drupal_set_message($result['msg'], 'warning');
	}
}
