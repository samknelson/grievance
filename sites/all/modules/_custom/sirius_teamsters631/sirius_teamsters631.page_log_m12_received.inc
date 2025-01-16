<?php

function sirius_teamsters631_node_tab_log_m12_received_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_received_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$form_state['m12_nid'] = $m12_node->nid;

	$m12_json = sirius_json_get($m12_node);


	$form['action'] = array(
		'#title' => t('Action'),
		'#type' => 'select',
		'#options' => array(
			'received' => t('Set Received Date'),
			'clear' => t('Clear Received Date'),
		),
		'#default_value' => 'received',
	);

	$form['asof_date'] = array(
		'#type' => 'textfield',
		'#title' => t('As Of'),
    '#states' => array(
	    'visible' => array(
        array(':input[name="action"]' => array('value' => 'received')),
	    )
    ),
		'#description' => t('(Leave blank to search for current dispatches.)'),
	);
	if ($m12_json['teamsters631_m12']['received']['ts']) {
		$form['asof_date']['#default_value'] = date('m/d/Y h:i A', $m12_json['teamsters631_m12']['received']['ts']);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Go'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_received_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$m12_node = node_load($form_state['m12_nid']);

	$action = $form_state['values']['action'];

	if ($action == 'clear') {
		$m12->received_clear($m12_node);
	} else if ($action == 'received') {
		$asof_date = $form_state['values']['asof_date'];
		$m12->received_set($m12_node, $asof_date);
	}

	drupal_set_message("Settings saved.");
}