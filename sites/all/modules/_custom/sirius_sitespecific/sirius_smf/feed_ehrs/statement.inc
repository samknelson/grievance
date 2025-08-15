<?php

function sirius_smf_feed_ehrs_statement_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
  $monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
  $access_context = array('wizard_node' => $wizard_node);

	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	if (!$eup_node) { return sirius_form_error("System error: eup node not found."); }

	$employer_node = $monthly->get_employer($eup_node);

	// We'd kind of like to show the invoice as of the uploaded month/year. That would be the most consistent for the employer.
	// But this leads to odd outcomes if you re-post an old upload, because charges are entered as of the current date, not as of the
	// work date. (I.e. if you upload a July hours entry in August, it doesn't change your July invoice, it changes your August invoice.)
	// So instead, we always show the invoice for the current month / year. You'll generally be uploading last month, so you'll want to see 
	// this months's invoice.
	
	$start_year = $monthly->get_year($eup_node);
	$start_month = $monthly->get_month($eup_node);
	if (!$employer_node || !$start_year || !$start_month) { return sirius_form_error("System error: employer, year, or month not found."); }

	$end_year = date('Y');
	$end_month = date('n');

	$interval_ids[] = "$start_year-$start_month";

	$month = $start_month;
	$year = $start_year;
	$count = 0;

	$interval_ids = array();
	while (TRUE) {
		$interval_ids[] = "$year-$month";

		++$count;
		++$month;
		if ($month > 12) { $month = 1; ++$year; }

		// Don't go past the end month
		if ( ($year > $end_year) || ( ($year == $end_year) && ($month > $end_month) ) ) { break; }

		// Don't show more than four months
		if ($count > 4) { break; }
	}
	$interval_ids[] = "$end_year-$end_month";
	$interval_ids = array_unique($interval_ids);

	$ea = Sirius_Ledger_EA::getInstance();

	foreach ($interval_ids as $interval_id) {
		$invoice_obj = sirius_ledger_invoice_obj();
		$interval = $invoice_obj->interval_from_id($interval_id);

		$account_nid = 2457512; // @todo
		$form_state['account_nid'] = $account_nid;

		$ea_node = $ea->fetch($employer_node->nid, $account_nid, TRUE);

		$invoice = $invoice_obj->invoice_load($employer_node->nid, $account_nid, $interval);

		$render_options = array();
		if ($interval_id != "$end_year-$end_month") {
			$render_options['hide_pending_payments'] = TRUE; 
		}
		$html .= $invoice_obj->invoice_render($invoice, $render_options);
	}

	$form['invoice'] = array('#markup' => $html);

	$balance = $ea->balance($ea_node);
	$currency = sirius_ledger_currency_from_account_nid($account_nid);
	$balance_render = $currency->render_amt($balance);

	$service = sirius_ledger_service();
	$customer_id = $employer_node->uuid;

	$online_payments = FALSE;
	if ($balance) {
		$online_payments = TRUE;
	}

	// Load the customer, or create one if it doesn't exist.
	$result = $service->customer_retrieve($customer_id);
	if (!$result['success']) { $result = $service->customer_sync($employer_node); }
	if (!$result['success']) {
		$online_payments = FALSE;
		drupal_set_message('Unable to fetch or sync customer: ' . $result['msg'], 'warning');
	}

	// Load the payment methods
	if ($online_payments) {
		$result = $service->paymentmethod_list($customer_id);
		if (!$result['success']) {
			$online_payments = FALSE;
			drupal_set_message("Unable to generate a list of payment methods: " . $result['msg'], 'warning');
		}
	}

	if ($online_payments) {
		$paymentmethods_json = $result['paymentmethods'];
		$paymentmethods_options = $service->paymentmethods_options($entity_node, $paymentmethods_json);
		if (!$paymentmethods_options) {
			$online_payments = FALSE;
			drupal_set_message('In order to make an online payment, please create at least one payment method.', 'warning');
		}
	}

	$form['noprint_open'] = array('#markup' => '<div class="sirius_noprint">');

	if ($balance > 0) {
		$paymentchoice_options = array(NULL => t('-- Select --'));
		$paymentchoice_options['offline'] = t('I will send a check');
		$paymentchoice_options['online'] = t('Pay Online');
		$form['paymentchoice'] = array(
			'#type' => 'select',
			'#title' => t('Payment'),
			'#options' => $paymentchoice_options,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Continue',
	);

	$form['noprint_close'] = array('#markup' => '</div>');

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_statement_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];
	$account_nid = $form_state['account_nid'];
	$data['paymentchoice'] = $form_state['values']['paymentchoice'];
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Payment Choice Confirmed", array('nonsequential' => FALSE));
}
