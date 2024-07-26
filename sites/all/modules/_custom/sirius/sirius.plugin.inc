<?php

function sirius_plugin_info_all($class) {
	$info_all = &drupal_static(__FUNCTION__, NULL);

	if (!isset($info_all[$class])) {
		$info_all[$class] = module_invoke_all('sirius_plugin_info', $class);
		if ($info_all[$class]) { ksort($info_all[$class]); }
	}

	return $info_all[$class];
}

function sirius_plugin_info($class, $key) {
	$info_all = sirius_plugin_info_all($class);
	return $info_all[$key];
}

function sirius_plugin_entity_plugins($entity_node, $class) {
	$entity_json = sirius_json_get($entity_node);

	if (!$entity_json['plugins'][$class]) { return array(); }
	$plugins = $entity_json['plugins'][$class];

	foreach ($plugins as $plugin_uuid => $plugin) {
		$plugins[$plugin_uuid]['uuid'] = $plugin_uuid;
		$plugins[$plugin_uuid]['info'] = sirius_plugin_info($class, $plugin['plugin']);
	}

	uksort($plugins, function($a, $b) use ($plugins) { return $plugins[$a]['weight'] <=> $plugins[$b]['weight']; });

	return $plugins;
}

function sirius_plugin_entity_plugin($entity_node, $class, $plugin_uuid) {
	$all_plugins = sirius_plugin_entity_plugins($entity_node, $class);
	return $all_plugins[$plugin_uuid];
}


class Sirius_Plugin extends Sirius_Singleton {
	public function entity_settings($entity_node, $plugin_uuid) {
		$entity_json = sirius_json_get($entity_node);
		return $entity_json['plugin_settings'][$plugin_uuid];
	}

	public function entity_settings_set($entity_node, $plugin_uuid, $settings) {
		$entity_json = sirius_json_get($entity_node);
		$entity_json['plugin_settings'][$plugin_uuid] = $settings;
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);
	}

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form_state['entity_nid'] = $entity_node->nid;
		$form_state['plugin_uuid'] = $plugin_uuid;

		$form['#tree'] = TRUE;

		$form['settings'] = array('#markup' => '<p>' . t('There are no settings for this plugin.') . '</p>');

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
			'#weight' => 10,
		);

		return $form;
	}

	public function entity_settings_form_validate($form, &$form_state) {
		return;
	}

	public function entity_settings_form_submit($form, &$form_state) {
		$entity_node = node_load($form_state['entity_nid']);
		$plugin_uuid = $form_state['plugin_uuid'];
		$this->entity_settings_set($entity_node, $plugin_uuid, $form_state['values']['settings']);
		return;
	}
}
