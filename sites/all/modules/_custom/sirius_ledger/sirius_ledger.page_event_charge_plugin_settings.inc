<?php

function sirius_ledger_node_tab_event_charge_plugin_settings_quicknav($event_node) {
	$items = sirius_ledger_charge_plugins_event();
	$base_path = 'node/' . $event_node->nid . '/sirius_event_charge_plugin_settings';

	$links = array();
	foreach ($items as $plugin_uuid => $item) {
		$link['title'] = $item['title'];
		$link['path'] = "$base_path/$plugin_uuid";
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

function sirius_ledger_node_tab_event_charge_plugin_settings_access($event_node) {
	$items = sirius_ledger_charge_plugins_event();
	if (!$items) { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_event_charge_plugin_settings($event_node) {
	$plugin_uuid = sirius_node_tab_arg(0);
	if (!$plugin_uuid) {
		$items = sirius_ledger_charge_plugins_event();
		drupal_goto(current_path() . '/' . reset(array_keys($items)));
	}

	return drupal_get_form('sirius_ledger_node_tab_event_charge_plugin_settings_form', $event_node, $plugin_uuid);
}

function sirius_ledger_node_tab_event_charge_plugin_settings_form($form, &$form_state, $event_node, $plugin_uuid) {
	$form['nav'] = array('#markup' => sirius_ledger_node_tab_event_charge_plugin_settings_quicknav($event_node), '#weight' => -10);
	$form_state['plugin_uuid'] = $plugin_uuid;
	$items = sirius_ledger_charge_plugins_event();
	return $items[$plugin_uuid]['info']['instance']->event_settings_form($form, $form_state, $event_node, $plugin_uuid);
}

function sirius_ledger_node_tab_event_charge_plugin_settings_form_validate($form, &$form_state) {
	$plugin_uuid = $form_state['plugin_uuid'];
	$items = sirius_ledger_charge_plugins_event();
	return $items[$plugin_uuid]['info']['instance']->event_settings_form_validate($form, $form_state);
}

function sirius_ledger_node_tab_event_charge_plugin_settings_form_submit($form, &$form_state) {
	$plugin_uuid = $form_state['plugin_uuid'];
	$items = sirius_ledger_charge_plugins_event();
	return $items[$plugin_uuid]['info']['instance']->event_settings_form_submit($form, $form_state);
}
