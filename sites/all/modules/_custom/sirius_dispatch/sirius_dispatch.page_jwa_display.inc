<?php

function sirius_dispatch_page_jwa_display() {
	return drupal_get_form('sirius_dispatch_page_jwa_display_form');
}

function sirius_dispatch_page_jwa_display_form($form, &$form_state) {
	$form['#tree'] = TRUE; 
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$defaults = $json['sirius']['dispatch']['jwa']['prefs'];

	$hour_type_options = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$department_options = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	$hour_options = array();
	$hour_options['TOTAL'] = t('ALL HOURS');
	foreach ($hour_type_options as $hour_type_tid => $hour_type_name) {
		$hour_options["$hour_type_tid"] = $hour_type_name . t(': TOTAL');
		foreach ($department_options as $department_tid => $department_name) {
			$hour_options["$hour_type_tid|$department_tid"] = "$hour_type_name: $department_name";
		}
	}
	$form['display']['hour_type'] = array(
		'#type' => 'select',
		'#title' => t('Hours to Display'),
		'#options' => $hour_options,
		'#size' => 10,
		'#multiple' => TRUE,
		'#default_value' => $defaults['display']['hour_type'],
	);

	$form['display']['date_range'] = array(
		'#type' => 'select',
		'#title' => t('Hours Date Range'),
		'#options' => array(
			'all' => t('All Time'),
			'this_month' => t('This Calendar Month'), 
			'last_month' => t('Last Calendar Month'), 
			'days' => t('Last X Days'), 
			'this_year' => t('This Calendar Year'), 
			'last_year' => t('Last Calendar Year'), 
			'months' => t('Last X Months'), 
		),
		'#default_value' => $defaults['display']['date_range'],
		'#id' => 'date_range',
	);

	$form['display']['date_range_days'] = array(
		'#type' => 'textfield',
		'#title' => t('Days'),
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[id="date_range"]' => array('value' => 'days')),
			)
		),
		'#default_value' => $defaults['display']['date_range_days'],
	);

	$form['display']['date_range_months'] = array(
		'#type' => 'textfield',
		'#title' => t('Months'),
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[id="date_range"]' => array('value' => 'months')),
			)
		),
		'#default_value' => $defaults['display']['date_range_months'],
	);

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

function sirius_dispatch_page_jwa_display_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$json['sirius']['dispatch']['jwa']['prefs']['display'] = $form_state['values']['display'];
	sirius_json_set($user_full, $json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
	if ($form_state['clicked_button']['#name'] == 'submit_assign') { drupal_goto('sirius/dispatch/jwa'); }
}