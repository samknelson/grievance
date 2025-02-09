<?php

function sirius_teamsters631_node_tab_log_m12_cancel_layoffs_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_cancel_layoffs_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['m12_nid'] = $m12_node->nid;
	$m12_json = sirius_json_get($m12_node);

	$form['worker_nids'] = $m12->layoff_render($m12_node, 'tableselect');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Cancel the selected ineligible requests'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_cancel_layoffs_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$m12_node = node_load($form_state['m12_nid']);
	$action = $form_state['values']['action'];

	$tmp = $form_state['values']['worker_nids'];
	if (!$tmp) { $tmp = array(); }
	$worker_nids = array_filter(array_values($tmp));

	if (!$worker_nids) {
		drupal_set_message("No workers selected.", 'warning');
		return;
	}

	foreach ($worker_nids as $worker_nid) {
		$m12->layoff_cancel($m12_node, $worker_nid);
		drupal_set_message("Canceled: " . sirius_node_title($worker_nid));
	}
}
