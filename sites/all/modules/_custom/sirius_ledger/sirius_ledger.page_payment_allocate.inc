<?php

function sirius_ledger_node_tab_payment_allocate_options($payment_node) {
	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	$account_json = sirius_jsonfield_util_node_json_get($account_node);
	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$payer_type = sirius_ledger_payer_type($payer_node->type);

	if (!$account_json['settings']['payments']['allocate'][$payer_type]) { return array(); }

	$options = array_filter(array_values($account_json['settings']['payments']['allocate'][$payer_type]));
	return $options;
}

/**
 * @todo: This should really be "node_tab_payment_view". It's just a view of the payment, with information about allocation.
 */

function sirius_ledger_node_tab_payment_allocate($payment_node) {
	$html = "";

	$json = sirius_jsonfield_util_node_json_get($payment_node);
	if ($json['allocate']['feed']['nid']) {
		$feed_nid = $json['allocate']['feed']['nid'];
		$feed_node = node_load($feed_nid);
		$html .= 'This payment was allocated via an upload feed: ';
		$html .= l('View the Feed', "node/$feed_nid/sirius_feed_wizard", array('attributes' => array('class' => array('tool-button'))));
		$html .= "<br /><br />";
	}

	$renderable = node_view($payment_node);
	$html .= drupal_render($renderable);

	$html .= '<hr><h2>Allocation</h2>';
	$entries = sirius_ledger_ar_fetch(array('ledger_reference' => $payment_node->nid));
	$html .= sirius_ledger_ar_render($entries, array('hide_reference' => TRUE));

	return $html;
}

function sirius_ledger_node_tab_payment_allocate_manual_access($payment_node) {
	if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') { return FALSE; }
	return in_array('manual', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_manual($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_allocate_manual_form', $payment_node);
}

function sirius_ledger_node_tab_payment_allocate_manual_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;

	$fields = array(
		'field_sirius_payer',
		'field_sirius_ledger_account',
		'field_sirius_dollar_amt',
		'field_sirius_payment_type',
	);
	foreach ($fields as $field) {
		$form["intro_$field"] = array('#markup' => sirius_dispatch_view_render_field($payment_node, $field));
	}

	$form['#tree'] = TRUE;

	$form['table_open'] = array('#markup' => '<table><tr><th>Worker ID</th><th>Amount</th></tr>');
	for ($i=0; $i<5; ++$i) {
		$form['allocate'][$i]['worker_id'] = array(
			'#type' => 'textfield',
			'#prefix' => '<tr><td>',
			'#suffix' => '</td>',
		);
		$form['allocate'][$i]['amt'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#prefix' => '<td>',
			'#suffix' => '</td></tr>',
		);
	}
	$form['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_allocate_manual_form_validate($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	$total = 0;
	foreach ($form_state['values']['allocate'] as $i => $allocation) {
		if ($allocation['amt']) {
			if (!is_numeric($allocation['amt'])) {
				form_set_error("allocate][$i][amt", 'Please enter a numeric value.');
			} else {
				$total += $allocation['amt'];
			}
		}

		if ($allocation['worker_id']) {
			$worker_node = sirius_worker_get_by_id($allocation['worker_id']);
			if (!$worker_node) {
				form_set_error("allocate][$i][worker_id", 'Please enter a valid worker ID.');
			}
		}
	} 

	if ($total != $payment_node->field_sirius_dollar_amt['und'][0]['value']) {
		form_set_error('field_sirius_dollar_amt', 'Total allocation must sum to $' . $payment_node->field_sirius_dollar_amt['und'][0]['value']);
	}
}

function sirius_ledger_node_tab_payment_allocate_manual_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	$account_json = sirius_jsonfield_util_node_json_get($account_node);
	$form_state['rebuild'] = TRUE;

	sirius_ledger_payment_clear($payment_node->nid);

	$memo = sirius_ledger_payment_memo($payment_node);
	foreach ($form_state['values']['allocate'] as $i => $allocation) {
		if (!$allocation['amt']) { continue; }
		$worker_node = sirius_worker_get_by_id($allocation['worker_id']);

		$entry = array();
		$entry['ledger_amount'] = -1 * $allocation['amt'];
		$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
		$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
		$entry['ledger_memo'] = $memo;
		$entry['ledger_ts'] = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
		$entry['ledger_reference'] = $payment_node->nid;
		$entry['ledger_participant'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
		sirius_ledger_ar_insert($entry);

		if ($account_json['settings']['payments']['allocate']['employer_autocharge'] == 'Yes') {
			$entry = array();
			$entry['ledger_amount'] = -1 * $allocation['amt'];
			$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
			$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
			$entry['ledger_memo'] = 'Charge: ' . $memo;
			$entry['ledger_ts'] = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
			$entry['ledger_reference'] = $payment_node->nid;
			$entry['ledger_participant'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
			sirius_ledger_ar_insert($entry);
		}

	}

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
	$payment_node->revision;
	node_save($payment_node);

	drupal_set_message('Entries created.');

}

function sirius_ledger_node_tab_payment_allocate_upload_access($payment_node) {
	if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') { return FALSE; }
	return in_array('upload', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_upload($payment_node) {
	$json = sirius_jsonfield_util_node_json_get($payment_node);
	if ($json['allocate']['feed']['nid']) {
		$feed_nid = $json['allocate']['feed']['nid'];
		$feed_node = node_load($feed_nid);
		$html .= 'This payment as already been allocated via an uploaded feed.<br />';
		$title = t('View the Feed');
		$path = "node/$feed_nid/sirius_feed_wizard";
	} else {
		$html .= "To allocate this payment, create a new feed of type \"A/R Payment Allocation\".<br />";
		$path = 'sirius/feeds/launch/ledger_feed_allocate';
		$title = t('Create the Feed');
	}

	if (drupal_valid_path($path)) {
		$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
	} else {
		$html .= '<em>' . t('Feed access denied.') . '</em>';
	}

	return $html;
}

function sirius_ledger_node_tab_payment_allocate_simple_access($payment_node) {
	if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') { return FALSE; }
	return in_array('simple', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_simple($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_allocate_simple_form', $payment_node);
}

function sirius_ledger_node_tab_payment_allocate_simple_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#required' => TRUE,
		'#title' => t('Credit this payment to accounts receivable for the account and participant below.'),
	);

	$fields = array(
		'field_sirius_payer',
		'field_sirius_ledger_account',
		'field_sirius_dollar_amt',
		'field_sirius_payment_type',
	);
	foreach ($fields as $field) {
		$form["intro_$field"] = array('#markup' => sirius_dispatch_view_render_field($payment_node, $field));
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_allocate_simple_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
	sirius_ledger_payment_allocate_simple($payment_node);
	drupal_set_message('Entry created.');
}