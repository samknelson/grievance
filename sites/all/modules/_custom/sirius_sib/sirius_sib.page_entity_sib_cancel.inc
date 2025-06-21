<?php

function sirius_sib_node_tab_entity_sib_cancel_form($form, &$form_state, $entity_node) {
	$sib_obj = Sirius_Sib::getInstance();

	$form['#tree'] = TRUE;
	$form_state['entity_nid'] = $entity_node->nid;

	$entries = $sib_obj->list($entity_node);
	$options = array();
	foreach ($entries as $sib_tid => $sib_entry) {
		if (!$sib_entry['active']) { continue; }
		$options[$sib_tid] = $sib_entry['term']->name;
	}
	if (!$options) {
		return sirius_form_error("There are no active access restrictions.");
	}

	$form['sib']['tid'] = array(
		'#type' => 'select',
		'#title' => t('Cancel an Access Restriction'),
		'#options' => array(NULL => t('-- Select --')) + $options,
		'#required' => TRUE,
	);

	$form['sib']['comment'] = array(
		'#type' => 'textarea',
		'#title' => t('Comment'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_sib_node_tab_entity_sib_cancel_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$sib_tid = $form_state['values']['sib']['tid'];
	$comment = $form_state['values']['sib']['comment'];

	$sib_obj = Sirius_Sib::getInstance();
	$sib_obj->cancel($entity_node, $sib_tid, $comment);
	drupal_set_message("Access restriction canceled.");
}