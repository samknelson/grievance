<?php

function sirius_teamsters631_node_tab_dispatch_job_group_seniority_form($form, &$form_state, $job_group_node) {
	$form['#tree'] = TRUE;
	$form_state['job_group_nid'] = $job_group_node->nid;

	$job_group_json = sirius_json_get($job_group_node);
	$form['teamsters631']['seniority']['disabled'] = array(
		'#title' => t('Disable seniority for this job group'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $job_group_json['teamsters631']['seniority']['disabled'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_dispatch_job_group_seniority_form_submit($form, &$form_state) {
	$job_group_node = node_load($form_state['job_group_nid']);
	$job_group_json = sirius_json_get($job_group_node);
	$job_group_json['teamsters631']['seniority'] = $form_state['values']['teamsters631']['seniority'];
	sirius_json_set($job_group_node, $job_group_json);
	node_save($job_group_node);
	drupal_set_message("Settings saved.");
}