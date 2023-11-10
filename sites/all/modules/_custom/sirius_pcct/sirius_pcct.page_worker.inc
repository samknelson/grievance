<?php

function sirius_pcct_node_tab_worker($worker_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$html .= sirius_fakerender_field(t('Phone:'), $worker_node->field_sirius_phone['und'][0]['value']);
	$html .= sirius_fakerender_field(t('Email:'), $worker_node->field_sirius_email['und'][0]['value']);
	$html .= sirius_fakerender_field(t('Member ID:'), $worker_node->field_sirius_id['und'][0]['value']);
	$html .= sirius_fakerender_field(t('Member Token:'), $worker_node->field_sirius_id2['und'][0]['value']);

	$result = sirius_pcct_member_lookup_by_worker($worker_node);

	$html .= '<div class="sirius_callout_basic">';
	$html .= '<h2>';
	$html .= t('Member Lookup');
	$html .= '</h2>';

	if ($result['success']) {
		$html .= '<p><em>' . t('Member lookup succeeded.') . '</em></p>';
 		$html .= sirius_fakerender_field(t('OOP:'), '$' . number_format($result['oop']));
 		$html .= sirius_fakerender_field(t('Deductible:'), '$' . number_format($result['deductible']));
	} else {
		$html .= '<p><em>' . t('Client lookup failed: ') . '<strong>' . $result['msg'] . '</strong></em></p>';
	}
	$html .= '</div>';

	return $html;
}