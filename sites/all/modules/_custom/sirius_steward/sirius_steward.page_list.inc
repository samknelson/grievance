<?php

function sirius_steward_page_list() {
	return drupal_get_form('sirius_steward_page_list_form');
}

function sirius_steward_page_list_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employers,
	);

	$form['name'] = array(
		'#title' => t('Steward Name'),
		'#type' => 'textfield',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Go'),
	);

	$search_params = array();
	$search_params['name'] = $form_state['values']['name'];
	$search_params['employer_nid'] = $form_state['values']['employer_nid'];
	$stewards = sirius_steward_search($search_params);
	if ($stewards) {
		$rows = array();
		foreach ($stewards as $steward_nid => $steward_node) {
			$tools = array();
			$tools[] = l('View', "node/$steward_nid", array('attributes' => array('class' => array('tool-button'))));
			$tools[] = l('Checkin', "node/$steward_nid/sirius_worker_steward/checkin", array('attributes' => array('class' => array('tool-button'))));
			$row = array();
			$row[] = $steward_node->title;
			$row[] = sirius_node_title($steward_node->field_grievance_shop['und'][0]['target_id']);
			$row[] = join('', $tools);
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Steward');
		$header[] = t('Employer');
		$header[] = t('Tools');
		$html = sirius_table_render($header, $rows);
	} else {
		$html = '<p>' . t('No stewards found.') . '</p>';
	}
	$form['results'] = array('#markup' => $html);

	return $form;
}

function sirius_steward_page_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}