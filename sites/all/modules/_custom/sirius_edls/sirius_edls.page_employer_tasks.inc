<?php

function sirius_edls_node_tab_employer_tasks_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config_tasks', array('employer_nid' => $employer_node->nid));
}


function sirius_edls_node_tab_employer_tasks_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$departments = sirius_edls_departments($employer_node);
	$tasks_by_department = sirius_edls_tasks_by_department($employer_node);
	foreach ($departments as $department_tid => $department_name) {
		$form['tasks'][$department_tid]['tasks'] = array(
			'#type' => 'textarea',
			'#title' => t('Tasks: ') . $department_name,
			'#default_value' => sirius_edls_tasks_encode($tasks_by_department[$department_tid]),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_node_tab_employer_tasks_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);

	$all_tasks = array();
	foreach ($form_state['values']['tasks'] as $department_tid => $record) {
		$tasks_encoded = $record['tasks'];
		$result = sirius_edls_tasks_decode($tasks_encoded);
		if (!$result['success']) {
			$form_state['rebuild'] = TRUE;
			drupal_set_message("Error: " . $result['msg'], 'error');
			return;
		}
		$all_tasks[$department_tid] = $result['tasks'];
	}

	$employer_json['edls']['tasks_by_department'] = $all_tasks;
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Tasks saved.");
}
