<?php

function sirius_teamsters631_node_tab_employer_m12_emails_form($form, &$form_state, $employer_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$form_state['employer_nid'] = $employer_node->nid;
	$form['#tree'] = TRUE;

	$employer_json = sirius_json_get($employer_node);


	$emails = $employer_json['teamsters631_m12']['emails'];
	if (!$emails) { $emails = array(); }
	$emails[] = '';
	$emails[] = '';

	foreach ($emails as $delta => $email) {
		$form['emails'][$delta] = array(
			'#type' => 'textfield',
			'#title' => 'Email Address #' . ($delta+1),
			'#default_value' => $emails[$delta],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save Email Addresses'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_employer_m12_emails_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['teamsters631_m12']['emails'] = array_filter($form_state['values']['emails']);
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);

	drupal_set_message("Email addresses saved.");
}