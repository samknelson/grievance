<?php

function sirius_employer_node_tab_labproj_status_access($labproj_node) {
	if ($labproj_node->field_sirius_category['und'][0]['value'] != 'labproj') { return FALSE; }
	if (!in_array($labproj_node->field_sirius_type['und'][0]['value'], array('draft', 'active'))) { return FALSE; }

	// Employers can access their own page only
	if (!user_access('sirius employer admin')) { 
		$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
		global $user;
		$user_full = user_load($user->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] != $employer_node->nid) { return FALSE; }
	}

	return TRUE; 
}


function sirius_employer_node_tab_labproj_status($labproj_node) {
	return drupal_get_form('sirius_employer_node_tab_labproj_status_form', $labproj_node);
}

function sirius_employer_node_tab_labproj_status_form($form, &$form_state, $labproj_node) {
	$form_state['labproj_nid'] = $labproj_node->nid; 

	$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
	$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');

	list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);

	$params = array();
	$ts = strtotime("$year-$month-01");
	$params['industry_tids'] = $industry_tids;
	$params['interval'] = array(
		'start_ts' => strtotime("-1 day", $ts),
		'end_ts' => strtotime("+1 month", $ts),
	);
	$params['nids_only'] = TRUE;
	$group_options = sirius_dispatch_group_search($params);

	$labproj_json = sirius_json_get($labproj_node);

	$status = sirius_employer_labproj_get_status($labproj_node);
	$status_options = sirius_employer_labproj_status_options();
	$form['status'] = array(
		'#title' => t('Status'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $status_options,
		'#default_value' => $status,
		'#required' => TRUE,
	);

	$form['comments'] = array(
		'#title' => t('Comments'),
		'#type' => 'textarea',
		'#default_value' => $labproj_node->field_sirius_summary['und'][0]['value'],
	);

	if ($group_options) { 
		$form['settings']['detail_by_group'] = array(
			'#type' => 'select',
			'#title' => t('Breakout Projections by Group or Show?'),
			'#options' => array(
				'Yes' => t('Yes, enter projections for each group / show'),
				'No' => t('No, enter total labor projections only')
			),
			'#default_value' => $labproj_json['settings']['detail_by_group'],
		);

		// $group_options = array(NULL => t('-- Select --')) + $group_options;
		$form['settings']['job_groups'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Groups or Shows'),
			'#options' => $group_options,
			'#states' => array('visible' => array(array(":input[name=\"settings[detail_by_group]\"]" => array('value' => 'Yes')))),
		);
		if ($labproj_json['settings']['job_groups']) { $form['settings']['job_groups']['#default_value'] = $labproj_json['settings']['job_groups']; }
	}


	$form['#tree'] = TRUE;
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_employer_node_tab_labproj_status_form_submit($form, &$form_state) {
	$labproj_node = node_load($form_state['labproj_nid']);
	$labproj_json = sirius_json_get($labproj_node);
	$labproj_json['settings'] = $form_state['values']['settings'];
	sirius_json_set($labproj_node, $labproj_json);
	$labproj_node->field_sirius_type['und'][0]['value'] = $form_state['values']['status'];
	$labproj_node->field_sirius_summary['und'][0]['value'] = $form_state['values']['comments'];

	$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
	list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);
  $labproj_node->title = sirius_employer_labproj_title($employer_node->nid, $year, $month);

	node_save($labproj_node);
	drupal_set_message("Status saved.");
}