<?php

function sirius_dispatch_term_tab_job_type_elig_plugin_form($form, &$form_state, $job_type_term, $plugin_key) {
	$form_state['job_type_tid'] = $job_type_term->tid;
	$form_state['plugin_key'] = $plugin_key;

	$form['#tree'] = TRUE;

	$plugin = sirius_dispatch_elig_instance($plugin_key);

	$form['plugin'][$plugin_key]['active'] = array(
		'#title' => t('Active'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#required' => TRUE,
	);

	$settings = $plugin->settings();
	if ($settings) {
		$form['plugin'][$plugin_key]['settings'] = $settings;
		$form['plugin'][$plugin_key]['settings']['#type'] = 'fieldset';
		$form['plugin'][$plugin_key]['settings']['#title'] = t('Settings');
		$form['plugin'][$plugin_key]['settings']['#states'] = array('visible' => array(":input[name=\"plugin[$plugin_key][active]\"]" => array('value' => 'Yes')));
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($job_type_term));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_term_tab_job_type_elig_plugin_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$plugin_key = $form_state['plugin_key'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_json_get($job_type_term);
	$json['plugin'][$plugin_key] = $form_state['values']['plugin'][$plugin_key];
	sirius_json_set($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

