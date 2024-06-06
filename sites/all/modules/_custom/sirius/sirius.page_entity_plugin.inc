<?php

function sirius_node_tab_entity_plugin_form($form, &$form_state, $entity_node, $plugin_class, $plugin_uuid) {
	$form['nav'] = array('#markup' => sirius_node_tab_entity_plugins_quicknav($entity_node, $plugin_class), '#weight' => -10);

	$form_state['entity_nid'] = $entity_node->nid;
	$form_state['plugin_class'] = $plugin_class;

	if (!$plugin_uuid) { return sirius_form_error("No plugin UUID."); }
	$form_state['plugin_uuid'] = $plugin_uuid;

	$plugin = sirius_plugin_entity_plugin($entity_node, $plugin_class, $plugin_uuid);
	if (!$plugin) { return sirius_form_error('plugin UUID not found.'); }

	$plugin_key = $plugin['plugin'];
	if (!$plugin_key) { return sirius_form_error('No plugin specified.'); }

	$info = sirius_plugin_info($plugin_class, $plugin_key);
	if (!$info) { return sirius_form_error("Plugin not defined: $plugin_key"); }

	return $info['instance']->entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);
}

function sirius_node_tab_entity_plugin_form_validate($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$plugin_class = $form_state['plugin_class'];
	$plugin_uuid = $form_state['plugin_uuid'];
	$plugin = sirius_plugin_entity_plugin($entity_node, $plugin_class, $plugin_uuid);
	$plugin_key = $plugin['plugin'];
	$info = sirius_plugin_info($plugin_class, $plugin_key);
	return $info['instance']->entity_settings_form_validate($form, $form_state);
}

function sirius_node_tab_entity_plugin_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$plugin_class = $form_state['plugin_class'];
	$plugin_uuid = $form_state['plugin_uuid'];
	$plugin = sirius_plugin_entity_plugin($entity_node, $plugin_class, $plugin_uuid);
	$plugin_key = $plugin['plugin'];
	$info = sirius_plugin_info($plugin_class, $plugin_key);
	drupal_set_message("Setting $plugin_class, $plugin_key");
	return $info['instance']->entity_settings_form_submit($form, $form_state);
}