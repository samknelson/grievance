<?php
/*

function sirius_node_tab_dispatch_payrate($dispatch_node) {
	return 'temporarily disabled';
	// return drupal_get_form('sirius_node_tab_dispatch_payrate_form', $dispatch_node);
}

function sirius_node_tab_dispatch_payrate_form($form, &$form_state, $dispatch_node) {
	$form_state['dispatch_nid'] = $dispatch_node->nid;

	$form['action'] = array(
		'#type' => 'select', 
		'#title' => t('Action'),
		'#options' => array(
			'explain' => t('Explain what the payrate should be.'),
			'recompute' => t('Explain and recompute the payrate.'),
		),
		'#required' => TRUE, 
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit'),
	);

	$log = sirius_minilog_render_html();
	if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_node_tab_dispatch_payrate_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$dispatch_node = node_load($form_state['dispatch_nid']);

	$old_payrate = $dispatch_node->field_sirius_payrate['und'][0]['value'];
	$result = sirius_dispatch_payrate_compute($dispatch_node, TRUE);
	sirius_minilog($result['msg']);

	++$count;
	if ($old_payrate != $dispatch_node->field_sirius_payrate['und'][0]['value']) { 
		drupal_set_message("The recomputed payrate does not match what appears in the dispatch.");
		if ($form_state['values']['action'] == 'recompute') {
			node_save($dispatch_node);
			sirius_log('dispatch:payrate', 'Recompute', "Payrate recomputed from dispatch tab.", '', '', array($dispatch_node->nid));
			drupal_set_message("New payrate saved.");
		}
	} else {
		drupal_set_message("Payrate unchanged.");
	}
}
*/