<?php

function sirius_bulk_node_tab_test($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_test_form', $bulk_node);
}

function sirius_bulk_node_tab_test_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;

	$form['to'] = array(
		'#type' => 'textfield',
		'#title' => t('Recipient'),
		'#description' => t('Enter the recipient: phone number, email address, or NID.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send test message'),
	);

	return $form;
}

function sirius_bulk_node_tab_test_form_submit($form, &$form_state) {
	$bulk_nid = $form_state['bulk_nid'];
	$bulk_node = node_load($bulk_nid);
	$form_state['rebuild'] = TRUE;

	$result = sirius_bulk_deliver_impl($bulk_node, $form_state['values']['to'], TRUE);

	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'warning');
	}
}