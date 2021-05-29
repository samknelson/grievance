<?php

function sirius_dispatch_term_tab_skill($skill_term) {
	return drupal_get_form('sirius_dispatch_term_tab_skill_form', $skill_term);
}

function sirius_dispatch_term_tab_skill_form($form, &$form_state, $skill_term) {
	$form_state['skill_tid'] = $skill_term->tid;
	$skill_json = sirius_json_get($skill_term);
	$form['#tree'] = TRUE;

	$form['dispatch']['job']['available'] = array(
		'#title' => t('Available on Job Creation?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $skill_json['dispatch']['job']['available'],
		'#description' => t('Skills with this value set to "No" will not appear on the "Job Creation" form. In other words, dispatches cannot be restricted to these skills.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_term_tab_skill_form_submit($form, &$form_state) {
	$skill_term = taxonomy_term_load($form_state['skill_tid']);
	$skill_json = sirius_json_get($skill_term);
	$skill_json['dispatch'] = $form_state['values']['dispatch'];
	sirius_json_set($skill_term, $skill_json);
	taxonomy_term_save($skill_term);
	drupal_set_message("Settings saved.");
}
