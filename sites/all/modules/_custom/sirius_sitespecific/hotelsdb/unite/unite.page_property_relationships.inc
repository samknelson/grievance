<?php

function unite_node_tab_property_relationships($property_node) {
	return 'Relationship editor goes here.';
	// return drupal_get_form('unite_node_tab_property_relationships_form', $property_node);
}

function unite_node_tab_property_relationships_form($form, &$form_state, $property_node) {
	$form['results'] = array('#markup' => $results);
	return $form;
}