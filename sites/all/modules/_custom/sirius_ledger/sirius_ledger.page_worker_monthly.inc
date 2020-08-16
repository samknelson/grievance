<?php

function sirius_ledger_node_tab_worker_monthly($worker_node) {
	return drupal_get_form('sirius_ledger_node_tab_worker_monthly_form', $worker_node);
}

function sirius_ledger_node_tab_worker_monthly_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	/*
	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#required' => TRUE,
		'#options' => array(
			'worker' => t('Compute for this worker'),
		),
	);
	*/

	$form['trigger'] = array(
		'#type' => 'select',
		'#title' => t('Trigger'),
		'#required' => TRUE,
		'#options' => sirius_ledger_policy_trigger_options('month'),
	);

	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('As Of'),
		'#states' => array(
			'visible' => array(
				":input[name=\"action\"]" => array('value' => 'worker')
			),
		),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}




function sirius_ledger_node_tab_worker_monthly_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$form_state['rebuild'] = TRUE;
	$ts = strtotime($form_state['values']['date']);
	if ($ts <= 1) { $ts = NULL; }
	sirius_ledger_monthly_charge_worker($worker_nid, $form_state['values']['mode'], $form_state['values']['trigger'], $ts);
}

