<?php

function sirius_teamsters631_node_tab_log_m12_followup_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_followup_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['m12_nid'] = $m12_node->nid;
	$m12_json = sirius_json_get($m12_node);

	$form['worker_nids'] = array('#markup' => $m12->layoff_render($m12_node, 'html'));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Check whether all workers were released'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_followup_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$m12_node = node_load($form_state['m12_nid']);
	$m12->checked_set($m12_node);
}
