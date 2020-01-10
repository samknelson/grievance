<?php

function sirius_node_tab_calllog_access($log_node) {
	$category = $log_node->field_sirius_category['und'][0]['value'];
	if ($category != 'twilio:call') { return FALSE; }

	$type = $log_node->field_sirius_type['und'][0]['value'];
	if ($type != 'initiated_incoming' && $type != 'initiated_outgoing') { return FALSE; }

	$call_sid = $log_node->field_sirius_notes['und'][0]['value'];
	if (!$call_sid) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_calllog($log_node) {
	$call_sid = $log_node->field_sirius_notes['und'][0]['value'];

	$json = json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
	$phone = $json['phone'];
	if ($phone) {
		$html .= '<h2>' . t('Phone number: ') . sirius_phone_format($phone) . '</h2>';
		$html .= l(t('Other calls for @phone', array('@phone' => sirius_phone_format($phone))), 'sirius/log/calls', array('query' => array('field_sirius_json_value' => $phone)));
	}

	$html .= '<h2>' . t('Full Entry') . '</h2>';
	$renderable = node_view($log_node);
	$html .= drupal_render($renderable);

	$html .= '<h2>Call Log</h2>';
	$html .= views_embed_view('sirius_log_calls_detail', 'block_1', $call_sid);

	return $html;
}