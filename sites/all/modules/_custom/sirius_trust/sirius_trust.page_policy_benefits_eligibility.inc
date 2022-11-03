<?php

function sirius_trust_node_tab_policy_benefits_eligibility($policy_node) {
	$plugin_key = sirius_node_tab_arg(0);

	$html = sirius_trust_node_tab_benefit_eligibility_quicknav($benefit_node);

	if ($plugin_key) {
		$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
		if (!$plugin_info) { 
			drupal_set_message("Eligibility plugin [$plugin_key] was not found.", 'error');
			return '';
		}

		$enabled_plugins = sirius_trust_eligibility_plugins_enabled($benefit_node);
		if (!in_array($plugin_key, $enabled_plugins)) {
			drupal_set_message("Eligibility plugin $plugin_info[title] is not enabled for this benefit.", 'error');
			return '';
		}

		sirius_trust_eligibility_plugin_load($plugin_key);

		$form = array();

		if ($plugin_info['callbacks']['benefit_settings']) {
			$form = drupal_get_form($plugin_info['callbacks']['benefit_settings'], $benefit_node, $plugin_key);
		} else {
			$form['nosettings'] = array('#markup' => '<p><em>' . t('There are no settings for this plugin') . '</em></p>');
		}

		if ($plugin_info['description']) {
			$form = array('intro' => array(
				'#markup' => '<div class="sirius_trust_eligibility_plugin_description">' . $plugin_info['description'] . '</div>',
			)) + $form;
		}
	} else {
		$form = drupal_get_form('sirius_trust_node_tab_benefit_eligibility_enabled_form', $benefit_node);
	}

	$html .= drupal_render($form);
	return $html;
}

function sirius_trust_node_tab_benefit_eligibility_quicknav($benefit_node) {
	$enabled_plugins = sirius_trust_eligibility_plugins_enabled($benefit_node);
	$enabled_plugins = array('ws', 'election');
	if (!$enabled_plugins) { return ''; }

	$base_path = 'node/' . $benefit_node->nid . '/sirius_trust_benefit_eligibility';

  $links = array();
  $link = array();
  $link['title'] = 'Enabled Plugins';
  $link['path'] = $base_path;
  $links[] = $link;

  foreach ($enabled_plugins as $plugin_key) {
  	$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	  $link['title'] = $plugin_info['title'];
	  $link['path'] = "$base_path/$plugin_key";
	  $links[] = $link;
  }
	return sirius_quicknav($links);
}

function sirius_trust_node_tab_benefit_eligibility_enabled_form($form, &$form_state, $benefit_node) {
	$form_state['benefit_nid'] = $benefit_node->nid;

	$benefit_json = sirius_json_get($benefit_node);

	$plugin_info_all = sirius_trust_eligibility_plugin_info_all();
	$plugin_options = array();
	foreach ($plugin_info_all as $plugin_key => $plugin_info) {
		$plugin_options[$plugin_key] = $plugin_info['title'];
	}

	$form['#tree'] = TRUE;

	$form['eligibility']['plugins']['enabled'] = array(
		'#title' => 'Enabled Eligigibility Plugins',
		'#type' => 'checkboxes',
		'#options' => $plugin_options,
		'#default_value' => sirius_form_array_default_value($benefit_json['eligibility']['plugins']['enabled']),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_benefit_eligibility_enabled_form_submit($form, &$form_state) {
	$benefit_node = node_load($form_state['benefit_nid']);
	$benefit_json = sirius_json_get($benefit_node);
	$benefit_json['eligibility']['plugins']['enabled'] = $form_state['values']['eligibility']['plugins']['enabled'];
	sirius_json_set($benefit_node, $benefit_json);
	node_save($benefit_node);
	drupal_set_message('Settings saved.');
}
