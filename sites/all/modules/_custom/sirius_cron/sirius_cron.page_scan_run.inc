<?php

function sirius_cron_node_tab_scan_run_access($scan_node) {
	if (!sirius_cron_scan_key($scan_node)) { return FALSE; }
	return TRUE;
}

function sirius_cron_node_tab_scan_run($scan_node) {
	return drupal_get_form('sirius_cron_node_tab_scan_run_form', $scan_node);
}

function sirius_cron_node_tab_scan_run_form($form, &$form_state, $scan_node) {
	$form_state['scan_nid'] = $scan_node->nid;

	$form['#tree'] = TRUE;

	$form['mode'] = array(
		'#title' => t('Mode'),
		'#type' => 'select',
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
		'#required' => TRUE,
	);

	$form['force'] = array(
		'#title' => t('Force?'),
		'#type' => 'select',
		'#options' => array(
			FALSE => t('No, run on the normal schedule'),
			TRUE => t('Yes, run even if it is too soon'),
		),
		'#required' => TRUE,
	);

	$scan_key = sirius_cron_scan_key($scan_node);
	$info = sirius_cron_scan_info($scan_key);
	if ($info['options_form']) {
		$form['options'] = array(
			'#type' => 'fieldset',
			'#title' => t('Options'),
		);

		$form['options'] += $info['options_form'];
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) {
  	$form['results'] = array('#markup' => '<hr>' . $log);

  	// We log to the node, but then clear the minilog, so ... reload the node.
  	$scan_node = node_load($scan_node->nid);
  	$scan_json = sirius_json_get($scan_node);
  	$log2 = $scan_json['cron']['log'];
  	$form['results2'] = array('#markup' => '<hr>' . $log2);
  }

  return $form;
}

function sirius_cron_node_tab_scan_run_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$scan_node = node_load($form_state['scan_nid']);
	$scan_key = sirius_cron_scan_key($scan_node);
	if (!$scan_key) {
		drupal_set_message("No scan key found.", 'error');
		return;
	}

	$options = $form_state['values']['options'];

	$mode = $form_state['values']['mode'];
	$force = $form_state['values']['force'];
	$result = sirius_cron_scan($scan_key, $force, $mode, $options);
	if ($result['success']) {
		drupal_set_message("Scan run complete: " . $result['msg']);
	} else {
		drupal_set_message("Scan run failed: " . $result['msg'], 'warning');
	}
	drupal_set_message("See the \"info\" tab for full results.");
}
