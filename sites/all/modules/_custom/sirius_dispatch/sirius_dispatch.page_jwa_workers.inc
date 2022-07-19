<?php

function sirius_dispatch_page_jwa_workers() {
	return drupal_get_form('sirius_dispatch_page_jwa_workers_form');
}

function sirius_dispatch_page_jwa_workers_form($form, &$form_state) {
	$form['#tree'] = TRUE; 
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$defaults = $json['sirius']['dispatch']['jwa']['prefs'];

	$ms_level_options = sirius_ledger_ms_level_options();
	$form['workers']['ms_level'] = array(
		'#title' => 'Member Status / Level',
		'#options' => $ms_level_options,
		'#checkall' => TRUE, 
		'#type' => 'checkboxes',
		'#default_value' => sirius_form_array_default_value($defaults['workers']['ms_level']),
	);

	$work_status_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$form['workers']['work_status'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Work Status'),
		'#options' => $work_status_options,
		'#default_value' => sirius_form_array_default_value($defaults['workers']['work_status']),
		'#checkall' => TRUE,
	);

	$dispstatus_options = sirius_taxonomy_load('sirius_worker_dispatch_status', 'tid', 'name');
	$form['workers']['dispstatus'] = array(
		'#title' => 'Dispatch Status',
		'#options' => $dispstatus_options,
		'#checkall' => TRUE, 
		'#default_value' => sirius_form_array_default_value($defaults['workers']['dispstatus']),
		'#type' => 'checkboxes',
	);

	$sort_options = array(
		'availdate' => t('Seniority Date'),
		'level_weight' => t('Apprentice Level'),
	);
	for ($i=0; $i<=1; ++$i) {
		$form['workers']['sort'][$i]['key'] = array(
			'#title' => 'Sort #' . ($i+1),
			'#options' => array(NULL => t('-- Select --')) + $sort_options,
			'#default_value' => $defaults['workers']['sort'][$i]['key'],
			'#type' => 'select',
		);
		$form['workers']['sort'][$i]['direction'] = array(
			'#options' => array('asc' => t('Ascending'), 'desc' => t('Descending')),
			'#default_value' => $defaults['workers']['sort'][$i]['direction'],
			'#type' => 'select',
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save')
	);

	$form['submit_assign'] = array(
		'#type' => 'submit',
		'#value' => t('Save and Assign Workers'),
		'#name' => 'submit_assign',
		'#prefix' => '&nbsp;',
	);

	return $form;
}

function sirius_dispatch_page_jwa_workers_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$json['sirius']['dispatch']['jwa']['prefs']['workers'] = $form_state['values']['workers'];
	sirius_json_set($user_full, $json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
	if ($form_state['clicked_button']['#name'] == 'submit_assign') { drupal_goto('sirius/dispatch/jwa'); }
}