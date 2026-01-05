<?php

function sirius_smf_feed_payments_row_verify($feed_node, $info, &$data, &$row, $precompute) {
  static $rowcount;
  $rowcount++;

  // if ($rowcount > 2) { return array('success' => FALSE, 'msg' => 'Throttle'); }

	$lookups = &drupal_static(__FUNCTION__);
  if (!isset($lookups['employer_nid'])) {
    $id = 'PAYMENTS_PENDING';
    $result = sirius_feed_lookup_by_field('grievance_shop', 'field_data_field_sirius_id', 'field_sirius_id_value', $id);
    if (!$result['success']) {
      return array('success' => FALSE, 'msg' => 'Please make sure that there is exactly one shop with the ID "PAYMENTS_PENDING"');
    }

    $lookups['payer_nid'] = $result['nid'];
  }

  $row['payer_nid'] = $lookups['payer_nid'];

  $tmp = $row['xxxrawxxx'];
  foreach ($tmp as $key => $value) {
  	$value = trim($value);
  	if (!$value || ($value == '-')) { unset($tmp[$key]); } else { $tmp[$key] = $value; }
  }
  $row['field_sirius_notes'] = print_r($tmp, 1);

  $input = $row['field_sirius_dollar_amt'];
  $input = trim($input);
  $input = preg_replace('/^\$/', '', $input);
  $input = preg_replace('/\,/', '', $input);
	if (!preg_match('/^-?\d+(\.\d+)?$/', $input)) {
		return array('success' => FALSE, 'msg' => 'Please enter a valid numeric amount.');
	}
	$row['field_sirius_dollar_amt'] = (float)$input;

	$id = $row['field_sirius_id'];
	if (!$id) { return array('success' => FALSE, 'msg' => 'Please enter a primary key.'); }

	$result = sirius_feed_lookup_by_field('sirius_payment', 'field_data_field_sirius_id', 'field_sirius_id_value', $id);

  if ($row['field_sirius_datetime'] !== '') {
    $ts = strtotime($row['field_sirius_datetime']);
    if ($ts === FALSE) {
      return array('success' => FALSE, 'msg' => 'Please enter a valid date.');
    }
  }

  if ($result['success']) {
    $row['payment_nid'] = $result['nid'];
    $msg = "An existing payment will be updated.";
  } else {
    if ($result['not_found'] && $data['create_new_records'] == 'Yes') {
      // Not found, but we can create new records.
      $msg = "A new payment will be created.";
    } else {
      // We can't create new records, or we have a different kind of error
      return $result;
    }
  }

  $payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
  foreach ($payment_types as $tid => $name) {
    if (!preg_match('/offline\s*payment/i', $name)) { continue; }
    $payment_type_tid = $tid;
  }
  if (!$payment_type_tid) {
    return array('success' => FALSE, 'msg' => 'Please make sure that there is a payment type called "offline payment".');
  }
  $row['field_sirius_payment_type'] = $payment_type_tid;

  return array('success' => TRUE, 'msg' => $msg);

  return array('success' => TRUE);
}
