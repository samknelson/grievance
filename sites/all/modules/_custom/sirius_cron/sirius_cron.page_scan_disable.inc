<?php

function sirius_cron_node_tab_scan_disable_access($scan_node) {
	if (!sirius_cron_scan_key($scan_node)) { return FALSE; }
	return TRUE;
}

function sirius_cron_node_tab_scan_disable_form($form, &$form_state, $scan_node) {
	$form_state['scan_nid'] = $scan_node->nid;
	$form['#tree'] = TRUE;
	$scan_json = sirius_json_get($scan_node);

	$form['cron']['disable'] = array(
		'#type' => 'select',
		'#title' => 'Disable this scan?',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $scan_json['cron']['disable'],
		'#description' => t('Cron scans are enabled or disabled for ALL DOMAINS. This is not a domain-specific setting.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  return $form;
}

function sirius_cron_node_tab_scan_disable_form_submit($form, &$form_state) {
	$scan_node = node_load($form_state['scan_nid']);
	$scan_json = sirius_json_get($scan_node);
	$scan_json['cron']['disable'] = $form_state['values']['cron']['disable'];
	sirius_json_set($scan_node, $scan_json);
	node_save($scan_node);
	drupal_set_message("Settings saved.");
}