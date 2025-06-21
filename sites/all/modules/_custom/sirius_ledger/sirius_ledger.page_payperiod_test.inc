<?php

function sirius_ledger_node_tab_payperiod_test_form($form, &$form_state, $payperiod_node) {
	$form_state['payperiod_nid'] = $payperiod_node->nid;
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
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

function sirius_ledger_node_tab_payperiod_test_form_submit($form, &$form_state) {
	$payperiod_node = node_load($form_state['payperiod_nid']);
	$form_state['rebuild'] = TRUE;

	sirius_ledger_payperiod_set($payperiod_node, $form_state['values']['mode']);
	drupal_set_message("Payperiod ledger entries updated.");
}
