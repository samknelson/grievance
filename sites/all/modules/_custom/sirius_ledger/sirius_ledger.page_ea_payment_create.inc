<?php

function sirius_ledger_node_tab_ea_payment_create_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_payment_create', array('ea_node' => $ea_node));
}

function sirius_ledger_node_tab_ea_payment_create_form($form, &$form_state, $ea_node) {
	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = $ea->account_node($ea_node);
	$payment_type_options = sirius_ledger_account_payment_type_options($account_node);
	if (!$payment_type_options) {
		return sirius_form_error("No payment types have been configured for this account. Please go to the account configuration page and add at least one payment type.");
	}

	$form_state['ea_nid'] = $ea_node->nid;

	$form['#tree'] = TRUE;
  $form['payment']['payment_type'] = array(
    '#title' => t('Type'),
		'#type' => 'select',
		'#options' => $payment_type_options,
		'#required' => TRUE,
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Create a Payment or Adjustment'),
  );

  return $form;
}

function sirius_ledger_node_tab_ea_payment_create_form_submit($form, &$form_state) {
	$ea = Sirius_Ledger_EA::getInstance();
	$ea_node = node_load($form_state['ea_nid']);

	$participant_node = $ea->participant_node($ea_node);
	$account_node = $ea->account_node($ea_node);

	$params = array();
	$params['ledger_account'] = $account_node->nid;
	$params['ledger_participant'] = $participant_node->nid;
	$params['payment_type'] = $form_state['values']['payment']['payment_type'];
	$result = sirius_ledger_payment_create($params);

	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
		return;
	}

	drupal_set_message("Payment created.");
	drupal_goto('node/' . $result['payment_node']->nid . '/sirius_payment_update');
}