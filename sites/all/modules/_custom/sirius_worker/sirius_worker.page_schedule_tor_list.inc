<?php

function sirius_worker_page_schedule_tor_list() {
	return drupal_get_form('sirius_worker_page_schedule_tor_list_form');
}

function sirius_worker_page_schedule_tor_list_form($form, &$form_state) {
	$form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => array(NULL => t('-- Select --')) + sirius_worker_schedule_tor_status_options(),
	);

	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius worker admin')) { 
		$employer_options = array(NULL => t('-- Select --')) + sirius_employer_list();
		$form['employer_nid'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => $employer_options,
		);
	} else {
		$my_employer = sirius_employer_get();
		$form['employer_nid'] = array(
			'#type' => 'hidden',
			'#value' => $my_employer->nid,
		);
	}

	$form['type_tid'] = array(
		'#title' => t('Type'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name'),
	);


	$form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Submitted after:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array('autocomplete' => 'off'),
  );

	$form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Submitted before:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array('autocomplete' => 'off'),
  );


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']); 
	}

	return $form;
}

function sirius_worker_page_schedule_tor_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUe;
	$params = array();
	$params['employer_nid'] = $form_state['values']['employer_nid'];
	$params['status'] = $form_state['values']['status'];
	$params['start_date'] = $form_state['values']['start_date'];
	$params['end_date'] = $form_state['values']['end_date'];
	$params['type_tid'] = $form_state['values']['type_tid'];
	$tors = sirius_worker_schedule_tor_list($params);

	$rows = array();
	foreach ($tors as $tor_nid => $tor_node) {
		$worker_node = sirius_log_handler($tor_node, 'sirius_worker');

		$tools = '';
		$tools .= l('View', "node/$tor_nid/sirius_worker_schedule_tor", array('attributes' => array('class' => array('tool-button'))));
		if (sirius_worker_schedule_tor_access('edit', $worker_node, $tor_node)) {
			$tools .= l('Edit', "node/$tor_nid/sirius_worker_schedule_tor/edit", array('attributes' => array('class' => array('tool-button'))));
		}
		if (drupal_valid_path("node/" . $worker_node->nid)) {
			$tools .= l('Worker', 'node/' . $worker_node->nid, array('attributes' => array('class' => array('tool-button'))));
		}

		$json = sirius_jsonfield_util_node_json_get($tor_node);
		$status_options = sirius_worker_schedule_tor_status_options();

		$row = array();
		$row[] = $worker_node->title;
		$row[] = $json['tor']['start_date'];
		$row[] = $json['tor']['end_date'];
		$row[] = sirius_text_to_html(sirius_fieldval($tor_node, 'field_sirius_summary'));
		$row[] = $status_options[$json['tor']['status']];
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($tors) {
		$header = array('Worker', 'Start', 'End', 'Description', 'Status', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_worker_schedule_tors'))));
	} else {
		$html .= '<p>No time-off requests found.</p>';
	}

	$html = '<hr><h2>' . t('Results') . '</h2>' . $html;
	$form_state['results'] = $html;
}
