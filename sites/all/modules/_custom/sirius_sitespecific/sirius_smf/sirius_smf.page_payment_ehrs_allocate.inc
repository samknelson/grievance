<?php

sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');

function sirius_smf_node_tab_payment_ehrs_allocate_access($payment_node) {
	// Only cleared payments can be allocated
	if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') {
		return FALSE;
	}

	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	$employer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$ea_node = $ea->fetch($employer_node->nid, $account_node->nid, FALSE);
	if (!$account_node || !$employer_node || !$ea_node) { return FALSE; }

	return Sirius_Ledger_Access::getInstance()->access('ea_view_employer_workers', array('ea_node' => $ea_node));
}

function sirius_smf_node_tab_payment_ehrs_allocate_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;
	$form['#tree'] = TRUE;

	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	$employer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$ea_node = $ea->fetch($employer_node->nid, $account_node->nid, FALSE);

	$currency = sirius_ledger_currency_info_from_account_nid($account_node->nid)['instance'];

	// Get the list of uploads
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
  $search_params = array();
  $search_params['employer_nid'] = $employer_node->nid;
  $eup_nodes = $ehrs_monthly->search($search_params);

  // Can only allocate uploads with withholding
  $options = array();
  foreach ($eup_nodes as $eup_nid => $eup_node) {
  	if ($ehrs_monthly->get_status($eup_node) != 'complete') { continue; }
  	$eup_json = sirius_json_get($eup_node);
  	if (!$eup_json['allocate']) { continue; }
  	if (!$eup_json['allocate']['total']) { continue; }

  	$total = $eup_json['allocate']['total'];
  	$options[$eup_nid] = $ehrs_monthly->get_ym($eup_node) . ': ' . $currency->render_amt($total, array('format' => 'text'));
  }

  if (!$options) {
  	return sirius_form_error("There are no employer uploads with allocation data.");
  }

  $form['eup_nid'] = array(
  	'#type' => 'select',
  	'#title' => 'Upload to Allocate',
  	'#options' => $options,
  	'#required' => TRUE,
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}


function sirius_smf_node_tab_payment_ehrs_allocate_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	$employer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$ea_node = $ea->fetch($employer_node->nid, $account_node->nid, FALSE);
	$eup_node = node_load($form_state['values']['eup_nid']);
	$eup_json = sirius_json_get($eup_node);

	$currency = sirius_ledger_currency_info_from_account_nid($account_node->nid)['instance'];

	$payment_amt = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
	$allocate_amt = -1 * $eup_json['allocate']['total'];

	if ($payment_amt != $allocate_amt) {
		$form_state['rebuild'] = TRUE;
		drupal_set_message(
			"The payment amount, " . 
			$currency->render_amt($payment_amt, array('format' => 'text')) . 
			", is not equal to the uploaded allocation total, " . 
			$currency->render_amt($allocate_amt, array('format' => 'text')), 
			'error'
		);
		return;
	}

	// Allocate!

	sirius_ledger_payment_clear($payment_node->nid);

	if ($payment_node->field_sirius_datetime['und'][0]['value']) {
		$ts = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
	} else {
		$ts = time(); 
	}

	$msg = array();
	foreach ($eup_json['allocate']['workers'] as $worker_nid => $worker_amt) {
		$worker_node = node_load($worker_nid);
		$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

		$entry = array();
		$entry['ledger_amount'] = -1 * $worker_amt;
		$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
		$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
		$payment_type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);
		$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
		$entry['ledger_memo'] = sirius_ledger_payment_memo($payment_node);
		$entry['ledger_ts'] = $ts;
		$entry['ledger_reference'] = $payment_node->nid;
		$entry['ledger_participant'] = $contact_nid;
		sirius_ledger_ar_insert($entry);
	}

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
	node_save($payment_node);
	drupal_set_message("Payment allocated.");
}
