<?php

function sirius_twilio_node_tab_recording_access($recording_node) {
	$category = $recording_node->field_sirius_category['und'][0]['value'];
	if ($category != 'twilio:recording') { return FALSE; }

	return TRUE;
}

function sirius_twilio_node_tab_recording($recording_node) {
	sirius_backlink_add('sirius/twilio/recordings', array('name' => 'All Recordings'));

	$json = sirius_json_get($recording_node);
	$recording = $json['recording'];
	$call = $json['call'];

  $html_id_call = 'sirius_jqtree_' . $recording_node->nid . '_call';
  $html_id_recording = 'sirius_jqtree_' . $recording_node->nid . '_recording';

  $call_node = sirius_log_handler($recording_node, 'sirius_log');
  $contact_node = sirius_log_handler($recording_node, 'sirius_contact');
  $worker_node = sirius_log_handler($recording_node, 'sirius_worker');

	$html = '<h2>' . t('Recording') . '</h2>';
	$html .= sirius_fakerender_field('SID:', $recording['sid']);
	$ts = strtotime($recording['startTime']['date'] . ' GMT');
	$datestr = date('Y-m-d h:i:s a', $ts);
	$datestr .= ' (' . format_interval(time() - $ts) . ' ago' . ')';
	$html .= sirius_fakerender_field('Comments:', sirius_text_to_html($json['comments']));
	$html .= sirius_fakerender_field('Date:', $datestr);
	$html .= sirius_fakerender_field('Duration:', format_interval($recording['duration']));
	$html .= sirius_recording_render_player_local($recording_node);

	$html .= '<h2>' . t('Call') . '</h2>';
	$html .= sirius_fakerender_field('SID:', $call['sid']);
	$html .= sirius_fakerender_field('From:', sirius_phone_format($call['from']));
	$html .= sirius_fakerender_field('To:', sirius_phone_format($call['to']));
	$ts = strtotime($recording['startTime']['date'] . ' GMT');
	$datestr = date('Y-m-d h:i:s a', $ts);
	$datestr .= ' (' . format_interval(time() - $ts) . ' ago' . ')';
	$html .= sirius_fakerender_field('Date:', $datestr);

	$html .= sirius_jsonfield_log_render($call_node, array('skip_user' => TRUE));

	return $html;
}