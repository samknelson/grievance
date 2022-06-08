<?php

function sirius_cron_page_scan_list() {
	$info_all = sirius_cron_scan_info_all();

	$header = array();
	$header[] = t('Title');
	$header[] = t('Last Run');
	$header[] = t('Duration');
	$header[] = t('Message');
	$header[] = t('Tools');

	$rows = array();
	foreach ($info_all as $key => $info) {
		$scan_node = sirius_cron_scan_get($key, TRUE);
		$scan_json = sirius_json_get($scan_node);

		$tools = array();
		$tools[] = l('Info', 'node/' . $scan_node->nid . '/sirius_cron_scan_info', array('attributes' => array('class' => array('tool-button'))));
		$tools[] = l('Run', 'node/' . $scan_node->nid . '/sirius_cron_scan_run', array('attributes' => array('class' => array('tool-button'))));

		$row = array();
		$row[] = sirius_tooltip_render($info['description'], $info['title']) . '<br>' . $key;
		$row[] = $scan_json['cron']['start_render'];
		$row[] = $scan_json['cron']['duration_render'];
		$row[] = $scan_json['cron']['msg'];
		$row[] = join('', $tools);
		$rows[] = $row;
	}

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}