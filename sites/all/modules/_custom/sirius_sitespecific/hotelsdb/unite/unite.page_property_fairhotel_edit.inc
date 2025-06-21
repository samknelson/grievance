<?php

function unite_node_tab_fairhotel_edit_access($property_node) {
	return $property_node->field_is_hotel['und'][0]['value'] == 'Y';
}

function unite_node_tab_fairhotel_edit($property_node) {
	return drupal_get_form('unite_node_tab_fairhotel_edit_form', $property_node);
}

function unite_node_tab_fairhotel_edit_form($form, &$form_state, $property_node) {
	return unite_node_subform($form, $form_state, $property_node, unite_fairhotel_fields());
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

	$property_node->revision = TRUE;
	node_save($property_node);
	drupal_set_message("Data saved.");
}
