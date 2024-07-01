<?php

function sirius_ledger_page_balance_search_worker() {
	return drupal_get_form('sirius_ledger_page_balance_search_worker_form');
}

function sirius_ledger_page_balance_search_worker_form($form, &$form_state) {
	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$account_options = sirius_ledger_account_list();

	$form['#tree'] = TRUE;

	$form['params']['account'] = array(
		'#type' => 'fieldset',
		'#title' => t('Account'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['params']['account']['balance_account'] = array(
		'#title' => t('Account'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $account_options,
	);

	// "min" and "max" are reveresed; we multiply by -1 below
	$form['params']['account']['balance_amount_max'] = array(
		'#title' => t('Minium Amount'),
		'#type' => 'textfield',
		'#size' => 4,
	);

	$form['params']['account']['balance_amount_min'] = array(
		'#title' => t('Maximum Amount'),
		'#type' => 'textfield',
		'#size' => 4,
	);

	$form['params']['worker'] = array(
		'#type' => 'fieldset',
		'#title' => t('Worker'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['params']['worker']['work_statuses'] = array(
		'#title' => t('Work Status'),
		'#type' => 'checkboxes',
		'#options' => $ws_options,
		'#checkall' => TRUE,
	);

	$form['params']['worker']['member_statuses'] = array(
		'#title' => t('Member Status'),
		'#type' => 'checkboxes',
		'#options' => $ms_options,
		'#checkall' => TRUE,
	);

	$form['output'] = array(
		'#type' => 'fieldset',
		'#title' => t('Output'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['output']['include_address'] = array(
		'#title' => t('Include Address?'),
		'#type' => 'select',
		'#options' => array(
			'no' => t('No'),
			'yes' => t('Yes'),
		),
	);

	$form['output']['display'] = array(
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

function sirius_ledger_page_balance_search_worker_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$search_params = $form_state['values']['params']['account'];
	$search_params += $form_state['values']['params']['worker'];

	if ($search_params['work_statuses']) { $search_params['work_statuses'] = array_filter(array_values($search_params['work_statuses'])); }
	if ($search_params['member_statuses']) { $search_params['member_statuses'] = array_filter(array_values($search_params['member_statuses'])); }

	if ($search_params['balance_amount_min']) { $search_params['balance_amount_min'] *= -1; }
	if ($search_params['balance_amount_max']) { $search_params['balance_amount_max'] *= -1; }

	$search_params['limit'] = 1000;
	$search_params['balance'] = TRUE;

	$entries = sirius_ledger_balance_search($search_params);

	$render_options = array();
	if ($form_state['values']['output']['include_address'] != 'yes') {
		$render_options['hide_phone'] = TRUE;
		$render_options['hide_address'] = TRUE;
		$render_options['hide_city'] = TRUE;
		$render_options['hide_state'] = TRUE;
		$render_options['hide_zip'] = TRUE;
	}

	if ($form_state['values']['output']['display'] == 'csv') {
		$render_options['display'] = 'csv';
		sirius_ledger_balance_render($entries, $render_options);
	} else {
		$html = '';
		if (count($entries) >= 1000) {
			$html = '<p>' . t('<em>Displaying the most recent 1000 entries.</em>');
		}
		$html .= sirius_ledger_balance_render($entries, $render_options);
		$form_state['results'] = $html;
	}
}
