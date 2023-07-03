<?php

function sirius_service_page_config_server_funcs() {
	return drupal_get_form('sirius_service_page_config_server_funcs_form');
}

function sirius_service_page_config_server_funcs_form($form, &$form_state) {
	$func_info_all = sirius_service_func_info_all();

	$form['#tree'] = TRUE;

	$form['funcs']['header'] = array('#markup' => '<table><tr><th>' . t('Function') . '</th><th>' . t('Remote Access Enabled?') . '</th></tr>');
	foreach ($func_info_all as $func_key => $func_info) {
		$form['funcs'][$func_key]['header'] = array('#markup' => '<tr>');

		$form['funcs'][$func_key]['info'] = array(
			'#markup' => sirius_tooltip_render($func_info['description'], $func_key),
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$form['funcs'][$func_key]['enabled'] = array(
			'#type' => 'select',
			'#options' => array(
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
			'#default_value' => $func_info['enabled'],
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$form['funcs'][$func_key]['footer'] = array('#markup' => '</tr>');
	}
	$form['funcs']['footer'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_service_page_config_server_funcs_form_submit($form, &$form_state) {
	sirius_domain_variable_set('sirius_service_server_funcs', $form_state['values']['funcs']);
	drupal_set_message("Settings saved.");
}