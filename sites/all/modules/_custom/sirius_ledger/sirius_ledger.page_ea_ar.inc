<?php

function sirius_ledger_node_tab_ea_ar_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_view', array('ea_node' => $ea_node));
}

function sirius_ledger_node_tab_ea_ar_form($form, &$form_state, $ea_node) {
	$form_state['ea_nid'] = $ea_node->nid;

	$ea = Sirius_Ledger_EA::getInstance();

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

	$form['filters']['display'] = array(
		'#type' => 'select',
		'#title' => 'Display',
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('CSV Export'),
		),
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if (!$form_state['results']) {
		$params = array();
		$params['ledger_participant'] = $ea->participant_node($ea_node)->nid;
		$params['ledger_account'] = $ea->account_node($ea_node)->nid;
		$entries = sirius_ledger_ar_fetch($params);
		$form_state['results'] = sirius_ledger_ar_render($entries, array('hide_participant' => TRUE, 'hide_id' => TRUE));
	}

	$form['results'] = array('#markup' => $form_state['results']);

	return $form;
}

function sirius_ledger_node_tab_ea_ar_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$ea_node = node_load($form_state['ea_nid']);
	$ea = Sirius_Ledger_EA::getInstance();

	$params = array();
	$params['ledger_participant'] = $ea->participant_node($ea_node)->nid;
	$params['ledger_account'] = $ea->account_node($ea_node)->nid;

	if ($form_state['values']['charge_or_payment'] == 'charge') {
		$params['ledger_amount_min'] = 0;
	} else if ($form_state['values']['charge_or_payment'] == 'payment') {
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

	$form_state['results'] = sirius_ledger_ar_render($entries, array('filename' => "ar_" . $ea_node->nid . ".csv", 'display' => $form_state['values']['display'], 'hide_id' => TRUE, 'hide_participant' => TRUE));
}
