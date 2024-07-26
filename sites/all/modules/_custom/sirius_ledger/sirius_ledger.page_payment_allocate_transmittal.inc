<?php

function sirius_ledger_node_tab_payment_allocate_transmittal_access($payment_node) {
	return in_array('transmittal', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_transmittal($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_allocate_transmittal_form', $payment_node);
}

function sirius_ledger_node_tab_payment_allocate_transmittal_form($form, &$form_state, $payment_node) {
	drupal_add_js('sites/all/modules/_custom/sirius_ledger/js/transmittal.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_twilio') . '/css/sirius_twilio_browserphone.css');
	$js_parameters = array('iam' => 'here');
	drupal_add_js(array('sirius_ledger_tramsmittal' => $js_parameters), array('type' => 'setting'));
	drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/transmittal.css');

	$form_state['payment_nid'] = $payment_node->nid;
	$payment_json = sirius_jsonfield_util_node_json_get($payment_node);

	$employer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	if (!$employer_node) {
		drupal_set_message("No payer found for this payment.", 'error');
		return array();
	}
	if ($employer_node->type != 'grievance_shop') {
		drupal_set_message("The payer for this payment is not an employer", 'error');
		return array();
	}

	// Get our transmittal NID
	$transmittal_nid = $payment_json['transmittal_nid'];
	$transmittal_node = node_load($transmittal_nid);
	$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);
	if (!$transmittal_node) { drupal_set_message("No transmittal form associated with this payment."); return array(); }
	$form['backlink'] = array('#markup' => l('Back to Transmittal Form', 'node/' . $employer_node->nid . '/sirius_employer_ledger/transmittal', array('attributes' => array('class' => array('tool-button')))));

	// Fetch the inddustries
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	if (!$account_node) {
		drupal_set_message("No account associated with this payment.", 'error');
		return array();
	}
	$account_nid = $account_node->nid;
	$account_name = $account_node->title;

	// Create ms_level dropdown options
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$employer_industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
	$employer_ms_tids = array();
	foreach ($ms_terms as $ms_tid => $ms_term) {
		$industry_tid = $ms_term->field_sirius_industry['und'][0]['tid'];
		if (in_array($industry_tid, $employer_industry_tids)) { $employer_ms_tids[] = $ms_tid; }
	}

	$ms_level_options = array(NULL => t('-- Select --'));
	foreach ($employer_ms_tids as $ms_tid) {
		$levels = sirius_training_levels_fetch($ms_tid);
		$ms_level_options[$ms_tid] = $ms_terms[$ms_tid]->name;
		foreach ($levels as $level_code => $level) {
			$ms_level_options["$ms_tid|$level_code"] = $ms_terms[$ms_tid]->name . ': ' . $level['title'];
		}
	}

	// Get all current dispatches for this employer. SQL for speed.
	$dispatches = $transmittal_json['transmittal']['dispatches'];

	$form['#tree'] = TRUE;

  $form['transmittal']['ledger_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Enter payments as of'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#description' => t('If there is no payment date in the upload, use this date.'),
    '#required' => TRUE,
  );
  if ($payment_json['transmittal']['ledger_date']) { $form['transmittal']['ledger_date']['#default_value'] = $payment_json['transmittal']['ledger_date']; }


	$table_open_html .= '<table class="sirius_ledger_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>' . t('Name') . '</th>';
	$table_open_html .= '<th>' . t('Member Status') . '</th>';
	$table_open_html .= '<th>' . t('Withheld') . '</th>';
	$balance_button = "<a href=\"#\" class=\"sirius_transmittal_autofill_all\">&larr;</a>";
	$table_open_html .= '<th>' . $balance_button . t('Current Balance') . '</th>';
	$table_open_html .= '</tr>';
	$form['transmittal']['table_open'] = array('#markup' => $table_open_html);

	$balance_total = 0;
	$deltas = array();
	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_nid = $dispatch['worker_nid'];
		if (!$worker_nid) { continue; }
		$form['transmittal']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');

		$form['transmittal']['dispatches'][$dispatch_nid]['name'] = array('#markup' => '<td>' . l($dispatch['worker_name'], "node/$worker_nid", array('attributes' => array('target' => '_blank'))) . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['ms_name'] = array('#markup' => '<td>' . $ms_level_options[$dispatch['ms_level']] . '</td>');

		$form['transmittal']['dispatches'][$dispatch_nid]['payment'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#prefix' => '<td class="container-inline">$',
			'#suffix' => '</td>',
			'#id' => "payment_$dispatch_nid",
		);
		if (isset($payment_json['transmittal']['dispatches'][$dispatch_nid]['payment'])) { $form['transmittal']['dispatches'][$dispatch_nid]['payment']['#default_value'] = $payment_json['transmittal']['dispatches'][$dispatch_nid]['payment']; }

		// Get the balance
		$balance_raw = 0;
		if ($dispatch['contact_nid'] && $account_nid) { $balance_raw = sirius_ledger_balance_fetch($dispatch['contact_nid'], $account_nid); }
		if (!$balance_raw) { $balance_raw = 0; }

		$params = array();
		$params['balance'] = TRUE;
		$params['ledger_account'] = $account_nid;
		$params['ledger_participant'] = $dispatch['contact_nid'];
		$params['ledger_reference'] = $payment_node->nid;
		$balance_payment = sirius_ledger_ar_fetch($params);
		if (!$balance_payment) { $balance_payment = 0; }

		if ($balance_payment) {
			$balance_with_payment = $balance_raw;
			$balance_without_payment = $balance_raw - $balance_payment;
			$balance_render = sirius_ledger_ar_format_balance($balance_without_payment) . ' + ' . sirius_ledger_ar_format_balance($balance_payment) . ' = ' . sirius_ledger_ar_format_balance($balance_with_payment);
		} else {
			$balance_without_payment = $balance_with_payment = $balance_raw;
			$balance_render = sirius_ledger_ar_format_balance($balance_raw);
		}

		if ($balance_without_payment > 0) { $balance_autofill = $balance_without_payment; } else { $balance_autofill = 0; }

		$balance_button = "<a href=\"#\" class=\"sirius_transmittal_autofill\" data-amount=\"$balance_autofill\" data-target=\"payment_$dispatch_nid\">&larr;</a>";
		$balance_render = $balance_button . $balance_render;


		$form['transmittal']['dispatches'][$dispatch_nid]['balance'] = array('#markup' => '<td>' . $balance_render . '</td>');

		// If a worker appears multiple times, only add their balance once.
		if (!$deltas[$worker_nid] && is_numeric($balance)) { $balance_total += $balance; }
		++$deltas[$worker_nid];
	}

	$form['transmittal']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');

	$payment_total = $payment_json['transmittal_calcs']['payment_total'];
	if ($payment_total) {
		$form['transmittal']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
		$form['transmittal']['dispatches']['total']['title'] = array('#markup' => '<th colspan="2">Total</th>');
		$form['transmittal']['dispatches']['total']['total'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($payment_total) . '</th>');
		$form['transmittal']['dispatches']['total']['balance'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($balance_total) . '</th>');
		$form['transmittal']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');
	}

	$form['transmittal']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_allocate_transmittal_form_validate($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
	$payment_json = sirius_jsonfield_util_node_json_get($payment_node);

	$transmittal_nid = $payment_json['transmittal_nid'];
	$transmittal_node = node_load($transmittal_nid);
	$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);

	$payment_total = 0;
	$balance_total = 0;
	foreach ($form_state['values']['transmittal']['dispatches'] as $dispatch_nid => $transmittal) {
		$payment = $transmittal['payment'];
		if ($payment < 0) {
			form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'Payment cannot be negative');
			continue;
		}

		$worker_nid = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_nid'];
		$contact_nid = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['contact_nid'];
		if (!$worker_nid) {
			form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'Worker not found');
			continue;
		}
		if (!$contact_nid) {
			form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'Contact not found');
			continue;
		}

		if (is_numeric($payment)) { $payment_total += $payment; }
		if (is_numeric($transmittal['balance_calc'])) { $balance_total += $transmittal['balance_calc']; }
	}
	$form_state['storage']['payment_total'] = $payment_total;
	$form_state['storage']['balance_total'] = $balance_total;
	$check_value = -1 * $payment_node->field_sirius_dollar_amt['und'][0]['value'];

	if (round($check_value,2) != round($payment_total,2)) {
		form_set_error('transmittal][dispatches', 'The total payment amount (' . $payment_total . '), must match the payment value (' . $check_value . ')');
	}
}


function sirius_ledger_node_tab_payment_allocate_transmittal_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
	$payment_json = sirius_jsonfield_util_node_json_get($payment_node);

	$year = $form_state['year'];
	$month = $form_state['month'];
	$day = 1;
	$domain_nid = sirius_domain_nid();

	$payment_json['transmittal'] = $form_state['values']['transmittal'];
	$payment_json['transmittal_calcs'] = $form_state['storage'];
	unset($payment_json['allocate']);
	sirius_jsonfield_util_node_json_set($payment_node, $payment_json);
	node_save($payment_node);

	$transmittal_nid = $payment_json['transmittal_nid'];
	$transmittal_node = node_load($transmittal_nid);
	$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);

	sirius_ledger_payment_clear($payment_node->nid);
  $memo = sirius_ledger_payment_memo($payment_node);
	$ledger_ts = strtotime($form_state['values']['transmittal']['ledger_date']);

	foreach ($form_state['values']['transmittal']['dispatches'] as $dispatch_nid => $transmittal) {
		$payment = $transmittal['payment'];
		if (!$payment) { continue; }

		$worker_nid = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_nid'];
		$contact_nid = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['contact_nid'];

	  $entry = array();
	  $entry['ledger_amount'] = -1 * $payment;
	  $entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
	  $entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
	  $entry['ledger_memo'] = $memo;
	  $entry['ledger_ts'] = $ledger_ts;
	  $entry['ledger_reference'] = $payment_node->nid;
	  $entry['ledger_participant'] = $contact_nid;
		sirius_ledger_ar_insert($entry);
	}

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
	$payment_node->field_sirius_payment_status['und'][0]['value'] = 'Cleared';
	$payment_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s');
	node_save($payment_node);
  drupal_set_message("Allocation saved.");
}

