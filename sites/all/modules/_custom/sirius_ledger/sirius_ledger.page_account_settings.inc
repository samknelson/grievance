<?php

function sirius_ledger_node_tab_account_settings_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$account_json = sirius_jsonfield_util_node_json_get($account_node);

	$form['#tree'] = TRUE;

	$form['settings']['ea']['participant'] = array(
		'#title' => t('Participant Type'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'worker' => t('Worker'),
			'employer' => t('Employer'),
		),
	);

	$form['settings']['ea']['invoices'] = array(
		'#title' => t('Invoices?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$form['settings']['ea']['access']['view']['roles'] = array(
		'#title' => t('Roles that can view their own account'),
		'#type' => 'checkboxes',
		'#options' => user_roles(TRUE),
		'#description' => t('Admin roles can view all accounts.'),
	);

	$payment_type_options = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
	if ($payment_type_options) {
		$form['settings']['payments']['types'] = array(
			'#title' => 'Available Payment Types',
			'#type' => 'checkboxes',
			'#options' => $payment_type_options,
		);
	}


	$form['settings']['payments']['allocate']['employer_intro'] = array('#markup' => '<h2>Payments from Employers</h2>');
	$form['settings']['payments']['allocate']['employer'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Allocate to A/R via:',
		'#options' => array(
			'simple' => t('Simple: one entry per check, credited to the payer'),
			'manual' => t('Manual: multiple entries per check, credited to workers'),
			'upload' => t('Upload: multiple entries per check, credited to workers'),
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
		);
	}

	sirius_jsonfield_util_set_defaults($form, $account_json);

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
	$account_json['settings']['ea'] = $form_state['values']['settings']['ea'];
	sirius_jsonfield_util_node_json_set($account_node, $account_json);
	node_save($account_node);
	drupal_set_message("Settings saved");
}

