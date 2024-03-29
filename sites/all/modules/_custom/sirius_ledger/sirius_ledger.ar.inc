<?php

/*
CREATE TABLE sirius_ledger_ar (
ledger_id integer primary key AUTO_INCREMENT,
ledger_amount numeric(10,2),
ledger_status varchar(100),
ledger_account integer,
ledger_participant integer,
ledger_reference integer,
ledger_ts integer,
ledger_memo varchar(255),
ledger_key varchar(255)
);
CREATE INDEX sirius_ledger_ar_account on sirius_ledger_ar (ledger_account);
CREATE INDEX sirius_ledger_ar_participant on sirius_ledger_ar (ledger_participant);
CREATE INDEX sirius_ledger_ar_reference on sirius_ledger_ar (ledger_reference);
CREATE INDEX sirius_ledger_ar_key on sirius_ledger_ar (ledger_key);
*/

function sirius_ledger_ar_fetch($params = array()) {
	$sql_args = array();
	if ($params['balance']) { 
		$sql = "select sum(ledger_amount) as s ";
	} else {
		$sql = "select sirius_ledger_ar.*, ";
		$sql .= "participant_node.title as participant_title, ";
		$sql .= "field_data_field_sirius_contact.entity_id as worker_nid, ";
		$sql .= "account_node.title as account_title, ";
		$sql .= "reference_node.title as reference_title ";
	}
	$sql .= "from sirius_ledger_ar ";
	if ($params['balance']) {
	} else {
		$sql .= "left join node participant_node on participant_node.nid = ledger_participant ";
		$sql .= "left join node account_node on account_node.nid = ledger_account ";
		$sql .= "left join node reference_node on reference_node.nid = ledger_reference ";
		$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.field_sirius_contact_target_id = participant_node.nid and field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.bundle = 'sirius_worker' ";
	}

	$sql .= "where 1 = 1 ";

	if (isset($params['ledger_amount_min'])) {
		$sql .= "and ledger_amount >= :ledger_amount_min ";
		$sql_args[':ledger_amount_min'] = $params['ledger_amount_min'];
	}
	if (isset($params['ledger_amount_max'])) {
		$sql .= "and ledger_amount <= :ledger_amount_max ";
		$sql_args[':ledger_amount_max'] = $params['ledger_amount_max'];
	}
	if ($params['ledger_status']) { 
		$sql .= "and ledger_status = :ledger_status ";
		$sql_args[':ledger_status'] = $params['ledger_status'];
	}
	if ($params['ledger_account']) { 
		$sql .= "and ledger_account = :ledger_account ";
		$sql_args[':ledger_account'] = $params['ledger_account'];
	}
	if ($params['ledger_accounts']) { 
		$sql .= "and ledger_account in (:ledger_accounts) ";
		$sql_args[':ledger_accounts'] = $params['ledger_accounts'];
	}
	if ($params['ledger_participant']) { 
		$sql .= "and ledger_participant = :ledger_participant ";
		$sql_args[':ledger_participant'] = $params['ledger_participant'];
	}
	if ($params['ledger_reference']) { 
		$sql .= "and ledger_reference = :ledger_reference ";
		$sql_args[':ledger_reference'] = $params['ledger_reference'];
	}
	if ($params['ledger_key']) { 
		$sql .= "and ledger_key = :ledger_key ";
		$sql_args[':ledger_key'] = $params['ledger_key'];
	}
	if ($params['ledger_ts_min']) {
		$sql .= "and ledger_ts >= :ledger_ts_min ";
		$sql_args[':ledger_ts_min'] = $params['ledger_ts_min'];
	}
	if ($params['ledger_ts_max']) {
		$sql .= "and ledger_ts <= :ledger_ts_max ";
		$sql_args[':ledger_ts_max'] = $params['ledger_ts_max'];
	}
	if ($params['ledger_memo']) {
		$sql .= "and ledger_memo like :ledger_memo ";
		$sql_args[':ledger_memo'] = $params['ledger_memo'];
	}

	if ($params['balance']) {
	} else {
		if (!$params['unsorted']) {
			$sql .= "order by ledger_ts desc ";
		}
	}
	if ($params['limit']) {
		$sql .= "limit " . $params['limit'];
	}

	$result = array();
	$stmt = sirius_sql_query($sql, $sql_args);
	if ($params['balance']) {
		$hr = $stmt->fetchAssoc();
		if (!$hr['s']) { $hr['s'] = 0; }
		return $hr['s'];
	}
	while ($hr = $stmt->fetchAssoc()) {  $result[] = $hr; }
	return $result;
}

function sirius_ledger_ar_render($entries, $options = array()) {
	if (!$entries) { return '<p>' . t('No A/R entries found.') . '</p>'; }
	
	if ($options['display'] == 'csv') {
		$header = array(
			'account' => 'Account',
			'id' => 'Id',
			'date' => 'Date',
			'amount' => 'Amount',
			'status' => 'Status',
			'participant' => 'Participant',
			'reference' => 'Reference',
			'memo' => 'Memo',
		);
	} else {
		$header = array();
		if ($options['include_account']) { $header['account'] = 'Account'; }
		$header += array(
			'id' => 'Id',
			'date' => 'Date',
			'amount' => array('data' => 'Amount'),
			'status' => array('data' => 'Status'),
			'participant' => 'Participant',
			'reference' => array('data' => 'Reference'),
			'memo' => array('data' => 'Memo'),
			'tools' => 'Tools',
		);
	}
	$keys = array_keys($header);
	foreach ($keys as $key) { 
		if ($options["hide_$key"]) { unset($header[$key]); }
	}

	if ($options['display'] == 'csv') {
		$fp = fopen('php://output', 'w');
		$filename = $options['filename'];
		if (!$filename) { $filename = 'ar.csv'; }
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, array_values($header));
	}

	$rows = array();
	$totals = array();
	foreach ($entries as $entry) {

		$row = array();

		$tools = '';
		if ($header['account']) { $row[] = $entry['account_title']; }
		if ($header['id']) { $row[] = $entry['ledger_id']; }
		if ($header['date']) { $row[] = date('Y-m-d H:i:s', $entry['ledger_ts']); }
		if ($header['amount']) { $row[] = '$' . number_format($entry['ledger_amount'], 2); }
		if ($header['status']) { $row[] = $entry['ledger_status']; }
		if ($header['participant']) {
			$row[] = $entry['participant_title'];
			if ($entry['worker_nid']) {
				$tools .= l('Worker', 'node/' . $entry['worker_nid'], array('attributes' => array('class' => array('tool-button'))));
			} else {
				$tools .= l('Participant', 'node/' . $entry['ledger_participant'], array('attributes' => array('class' => array('tool-button'))));
			}
		}
		if ($header['reference']) {
			$row[] = $entry['reference_title'];
			$tools .= l('Reference', 'node/' . $entry['ledger_reference'], array('attributes' => array('class' => array('tool-button'))));
		}
		if ($header['memo']) { $row[] = $entry['ledger_memo']; }
		if ($header['tools']) { $row[] = $tools; }

		if ($options['display'] == 'csv') {
			fputcsv($fp, $row); 
		} else {
			$rows[$entry['account_title']][] = $row;
			$totals[$entry['account_title']] += $entry['ledger_amount'];
		}
	}

	if ($options['display'] == 'csv') { 
		fclose($fp);
		exit();
	}

	foreach ($rows as $account_title => $sub_rows) {
		$html .= '<h2>' . $account_title . '</h2>';
		$html .= 'Total: <strong>$' . number_format($totals[$account_title], 2) . '</strong>';
		$html .= theme_table(array('header' => array_values($header), 'rows' => $sub_rows, 'attributes' => array('style' => 'table-layout: fixed;')));
	}

	return $html;
}

function sirius_ledger_ar_insert($entry) {
	$sql = 'insert into sirius_ledger_ar (ledger_amount, ledger_status, ledger_account, ledger_participant, ledger_reference, ledger_ts, ledger_memo, ledger_key) ';
	$sql .= 'values (:ledger_amount, :ledger_status, :ledger_account, :ledger_participant, :ledger_reference, :ledger_ts, :ledger_memo, :ledger_key) ';

	$sql_args = array();
	$sql_args[':ledger_amount'] = $entry['ledger_amount'];
	$sql_args[':ledger_status'] = $entry['ledger_status'];
	$sql_args[':ledger_account'] = $entry['ledger_account'];
	$sql_args[':ledger_participant'] = $entry['ledger_participant'];
	$sql_args[':ledger_reference'] = $entry['ledger_reference'];
	$sql_args[':ledger_ts'] = $entry['ledger_ts'];
	$sql_args[':ledger_memo'] = $entry['ledger_memo'];
	$sql_args[':ledger_key'] = $entry['ledger_key'];

	$stmt = sirius_sql_query($sql, $sql_args);

	sirius_ledger_balance_update($entry['ledger_participant'], $entry['ledger_account']);
}

function sirius_ledger_ar_update($entry) {
	$sql = 'update sirius_ledger_ar set ';
	$sql .= 'ledger_amount = :ledger_amount, ';
	$sql .= 'ledger_status = :ledger_status, ';
	$sql .= 'ledger_account = :ledger_account, ';
	$sql .= 'ledger_participant = :ledger_participant, ';
	$sql .= 'ledger_reference = :ledger_reference, ';
	$sql .= 'ledger_ts = :ledger_ts, ';
	$sql .= 'ledger_memo = :ledger_memo, ';
	$sql .= 'ledger_key = :ledger_key ';
	$sql .= 'where ledger_id = :ledger_id ';

	$sql_args = array();
	$sql_args[':ledger_amount'] = $entry['ledger_amount'];
	$sql_args[':ledger_status'] = $entry['ledger_status'];
	$sql_args[':ledger_account'] = $entry['ledger_account'];
	$sql_args[':ledger_participant'] = $entry['ledger_participant'];
	$sql_args[':ledger_reference'] = $entry['ledger_reference'];
	$sql_args[':ledger_ts'] = $entry['ledger_ts'];
	$sql_args[':ledger_memo'] = $entry['ledger_memo'];
	$sql_args[':ledger_key'] = $entry['ledger_key'];
	$sql_args[':ledger_id'] = $entry['ledger_id'];

	$stmt = sirius_sql_query($sql, $sql_args);

	sirius_ledger_balance_update($entry['ledger_participant'], $entry['ledger_account']);
}

function sirius_ledger_ar_delete($entry) {
	$sql = 'delete from sirius_ledger_ar where ledger_id = :ledger_id';
	$sql_args = array(':ledger_id' => $entry['ledger_id']);
	$stmt = db_query($sql, $sql_args);

	sirius_ledger_balance_update($entry['ledger_participant'], $entry['ledger_account']);
}

function sirius_ledger_ar_delete_by_reference($ledger_reference) {
	$sql = "select distinct ledger_participant, ledger_account from sirius_ledger_ar where ledger_reference = :ledger_reference ";
	$stmt = sirius_sql_query($sql, array(':ledger_reference' => $ledger_reference));
	$to_update = array();
	while ($hr = $stmt->fetchAssoc()) { $to_update[] = $hr; }

	$sql = 'delete from sirius_ledger_ar where ledger_reference = :ledger_reference ';
	$stmt = sirius_sql_query($sql, array(':ledger_reference' => $ledger_reference));
	$rowcount = $stmt->rowCount();

	foreach ($to_update as $hr) {
		sirius_ledger_balance_update($hr['ledger_participant'], $hr['ledger_account']);
	}
	
	return $rowcount;
}

function sirius_ledger_ar_format_balance($balance) {
	if (!$balance) { $balance = 0; }
	if (!is_numeric($balance)) {
		return '<span class="sirius_ledger_balance>' . $balance . '</span>';
	} else if ($balance < 0) { 
		return '<span class="sirius_ledger_balance sirius_ledger_balance_negative">' . '($' . number_format(-1 * $balance, 2) . ')' . '</span>';
	} else {
		return '<span class="sirius_ledger_balance">' . '$' . number_format($balance, 2) . '</span>';
	}
}

function sirius_ledger_page_ar_purge() {
	return drupal_get_form('sirius_ledger_page_ar_purge_form');
}

function sirius_ledger_page_ar_purge_form($form, &$form_state) {
	 $accounts = array(NULL => t('-- Select --')) + sirius_ledger_account_list();

	 $form['account_nid'] = array(
	 	'#type' => 'select',
	 	'#options' => $accounts,
	 	'#required' => TRUE,
	 	'#title' => t('Account'),
	 );

	 $form['confirm'] = array(
	 	'#type' => 'checkbox',
	 	'#title' => t('I understand that his will permanently and completely delete entries from the ledger. There will be no way to retrieve or restore any data lost.'),
	 	'#required' => TRUE,
	 );

	 $form['submit'] = array(
	 	'#type' => 'submit',
	 	'#value' => t('Submit'),
	 );

	 return $form;
}

function sirius_ledger_page_ar_purge_form_submit($form, &$form_state) {
	$account_nid = $form_state['values']['account_nid'];
	if (!$account_nid) {
		drupal_set_message("Please select an account.");
		return;
	}

	$params = array();
	$params['ledger_account'] = $account_nid;
	$count = sirius_ledger_ar_purge($params);
	drupal_set_message("Account purged. $count records deleted.");
}

function sirius_ledger_ar_purge($params) {
	$sql_args = array();
	$sql = "delete from sirius_ledger_ar ";
	$sql .= "where 1 = 1 ";

	if ($params['ledger_status']) { 
		$sql .= "and ledger_status = :ledger_status ";
		$sql_args[':ledger_status'] = $params['ledger_status'];
	}
	if ($params['ledger_account']) { 
		$sql .= "and ledger_account = :ledger_account ";
		$sql_args[':ledger_account'] = $params['ledger_account'];
	}
	if ($params['ledger_participant']) { 
		$sql .= "and ledger_participant = :ledger_participant ";
		$sql_args[':ledger_participant'] = $params['ledger_participant'];
	}
	if ($params['ledger_reference']) { 
		$sql .= "and ledger_reference = :ledger_reference ";
		$sql_args[':ledger_reference'] = $params['ledger_reference'];
	}
	if ($params['ledger_key']) { 
		$sql .= "and ledger_key = :ledger_key ";
		$sql_args[':ledger_key'] = $params['ledger_key'];
	}

	$stmt = sirius_sql_query($sql, $sql_args);
	return $stmt->rowCount();

	sirius_ledger_balance_purge($params['ledger_participant'], $params['ledger_account']);
}
