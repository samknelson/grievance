<?php

function sirius_steward_page_sched_list() {
	return drupal_get_form('sirius_steward_page_sched_list_form');
}

function sirius_steward_page_sched_list_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employers,
	);

	$form['start_date'] = array(
		'#title' => t('Start Date'),
		'#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d'),
	);

	$form['end_date'] = array(
		'#title' => t('End Date'),
		'#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d', strtotime('+7 days')),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Go'),
	);

	if ($form_state['submitted']) {
		$search_params = array();
		$search_params['employer_nid'] = $form_state['values']['employer_nid'];
		$search_params['date'] = $form_state['values']['date'];
		$search_params['start_date'] = $form_state['values']['start_date'];
		$search_params['end_date'] = $form_state['values']['end_date'];
		$sched_nids = sirius_steward_sched_list($search_params);
		$sched_nodes = node_load_multiple($sched_nids);
		if ($sched_nodes) {



			$header = array('Employer', 'Date', 'Tools');

			// @todo It's challenging to sort these, because we don't have the employer name until we load the node and the log handler, and
			//       we don't have the date until we extract the JSON. This code is ugly and should be moved into the sched library,
			//       and generally made better.

			$data = array();
			foreach ($sched_nodes as $sched_nid => $sched_node) {
				$employer_node = sirius_log_handler($sched_node, 'grievance_shop');
				$sched_json = sirius_json_get($sched_node);

				$tools = array();
				$tools[] = l('View', 'node/' . $sched_nid . '/sirius_steward_log_sched/view', array('attributes' => array('class' => array('tool-button'))));
				$tools[] = l('Edit', 'node/' . $sched_nid . '/sirius_steward_log_sched/edit', array('attributes' => array('class' => array('tool-button'))));

				$row = array();
				$row[] = $employer_node->title;
				$row[] = date('l, F j, Y', strtotime($sched_json['date']));
				$row[] = join(' ', $tools);

				$key = $employer_node->title . '|' . $sched_json['date'];
				$data[$key] = $row;
			}
			ksort($data);
			$rows = array_values($data);
			$html = sirius_table_render($header, $rows);
		} else {
			$html = '<p>' . t('No steward schedule records found.') . '</p>';
		}
		$form['results'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_steward_page_sched_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}