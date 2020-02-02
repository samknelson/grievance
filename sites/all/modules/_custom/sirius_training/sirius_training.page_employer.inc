<?php

function sirius_training_node_tab_employer_access($employer_node) {
	return sirius_employer_node_tab_employer_users_access($employer_node);
}

function sirius_training_node_tab_employer($employer_node) {
	return drupal_get_form('sirius_training_node_tab_employer_form', $employer_node);
}

function sirius_training_node_tab_employer_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$all_levels = sirius_training_levels_fetch_all();
	$form['#tree'] = TRUE;

	foreach ($all_levels as $ms_tid => $levels) {
		$ms_term = taxonomy_term_load($ms_tid);
		$form["ms_$ms_tid"] = array(
			'#type' => 'fieldset',
			'#title' => $ms_term->name
		);

		foreach ($levels as $level_code => $level) {
			$form["ms_$ms_tid"][$level_code]['notification_emails'] = array(
				'#title' => t('Notification Emails for @level', array('@level' => $level['title'])),
				'#type' => 'textarea',
				'#default_value' => $json['training']["ms_$ms_tid"][$level_code]['notification_emails'],
				'#description' => t('Enter email addresses which should be notified when a worker attains this level, one per line.')
			);
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_node_tab_employer_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$values = $form_state['values'];

	unset($values['op']);
	unset($values['submit']);
	unset($values['form_id']);
	unset($values['form_token']);
	unset($values['form_build_id']);

	$json['training'] = $values;
	$employer_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($employer_node);
	drupal_set_message(t('Settings saved.'));

}