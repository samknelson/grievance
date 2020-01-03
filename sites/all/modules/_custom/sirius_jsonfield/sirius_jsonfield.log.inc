<?php

/**
 * Adds an entry to the JSON for the node. Does not save the node.
 */

function sirius_jsonfield_log($node, $type, $msg, $extra = array()) {
	global $user;
	$ts = time();
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);

	if ($json['log']) {
		$count = count($json['log']);
		$last_entry = &$json['log'][$count-1];
	}

	if ($last_entry['uid'] == $user->uid && $last_entry['type'] == $type && $last_entry['msg'] == $msg && !$extra) {
		$last_entry['ts'] = $ts;
	} else {
		$json['log'][] = array(
			'ts' => $ts,
			'uid' => $user->uid,
			'msg' => $msg,
			'type' => $type,
			'extra' => $extra
		);
	}
	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
}

function sirius_jsonfield_log_render($node) {
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$log = $json['log'];
	if (!$log) { return array('no log entries.'); }

	$header = array(
		'Date / Time',
		'User',
		'Type',
		'Message',
		'Extra'
	);

	$rows = array();
	foreach ($log as $entry) {
		$account = user_load($entry['uid']);

		$row = array();
		$row[] = date('Y-m-d H:i:s', $entry['ts']);
		$row[] = $account->name . ' [' . $account->mail . ']';
		$row[] = $entry['type'];
		$row[] = $entry['msg'];
		if ($row['extra']) {
			$row[] = '<pre>' . print_r($extra, 1) . '</pre>';
		} else {
			$row[] = '';
		}
		$rows[] = $row;
	}

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}