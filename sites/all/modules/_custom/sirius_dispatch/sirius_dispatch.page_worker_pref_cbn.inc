<?php

function sirius_dispatch_node_tab_worker_pref_cbn_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
	  $sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_cbn_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['cbn'] = array(
		'#type' => 'select',
		'#title' => t('Call-by-name Only?'),
		'#options' => array(
			'No' => 'No, I want both open calls and call-by-names',
			'Yes' => 'Yes, I want call by names only. I will not receive open calls.',
		),
		'#default_value' => $worker_node->field_sirius_dispatch_cbn['und'][0]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_cbn_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$val = $form_state['values']['cbn'];
	if ($val != $worker_node->field_sirius_dispatch_cbn['und'][0]['value']) {
		$worker_node->field_sirius_dispatch_cbn['und'][0]['value'] = $val;
		drupal_set_message(t('"Call by Name Only?" set  to @val', array('@val' => ($val?$val:"[NULL]"))));
		node_save($worker_node);
		sirius_log('worker:cbn', 'set', 'CBN preferences have been set to ' . $val, '', '', array($worker_node->nid), array());
	}
}