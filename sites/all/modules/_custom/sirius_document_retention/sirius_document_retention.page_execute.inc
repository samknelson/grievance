<?php

function sirius_document_retention_page_execute($key) {
	return drupal_get_form('sirius_document_retention_page_execute_form', $key);
}

function sirius_document_retention_page_execute_form($form, &$form_state, $key) {
	$policy = sirius_document_retention_policy_info($key);
	if (!$policy) {
		drupal_set_message("Unknown key: $key", 'error');
		return NULL;
	}

	$form['key'] = array(
		'#type' => 'value',
		'#value' => $key,
	);

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius-form-intro sirius-document-retention-execute-intro">' . 
			t('Use the form below to execute the document retention policy: ') . 
			'<strong>' . $policy['name'] . '</strong> - ' . $policy['description'] .
			'</div>',
	);

	$form['mode'] = array(
		'#type' => 'select',
		'#options' => array(
			'dry' => t('Dry run - Run a test execution, but do not make any changes.'),
			'live' => t('Live - Implement the live execution.  Data may be deleted.')
		)
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Execute Now',
	);

	return $form;
}

function sirius_document_retention_page_execute_form_submit($form, &$form_state) {
	$key = $form_state['values']['key'];
	$mode = $form_state['values']['mode'];
	sirius_document_retention_policy_execute($key, $mode);
	drupal_set_message('<strong>Results</strong><pre>' . sirius_document_retention_log_render() . '</pre>');
}
