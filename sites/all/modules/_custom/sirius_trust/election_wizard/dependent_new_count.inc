<?php

function sirius_trust_election_wizard_dependent_new_count($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_dependent_new_count_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_dependent_new_count_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {

	$form['intro'] = array(
		'#markup' => '<p>' . t('If there are family members that were not listed in the previous steps, you can add them now. Select the number of <strong>new family members</strong> that you would like to create and cover.') . '</p>',
	);

	$form['dependents']['general']['count_new'] = array(
		'#title' => t('Add and Cover New Family Members:'),
		'#type' => 'select',
		'#default_value' => $data['dependents']['general']['count_new'],
		'#required' => TRUE,
		'#options' => array(
			0 => 0,
			1 => 1,
			2 => 2,
			3 => 3,
			4 => 4,
			5 => 5,
			6 => 6,
		),
	);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependent_new_count_form_validate($form, &$form_state) {
}

function sirius_trust_election_wizard_dependent_new_count_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];
	$data['dependents']['general'] = $form_state['values']['dependents']['general'];
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Name saved.", array('nonsequential' => FALSE));

	sirius_trust_election_set_title($wizard_node);
}
