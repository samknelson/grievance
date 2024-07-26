<?php

function sirius_sib_node_tab_entity_sib_assign_form($form, &$form_state, $entity_node) {
	$sib_obj = Sirius_Sib::getInstance();

	$form['#tree'] = TRUE;
	$form_state['entity_nid'] = $entity_node->nid;

	$options = sirius_taxonomy_load('sirius_dispatch_sib', 'tid', 'name');
	$form['sib']['tid'] = array(
		'#type' => 'select',
		'#title' => t('Add an Access Restriction'),
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

function sirius_sib_node_tab_entity_sib_assign_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$sib_tid = $form_state['values']['sib']['tid'];
	$comment = $form_state['values']['sib']['comment'];

	$sib_obj = Sirius_Sib::getInstance();
	$sib_obj->assign($entity_node, $sib_tid, $comment);
	drupal_set_message("Access restriction added.");
}