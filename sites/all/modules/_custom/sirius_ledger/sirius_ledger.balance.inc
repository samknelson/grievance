<?php

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
