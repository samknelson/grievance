<?php

function sirius_node_tab_dispatch_job_1for1_list_access($job_node) {
	// 1for1 plugin must be active
	if (!sirius_dispatch_elig_plugin_active($job_node, '1for1')) { return FALSE; }

	// Managers can access
	if (!sirius_node_tab_dispatch_job_manage_access($job_node)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_dispatch_job_1for1_list_form($form, &$form_state, $job_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['job_nid'] = $job_node->nid;

	$for1 = sirius_dispatch_elig_instance('1for1');
  $data = $for1->for1_data($job_node);

  $statuses = $for1->for1_statuses();

	$html = '<h2>' . 'Extra Slots' . '</h2>';

	$total = $for1->extraslots_get_total($job_node);
	$dispatch_nids = $for1->extraslots_dispatch_nids($job_node);
	if ($dispatch_nids) {
		$rows = array();

		$count = 0;
		foreach ($dispatch_nids as $dispatch_nid) {
			++$count;

			$dispatch_node = node_load($dispatch_nid);
			$row = array();
			$row[] = $count;
			$row[] = sirius_node_title($dispatch_node->field_sirius_worker['und'][0]['target_id']);
			$rows[] = $row;
		}

		while ($count < $total) {
			++$count;
			$row = array();
			$row[] = $count;
			$row[] = '';
			$rows[] = $row;
		}
		$html .= sirius_table_render(array('Slot', 'Replaced By'), $rows);
	}
	$form['extraslots'] = array('#markup' => $html);

	$form['list_intro'] = array('#markup' => '<h2>' . t('Replaced workers') . '</h2>');
  $header = array();
  $header['id'] = t('ID');
  $header['name'] = t('Name');
  $header['status'] = t('Status');
  $header['replaced_by'] = t('Replaced By');

  $options = array();
  foreach ($data as $worker_nid => $record) {
  	$worker_node = node_load($worker_nid);

  	if ($record['status'] == 'ready') { $class = 'green'; } else { $class = 'gray'; }
  	$options[$worker_nid] = array(
  		'id' => $worker_node->field_sirius_id['und'][0]['value'],
  		'name' => $worker_node->title,
  		'status' => $statuses[$record['status']],
  		'replaced_by' => sirius_node_title($record['replaced_by']),
  		'#attributes' => array(
  			'class' => array($class)
  		)
  	);
  }

	$form['worker_nids'] = array(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $options,
		'#empty' => t('There are no workers ready to be replaced.'),
		'#required' => TRUE,
		'#attributes' => array('class' => array('sirius_table')),
	);

	if (user_access('sirius dispatch admin')) {
		$form['force'] = array(
			'#type' => 'checkbox',
			'#title' => t('<em>Admin only:</em> Remove even if the worker has already been replaced.'),
		);
	}

	if ($data) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Remove Selected Workers'),
		);
	}

	return $form;
}

function sirius_node_tab_dispatch_job_1for1_list_form_submit($form, &$form_state) {
	$job_node = node_load($form_state['job_nid']);

	$for1 = sirius_dispatch_elig_instance('1for1');

	$worker_nids = array_filter(array_values($form_state['values']['worker_nids']));
	if (!$worker_nids) {
		drupal_set_message("Please select at least one workers", 'error');
		return;
	}

	foreach ($worker_nids as $worker_nid) {
		$worker_node = node_load($worker_nid);
		$result = $for1->for1_remove($job_node, $worker_node, $form_state['values']['force']);
		if (!$result['success']) {
			drupal_set_message($result['msg'], 'error');
		} else {
			drupal_set_message($result['msg']);
		}
	}
}

