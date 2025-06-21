<?php

function sirius_node_tab_json_definition_cardcheck_access($cardcheck_definition_node) {
	return ($cardcheck_definition_node->field_sirius_type['und'][0]['value'] == 'sirius:cardcheck');
}

function sirius_node_tab_json_definition_cardcheck($cardcheck_definition_node) {
	return drupal_get_form('sirius_cardcheck_definition_form', $cardcheck_definition_node);
}