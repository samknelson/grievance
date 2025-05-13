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
	// $year = $monthly->get_year($eup_node);
	// $month = $monthly->get_month($eup_node);
	$year = date('Y'); $month = date('n');
	if (!$employer_node || !$year || !$month) { return sirius_form_error("System error: employer, year, or month not found."); }

	$ea = Sirius_Ledger_EA::getInstance();

	$invoice_obj = sirius_ledger_invoice_obj();
	$interval_id = "$year-$month";
	$interval = $invoice_obj->interval_from_id($interval_id);

	$account_nid = 2457512; // @todo
	$form_state['account_nid'] = $account_nid;

	$ea_node = $ea->fetch($employer_node->nid, $account_nid, TRUE);

	$invoice = $invoice_obj->invoice_load($employer_node->nid, $account_nid, $interval);
	$html .= $invoice_obj->invoice_render($invoice);

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
		$payment_choice_options = array(NULL => t('-- Select --'));
		$payment_choice_options['offline'] = t('I will send a check');
		if ($online_payments) { $payment_choice_options['online'] = t('Pay Online'); }
		$form['payment_choice'] = array(
			'#type' => 'select',
			'#title' => t('Payment'),
			'#options' => $payment_choice_options,
		);

		$form['payment'] = array(
			'#type' => 'fieldset',
			'#title' => t('Online Payment'),
	 		'#states' => array('visible' => array(":input[name=\"payment_choice\"]" => array('value' => 'online'))),
	 	);

		$form['payment']['amount'] = array(
			'#type' => 'textfield',
			'#title' => 'Payment Amount',
			'#size' => 6,
			'#field_prefix' => '$',
		);

		if ($online_payments) {
			$form['payment']['paymentmethod'] = array(
				'#type' => 'select',
				'#title' => 'Payment Method',
				'#options' => array(NULL => t('-- Select --')) + $paymentmethods_options,
			);
		}

		$form['payment']['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => 'Yes, please charge the amount entered above to the selected payment method.',
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Continue',
	);

	$form['noprint_close'] = array('#markup' => '</div>');

	return sirius_sms_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_statement_form_validate($form, &$form_state) {
	if ($form_state['values']['payment_choice'] != 'online') { return; }

	$amount = $form_state['values']['payment']['amount'];
	if (!is_numeric($amount) || !($amount > 0)) {
		form_set_error('payment][amount', 'Please enter a positive number.');
	}
	if (!$form_state['values']['payment']['paymentmethod']) {
		form_set_error('payment][paymentmethod', 'Please select a payment method.');
	}
	if (!$form_state['values']['payment']['confirm']) {
		form_set_error('payment][confirm', 'Please check the confirmation box.');
	}
}


function sirius_smf_feed_ehrs_statement_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$account_nid = $form_state['account_nid'];
  $monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
  $access_context = array('wizard_node' => $wizard_node);
	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	$employer_node = $monthly->get_employer($eup_node);
	if (!$eup_node || !$employer_node || !$account_nid) {
		drupal_set_message("System error: employer or eup not found.", 'error');
		return;
	}

	// Handle online payment
	if ($form_state['values']['payment_choice'] == 'online') {
		$service = sirius_ledger_service();
		$customer_id = $employer_node->uuid;
		$result = $service->paymentmethod_list($customer_id);
		if (!$result['success']) {
			drupal_set_message("Unable to generate a list of payment methods: " . $result['msg'], 'warning');
			return;
		}
		$paymentmethods_json = $result['paymentmethods'];
		$paymentmethods_options = $service->paymentmethods_options($entity_node, $paymentmethods_json);
		if (!$paymentmethods_options) {
			drupal_set_message('In order to make an online payment, please create at least one payment method.', 'warning');
			return;
		}

		$amount = $form_state['values']['payment']['amount'];
		$paymentmethod = $form_state['values']['payment']['paymentmethod'];

		$currency = sirius_ledger_currency_from_account_nid($account_nid);
		
		$result = $service->paymentmethod_charge(
			$employer_node, 
			$amount,
			$account_nid,
			$paymentmethod
		);

		if (!$result['success']) {
			drupal_set_message("Transaction failed: " . $result['msg'], 'error');
			return;
		}

		drupal_set_message("A charge of " . $currency->render_amt($amount) . " has been applied to " . $paymentmethods_options[$paymentmethod] . ". When it clears, the account balance will be updated to reflect the payment.");
	}

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Payment Choice Confirmed", array('nonsequential' => FALSE));
}
