<?php

function sirius_smf_node_tab_worker_autotag_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['intro'] = array('#markup' => sirius_smf_autotag_status_render($worker_node));

	$form['asof_year'] = array(
		'#type' => 'textfield',
		'#title' => t('Year'),
		'#size' => 4,
	);

	$form['asof_month'] = array(
		'#type' => 'textfield',
		'#title' => t('Month'),
		'#size' => 2,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Run Autotags Now'),
	);

	return $form;
}

function sirius_smf_node_tab_worker_autotag_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$is_current = TRUE;

	$asof_year = $form_state['values']['asof_year'];
	$asof_month = $form_state['values']['asof_month'];
	if ($asof_month > 0 && $asof_year > 0) {
		$asof_ts = strtotime("$asof_year-$asof_month-01");
		$is_current = FALSE;
	} else {
		$asof_ts = NULL;
		$now_ts = time();
		$asof_month = date('n', $now_ts);
		$asof_year = date('Y', $now_ts);
	}

	SiriusResult::create(sirius_smf_autotag_worker($worker_node, $asof_ts, 'live'))->drupalSetMessage();

	if ($is_current) {
		drupal_set_message("Tags added to the worker.");
	} 
	$wym_node = sirius_smf_wym_fetch($worker_node->nid, $asof_year, $asof_month, FALSE);
	if ($wym_node) {
		drupal_set_message("Tags added to the worker year/month record: " . l($wym_node->title, 'node/' . $wym_node->nid));
	} 
}
