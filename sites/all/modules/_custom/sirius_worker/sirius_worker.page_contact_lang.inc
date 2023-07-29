<?php

function sirius_node_tab_worker_contact_lang_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_contact_lang_form($form, &$form_state, $worker_node) {
	$contact_node = sirius_worker_contact_get($worker_node);
	if (!$contact_node) { return sirius_form_error('Contact not found.'); }

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	for ($i=0; $i<5; ++$i) {
		$form['lang'][$i] = array(
			'#type' => 'select',
			'#title' => t('Language #@i', array('@i' => $i+1)),
			'#options' => array(NULL => t('-- Select --')) + sirius_lang_options(),
			'#default_value' => $contact_node->field_sirius_lang['und'][$i]['value'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_worker_contact_lang_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
	$contact_node = sirius_worker_contact_get($worker_node);

	$fieldval = array();
	for ($i=0; $i<5; ++$i) {
		$val = $form_state['values']['lang'][$i];
		if (!$val) { continue; }
		$fieldval[] = array('value' => $val);
	}

	$contact_node->field_sirius_lang['und'] = $fieldval; 
	node_save($contact_node);
	drupal_set_message("Languages saved.");
}
