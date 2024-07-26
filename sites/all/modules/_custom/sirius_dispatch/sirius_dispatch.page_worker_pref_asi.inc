<?php

function sirius_dispatch_node_tab_worker_pref_asi_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
		if (class_exists('Sirius_Sib') && !Sirius_Sib::getInstance()->access('dispatch_signin_self', array('entity_node' => $worker_node))) { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_asi_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['asi'] = array(
		'#title' => 'Auto Sign-In',
		'#type' => 'select',
		'#options' => array(
			'No' => t('No, when a dispatch ends I will sign in to the out of work list manually'),
			'Yes' => t('Yes, when a dispatch ends I should be automatically signed in to the out of work list'),
		),
		'#default_value' => $worker_node->field_sirius_dispatch_asi['und'][0]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_asi_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$old_asi = $worker_node->field_sirius_dispatch_asi['und']['0']['value'];
	$new_asi = $form_state['values']['asi'];
	if ($new_asi && $old_asi != $new_asi) {
		$worker_node->field_sirius_dispatch_asi['und']['0']['value'] = $new_asi;
		node_save($worker_node);
		sirius_log('worker:asi', 'set', 'Auto-sign-in preferences have been set to ' . $new_asi, '', '', array($worker_node->nid), array());
		drupal_set_message('Auto-sign-in preferences have been set to ' . $new_asi . '.');
	}
}