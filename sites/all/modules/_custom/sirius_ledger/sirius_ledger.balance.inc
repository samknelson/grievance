<?php

function sirius_ledger_balance_search($params = array()) {
	$sql_args = array();
	$sql = "select sirius_ledger_balance.*, ";
	$sql .= "participant_node.nid as participant_nid, ";
	$sql .= "participant_node.title as participant_title, ";
	$sql .= "worker_node.nid as worker_nid, ";
	$sql .= "worker_node.title as worker_title, ";
	$sql .= "account_node.nid as account_nid, ";
	$sql .= "account_node.title as account_title ";
	$sql .= "from sirius_ledger_balance ";

	$sql .= "left join node participant_node on participant_node.nid = balance_participant ";
	$sql .= "left join node account_node on account_node.nid = balance_account ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.field_sirius_contact_target_id = participant_node.nid and field_data_field_sirius_contact.bundle = 'sirius_worker' ";
	$sql .= "left join node worker_node on worker_node.nid = field_data_field_sirius_contact.entity_id ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = participant_node.nid ";

	if ($params['work_statuses']) {
		$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = worker_node.nid ";
	}
	if ($params['member_statuses']) {
		$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
	}


	$sql .= "where 1 = 1 ";

	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}

	if (isset($params['balance_amount_min']) && is_numeric($params['balance_amount_min'])) {
		$sql .= "and balance_amount >= :balance_amount_min ";
		$sql_args[':balance_amount_min'] = $params['balance_amount_min'];
	}
	if (isset($params['balance_amount_max']) && is_numeric($params['balance_amount_min'])) {
		$sql .= "and balance_amount <= :balance_amount_max ";
		$sql_args[':balance_amount_max'] = $params['balance_amount_max'];
	}
	if ($params['balance_account']) { 
		$sql .= "and balance_account = :balance_account ";
		$sql_args[':balance_account'] = $params['balance_account'];
	}
	if ($params['balance_account']) { 
		$sql .= "and balance_account in (:balance_account) ";
		$sql_args[':balance_account'] = $params['balance_account'];
	}
	if ($params['balance_participant']) { 
		$sql .= "and balance_participant = :balance_participant ";
		$sql_args[':balance_participant'] = $params['balance_participant'];
	}
	if ($params['work_statuses']) {
		$sql .= "and field_sirius_work_status_tid in (:work_statuses) ";
		$sql_args[':work_statuses'] = $params['work_statuses'];
	}
	if ($params['member_statuses']) {
		$sql .= "and field_sirius_member_status_tid in (:member_statuses) ";
		$sql_args[':member_statuses'] = $params['member_statuses'];
	}
	$sql .= "group by balance_participant, balance_account ";
	$sql .= "order by balance_participant, balance_account ";
	if (!$params['limit']) { $params['limit'] = 1000; }
	$sql .= "limit " . $params['limit'];

	$result = array();
	$stmt = sirius_sql_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {  $result[] = $hr; }
	return $result;
}

function sirius_ledger_balance_update($participant_id, $account_id) {
	$sql = "insert into sirius_ledger_balance ";
	$sql .= "(balance_participant, balance_account, balance_amount) ";
	$sql .= "values ";
	$sql .= "(:participant_id, :account_id, (select sum(ledger_amount) from sirius_ledger_ar where ledger_participant = :participant_id and ledger_account = :account_id) ) ";
	$sql .= "on duplicate key update ";
	$sql .= "balance_amount = (select sum(ledger_amount) from sirius_ledger_ar where ledger_participant = :participant_id and ledger_account = :account_id)";
	$stmt = sirius_sql_query($sql, array(':participant_id' => $participant_id, ':account_id' => $account_id));
}

function sirius_ledger_balance_fetch($participant_id, $account_id) {
	$sql = "select balance_amount from sirius_ledger_balance where balance_participant = :participant_id and balance_account = :account_id ";
	$stmt = sirius_sql_query($sql, array(':participant_id' => $participant_id, ':account_id' => $account_id));
	$hr = $stmt->fetchAssoc();
	return $hr['balance_amount'];
}

function sirius_ledger_balance_purge($participant_id = NULL, $account_id = NULL) {
	$sql_args = array();

	$sql = "delete from sirius_ledger_balance ";
	$sql .= "where 1 = 1 ";
	if ($participant_id) {
		$sql .= "and balance_participant = :participant_id ";
		$sql_args[':participant_id'] = $participant_id;
	}
	if ($account_id) {
		$sql .= "and balance_account = :account_id ";
		$sql_args[':account_id'] = $account_id;
	}
	$stmt = sirius_sql_query($sql, $sql_args);
}

/**
 * Intended to be run from the command line.
 */

function sirius_ledger_balance_update_all() {
	sirius_ledger_balance_purge();
	$sql = "select distinct ledger_participant, ledger_account from sirius_ledger_ar order by ledger_participant, ledger_account ";
	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		print "Computing balance for $hr[ledger_participant], $hr[ledger_account]\n";
		sirius_ledger_balance_update($hr['ledger_participant'], $hr['ledger_account']);
	}
}


function sirius_ledger_balance_render($entries, $options = array()) {
	if (!$entries) {
		$empty_txt = $options['empty_txt'];
		if (!$empty_txt) { $empty_txt = t('No entries found.'); }
		return "<p>$empty_txt</p>";
	}

	$access = sirius_access_instance('ledger');
	$is_admin = $access->access('admin');

	if ($options['display'] == 'csv') {
		$header = array(
			'account' => 'Account',
			'participant' => 'Participant',
			'phone' => 'Phone',
			'address' => 'Address',
			'city' => 'City',
			'state' => 'State',
			'zip' => 'Zip',
			'amount' => 'Amount',
		);
	} else {
		$header = array();
		$header['account'] = 'Account';
		$header += array(
			'participant' => 'Participant',
			'phone' => 'Phone',
			'address' => 'Address',
			'city' => 'City',
			'state' => 'State',
			'zip' => 'Zip',
			'amount' => array('data' => 'Amount'),
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
		if (!$filename) { $filename = 'balances.csv'; }
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, array_values($header));
	}

	$is_admin = user_access('sirius admin');

	$rows = array();

	foreach ($entries as $entry) {
		$account_nid = $entry['balance_account'];
		$currency_info = sirius_ledger_currency_info_from_account_nid($account_nid);

		$row = array();

		$tools = '';

		$worker_node = node_load($entry['worker_nid']);

		if ($header['account']) { $row[] = $entry['account_title']; }
		if ($header['amount']) {
			if ($options['display'] == 'csv') { $row[] = round(-1 * $entry['balance_amount'], 2); }
			else { $row[] = $currency_info['instance']->render_amt(-1 * $entry['balance_amount']); }
		}

		if ($header['phone']) { $row[] = $worker_node->field_sirius_phone['und'][0]['value']; }
		if ($header['address']) { $row[] = $worker_node->field_sirius_address['und'][0]['thoroughfare']; }
		if ($header['city']) { $row[] = $worker_node->field_sirius_address['und'][0]['locality']; }
		if ($header['state']) { $row[] = $worker_node->field_sirius_address['und'][0]['administrative_area']; }
		if ($header['zip']) { $row[] = $worker_node->field_sirius_address['und'][0]['postal_code']; }

		if ($header['participant']) {
			$row[] = $entry['participant_title'];
			if ($entry['worker_nid']) {
				$tools .= l('Worker', 'node/' . $entry['worker_nid'], array('attributes' => array('class' => array('tool-button'))));
			} else {
				$tools .= l('Participant', 'node/' . $entry['balance_participant'], array('attributes' => array('class' => array('tool-button'))));
			}
		}
		if ($header['tools']) { $row[] = $tools; }

		if ($options['display'] == 'csv') {
			fputcsv($fp, $row); 
		} else {
			$rows[] = $row;
		}
	}

	if ($options['display'] == 'csv') { 
		fclose($fp);
		exit();
	}

	$html .= theme_table(array('header' => array_values($header), 'rows' => $rows, 'attributes' => array('style' => 'table-layout: fixed;')));

	return $html;
}
