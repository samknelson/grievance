<?php

function sirius_node_tab_json_definition_cardcheck_preview_access($cardcheck_definition_node) {
	return ($cardcheck_definition_node->field_sirius_type['und'][0]['value'] == 'sirius:cardcheck');
}

function sirius_node_tab_json_definition_cardcheck_preview_form($form, &$form_state, $definition_node) {
	$instance = Sirius_Cardcheck::getInstance();
	$cardcheck_node = $instance->test_cardcheck_node($definition_node->nid);

	return $instance->cardcheck_form($form, $form_state, $cardcheck_node, array('can_clear' => TRUE));
}

function sirius_node_tab_json_definition_cardcheck_preview_form_submit($form, &$form_state) {
	$instance = Sirius_Cardcheck::getInstance();
	return $instance->cardcheck_form_submit($form, $form_state);
}
