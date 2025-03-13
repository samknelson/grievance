<?php

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

function sirius_ledger_ar_fetch_entry($ledger_id) {
	$sql = "select * from sirius_ledger_ar where ledger_id = :ledger_id";
	$stmt = sirius_sql_query($sql, array(':ledger_id' => $ledger_id));
	return $stmt->fetchAssoc();
}

function sirius_ledger_ar_render($entries, $options = array()) {
	if (!$entries) {
		$empty_txt = $options['empty_txt'];
		if (!$empty_txt) { $empty_txt = t('No A/R entries found.'); }
		return "<p>$empty_txt</p>";
	}

	$access = sirius_access_instance('ledger');
	$is_admin = $access->access('admin');

	if ($options['display'] == 'csv') {
		$header = array(
			'account' => 'Account',
			'id' => 'Id',
			'date' => 'Date',
			'amount' => 'Amount',
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
			'participant' => 'Participant',
			'reference' => array('data' => 'Reference'),
			'memo' => array('data' => 'Memo'),
		);
		if ($is_admin) { $header['tools'] = t('Tools'); }
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

	$is_admin = user_access('sirius admin');

	$rows = array();
	$totals = array();

	foreach ($entries as $entry) {
		$account_nid = $entry['ledger_account'];
		$currency_info = sirius_ledger_currency_info_from_account_nid($account_nid);

		if (!isset($rows[$account_nid])) {
			$rows[$account_nid] = array();
			$rows[$account_nid]['entries'] = array();
			$rows[$account_nid]['account_title'] = $entry['account_title'];

			$totals[$account_nid] = array();
			$totals[$account_nid]['amt'] = 0;
			$totals[$account_nid]['account_title'] = $entry['account_title'];
		}

		$row = array();

		$tools = '';

		if ($is_admin) {
			$tools .= l('Detail &#x21D7;', 'sirius/modal/ledger/ar/view/' . $entry['ledger_id'], array('html' => TRUE, 'attributes' => array('class' => array('tool-button'))));
		}
		if ($header['account']) { $row[] = $entry['account_title']; }
		if ($header['id']) { $row[] = $entry['ledger_id']; }
		if ($header['date']) { $row[] = date('m/d/Y h:i A', $entry['ledger_ts']); }
		if ($header['amount']) {
			if ($options['display'] == 'csv') { $row[] = round(-1 * $entry['ledger_amount'], 2); }
			else { $row[] = $currency_info['instance']->render_amt(-1 * $entry['ledger_amount']); }
		}
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
			if ($entry['ledger_reference']) {
				$tools .= l('Reference', 'node/' . $entry['ledger_reference'], array('attributes' => array('class' => array('tool-button'))));
			}
		}
		if ($header['memo']) { $row[] = $entry['ledger_memo']; }
		if ($header['tools']) { $row[] = $tools; }

		if ($options['display'] == 'csv') {
			fputcsv($fp, $row); 
		} else {
			$rows[$account_nid]['entries'][] = $row;
			$totals[$account_nid]['amt'] += $entry['ledger_amount'];
		}
	}

	if ($options['display'] == 'csv') { 
		fclose($fp);
		exit();
	}

	foreach ($rows as $account_nid => $record) {
		if (!$options['hide_account_header']) { $html .= '<h2>' . $record['account_title'] . '</h2>'; }
		if (!$options['hide_total']) {
			$currency_info = sirius_ledger_currency_info_from_account_nid($account_nid);
			$html .= 'Total: <strong>';
			$html .= $currency_info['instance']->render_amt(-1 * $totals[$account_nid]['amt']);
			$html .= '</strong>';
		}

		$html .= sirius_table_render(array_values($header), $record['entries'], array('style' => 'table-layout: fixed;', 'class' => array('sirius_data_table')));
	}

	return $html;
}

function sirius_ledger_ar_insert($entry) {
	if (!$entry['ledger_json']) { $entry['ledger_json'] = array(); }

	if (!$entry['ledger_json']['tracking']) {
		global $user;
		$entry['ledger_json']['tracking']['ts'] = time();
		$entry['ledger_json']['tracking']['uid'] = $user->uid;
		$entry['ledger_json']['tracking']['name'] = $user->name;
	}

	$sql = 'insert into sirius_ledger_ar (ledger_amount, ledger_status, ledger_account, ledger_participant, ledger_reference, ledger_ts, ledger_memo, ledger_key, ledger_json) ';
	$sql .= 'values (:ledger_amount, :ledger_status, :ledger_account, :ledger_participant, :ledger_reference, :ledger_ts, :ledger_memo, :ledger_key, :ledger_json) ';

	$sql_args = array();
	$sql_args[':ledger_amount'] = $entry['ledger_amount'];
	$sql_args[':ledger_status'] = $entry['ledger_status'];
	$sql_args[':ledger_account'] = $entry['ledger_account'];
	$sql_args[':ledger_participant'] = $entry['ledger_participant'];
	$sql_args[':ledger_reference'] = $entry['ledger_reference'];
	$sql_args[':ledger_ts'] = $entry['ledger_ts'];
	$sql_args[':ledger_memo'] = $entry['ledger_memo'];
	$sql_args[':ledger_key'] = $entry['ledger_key'];
	$sql_args[':ledger_json'] = json_encode($entry['ledger_json']);

	$stmt = sirius_sql_query($sql, $sql_args);

	sirius_ledger_balance_update($entry['ledger_participant'], $entry['ledger_account']);
}

function sirius_ledger_ar_update($entry) {
	if (!$entry['ledger_json']) { $entry['ledger_json'] = array('tracking' => array()); }
	if (!is_array($entry['ledger_json'])) { $entry['ledger_json'] = json_decode($entry['ledger_json'], TRUE); }
	if (!$entry['ledger_json']['tracking']) {
		global $user;
		$entry['ledger_json']['tracking']['ts'] = time();
		$entry['ledger_json']['tracking']['uid'] = $user->uid;
		$entry['ledger_json']['tracking']['name'] = $user->name;
	}

	$sql = 'update sirius_ledger_ar set ';
	$sql .= 'ledger_amount = :ledger_amount, ';
	$sql .= 'ledger_status = :ledger_status, ';
	$sql .= 'ledger_account = :ledger_account, ';
	$sql .= 'ledger_participant = :ledger_participant, ';
	$sql .= 'ledger_reference = :ledger_reference, ';
	$sql .= 'ledger_ts = :ledger_ts, ';
	$sql .= 'ledger_memo = :ledger_memo, ';
	$sql .= 'ledger_key = :ledger_key, ';
	$sql .= 'ledger_json = :ledger_json ';
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
	$sql_args[':ledger_json'] = json_encode($entry['ledger_json']);
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

function sirius_ledger_ar_format_balance($balance, $currency_type = 'USD') {
	return sirius_plugin_info('ledger_currency', $currency_type)['instance']->render_amt($balance);
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
	$rowcount = $stmt->rowCount();

	sirius_ledger_balance_purge($params['ledger_participant'], $params['ledger_account']);

	return $rowcount;
}

function sirius_ledger_ar_reset_all($account_nid, $date, $memo) {
	$sql = "select distinct ledger_participant from sirius_ledger_ar ";
	$sql .= "where ledger_account = :ledger_account ";
	$sql_args[':ledger_account'] = $account_nid;
	$stmt = sirius_sql_query($sql, $sql_args);

	$participants = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participants[] = $hr['ledger_participant'];
	}

	foreach ($participants as $participant_nid) {
		sirius_ledger_ar_reset($account_nid, $participant_nid, $date, $memo);
	}

	return count($participants);
}

function sirius_ledger_ar_reset($account_nid, $participant_nid, $date, $memo) {
	$ts = strtotime($date);
	if ($ts === FALSE) { $ts = time(); }

	$params = array();
	$params['ledger_account'] = $account_nid;
	$params['ledger_participant'] = $participant_nid;
	$params['ledger_ts_max'] = $ts;
	$params['balance'] = TRUE;
	$balance = sirius_ledger_ar_fetch($params);

	if (!$balance) { return; }

	$entry = array();
	$entry['ledger_amount'] = -1 * $balance;
	$entry['ledger_memo'] = $memo;
	$entry['ledger_ts'] = $ts;
	$entry['ledger_account'] = $account_nid;
	$entry['ledger_participant'] = $participant_nid;
	$entry['ledger_json'] = array('reset' => array("Account Reset"));
	$entry['ledger_status'] = 'Cleared';


	sirius_ledger_ar_insert($entry);

	drupal_set_message("Resetting: $account_nid, $participant_nid, " . date('c', $ts) . ", $memo");
}