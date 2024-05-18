<?php

function sirius_cardcheck_node_tab_cardcheck_access($cardcheck_node) {
	$access = Sirius_Cardcheck_Access::getInstance();
	return $access->access("view cardcheck", array('cardcheck_node' => $cardcheck_node));
}

function sirius_cardcheck_node_tab_cardcheck_form($form, &$form_state, $cardcheck_node) {
	$instance = Sirius_Cardcheck::getInstance();
	return $instance->cardcheck_form($form, $form_state, $cardcheck_node);
}

function sirius_cardcheck_node_tab_cardcheck_form_submit($form, &$form_state) {
	$instance = Sirius_Cardcheck::getInstance();
	return $instance->cardcheck_form_submit($form, $form_state);
}

