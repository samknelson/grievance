<?php

/**
 * @todo: I think everything about this is now deprecated, but it's still referenced by letter_wizard, so I can't remove it yet.
 */

function sirius_field_overrides_list() {
	$list = &drupal_static(__FUNCTION__);
	if (!isset($list)) { 
		$list = json_decode(sirius_domain_variable_get('sirius_field_overrides'), TRUE);
		if (!$list) { $list = array(); }
	}
	return $list;
}

function sirius_field_overrides_form_alter(&$form, &$form_state, $form_id) {
	if (preg_match('/(.+)_node_form$/', $form_id, $matches)) {
		$entity_type = 'node';
		$bundle = $matches[1];
		$overrides = sirius_field_overrides_list();

		foreach ($overrides as $override) {
			if ($bundle != $override['bundle']) { continue; }
			if ($entity_type != $override['entity_type']) { continue; }

			$field_name = $override['field_name'];
			if (!$form[$field_name]) { continue; }

			if ($override['label']) {
				if ($form[$field_name]['#title']) { $form[$field_name]['#title'] = $override['label']; }
				if ($form[$field_name]['und']['#title']) { $form[$field_name]['und']['#title'] = $override['label']; }
				if ($form[$field_name]['und'][0]['#title']) { $form[$field_name]['und'][0]['#title'] = $override['label']; }
				if ($form[$field_name]['und'][0]['value']['#title']) { $form[$field_name]['und'][0]['value']['#title'] = $override['label']; }
			}
		}
	}
}

function sirius_field_overrides_node_view(&$node, $view_mode, $langcode) {
	if ($node->content) {
		$entity_type = 'node';
		$bundle = $node->type;
		$overrides = sirius_field_overrides_list();

		foreach ($overrides as $override) {
			if ($bundle != $override['bundle']) { continue; }
			if ($entity_type != $override['entity_type']) { continue; }
			$field_name = $override['field_name'];

			if ($override['label']) {
				if ($node->content[$field_name]['#title']) { $node->content[$field_name]['#title'] = $override['label']; }
			}
		}
	}
}

function sirius_field_overrides_label($entity_type, $bundle, $field_name) {
	$override_list = sirius_field_overrides_list();

	$info = field_info_instance($entity_type, $field_name, $bundle);
	$label = $info['label'];

	foreach ($override_list as $override) { 
		if ($override['entity_type'] == 'entity_type' && 
			$override['bundle'] == $bundle &&
			$override['field_name'] == $field_name) {
			$label = $override['label'];
		}
	}

	return $label;
}