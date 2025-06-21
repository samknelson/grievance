<?php

function sirius_trust_node_tab_worker_benefit_scan_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form_state['#tree'] = TRUE;

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => 'Test mode - no changes will be saved',
			'live' => 'Live mode - benefits may be started or stopped',
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$mode = $form_state['values']['mode'];
		sirius_trust_wb_scan_worker($mode, $worker_node);

		$html = '<hr>';
		$html .= '<h2>' . t('Results') . '</h2>';
		$html .= sirius_minilog_render_html();
		$form['results'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_trust_node_tab_worker_benefit_scan_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);
}
