<?php

function sirius_node_tab_employer_dispatch_payrate($employer_node) {
	return drupal_get_form('sirius_node_tab_employer_dispatch_payrate_form', $employer_node);
}

function sirius_node_tab_employer_dispatch_payrate_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$skills = sirius_taxonomy_load('sirius_skill', 'tid', 'name');
	$member_statuses = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$form['#tree'] = TRUE;

	$form['pay_at_skilled_rate'] = array(
		'#type' => 'fieldset',
		'#title' => t('Pay at Skilled Rate'),
		'#description' => t('For jobs created by this employer, workers who have the one of the following skills AND one of the following member statuses should ALWAYS be paid at the skilled rate, regardless of whether the job explicitly calls for those skills.'),
	);

	$form['pay_at_skilled_rate']["skill"] = array(
		'#type' => 'select',
		'#options' => $skills,
		'#title' => t('Skills'),
		'#multiple' => TRUE,
		'#size' => 20,
	);
	if ($json['dispatch']['pay_at_skilled_rate']['skill']) { $form['pay_at_skilled_rate']["skill"]['#default_value'] = $json['dispatch']['pay_at_skilled_rate']['skill']; }

	$form['pay_at_skilled_rate']["member_status"] = array(
		'#type' => 'select',
		'#options' => $member_statuses,
		'#title' => t('Member Statuses'),
		'#multiple' => TRUE,
		'#size' => 10,
		'#description' => t('(If nothing is selected, the higher skill rate will apply for ALL member statuses.'),
	);
	if ($json['dispatch']['pay_at_skilled_rate']['member_status']) { $form['pay_at_skilled_rate']["member_status"]['#default_value'] = $json['dispatch']['pay_at_skilled_rate']['member_status']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	return $form;
}

function sirius_node_tab_employer_dispatch_payrate_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['dispatch']['pay_at_skilled_rate']['skill'] = $form_state['values']['pay_at_skilled_rate']['skill'];
	$json['dispatch']['pay_at_skilled_rate']['member_status'] = $form_state['values']['pay_at_skilled_rate']['member_status'];
	$employer_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($employer_node);
	drupal_set_message(t('Settings saved.'));
}
