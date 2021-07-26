<?php

function sirius_ledger_page_ar_search() {
	return drupal_get_form('sirius_ledger_page_ar_search_form');
}

function sirius_ledger_page_ar_search_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['ledger_account'] = array(
		'#title' => t('Account'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + sirius_ledger_account_list(),
	);

	$form['ledger_date_min'] = array(
		'#title' => t('Start Date'),
		'#type' => 'date_popup',
		'#date_format' => 'm/d/Y',
		'#attributes' => array('autocomplete' => 'off'),
	);

	$form['ledger_date_max'] = array(
		'#title' => t('End Date'),
		'#type' => 'date_popup',
		'#date_format' => 'm/d/Y',
		'#attributes' => array('autocomplete' => 'off'),
	);

	$form['ledger_amount_min'] = array(
		'#title' => t('Minium Amount'),
		'#type' => 'textfield',
		'#size' => 4,
	);

	$form['ledger_amount_max'] = array(
		'#title' => t('Maximum Amount'),
		'#type' => 'textfield',
		'#size' => 4,
	);

	$form['ledger_memo'] = array(
		'#title' => t('Memo'),
		'#type' => 'textfield',
	);

	$form['display'] = array(
		'#title' => t('Display'),
		'#type' => 'select',
		'#options' => array(
			'screen' => t('Show results on the screen'),
			'csv' => t('Output results to a CSV file'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr><h2>' . t('Results') . '</h2>' . $form_state['results']);
	}

	return $form;
}

function sirius_ledger_page_ar_search_form_submit($form, &$form_state) {
	$search_params = array();

	if ($form_state['values']['ledger_account']) {
		$search_params['ledger_account'] = $form_state['values']['ledger_account'];
	} else {
		$search_params['ledger_accounts'] = array_keys(sirius_ledger_account_list());
	}

	if (isset($form_state['values']['ledger_amount_min']) && $form_state['values']['ledger_amount_min'] !== '') {
		$search_params['ledger_amount_min'] = $form_state['values']['ledger_amount_min'];
	}

	if (isset($form_state['values']['ledger_amount_max']) && $form_state['values']['ledger_amount_max'] !== '') {
		$search_params['ledger_amount_max'] = $form_state['values']['ledger_amount_max'];
	}

	if (isset($form_state['values']['ledger_date_min']) && $form_state['values']['ledger_date_min'] !== '') {
		$ts = strtotime($form_state['values']['ledger_date_min']);
		if ($ts > 1) { $search_params['ledger_ts_min'] = $ts; }
	}

	if (isset($form_state['values']['ledger_date_max']) && $form_state['values']['ledger_date_max'] !== '') {
		$ts = strtotime($form_state['values']['ledger_date_max']);
		if ($ts > 1) { $search_params['ledger_ts_max'] = $ts; }
	}

	if ($form_state['values']['ledger_memo'] !== '') {
		$search_params['ledger_memo'] = '%' . $form_state['values']['ledger_memo'] . '%';
	}

	$search_params['limit'] = 1000;

	$form_state['rebuild'] = TRUE;

	$entries = sirius_ledger_ar_fetch($search_params);

	if ($form_state['values']['display'] == 'csv') {
		sirius_ledger_ar_render($entries, array('display' => 'csv'));
	} else {
		$html = '';
		if (count($entries) >= 1000) {
			$html = '<p>' . t('<em>Displaying the most recent 1000 entries.</em>');
		}
		$html .= sirius_ledger_ar_render($entries);
		$form_state['results'] = $html;
	}
}