<?php

/**********************************************************
 * Landing page
 **********************************************************/

function sirius_worker_page_ein_access() {
	if (user_access('sirius worker admin')) { return TRUE; }
	if (user_access('sirius dispatch employer')) { return TRUE; }
	return FALSE;
}

function sirius_worker_page_ein() {
	if (!user_access('sirius worker admin')) {
		$employer_node = sirius_employer_get();
		if (!$employer_node) { return 'There is no employer associated with your user.'; }
		drupal_goto(current_path() . '/' . $employer_node->nid);
	}

	return drupal_get_form('sirius_worker_page_ein_form');
}

function sirius_worker_page_ein_form($form, &$form_state) {
	$options = sirius_employer_list();
	$form['employer_nid'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Select an Employer'),
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_page_ein_form_submit($form, &$form_state) {
	drupal_goto(current_path() . '/' . $form_state['values']['employer_nid']);
}

/**********************************************************
 * Employer Landing Page
 **********************************************************/

function sirius_worker_page_ein_employer_access($employer_nid, $action) {
	$employer_node = node_load($employer_nid);
	if (!$employer_node) { return FALSE; }
	if ($employer_node->type != 'grievance_shop') { return FALSE; }

	$domain_nid = sirius_domain_nid();
	if ($domain_nid != $employer_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	if (user_access('sirius worker admin')) { return TRUE; }

	if (user_access('sirius dispatch employer')) { 
		$test_employer_node = sirius_employer_get();
		return $test_employer_node->nid == $employer_node->nid;
	}

	return FALSE;
}

function sirius_worker_page_ein_employer($employer_nid, $action) {
	$employer_node = node_load($employer_nid);
	drupal_set_title(drupal_get_title() . ': ' . $employer_node->title);
	if ($action == 'lookup') {
		return drupal_get_form('sirius_worker_page_ein_employer_lookup_form', $employer_node);
	} else if ($action == 'assign') {
		return drupal_get_form('sirius_worker_page_ein_employer_assign_form', $employer_node);
	} else if ($action == 'upload') {
		return 'To upload lists of employees, please go to the ' . l('feed list page.', 'sirius/feeds/feed_employee');
	} else {
		return "Action not implemented: $action";
	}
}

function sirius_worker_page_ein_employer_lookup_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$form['ein'] = array(
		'#type' => 'textfield',
		'#title' => 'EIN',
		'#size' => 10,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;

}

function sirius_worker_page_ein_employer_lookup_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$employer_nid = $form_state['employer_nid'];
	$ein = $form_state['values']['ein'];

	$worker_nid = sirius_worker_ein_lookup($employer_nid, $ein);

	if (!$worker_nid) {
		drupal_set_message("No worker found with the EIN $ein.", 'warning');
	} else {
		$worker_node = node_load($worker_nid);
		drupal_set_message("EIN $ein corresponds to worker $worker_node->title.");
	}
}

function sirius_worker_page_ein_employer_assign_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'SSN',
		'#size' => 10,
	);

	$form['ein'] = array(
		'#type' => 'textfield',
		'#title' => 'EIN',
		'#size' => 10,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_page_ein_employer_assign_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$employer_nid = $form_state['employer_nid'];
	$ssn = $form_state['values']['ssn'];
	$ein = $form_state['values']['ein'];

	$result = sirius_worker_ein_set($ssn, $employer_nid, $ein);
	if ($result['success']) { drupal_set_message($result['msg']); } 
	else { drupal_set_message($result['msg'], 'warning'); }
}

function sirius_worker_page_ein_employer_lookup($employer_nid) {
	drupal_set_title(drupal_get_title() . ': ' . $employer_node->title);
	return drupal_get_form('sirius_worker_page_ein_employer_lookup_form', $employer_node);
}