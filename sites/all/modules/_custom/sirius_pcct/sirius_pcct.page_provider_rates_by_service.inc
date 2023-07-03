<?php

/**
 * @todo: This is a temporary solution, based on having not too many services and not too many providers.
 *        See comment #3 in https://projects.clevernamehere.com/map/node/19515
 */

function sirius_pcct_node_tab_provider_rates_by_service_form($form, &$form_state, $provider_node) {
	$form_state['provider_nid'] = $provider_node->nid;
	$form['#tree'] = TRUE;

	$json = sirius_json_get($provider_node);

	$services = sirius_trust_service_options();
	if (!$services) {
		return sirius_form_error('No services have been defined. Please ' . l('add at least one service.', 'sirius/trust/services'));
	}

	foreach ($services as $service_nid => $service_name) {
		$form['rates']['by_service'][$service_nid] = array(
			'#type' => 'textfield',
			'#title' => $service_name,
			'#size' => 4,
			'#default_value' => $json['rates']['by_service'][$service_nid],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_pcct_node_tab_provider_rates_by_service_form_submit($form, &$form_state) {
	$provider_node = node_load($form_state['provider_nid']);
	$json = sirius_json_get($provider_node);
	$json['rates']['by_service'] = $form_state['values']['rates']['by_service'];
	sirius_json_set($provider_node, $json);
	node_save($provider_node);
	drupal_set_message("Rates saved.");
}