<?php

function sirius_dispatch_node_tab_worker_pref_dept_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
	  $sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') { return FALSE; }
	}

	// There must be at least one department
	$departments = sirius_worker_department_options($worker_node);
	if (!$departments) { return FALSE; }

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_dept_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$departments = sirius_worker_department_options($worker_node); // @todo: departments
	$department_options = array();
	foreach ($departments as $department_tid => $department_term) {
		$department_options[$department_tid] = $department_term->name;
	}
	$form['department_not_avail'] = array(
		'#title' => t('I would prefer <strong>not</strong> to receive dispatches for:'),
		'#type' => 'checkboxes',
		'#options' => $department_options,
		'#default_value' => sirius_fieldvals($worker_node, 'field_grievance_departments', 'tid'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_dept_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	// Department Preferences
	$new_departments = array();
	if ($form_state['values']['department_not_avail']) {
		foreach ($form_state['values']['department_not_avail'] as $key => $value) {
			if ($value) {
				$new_departments[] = $value;
			}
		}
	}
	$old_departments = sirius_fieldvals($worker_node, 'field_grievance_departments', 'tid');
	if ($new_departments != $old_departments) {
		$need_save = TRUE;
		$worker_node->field_grievance_departments['und'] = array();
		foreach ($new_departments as $new_department) {
			$worker_node->field_grievance_departments['und'][] = array('tid' => $new_department);
		}
		drupal_set_message("Department preferences have been updated.");
	}

	if ($need_save) { node_save($worker_node); }
}