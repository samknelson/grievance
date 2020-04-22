<?php

function sirius_sql_pretty($sql, $sql_args = array()) {
	require_once DRUPAL_ROOT . '/sites/all/libraries/sql_formatter/lib/SqlFormatter.php';

	$html = SqlFormatter::format($sql);
	if ($sql_args) {
		$html .= '<hr><pre style="color: black; background-color: white;">' . print_r($sql_args, 1) . '</pre>';
	}

	return $html;
}

function sirius_sql_query($sql, $sql_args = array()) {
	try {
		$stmt = db_query($sql, $sql_args);
	} catch (exception $e) {
		drupal_set_message("There was a sql error running your query. The details are below.", 'error');
		drupal_set_message(sirius_sql_pretty($sql, $sql_args));
		drupal_set_message($e, 'warning');
	}
	return $stmt;
}