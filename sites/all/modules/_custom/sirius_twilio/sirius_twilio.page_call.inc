<?php

function sirius_twilio_node_tab_call_access($call_node) {
	$category = $call_node->field_sirius_category['und'][0]['value'];
	if ($category != 'twilio:call') { return FALSE; }
	return TRUE;
}

function sirius_twilio_node_tab_call($call_node) {
	$call_json = sirius_json_get($call_node);
	if ($call_json['context']['operator_nid']) { sirius_backlink_add('node/' . $call_json['context']['operator_nid'] . '/sirius_operator_calls', array('name' => 'Incoming Calls')); }
	$recording = $call_json['recording'];
	$call = $call_json['context']['request'];
	$call_sid = $call_json['context']['request']['CallSid'];

	$html .= sirius_fakerender_field('SID:', $call_sid);
	$html .= sirius_fakerender_field('From:', sirius_phone_format($call['From']));
	$html .= sirius_fakerender_field('To:', sirius_phone_format($call['To']));
	$ts = $call_node->created;
	$datestr = date('Y-m-d h:i:s a', $ts);
	$datestr .= ' (' . format_interval(time() - $ts) . ' ago' . ')';
	$html .= sirius_fakerender_field('Date:', $datestr);

	$html .= sirius_jsonfield_log_render($call_node, array('skip_user' => TRUE));

	return $html;
}