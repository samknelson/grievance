<?php

function sirius_ledger_node_tab_payment_post($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_post_form', $payment_node);
}

function sirius_ledger_node_tab_payment_post_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;


	$json = sirius_jsonfield_util_node_json_get($payment_node);
	if ($json['allocate']['feed']['nid']) {
		$feed_nid = $json['allocate']['feed']['nid'];
		$feed_node = node_load($feed_nid);
		$backlink_html = 'This payment was allocated via an upload feed: ';
		$backlink_html .= l('View the Feed', "node/$feed_nid/sirius_feed_wizard", array('attributes' => array('class' => array('tool-button'))));
		$backlink_html .= "<br /><br />";
		$form['backlink'] = array('#markup' => $backlink_html);
	}

	$form['#tree'] = TRUE;

	$form['payment']['payment_status'] = array(
		'#title' => t('Payment Status'),
		'#type' => 'select',
		'#options' => sirius_ledger_payment_status_options(),
		'#required' => TRUE,
		'#default_value' => $payment_node->field_sirius_payment_status['und'][0]['value'],
	);

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

function sirius_ledger_node_tab_payment_post_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
	$payment_node->field_sirius_payment_status['und'][0]['value'] = $form_state['values']['payment']['payment_status'];
	$ts = strtotime($form_state['values']['payment']['datetime']);
	if (!$ts) { $ts = time(); }
	$payment_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', $ts);

	$entries = sirius_ledger_ar_fetch(array('ledger_reference' => $payment_node->nid));
	$count = 0;
	foreach ($entries as $entry) {
		++$count;
		$entry['ledger_status'] = $form_state['values']['payment']['payment_status'];
		$entry['ledger_ts'] = $ts;
		sirius_ledger_ar_update($entry);
	}
	if ($count) { 
		drupal_set_message("$count A/R entries updated.");
	}

	
	sirius_ledger_payment_set_title($payment_node);

	$payment_node->revision = TRUE;
	node_save($payment_node);
	drupal_set_message("Payment updated.");
}