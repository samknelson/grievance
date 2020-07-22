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
	$sql .= "left join node participant_node on participant_node.nid = ledger_participant ";
	$sql .= "left join node account_node on account_node.nid = ledger_account ";
	$sql .= "left join node reference_node on reference_node.nid = ledger_reference ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.field_sirius_contact_target_id = participant_node.nid and field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.bundle = 'sirius_worker' ";

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
	$sql .= "order by ledger_ts desc ";

	$result = array();
	$stmt = db_query($sql, $sql_args);
	if ($params['balance']) {
		$hr = $stmt->fetchAssoc();
		return $hr['s'];
	}
	while ($hr = $stmt->fetchAssoc()) {  $result[] = $hr; }
	return $result;
}

function sirius_ledger_ar_render($entries, $options = array()) {
	if (!$entries) { return '<p>' . t('No A/R entries found.') . '</p>'; }
	
	$header = array(
		'id' => 'Id',
		'amount' => 'Amount',
		'status' => 'Status',
		'participant' => 'Participant',
		'reference' => 'Reference',
		'memo' => 'Memo',
		'date' => 'Date',
		'tools' => 'Tools',
	);

	$keys = array_keys($header);
	foreach ($keys as $key) { 
		if ($options["hide_$key"]) { unset($header[$key]); }
	}
	$header = array_values($header);

	$rows = array();
	$totals = array();
	foreach ($entries as $entry) {

		$row = array();

		$tools = '';
		if (!$options['hide_id']) { $row[] = $entry['ledger_id']; }
		if (!$options['hide_amount']) { $row[] = '$' . number_format($entry['ledger_amount'], 2); }
		if (!$options['hide_status']) { $row[] = $entry['ledger_status']; }
		if (!$options['hide_participant']) {
			$row[] = $entry['participant_title'];
			if ($entry['worker_nid']) {
				$tools .= l('Worker', 'node/' . $entry['worker_nid'], array('attributes' => array('class' => array('tool-button'))));
			} else {
				$tools .= l('Participant', 'node/' . $entry['ledger_participant'], array('attributes' => array('class' => array('tool-button'))));
			}
		}
		if (!$options['hide_reference']) {
			$row[] = $entry['reference_title'];
			$tools .= l('Reference', 'node/' . $entry['ledger_reference'], array('attributes' => array('class' => array('tool-button'))));
		}
		if (!$options['hide_memo']) { $row[] = $entry['ledger_memo']; }
		if (!$options['hide_date']) { $row[] = date('Y-m-d H:i:s', $entry['ledger_ts']); }
		if (!$options['hide_tools']) { $row[] = $tools; }

		$rows[$entry['account_title']][] = $row;

		$totals[$entry['account_title']] += $entry['ledger_amount'];
	}

	foreach ($rows as $account_title => $sub_rows) {
		$html .= '<h2>' . $account_title . '</h2>';
		$html .= theme_table(array('header' => $header, 'rows' => $sub_rows, 'attributes' => array()));
		$html .= 'Total: <strong>$' . number_format($totals[$account_title], 2) . '</strong>';
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

	$stmt = db_query($sql, $sql_args);
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
}

function sirius_ledger_ar_delete($entry) {
	$sql = 'delete from sirius_ledger_ar where ledger_id = :ledger_id';
	$sql_args = array(':ledger_id' => $entry['ledger_id']);
	$stmt = db_query($sql, $sql_args);
}

function sirius_ledger_ar_format_balance($balance) {
	if ($balance < 0) { 
		return '<span class="sirius_ledger_balance sirius_ledger_balance_negative">' . '($' . number_format(-1 * $balance, 2) . ')' . '</span>';
	} else {
		return '<span class="sirius_ledger_balance">' . '$' . number_format($balance, 2) . '</span>';
	}
}