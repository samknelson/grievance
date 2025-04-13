<?php

function sirius_teamsters631_node_tab_log_m12_request_layoffs2_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_request_layoffs2_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$form_state['m12_nid'] = $m12_node->nid;

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$dispatch_nids = $m12->dispatches_list($m12_node, 'nosuchdate');
	if (!$dispatch_nids) { return sirius_form_error("No dispatches found."); }

	//
	// Render the tableselect
	//

	$rows = array();
	$notfound = 0;
	$count = 0;
	foreach ($dispatch_nids as $dispatch_nid) {
		++$count;
		if ($count > 100) {
			drupal_set_message("Too many dispatches, showing the first 100.", 'warning');
			break;
		}
		$dispatch_node = node_load($dispatch_nid);
		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
		$record = $m12->layoff_search_worker($m12_node, $worker_nid);

		$row['worker'] = sirius_node_title($worker_nid);
		$row['ws'] = $record['ws'];
		$row['msg'] = $record['msg'];
		$row['elig'] = $record['elig'];
		$row['tools'] = l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
		$row['#attributes'] = array('class' => array($record['color']));
		$rows[$worker_nid] = $row;
	}

	$header = array();
	$header['worker'] = t('Worker');
	$header['ws'] = t('Work Status');
	$header['msg'] = t('Dispatch');
	$header['elig'] = t('Eligible?');
	$header['tools'] = t('Tools');

	$form['worker_nids'] = array(
		'#title' => t('Active Workers'),
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $rows,
		'#empty' => t('No input found.'),
		'#attributes' => array('class' => array('sirius_table')),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add the selected workers to the "Ineligible" list'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_request_layoffs2_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$m12_node = node_load($form_state['m12_nid']);
	$action = $form_state['values']['action'];

	$tmp = $form_state['values']['worker_nids'];
	if (!$tmp) { $tmp = array(); }
	$worker_nids = array_filter(array_values($tmp));

	if (!$worker_nids) {
		drupal_set_message("No workers added.");
		return;
	}

	foreach ($worker_nids as $worker_nid) {
		$m12->layoff_add($m12_node, $worker_nid);
		drupal_set_message("Added: " . sirius_node_title($worker_nid));
	}
}
