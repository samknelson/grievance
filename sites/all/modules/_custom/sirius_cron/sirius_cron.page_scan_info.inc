<?php

function sirius_cron_node_tab_scan_info_access($scan_node) {
	if (!sirius_cron_scan_key($scan_node)) { return FALSE; }
	return TRUE;
}

function sirius_cron_node_tab_scan_info($scan_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/minilog.css');

	$scan_json = sirius_json_get($scan_node);

	$scan_key = sirius_cron_scan_key($scan_node);
	$info = sirius_cron_scan_info($scan_key);

	$html = '<h2>' . t('Type of Scan') . '</h2>';
	$html .= sirius_fakerender_field('Title:', $info['title']);
	$html .= sirius_fakerender_field('Schedule:', $info['schedule']);
	if ($info['description']) { $html .= '<p>' . $info['description'] . '</p>'; }

	$html .= '<h2>' . t('Last Run') . '</h2>';
	$html .= sirius_fakerender_field('Start:', $scan_json['cron']['start_render']);
	$html .= sirius_fakerender_field('End:', $scan_json['cron']['end_render']);
	$html .= sirius_fakerender_field('Duration:', $scan_json['cron']['duration_render']);
	$html .= sirius_fakerender_field('Message:', $scan_json['cron']['msg']);
	$html .= $scan_json['cron']['log'];

	return $html;
}