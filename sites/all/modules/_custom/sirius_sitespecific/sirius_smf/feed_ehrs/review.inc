<?php

function sirius_smf_feed_ehrs_review_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {

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

	$balance = $ea->balance($ea_node);
	$currency = sirius_ledger_currency_from_account_nid($account_nid);
	$balance_render = $currency->render_amt($balance);





	$html = '<p>' . t('Thank you for completing the monthly hours upload.') . '</p>';
	$html .= l('View All Invoices and Monitor Payment Status', 'node/' . $employer_node->nid . '/grievance_shop_ledger', array('attributes' => array('class' => array('tool-button'))));

	$form['html'] = array('#markup' => $html);



	return sirius_sms_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

