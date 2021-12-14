<?php

function unite_node_tab_fairhotel_edit($property_node) {
	return drupal_get_form('unite_node_tab_fairhotel_edit_form', $property_node);
}

function unite_node_tab_fairhotel_edit_form($form, &$form_state, $property_node) {
	$form_state['property_nid'] = $property_node->nid;

	$fields = unite_fairhotel_fields();
	foreach ($fields as $field) {
		field_attach_form('node', $property_node, $form, $form_state, NULL, array('field_name' => $field));
 }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 1000,
	);

	return $form;
}

function unite_node_tab_fairhotel_edit_form_submit($form, &$form_state) {
	$property_node = node_load($form_state['property_nid']);

	$fields = unite_fairhotel_fields();
	foreach ($fields as $field) {
		if (!unite_submitted_empty($form_state, $field)) { 
			$property_node->{$field} = $form_state['values'][$field];
		} else {
			$property_node->{$field}['und'] = NULL;
		}
	}

	node_save($property_node);
	drupal_set_message("Data saved.");
}
