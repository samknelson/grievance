<?php

function sirius_cardcheck_node_tab_cardcheck_offline_access($cardcheck_node) {
	if ($cardcheck_node->field_sirius_category['und'][0]['value'] != 'cardcheck') { return FALSE; }
	$access = Sirius_Cardcheck_Access::getInstance();
	return $access->access("create offline cardcheck", array('cardcheck_node' => $cardcheck_node));
}

function sirius_cardcheck_node_tab_cardcheck_offline_form($form, &$form_state, $cardcheck_node) {
	$instance = Sirius_Cardcheck::getInstance();
	return $instance->cardcheck_offline_form($form, $form_state, $cardcheck_node);
}

function sirius_cardcheck_node_tab_cardcheck_offline_form_submit($form, &$form_state) {
	$instance = Sirius_Cardcheck::getInstance();
	$result = $instance->cardcheck_offline_form_submit($form, $form_state);
	if ($result->success()) {
		drupal_goto('node/' . $form_state['cardcheck_nid'] . "/sirius_log_cardcheck");
	}
}

