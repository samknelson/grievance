<?php

function sirius_pcct_node_tab_service_rates_by_level_form($form, &$form_state, $service_node) {
	$form_state['service_nid'] = $service_node->nid;
	$form['#tree'] = TRUE;

	$json = sirius_json_get($service_node);

	$levels = sirius_taxonomy_load('sirius_trust_provider_level', 'tid', 'name');
	if (!$levels) {
		return sirius_form_error('No levels have been defined. Please ' . l('add at least one provider level.', 'sirius/taxonomy/sirius_trust_provider_level'));
	}

	foreach ($levels as $level_tid => $level_name) {
		$form['rates']['by_level'][$level_tid] = array(
			'#type' => 'textfield',
			'#title' => $level_name,
			'#size' => 4,
			'#default_value' => $json['rates']['by_level'][$level_tid],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_pcct_node_tab_service_rates_by_level_form_submit($form, &$form_state) {
	$service_node = node_load($form_state['service_nid']);
	$json = sirius_json_get($service_node);
	$json['rates']['by_level'] = $form_state['values']['rates']['by_level'];
	sirius_json_set($service_node, $json);
	node_save($service_node);
	drupal_set_message("Rates saved.");
}