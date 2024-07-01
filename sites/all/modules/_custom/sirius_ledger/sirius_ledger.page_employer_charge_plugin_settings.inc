<?php

function sirius_ledger_node_tab_employer_charge_plugin_settings_quicknav($employer_node, $policy_node) {
	$items = sirius_plugin_entity_plugins($policy_node, 'ledger_charge');

	$base_path = 'node/' . $employer_node->nid . '/sirius_employer_ledger/charge_plugin_settings';

	$links = array();
	foreach ($items as $plugin_uuid => $item) {
		$info = sirius_plugin_info('ledger_charge', $item['plugin']);
		if (!$info) { continue; }
		if (!$info['instance']) { continue; }
		if (!$info['instance']->employer_settings_enabled()) { continue; }
		$link['title'] = $item['title'];
		$link['path'] = "$base_path/$plugin_uuid";
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

function sirius_ledger_node_tab_employer_charge_plugin_settings_access($employer_node) {
	$policy_node = sirius_ledger_policy_lookup(NULL, $employer_node);
	if (!$policy_node) { return FALSE; }

	$items = sirius_plugin_entity_plugins($policy_node, 'ledger_charge');
	if (!$items) { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_employer_charge_plugin_settings($employer_node) {
	$plugin_uuid = sirius_node_tab_arg(0);
	$policy_node = sirius_ledger_policy_lookup(NULL, $employer_node);

	if (!$plugin_uuid) {
		$items = sirius_plugin_entity_plugins($policy_node, 'ledger_charge');
		drupal_goto(current_path() . '/' . reset(array_keys($items)));
	}

	return drupal_get_form('sirius_ledger_node_tab_employer_charge_plugin_settings_form', $employer_node, $policy_node, $plugin_uuid);
}

function sirius_ledger_node_tab_employer_charge_plugin_settings_form($form, &$form_state, $employer_node, $policy_node, $plugin_uuid) {
	$form['nav'] = array('#markup' => sirius_ledger_node_tab_employer_charge_plugin_settings_quicknav($employer_node, $policy_node), '#weight' => -10);

	$form_state['policy_nid'] = $policy_node->nid;

	if (!$plugin_uuid) { return sirius_form_error("No item UUID."); }
	$form_state['plugin_uuid'] = $plugin_uuid;

	$item = sirius_plugin_entity_plugin($policy_node, 'ledger_charge', $plugin_uuid);
	if (!$item) { return sirius_form_error('Item UUID not found.'); }

	$plugin_key = $item['plugin'];
	if (!$plugin_key) { return sirius_form_error('No plugin specified.'); }

	$info = sirius_plugin_info('ledger_charge', $plugin_key);
	if (!$info) { return sirius_form_error("Plugin not defined: $plugin_key"); }

	return $info['instance']->employer_settings_form($form, $form_state, $employer_node, $policy_node, $plugin_uuid);
}

function sirius_ledger_node_tab_employer_charge_plugin_settings_form_validate($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$plugin_uuid = $form_state['plugin_uuid'];
	$item = sirius_plugin_entity_plugin($policy_node, 'ledger_charge', $plugin_uuid);
	$plugin_key = $item['plugin'];
	$info = sirius_plugin_info('ledger_charge', $plugin_key);
	return $info['instance']->employer_settings_form_validate($form, $form_state);
}

function sirius_ledger_node_tab_employer_charge_plugin_settings_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$plugin_uuid = $form_state['plugin_uuid'];
	$item = sirius_plugin_entity_plugin($policy_node, 'ledger_charge', $plugin_uuid);
	$plugin_key = $item['plugin'];
	$info = sirius_plugin_info('ledger_charge', $plugin_key);
	return $info['instance']->employer_settings_form_submit($form, $form_state);
}

/*
function sirius_ledger_node_tab_employer_charge_plugin_settings_form($form, &$form_state, $employer_node, $policy_node, $plugin_uuid) {
	$form['nav'] = array('#markup' => sirius_ledger_node_tab_policy_charge_plugins_quicknav($policy_node), '#weight' => -10);

	$form_state['policy_nid'] = $policy_node->nid;

	if (!$plugin_uuid) { return sirius_form_error("No item UUID."); }
	$form_state['plugin_uuid'] = $plugin_uuid;

	$item = sirius_plugin_entity_plugin($policy_node, 'ledger_charge', $plugin_uuid);
	if (!$item) { return sirius_form_error('Item UUID not found.'); }

	$plugin_key = $item['plugin'];
	if (!$plugin_key) { return sirius_form_error('No plugin specified.'); }

	$info = sirius_plugin_info('ledger_charge', $plugin_key);
	if (!$info) { return sirius_form_error("Plugin not defined: $plugin_key"); }
	return $info['instance']->employer_settings_form($form, $form_state, $policy_node, $plugin_uuid);
}
*/