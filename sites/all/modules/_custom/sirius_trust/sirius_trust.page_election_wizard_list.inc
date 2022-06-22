<?php

function sirius_trust_page_election_wizard_list() {
	return drupal_get_form('sirius_trust_page_election_wizard_list_form');
}

function sirius_trust_page_election_wizard_list_form($form, &$form_state) {
	$form['status'] = array(
		'#type' => 'select',
		'#title' => 'Status',
		'#options' => array(NULL => t('-- Select --')) + sirius_trust_election_wizard_status_options(),
		'#default_value' => 'submitted',
	);

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'SSN',
		'#size' => 12,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$params = array();
	$params['limit'] = 100;
	if ($form_state['submitted']) {
		$params['status'] = $form_state['values']['status'];
	} else {
		$params['status'] = 'submitted';
	}
	$params['ssn'] = $form_state['values']['ssn'];

	$records = sirius_trust_election_wizard_search($params);

	if ($records) {
		$header = array('SSN', 'Name', 'Status', 'Tools');
		$rows = array();
		foreach ($records as $nid => $record) {
			$ssn = $record['field_sirius_notes_value'];
			$wizard_node = node_load($nid);
			$worker_node = sirius_log_handler($wizard_node, 'sirius_worker');
			if (!$worker_node) { $worker_node = sirius_worker_get_by_ssn($ssn); }
			$election_node = sirius_log_handler($wizard_node, 'sirius_trust_worker_election');

			$tools = array();
			$tools[] = l('Wizard', 'node/' . $nid . '/sirius_trust_election_wizard', array('attributes' => array('class' => array('tool-button'))));
			if ($worker_node) {
				$tools[] = l('Worker', 'node/' . $worker_node->nid, array('attributes' => array('class' => array('tool-button'))));
			}
			if ($election_node) {
				$tools[] = l('Election', 'node/' . $election_node->nid, array('attributes' => array('class' => array('tool-button'))));
			}

			$row = array();
			$row[] = $ssn;
			$row[] = $worker_node->title;
			$row[] = $wizard_node->field_sirius_type['und'][0]['value'];
			$row[] = join(' ', $tools);
			$rows[] = $row;
		}
		$html = sirius_table_render($header, $rows);
	} else {
		$html = '<p>' . t('No election wizard records found.') . '</p>';
	}
	$form['results'] = array('#markup' => $html);

	return $form;
}

function sirius_trust_page_election_wizard_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
