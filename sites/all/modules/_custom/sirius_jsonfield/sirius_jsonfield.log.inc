<?php

/**
 * Adds an entry to the JSON for the node. Does not save the node.
 */

function sirius_jsonfield_log($node, $type, $msg, $extra = array()) {
	$json = sirius_json_get($node);
	sirius_jsonfield_log_impl($json, $type, $msg, $extra);
	sirius_json_set($node, $json);
}

function sirius_jsonfield_log_impl(&$json, $type, $msg, $extra = array()) {
	global $user;
	$ts = time();

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
}

function sirius_jsonfield_node_tab_entity_sublog_access($node) {
	$json = sirius_jsonfield_util_node_json_get($node);
	if (!$json['log']) { return FALSE; }
	return TRUE;
}

function sirius_jsonfield_log_render($node, $options = array()) {
	return sirius_jsonfield_node_tab_entity_sublog($node, $options);
}

function sirius_jsonfield_node_tab_entity_sublog($node, $options = array()) {
	$json = sirius_jsonfield_util_node_json_get($node);
	$log = $json['log'];
	if (!$log) { return array('no log entries.'); }

	$header = array();
	$header[] = 'Date / Time';
	if (!$options['skip_user']) { $header[] = 'User'; }
	$header[] = 'Type';
	$header[] = 'Message';

	$rows = array();
	foreach ($log as $entry) {
		$account = user_load($entry['uid']);

		$row = array();
		$row[] = date('Y-m-d H:i:s', $entry['ts']);
		if (!$options['skip_user']) { $row[] = $account->name . ' [' . $account->mail . ']'; }
		$row[] = $entry['type'];
		$row[] = $entry['msg'];
		$rows[] = $row;
	}

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}