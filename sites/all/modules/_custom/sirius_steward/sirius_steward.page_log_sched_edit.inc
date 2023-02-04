<?php

function sirius_steward_node_tab_log_sched_edit_access($sched_node) {
	if ($sched_node->field_sirius_category['und'][0]['value'] != 'steward:sched') { return FALSE; }
	return TRUE;
}

function sirius_steward_node_tab_log_sched_edit_form($form, &$form_state, $sched_node) {
	$form['#tree'] = TRUE;
	$form_state['sched_nid'] = $sched_node->nid;
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$sched_json = sirius_json_get($sched_node);

	// Dropdowns
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	$facilities = sirius_dispatch_facility_dropdown();
	$job_groups = sirius_dispatch_group_list_active(NULL, strtotime($sched_json['date']) . ' 12:00:00');

	// Employer
	$employer_node = sirius_log_handler($sched_node, 'grievance_shop');
	if (!$employer_node) { return sirius_form_error('This record is not associated with an employer.'); }

	// Stewards. The list should include all the current stewards, as well as any stewards which were entered
	// when the record was originally created.
	$stewards = sirius_steward_search(array('employer_nid' => $employer_node->nid));
	if ($sched_json['sched']['stewards']) {
		$stewards += node_load_multiple(array_keys($sched_json['sched']['stewards']));
	}
	if (!$stewards) { return sirius_form_error('There are no stewards associated with this employer.'); }


	$form['sched']['stewards']['table_open'] = array('#markup' => 
		'<table class="sirius_table sirius_table_form">' . 
		'<tr>' .
		'<th>' . t('Steward') . '</th>' .
		'<th>' . t('Worker Count') . '</th>' .
		'<th>' . t('Time') . '</th>' .
		'<th>' . t('Show') . '<br />' . t('Location') . '</th>' .
		'<th>' . t('Last Crew') . '</th>' .
		'<th>' . t('Department') . '<br />' . t('Lead') . '</th>' .
		'</tr>'
	);
	foreach ($stewards as $worker_nid => $worker_node) {
		$steward_json = $sched_json['sched']['stewards'][$worker_nid];

		$form['sched']['stewards'][$worker_nid]['tr_open'] = array('#markup' => '<tr>');
		$form['sched']['stewards'][$worker_nid]['steward'] = array('#markup' => '<td>' . $worker_node->title . '</td>');

		$form['sched']['stewards'][$worker_nid]['worker_count'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $steward_json['worker_count'],
		);

		$form['sched']['stewards'][$worker_nid]['time'] = array(
			'#type' => 'date_popup',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
	    '#date_format' => 'H:i',
	    '#default_value' => '0000-00-00 ' . $steward_json['time'],
	    '#timeonly' => TRUE,
	    '#attributes' => array(
	      'autocomplete' => 'off',
	    ),
		);

		$form['sched']['stewards'][$worker_nid]['job_group_nid'] = array(
			'#type' => 'select',
			'#options' => array(NULL => t('-- Select --')) + $job_groups,
			'#prefix' => '<td>',
			'#suffix' => '<br />',
			'#default_value' => $steward_json['job_group_nid'],
		);

		$form['sched']['stewards'][$worker_nid]['facility_nid'] = array(
			'#type' => 'select',
			'#options' => array(NULL => t('NOT SCHEDULED')) + $facilities,
			'#prefix' => '',
			'#suffix' => '</td>',
			'#default_value' => $steward_json['facility_nid'],
		);

		$form['sched']['stewards'][$worker_nid]['last_crew'] = array(
			'#type' => 'date_popup',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
	    '#date_format' => 'H:i',
	    '#default_value' => '0000-00-00 ' . $steward_json['last_crew'],
	    '#timeonly' => TRUE,
	    '#attributes' => array(
	      'autocomplete' => 'off',
	    ),
		);

		$form['sched']['stewards'][$worker_nid]['department_tid'] = array(
			'#type' => 'select',
			'#options' => array(NULL => t('-- Select --')) + $departments,
			'#prefix' => '<td>',
			'#suffix' => '<br />',
			'#default_value' => $steward_json['department_tid'],
		);

		$form['sched']['stewards'][$worker_nid]['lead'] = array(
			'#type' => 'textfield',
			'#suffix' => '</td>',
			'#default_value' => $steward_json['lead'],
			'#size' => 20,
		);

		$form['sched']['stewards'][$worker_nid]['tr_close'] = array('#markup' => '</tr>');
	}

	$form['sched']['stewards']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);

	return $form;
}

function sirius_steward_node_tab_log_sched_edit_form_submit($form, &$form_state) {
	/*
	foreach ($form_state['values']['sched']['stewards'] as $worker_nid => $record) {
		$raw = $record['time'];
		$formatted = date('g:i a', strtotime($raw));
	}
	*/

	$sched_nid = $form_state['sched_nid'];
	$sched_node = node_load($sched_nid);
	$json = sirius_json_get($sched_node);
	$json['sched'] = $form_state['values']['sched'];
	sirius_json_set($sched_node, $json);

	if ($form_state['values']['sched']['stewards']) {
		foreach ($form_state['values']['sched']['stewards'] as $steward_nid => $ignore) {
			sirius_log_handler_set($sched_node, $steward_nid);
		}
	}

	node_save($sched_node);
	drupal_set_message("Schedule saved.");
}
