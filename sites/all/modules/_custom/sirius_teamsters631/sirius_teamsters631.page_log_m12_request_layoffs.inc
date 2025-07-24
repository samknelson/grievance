<?php

function sirius_teamsters631_node_tab_log_m12_request_layoffs_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_request_layoffs_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['m12_nid'] = $m12_node->nid;
	$m12_json = sirius_json_get($m12_node);

	if (!$form_state['submitted']) {
		$form['action'] = array('#type' => 'value', '#value' => 'search');

		$form['names'] = array(
			'#type' => 'textarea',
			'#size' => 6,
			'#title' => t('Paste up to 100 names provided by the employer'),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Search'),
		);

		return $form;
	}

	$form['names'] = array('#type' => 'hidden');

	$form['action'] = array('#type' => 'value', '#value' => 'add');

	$data = $m12->layoff_search_names($m12_node, $form_state['values']['names']);

	//
	// Render the tableselect
	//

	$rows = array();
	$notfound = 0;
	foreach ($data as $delta => $record) {
		if (!$record['candidates']) {
			$row = array();
			$row['input'] = $record['input'];
			$row['candidate'] = '<em>No candidates found</em>';
			$row['#disabled'] = TRUE;
			$row['#attributes'] = array('class' => array('inactive'));
			--$notfound;
			$rows[$notfound] = $row;
		}	else {
			$is_first = TRUE;
			foreach ($record['candidates'] as $worker_nid => $candidate) {
				$row = array();
				if ($is_first) {
					$row['input'] = $record['input'];
					$is_first = FALSE;
				} else {
					$row['input'] = '';
				}
				$row['candidate'] = $candidate['title'];
				$row['ws'] = $candidate['ws'];
				$row['msg'] = $candidate['msg'];
				$row['elig'] = $candidate['elig'];
				$row['tools'] = l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
				$row['#attributes'] = array('class' => array($candidate['color']));
				$rows[$worker_nid] = $row;
			}
		}
	}

	$header = array();
	$header['input'] = t('Input');
	$header['candidate'] = t('Candidate');
	$header['ws'] = t('Work Status');
	$header['msg'] = t('Dispatch');
	$header['elig'] = t('Eligible?');
	$header['tools'] = t('Tools');

	$form['worker_nids'] = array(
		'#title' => t('Candidates'),
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

function sirius_teamsters631_node_tab_log_m12_request_layoffs_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$m12_node = node_load($form_state['m12_nid']);
	$action = $form_state['values']['action'];

	if ($action == 'search') {
		$form_state['rebuild'] = TRUE;
		return;
	}

	if ($action == 'add') {
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
}
