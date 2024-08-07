<?php

function sirius_ledger_page_stripe_test() {
	return drupal_get_form('sirius_ledger_page_stripe_test_form');
}

function sirius_ledger_page_stripe_test_form($form, &$form_state) {
	$service = sirius_ledger_service();

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			NULL => t('System Default: ') . $service->mode(),
			'test' => t('Test Mode'),
			'live' => t('Live Mode'),
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

function sirius_ledger_page_stripe_test_form_submit($form, &$form_state) {
	$service = sirius_ledger_service();

	$form_state['rebuild'] = TRUE;

	$result = $service->ping($form_state['values']['mode']);

	if ($result['success']) {
		sirius_minilog('<pre>' . print_r($result, 1) . '</pre>');
		drupal_set_message("Stripe request succeeded.");
	} else {
		drupal_set_message("Request failed: " . $result['msg'], 'warning');
	}
}

