<?php

/*** 
 * TOR = "Time-Off Request".
 */

function sirius_worker_page_schedule_tor_access() {
	// Admins can access
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	if (user_access('sirius worker admin')) { return TRUE; }

	// Employers can access
	$my_employer = sirius_employer_get();
	if (user_access('sirius dispatch employer') && $my_employer) { return TRUE; }

	return FALSE;
}

function sirius_worker_page_schedule_tor() {
	return drupal_get_form('sirius_worker_page_schedule_tor_form');
}

function sirius_worker_page_schedule_tor_form($form, &$form_state) {
	$form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => sirius_worker_schedule_tor_status_options(),
		'#default_value' => 'requested',
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

function sirius_worker_page_schedule_tor_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUe;
	$params = array();
	$params['employer_nid'] = $form_state['values']['employer_nid'];
	$params['status'] = $form_state['values']['status'];
	$params['start_date'] = $form_state['values']['start_date'];
	$params['end_date'] = $form_state['values']['end_date'];
	$tors = sirius_worker_schedule_tor_list_all($params);

	$rows = array();
	foreach ($tors as $tor_nid => $tor_node) {
		$handler_nid = $tor_node->field_sirius_log_handler['und'][0]['target_id'];
		$sql = "select title from node where nid = :handler_nid";
		$stmt = db_query($sql, array(':handler_nid' => $handler_nid)); 
		$hr = $stmt->fetchAssoc();
		$handler_title = $hr['title'];

		$tools = '';
		/*
		if (sirius_worker_schedule_tor_access('view', $entity, $tor_node)) {
			$tools .= l('View', 'node/' . $handler_nid . '/sirius_worker_schedule/tor/view/' . $tor_nid, array('attributes' => array('class' => array('tool-button'))));
		}
		*/
		if (sirius_worker_schedule_tor_access('edit', $entity, $tor_node)) {
			$tools .= l('Edit', 'node/' . $handler_nid . '/sirius_worker_schedule/tor/edit/' . $tor_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		if (drupal_valid_path("node/" . $handler_nid)) {
			$tools .= l('Worker', 'node/' . $handler_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}

		$json = sirius_jsonfield_util_node_json_get($tor_node);
		$status_options = sirius_worker_schedule_tor_status_options();

		$row = array();
		$row[] = $handler_title;
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
