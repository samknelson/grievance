<?php

function sirius_bulk_page_oneclick_list() {
	$all = sirius_bulk_oneclick_all();

	if (!$all) {
		return t('No one-click messages have been configured.');
	}

	$html = '<ul>';
	foreach ($all as $key => $item) {
		$html .= '<li>';
		$html .= l($item['title'], current_path() . '/' . $key);
		$html .= '</li>';
	}
	$html .= '</ul>';

	return $html;
}

function sirius_bulk_page_oneclick($which) {
	$all = sirius_bulk_oneclick_all();
	$item = $all[$which];
	if (!$item) { return t('Unable to find one-click message @which.', array('@which' => $which)); }
	drupal_set_title(drupal_get_title() . ': ' . $item['title']);
	return drupal_get_form($item['form']);
}

