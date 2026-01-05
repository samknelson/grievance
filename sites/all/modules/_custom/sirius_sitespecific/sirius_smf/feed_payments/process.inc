<?php

function sirius_smf_feed_payments_row_process($feed_node, $info, $data, $row) {
	$result = sirius_smf_feed_payments_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

	$domain_nid = sirius_domain_nid();

	// ////////////////////////////////////////////////////////
  // Worker 
	// ////////////////////////////////////////////////////////

  if ($row['payment_nid']) {
    $payment_is_new = FALSE;
    $payment_node = node_load($row['payment_nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $payment_node = new stdClass();
    $payment_node->type = 'sirius_payment';
    $payment_node->language = LANGUAGE_NONE;
    $payment_node->uid = $user->uid;
    $payment_node->status = 1;
    $payment_node->promote = 0;
    $payment_node->comment = 0;
  }
  if ($domain_nid) { $payment_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

  $payment_node->field_sirius_id['und'][0]['value'] = $row['field_sirius_id'];
  $payment_node->field_sirius_dollar_amt['und'][0]['value'] = $row['field_sirius_dollar_amt'];
  $payment_node->field_sirius_merchant_name['und'][0]['value'] = $row['field_sirius_merchant_name'];
  $payment_node->field_sirius_check_number['und'][0]['value'] = $row['field_sirius_check_number'];
  $payment_node->field_sirius_check_number['und'][0]['value'] = $row['field_sirius_id'];
  $payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
  $payment_node->field_sirius_payment_status['und'][0]['value'] = 'Received';
  $payment_node->field_sirius_notes['und'][0]['value'] = $row['field_sirius_notes'];
  $payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $data['account_nid'];
  $payment_node->field_sirius_payer['und'][0]['target_id'] = $row['payer_nid'];
  if ($row['field_sirius_datetime']) {
    $payment_node->field_sirius_datetime['und'][0]['value'] = 
      $payment_node->field_sirius_datetime_created['und'][0]['value'] = 
        date('Y-m-d 12:00:00', strtotime($row['field_sirius_datetime'] . ' PST'));
  }
  $payment_node->field_sirius_payment_type['und'][0]['tid'] = $row['field_sirius_payment_type'];

	$payment_node = node_submit($payment_node);

  sirius_ledger_payment_set_title($payment_node);

  node_save($payment_node);

  if ($is_new) { $msg = 'Created a new payment.'; } else { $msg = 'Updated an existing payment.'; }
  return array('success' => TRUE, 'msg' => $msg, 'warnings' => $warnings);
}
