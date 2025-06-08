<?php

function sirius_trust_node_tab_benefit_dashboard_settings_form($form, &$form_state, $benefit_node) {
	$form['#tree'] = TRUE;
	$form_state['benefit_nid'] = $benefit_node->nid;
	
	$form['dashboard']['standalone'] = array(
		'#type' => 'select',
		'#title' => t('Standalone Dashboard Card?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Should this benefit appear as a standalone dashboard card?'),
	);

	$form['dashboard']['details'] = array(
		'#type' => 'fieldset',
		'#title' => t('Dashboard Details'),
	 	'#states' => array('visible' => array(":input[name=\"dashboard[standalone]\"]" => array('value' => 'Yes'))),
	);

	$form['dashboard']['details']['description'] = array(
		'#type' => 'text_format',
		'#title' => t('Description'),
		'#description' => t('Enter a brief description that should appear on the dashboard.'),
	);

	$benefit_json = sirius_json_get($benefit_node);
	sirius_jsonfield_util_set_defaults($form['dashboard'], $benefit_json['dashboard']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_benefit_dashboard_settings_form_submit($form, &$form_state) {
	$benefit_node = node_load($form_state['benefit_nid']);
	$benefit_json = sirius_json_get($benefit_node);
	$benefit_json['dashboard'] = $form_state['values']['dashboard'];
	sirius_json_set($benefit_node, $benefit_json);
	node_save($benefit_node);
	drupal_set_message("Settings saved.");
}