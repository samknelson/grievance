<?php

function sirius_ledger_node_tab_worker_ar($worker_node) {
	return drupal_get_form('sirius_ledger_node_tab_worker_ar_form', $worker_node);
}

function sirius_ledger_node_tab_worker_ar_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { 
		drupal_set_message('There is no contact associated with this worker.', 'error');
		return array();
	}

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#title' => t('Filters'),
	);
	if (!$form_state['submitted']) { $form['filters']['#collapsed'] = TRUE; }

	$form['filters']['charge_or_payment'] = array(
		'#type' => 'select',
		'#title' => 'Show Charges or Payments?',
		'#options' => array(
			'both' => t('Both'),
			'charge' => t('Charges Only'),
			'payment' => t('Payments Only'),
			'range' => t('Custom Range'),
		),
	);

	$form['filters']['ledger_amount_min'] = array(
		'#type' => 'textfield',
		'#title' => 'Minimum Value',
		'#size' => 5,
		'#description' => t('(Remember that charges are positive and payments are negative)'),
		'#states' => array('visible' => array(":input[name=\"charge_or_payment\"]" => array('value' => 'range'))),
	);

	$form['filters']['ledger_amount_max'] = array(
		'#type' => 'textfield',
		'#title' => 'Maximum Value',
		'#size' => 5,
		'#states' => array('visible' => array(":input[name=\"charge_or_payment\"]" => array('value' => 'range'))),
	);

	$form['filters']['ledger_date_min'] = array(
		'#type' => 'date_popup',
		'#title' => 'Show entries AFTER',
		'#attributes' => array('autocomplete' => 'off'),
	);
	$form['filters']['ledger_date_max'] = array(
		'#type' => 'date_popup',
		'#title' => 'Show entries BEFORE',
		'#attributes' => array('autocomplete' => 'off'),
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if (!$form_state['results']) {
		$entries = sirius_ledger_ar_fetch(array('ledger_participant' => $contact_node->nid));
		$form_state['results'] = sirius_ledger_ar_render($entries, array('hide_participant' => TRUE, 'hide_id' => TRUE));
	}

	$form['results'] = array('#markup' => $form_state['results']);

	return $form;
}

function sirius_ledger_node_tab_worker_ar_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);

	$params = array('ledger_participant' => $contact_node->nid);

	if ($form_state['values']['charge_or_payment'] == 'charge') {
		$params['ledger_amount_min'] = 0;
	} else if ($form_state['values']['charge_or_payment'] == 'charge') {
		$params['ledger_amount_max'] = 0;
	} else if ($form_state['values']['charge_or_payment'] == 'range') {
		if (isset($form_state['values']['ledger_amount_max'])) { $params['ledger_amount_max'] = $form_state['values']['ledger_amount_max']; }
		if (isset($form_state['values']['ledger_amount_min'])) { $params['ledger_amount_min'] = $form_state['values']['ledger_amount_min']; }
	}

	if (!empty($form_state['values']['ledger_date_min'])) {
		$params['ledger_ts_min'] = strtotime($form_state['values']['ledger_date_min']);
	}
	if (!empty($form_state['values']['ledger_date_max'])) {
		$params['ledger_ts_max'] = strtotime($form_state['values']['ledger_date_max']);
	}
	$entries = sirius_ledger_ar_fetch($params);
	$form_state['results'] = sirius_ledger_ar_render($entries, array('hide_id' => TRUE, 'hide_participant' => TRUE));
}