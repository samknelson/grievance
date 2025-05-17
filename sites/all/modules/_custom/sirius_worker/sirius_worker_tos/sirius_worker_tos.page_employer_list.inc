<?php

function sirius_worker_tos_page_employer_list_access($employer_node) {
	return Sirius_Worker_Tos_Access::getInstance()->access('employer_list', array('employer_node' => $employer_node));
}

function sirius_worker_tos_page_employer_list_form($form, &$form_state, $employer_node) {
	$tos = Sirius_Worker_Tos::getInstance();
	$form_state['employer_nid'] = $employer_node->nid;

	$form['#tree'] = TRUE;

  $form['filters'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'filters',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
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
		$start_ts = strtotime($form_state['values']['filters']['start_date']);
		$end_ts = strtotime($form_state['values']['filters']['end_date']);
		$search_params['start_ts'] = $start_ts;
		$search_params['end_ts'] = $end_ts;
		$search_params['worker_name_match'] = $form_state['values']['filters']['worker_name'];
	}

	$search_params['status'] = 'active';
	$search_params['employer_nid'] = $employer_node->nid;
	$tos_nodes = $tos->search($search_params);

	$display_params = array();
	$display_params['hide_tools'] = TRUE;
	$form['results'] = array('#markup' => $tos->render_list($tos_nodes, $display_params));

	return $form;
}

function sirius_worker_tos_page_employer_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}