<?php

function sirius_ledger_node_tab_payment_update($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_update_form', $payment_node);
}

function sirius_ledger_node_tab_payment_update_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;

	$form['#tree'] = TRUE;

	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$worker_node = sirius_worker_get_by_contact_nid($payer_node->nid);
	$employers = sirius_employer_list(FALSE);
	$form['payment']['payer_type'] = array(
		'#title' => t('Payment From'),
		'#type' => 'radios',
		'#options' => array(
			'worker' => 'Worker',
			'employer' => 'Employer',
		),
		'#required' => TRUE,
		'#default_value' => sirius_ledger_payer_type($payer_node->type),
	);
	$form['payment']['payer_employer'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => $employers,
		'#default_value' => $payer_node->nid,
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'employer')
			),
		),
	);
	$form['payment']['payer_worker'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker ID'),
		'#default_value' => $worker_node->field_sirius_id['und'][0]['value'],
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'worker')
			),
		),
	);

	$accounts = sirius_ledger_account_list();
	$form['payment']['ledger_account'] = array(
		'#title' => t('To'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $accounts,
		'#default_value' => $payment_node->field_sirius_ledger_account['und'][0]['target_id'],
	);

	$payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
  $form['payment']['payment_type'] = array(
    '#title' => t('Type'),
		'#type' => 'select',
		'#options' => $payment_types,
		'#required' => TRUE,
		'#default_value' => $payment_node->field_sirius_payment_type['und'][0]['tid'],
  );

  $form['payment']['dollar_amt'] = array(
    '#title' => t('Amount'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#size' => 5,
		'#default_value' => $payment_node->field_sirius_dollar_amt['und'][0]['value'],
  );

  $form['payment']['merchant_name'] = array(
    '#title' => t('Merchant'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_merchant_name['und'][0]['value'],
  );

  $form['payment']['check_number'] = array(
    '#title' => t('Check / Txn #'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_check_number['und'][0]['value'],
  );

  $form['payment']['notes'] = array(
    '#title' => t('Memo'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_notes['und'][0]['value'],
  );

  $form['payment']['datetime_created'] = array(
    '#title' => t('Received'),
		'#type' => 'date_popup',
		'#required' => FALSE,
		'#default_value' => NULL,
		'#attributes' => array('autocomplete' => 'off'),
  );
  if ($payment_node->field_sirius_datetime_created['und'][0]['value']) {
  	$ts = sirius_fieldval($payment_node, 'field_sirius_datetime_created', 'ts');
  	if ($ts > 1) { $form['payment']['datetime_created']['#default_value'] = date('Y-m-d H:i:s', $ts); }
  }

  $form['payment']['datetime'] = array(
    '#title' => t('Posted'),
		'#type' => 'date_popup',
		'#required' => FALSE,
		'#default_value' => NULL,
		'#attributes' => array('autocomplete' => 'off'),
  );
  if ($payment_node->field_sirius_datetime['und'][0]['value']) {
  	$ts = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
  	if ($ts > 1) { $form['payment']['datetime']['#default_value'] = date('Y-m-d H:i:s', $ts); }
  }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_update_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	// Clear the ledger
	$count = sirius_ledger_payment_clear($payment_node->nid);
	if ($count) { drupal_set_message("$count A/R entries have been cleared. This payment must be re-allocated."); }
	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
	$payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $form_state['values']['payment']['ledger_account'];
	$payment_node->field_sirius_payment_type['und'][0]['tid'] = $form_state['values']['payment']['payment_type'];
	$payment_node->field_sirius_dollar_amt['und'][0]['value'] = $form_state['values']['payment']['dollar_amt'];
	$payment_node->field_sirius_merchant_name['und'][0]['value'] = $form_state['values']['payment']['merchant_name'];
	$payment_node->field_sirius_check_number['und'][0]['value'] = $form_state['values']['payment']['check_number'];
	$payment_node->field_sirius_notes['und'][0]['value'] = $form_state['values']['payment']['notes'];

	if ($form_state['values']['payment']['payer_type'] == 'employer') {
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $form_state['values']['payment']['payer_employer'];
	} else {
		$worker_node = sirius_worker_get_by_id($form_state['values']['payment']['payer_worker']);
		if (!$worker_node) { drupal_set_message("Worker not found.", 'error'); return; }
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
	}

	if ($form_state['values']['payment']['datetime_created']) {
		$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['payment']['datetime_created']));
	} else {
		unset($payment_node->field_sirius_datetime_created['und']);
	}

	if ($form_state['values']['payment']['datetime']) {
		$payment_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['payment']['datetime']));
	} else {
		unset($payment_node->field_sirius_datetime['und']);
	}
	
	sirius_ledger_payment_set_title($payment_node);

	$payment_node->revision = TRUE;
	node_save($payment_node);
	drupal_set_message("Payment updated.");
}