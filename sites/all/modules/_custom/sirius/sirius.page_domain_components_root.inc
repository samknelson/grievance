<?php

function sirius_page_domain_components_root() {
	return drupal_get_form('sirius_page_domain_components_root_form');
}

function sirius_page_domain_components_root_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$all_info = sirius_domain_component_info_all();

	foreach ($all_info as $key => $info) {
    $variable_name = 'sirius_domain_root_component_enabled_' . $key;
		$form['component'][$key]['enabled'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#title' => $info['title'],
			'#description' => $info['description'],
			'#default_value' => variable_get($variable_name, 'Yes'),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_domain_components_root_form_submit($form, &$form_state) {
	foreach ($form_state['values']['component'] as $key => $record) {
    $variable_name = 'sirius_domain_root_component_enabled_' . $key;
    variable_set($variable_name, $record['enabled']);
	}
	drupal_set_message("Components saved.");
}