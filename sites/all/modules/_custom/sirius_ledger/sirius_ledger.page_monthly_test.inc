<?php

function sirius_ledger_page_monthly_test() {
	return drupal_get_form('sirius_ledger_page_monthly_test_form');
}

function sirius_ledger_page_monthly_test_form($form, &$form_state) {
	$html = '<h2>Monthly and Semi-Monthly Dues Batch Status</h2>';
	$batch_status = json_decode(sirius_domain_variable_get('sirius_ledger_monthly_batch_status'), TRUE);
	$date = $batch_status['date'];
	if (!$date) { $date = '[none]'; }
	$html .= sirius_fakerender_field('Date:', $date);
	$complete = $batch_status['complete'];
	if ($complete) { $complete = 'True'; } else { $complete = 'False'; }
	$html .= sirius_fakerender_field('Complete:', $complete);
	$last_worker_nid = $batch_status['last_worker_nid'];
	if (!$last_worker_nid) { $last_worker_nid = '[none]'; }
	$html .= sirius_fakerender_field('Last Worker NID:', $last_worker_nid);
	$form['status'] = array('#markup' => $html);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#required' => TRUE,
		'#options' => array(
			'worker' => t('Compute for a single worker'),
			'batch' => t('Compute the next batch'),
			'batch_reset' => t('Restart batch computations for today'),
		),
	);

	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('As Of'),
    '#attributes' => array('autocomplete' => 'off'),
    '#description' => t('Leave blank to make charges "as of" today. If running in batch mode, selecting a date will clear the batch progress for today.'),
	);

	$form['worker_nid'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker NID'),
		'#states' => array(
			'visible' => array(
				":input[name=\"action\"]" => array('value' => 'worker')
			),
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

function sirius_ledger_page_monthly_test_form_submit($form, &$form_state) {

	$ts = strtotime($form_state['values']['date']);
	if ($ts <= 1) { $ts = NULL; }

	$form_state['rebuild'] = TRUE;
	if ($form_state['values']['action'] == 'worker') {
		$ts = strtotime($form_state['values']['date']);
		if ($ts <= 1) { $ts = NULL; }
		sirius_ledger_monthly_charge_worker($form_state['values']['worker_nid'], $form_state['values']['mode'], $ts);
	} else if ($form_state['values']['action'] == 'batch_reset') {
		sirius_ledger_monthly_charge_batch_reset();
	} else {
		sirius_ledger_monthly_charge_batch($form_state['values']['mode'], $ts, $form_state['values']['trigger']);
	}
}