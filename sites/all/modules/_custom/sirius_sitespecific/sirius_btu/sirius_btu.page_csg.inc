<?php

function sirius_btu_page_csg() {
	return drupal_get_form('sirius_btu_page_csg_form');
}

function sirius_btu_page_csg_form($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();

	$form['#tree'] = TRUE;

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => 'Filters',
	);

	$form['filters']['start_date'] = array(
		'#title' => 'Start Date',
		'#type' => 'date_popup',
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'Y-m-d',
		'#default_value' => NULL,
	);

	$form['filters']['end_date'] = array(
		'#title' => 'End Date',
		'#type' => 'date_popup',
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'Y-m-d',
		'#default_value' => NULL,
	);

	$form['filters']['output'] = array(
		'#title' => t('Output'),
		'#type' => 'select',
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('Spreadsheet (CSV) export'),
		),
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$search_params = array();

	if ($form_state['submitted']) {
		if ($form_state['values']['filters']['start_date']) {
			$search_params['start_ts'] = strtotime($form_state['values']['filters']['start_date'] . ' 00:00:00');
		}
		if ($form_state['values']['filters']['end_date']) {
			$search_params['end_ts'] = strtotime($form_state['values']['filters']['end_date'] . ' 23:59:59');
		}
	}
	$search_params['limit'] = 1000;

	$csg_nodes = $csg->search($search_params);
	$count = count($csg_nodes);
	if ($count) {
		$summary = '<p>';
		$summary .= "Showing records 1 to $count.";
		if ($count >= $search_params['limit']) {
			$summary .= " (Limiting to $search_params[limit] records.)";
		}
		$summary .= '</p>';
		$form['summary'] = array('#markup' => $summary);
	}

	$render_params = array();
	if (!($csg->access('staff')) ) { $render_params['hide_tools'] = TRUE; }

	if ($form_state['values']['filters']['output'] == 'csv') {
		$csg->export($csg_nodes, $render_params);
	}

	$html = $csg->render_list($csg_nodes, $render_params);
	$form['results'] = array('#markup' => $html);
	return $form;
}

function sirius_btu_page_csg_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}