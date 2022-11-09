<?php

/**
 * OBSOLETE
 * 
 * I believe this page has been rewritten as e.g. page_jwa_jobs, page_jwa_workers, etc.
 * 
 * It should be safe to delete.
 */

function sirius_dispatch_page_jwa_prefs() {
	return drupal_get_form('sirius_dispatch_page_jwa_prefs_form');
}

function sirius_dispatch_page_jwa_prefs_form($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
	$defaults = $json['sirius']['dispatch']['jwa']['prefs'];

  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');
	global $user;

  $form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

  $form['accordion_open_job'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Jobs') . '</div><div class="sirius_accordion_body">');

	$job_type_options = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'name');
	$form['job_type'] = array(
		'#type' => 'select',
		'#title' => t('Job Type'),
		'#options' => $job_type_options,
		'#size' => 10,
		'#multiple' => TRUE,
		'#default_value' => $defaults['job_type'],
		'#required' => TRUE,
	);

  $form['accordion_close_job'] = array('#markup' => '</div>');

  $form['accordion_open_worker'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Workers') . '</div><div class="sirius_accordion_body">');

	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$form['member_status'] = array(
		'#type' => 'select',
		'#title' => t('Member Status'),
		'#options' => $member_status_options,
		'#size' => 10,
		'#multiple' => TRUE,
		'#default_value' => $defaults['member_status'],
	);

	$work_status_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$form['work_status'] = array(
		'#type' => 'select',
		'#title' => t('Work Status'),
		'#options' => $work_status_options,
		'#size' => 10,
		'#multiple' => TRUE,
		'#default_value' => $defaults['work_status'],
	);

  $form['accordion_close_worker'] = array('#markup' => '</div>');

  $form['accordion_open_display'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Display') . '</div><div class="sirius_accordion_body">');

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
	$form['hour_type'] = array(
		'#type' => 'select',
		'#title' => t('Hours to Display'),
		'#options' => $hour_options,
		'#size' => 10,
		'#multiple' => TRUE,
		'#default_value' => $defaults['hour_type'],
	);

	$form['date_range'] = array(
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
		'#default_value' => $defaults['date_range'],
	);

	$form['date_range_days'] = array(
		'#type' => 'textfield',
		'#title' => t('Days'),
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[name="date_range"]' => array('value' => 'days')),
			)
		),
		'#default_value' => $defaults['date_range_days'],
	);

	$form['date_range_months'] = array(
		'#type' => 'textfield',
		'#title' => t('Months'),
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[name="date_range"]' => array('value' => 'months')),
			)
		),
		'#default_value' => $defaults['date_range_months'],
	);

  $form['accordion_close_display'] = array('#markup' => '</div>');

  $form['accordion_close'] = array('#markup' => '</div>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);

	return $form;
}

function sirius_dispatch_page_jwa_prefs_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
	$json['sirius']['dispatch']['jwa']['prefs'] = $form_state['values'];
	$user_full->field_sirius_json['und'][0]['value'] = json_encode($json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
}