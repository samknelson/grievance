<?php

function sirius_worker_tos_page_list() {
	return drupal_get_form('sirius_worker_tos_page_list_form');
}

function sirius_worker_tos_page_list_form($form, &$form_state) {
	$tos = Sirius_Worker_Tos::getInstance();

	$form['#tree'] = TRUE;

  $form['filters'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'filters',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  $form['filters']['status'] = array(
  	'#type' => 'select',
  	'#title' => t('Status'),
  	'#options' => array(NULL => t('-- Select --')) + $tos->status_options(),
  	'#default_value' => 'active',
  );

  $form['filters']['start_date'] = array(
		'#title' => t('Created After'),
		'#type' => 'date_popup',
		'#date_format' => 'Y-m-d H:i',
		'#attributes' => array('autocomplete' => 'off'),
	);

  $form['filters']['end_date'] = array(
		'#title' => t('Created Before'),
		'#type' => 'date_popup',
		'#date_format' => 'Y-m-d H:i',
		'#attributes' => array('autocomplete' => 'off'),
	);

  $form['filters']['worker_name'] = array(
		'#title' => t('Worker Name'),
		'#type' => 'textfield',
		'#description' => t('Enter all or part of the worker name.'),
	);

  $form['filters']['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

	$search_params = array();
	if ($form_state['submitted']) {
		$search_params['status'] = $form_state['values']['filters']['status'];
		$start_ts = strtotime($form_state['values']['filters']['start_date']);
		$end_ts = strtotime($form_state['values']['filters']['end_date']);
		$search_params['start_ts'] = $start_ts;
		$search_params['end_ts'] = $end_ts;
		$search_params['worker_name_match'] = $form_state['values']['filters']['worker_name'];
	} else {
		$search_params['status'] = 'active';
	}
	$tos_nodes = $tos->search($search_params);

	$display_params = array();
	$form['results'] = array('#markup' => $tos->render_list($tos_nodes, $display_params));

	return $form;
}

function sirius_worker_tos_page_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}