<?php

function sirius_teamsters631_node_tab_log_m12_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12($m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$m12_json = sirius_json_get($m12_node);

 	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$html .= sirius_fakerender_field("Dispatches:", $m12->dispatches_render($m12_node));
	$html .= sirius_fakerender_field("Received:", $m12->received_render($m12_node));
	$html .= sirius_fakerender_field('Layoffs:', $m12->layoff_render($m12_node, 'summary'));
	$html .= sirius_fakerender_field('Notification:', $m12->notify_render($m12_node));
	$html .= sirius_fakerender_field('Follow-up:', $m12->checked_render($m12_node));

	$html .= '<h2>' . t('Ineligible Workers') . '</h2>';
	$html .= $m12->layoff_render($m12_node);

	return $html;
}