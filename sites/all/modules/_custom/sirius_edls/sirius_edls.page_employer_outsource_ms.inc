<?php

function sirius_edls_node_tab_employer_outsource_ms_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config_outsource_ms', array('employer_nid' => $employer_node->nid));
}


function sirius_edls_node_tab_employer_outsource_ms_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$options = sirius_edls_outsource_ms_list($employer_node);
	$options_encode = sirius_edls_outsource_ms_encode($options);

	$form['options'] = array(
		'#type' => 'textarea',
		'#rows' => 20,
		'#title' => t('Member Status Options'),
		'#description' => t("Enter the member status options one per line. Each line should contain the member status key and title, i.e. \"APP|Apprentice\""),
		'#default_value' => $options_encode,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_node_tab_employer_outsource_ms_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$options_encode = $form_state['values']['options'];
	$result = sirius_edls_outsource_ms_decode($options_encode);
	if (!$result['success']) {
		drupal_set_message("Error: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
		return;
	}

	$options = $result['options'];

	sirius_edls_outsource_ms_set_all($employer_node, $options);

	drupal_set_message("Member Status Options Set");
}
