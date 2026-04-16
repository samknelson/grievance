<?php

function sirius_ledger_node_tab_account_ar($account_node) {
	$entries = sirius_ledger_ar_fetch(array('ledger_account' => $account_node->nid, 'limit' => 1000));
	return sirius_ledger_ar_render($entries, array('hide_id' => TRUE));
}

function sirius_ledger_node_tab_account_ar_export_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$years = array();
	$year_now = date('Y');
	for ($i=$year_now-20; $i<=$year_now; ++$i) {
		$years[$i] = $i;
	}

	$form['year'] = array(
		'#type' => 'select',
		'#title' => t('Year'),
		'#options' => $years,
		'#default_value' => $year_now - 1,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_account_ar_export_form_submit($form, &$form_state) {
	$account_nid = $form_state['account_nid'];
	$year = $form_state['values']['year'];

	// First second of the year (Jan 1, 00:00:00)
	$ledger_ts_min = mktime(0, 0, 0, 1, 1, $year);

	// Last second of the year (Dec 31, 23:59:59)
	$ledger_ts_max = mktime(23, 59, 59, 12, 31, $year);

	$stmt = sirius_ledger_ar_fetch(
		array(
			'ledger_account' => $account_node->nid, 
			'limit' => 1000000000,
			'ledger_ts_min' => $ledger_ts_min,
			'ledger_ts_max' => $ledger_ts_max,
			'return' => 'stmt',
		)
	);

	$header = array(
		'account' => 'Account',
		'id' => 'Id',
		'date' => 'Date',
		'amount' => 'Amount',
		'participant' => 'Participant',
		'reference' => 'Reference',
		'memo' => 'Memo',
	);
	$keys = array_keys($header);

	$fp = fopen('php://output', 'w');
	$filename = "ar_export_$year.csv";
	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	fputcsv($fp, array_values($header));

	while ($entry = $stmt->fetchAssoc()) {
		$row = array();

		if ($header['account']) { $row[] = $entry['account_title']; }
		if ($header['id']) { $row[] = $entry['ledger_id']; }
		if ($header['date']) { $row[] = date('m/d/Y h:i A', $entry['ledger_ts']); }
		if ($header['amount']) { $row[] = round(-1 * $entry['ledger_amount'], 2); }
		if ($header['participant']) { $row[] = $entry['participant_title']; }
		if ($header['reference']) { $row[] = $entry['reference_title']; }
		if ($header['memo']) { $row[] = $entry['ledger_memo']; }

		fputcsv($fp, $row);
	}

	fclose($fp);
	exit();
}