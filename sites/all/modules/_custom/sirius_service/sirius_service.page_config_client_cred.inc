<?php

function sirius_service_page_config_client_cred() {
	return drupal_get_form('sirius_service_page_config_client_cred_form');
}


function sirius_service_page_config_client_cred_form($form, &$form_state) {
	$instance = sirius_service_client_instance();
	if (!$instance) { return sirius_form_error('Client not implemented.'); }

	$settings = sirius_service_client_settings(); 

	$form['#tree'] = TRUE;

	$form['settings'] = $instance->settings_form();
 	sirius_jsonfield_util_set_defaults($form['settings'], $settings);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_service_page_config_client_cred_form_submit($form, &$form_state) {
	sirius_domain_variable_set('sirius_service_client_settings', json_encode($form_state['values']['settings']));
	drupal_set_message(t('Settings saved.'));
}
