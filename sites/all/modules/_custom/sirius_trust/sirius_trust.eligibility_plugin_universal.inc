<?php

function sirius_trust_eligibility_plugin_universal_settings_form($json) {

	$form = array();

	if (!$json['show']) { $json['show'] = 'Eligible'; }
	$form['show'] = array(
		'#title' => 'Show As', 
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Eligible' => t('Eligible'),
			'Warning' => t('Eligible with Warning'),
			'Ineligible' => t('Ineligible'),
		),
		'#default_value' => $json['show'],
	);

	if (!$json['msg']) { $json['msg'] = 'There are no restrictions on this benefit.'; }
	$form['msg'] = array(
		'#title' => 'Message', 
		'#type' => 'textfield',
		'#default_value' => $json['msg'],
	);

	return $form;
}


function sirius_trust_eligibility_plugin_universal_check($settings, $params) {
	if (!$settings['show']) { $settings['show'] = 'Eligible'; }
	if (!$settings['msg']) { $settings['msg'] = 'There are no restrictions on this benefit.'; }

	$result = array();
	if ($settings['show'] == 'Eligible') {
		$result['success'] = TRUE; 
	} else if ($settings['show'] == 'Ineligible') {
		$result['success'] = FALSE;
	} else if ($settings['show'] == 'Warning') {
		$result['success'] = TRUE; 
		$result['warning'] = TRUE;
	}
	$result['msg'] = $settings['msg'];


	return $result;
}