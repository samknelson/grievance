<?php

function sirius_ledger_node_tab_account_settings($account_node) {
	return drupal_get_form('sirius_ledger_node_tab_account_settings_form', $account_node);
}

function sirius_ledger_node_tab_account_settings_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$account_json = sirius_jsonfield_util_node_json_get($account_node);

	$form['#tree'] = TRUE;

	$form['settings']['payments']['allocate']['employer_intro'] = array('#markup' => '<h2>Payments from Employers</h2>');
	$form['settings']['payments']['allocate']['employer'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Allocate to A/R via:',
		'#options' => array(
			'simple' => t('Simple: one entry per check, credited to the payer'),
			'manual' => t('Manual: multiple entries per check, credited to workers'),
			'upload' => t('Upload: multiple entries per check, credited to workers'),
			'transmittal' => t('Transmittal form: Multiple entries per check including hours and withholding, credited to workers'),
		),
	);
	if ($account_json['settings']['payments']['allocate']['employer']) { $form['settings']['payments']['allocate']['employer']['#default_value'] = $account_json['settings']['payments']['allocate']['employer']; }

	$form['settings']['payments']['allocate']['employer_autoallocate'] = array(
		'#title' => 'Automatically Allocate?',
		'#type' => 'select',
		'#options' => array(
			NULL => '-- Select --',
			'Yes' => 'Yes', 
			'No' => 'No'
		),
		'#description' => 'Use this option when the payment should always be credited directly into the employer\'s own A/R account. This setting is usually appropriate for benefit funds, but NOT for dues.',
		'#default_value' => $account_json['settings']['payments']['allocate']['employer_autoallocate'],
		'#states' => array(
			'visible' => array(
				":input[name=\"settings[payments][allocate][employer][simple]\"]" => array('checked' => TRUE)
			),
		),
	);

	$form['settings']['payments']['allocate']['employer_autocharge'] = array(
		'#title' => 'Create offsetting A/R charges?',
		'#type' => 'select',
		'#options' => array(
			NULL => '-- Select --',
			'Yes' => 'Yes', 
			'No' => 'No'
		),
		'#description' => 'Use this option when the employer\'s remittances are the source of dues data. It is appropriate only when the payment is accompanied by a detailed remittance which is considered the authoritative source of dues data. This setting is NOT appropriate when dues are calculated in any other manner, such as via hours entries.',
		'#default_value' => $account_json['settings']['payments']['allocate']['employer_autocharge'],
		'#states' => array(
			'visible' => array(
				array(":input[name=\"settings[payments][allocate][employer][manual]\"]" => array('checked' => TRUE)),
				'or',
				array(":input[name=\"settings[payments][allocate][employer][upload]\"]" => array('checked' => TRUE)),
			),
		),
	);

	$form['settings']['payments']['allocate']['worker_intro'] = array('#markup' => '<h2>Payments from Workers</h2>');
	$form['settings']['payments']['allocate']['worker'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Allocate to A/R via:',
		'#options' => array(
			'simple' => t('Simple: one entry per check, credited to the payer'),
		),
	);
	if ($account_json['settings']['payments']['allocate']['worker']) { $form['settings']['payments']['allocate']['worker']['#default_value'] = $account_json['settings']['payments']['allocate']['worker']; }

	$form['settings']['payments']['allocate']['worker_autoallocate'] = array(
		'#title' => 'Automatically Allocate?',
		'#type' => 'select',
		'#options' => array(
			NULL => '-- Select --',
			'Yes' => 'Yes', 
			'No' => 'No'
		),
		'#description' => 'Use this option when the payment should always be credited directly into the worker\'s own A/R account. This setting is usually appropriate unless worker payments need to be allocated manually or credited to other worker\'s accounts.',
		'#default_value' => $account_json['settings']['payments']['allocate']['worker_autoallocate'],
		'#states' => array(
			'visible' => array(
				":input[name=\"settings[payments][allocate][worker][simple]\"]" => array('checked' => TRUE)
			),
		),
	);

	$form['settings']['aliases']['intro'] = array('#markup' => '<h2>Aliases</h2>');
	if ($account_json['settings']['aliases']) {
		$aliases = array_values(array_filter(array_values($account_json['settings']['aliases'])));
	} else {
		$aliases = array();
	}

	$count = count($aliases) + 2;
	for ($i=0; $i<$count; ++$i) {
		$form['settings']['aliases'][$i] = array(
			'#type' => 'textfield',
			'#title' => "Alias #" . ($i+1),
			'#default_value' => $aliases[$i],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_ledger_node_tab_account_settings_form_submit($form, &$form_state) {
	$account_node = node_load($form_state['account_nid']);
	$account_json = sirius_jsonfield_util_node_json_get($account_node);
	$account_json['settings']['payments'] = $form_state['values']['settings']['payments'];
	$account_json['settings']['aliases'] = $form_state['values']['settings']['aliases'];
	sirius_jsonfield_util_node_json_set($account_node, $account_json);
	node_save($account_node);
	drupal_set_message("Settings saved");
}

