<?php

function sirius_hour_node_tab_payperiod_entries_reconcile($payperiod_node) {
	return drupal_get_form('sirius_hour_node_tab_payperiod_entries_reconcile_form', $payperiod_node);
}

function sirius_hour_node_tab_payperiod_entries_reconcile_form($form, &$form_state, $payperiod_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_hour') . '/css/hours.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius_hour') . '/js/entry_form.js', 'file');


	$form_state['payperiod_nid'] = $payperiod_node->nid;
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);

	$form['#tree'] = TRUE;

	$form['reconcile']['msg'] = array(
		'#type' => 'hidden',
		'#default_value' => $json['reconcile']['msg'],
	);

	$intro = 'When errors or warnings are generated for a payperiod, use the "Edit Entries" tab to reconcile them, and then set the status to "Reconciliation Complete".';
	if ($json['reconcile']['msg']) {
		$intro .= '<br /><div class="sirius_hour_warning">Warning: ' . $json['reconcile']['msg'] . '</div>';
	}
	$form['reconcile']['intro'] = array('#markup' => $intro);

	$form['reconcile']['status'] = array(
		'#type' => 'select',
		'#title' => 'Reconciliation Status',
		'#options' => array(
			NULL => t('Reconciliation Not Needed'),
			'warning' => t('Reconciliation Needed'),
			'reconciled' => t('Reconciliation Complete'),
		),
		'#default_value' => $json['reconcile']['status'],
	);

	$form['reconcile']['comment'] = array(
		'#type' => 'textarea',
		'#title' => 'Comment',
		'#default_value' => $json['reconcile']['comment'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['wrap_close'] = array('#markup' => '</div>');

	return $form;
}

function sirius_hour_node_tab_payperiod_entries_reconcile_form_submit($form, &$form_state) {
	$payperiod_nid = $form_state['payperiod_nid'];
	$payperiod_node = node_load($payperiod_nid);
	$payperiod_type = $payperiod_node->field_sirius_payperiod_type['und'][0]['value'];
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['reconcile'] = $form_state['values']['reconcile'];
	$payperiod_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($payperiod_node);
}
