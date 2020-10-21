<?php

function sirius_ledger_node_tab_policy_definition_access($definition_node) {
	return $definition_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy';
}

function sirius_ledger_node_tab_policy_definition($definition_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_definition_form', $definition_node);
}

function sirius_ledger_ms_level_options() {
	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$ms_level_options = array();
	foreach ($ms_options as $ms_tid => $ms_name) {
		$levels = sirius_training_levels_fetch($ms_tid);
		$ms_level_options[$ms_tid] = $ms_name;
		foreach ($levels as $level_code => $level) {
			$ms_level_options["$ms_tid|$level_code"] = $ms_name . ': ' . $level['title'];
		}
	}
	return $ms_level_options;
}

function sirius_ledger_node_tab_policy_definition_form($form, &$form_state, $definition_node) {
	$form_state['definition_nid'] = $definition_node->nid;

	$json = sirius_jsonfield_util_node_json_get($definition_node);

	$form['#tree'] = TRUE;

	$form['ledger']['policy']['settings']['schedule'] = array(
		'#title' => t('Schedule'),
		'#type' => 'checkboxes',
		'#options' => sirius_ledger_policy_schedule_options(),
	);
	if ($json['ledger']['policy']['settings']['schedule']) { $form['ledger']['policy']['settings']['schedule']['#default_value'] = $json['ledger']['policy']['settings']['schedule']; }

	$account_options = array('pay' => t('Pay Rate'));
	$account_options += sirius_ledger_account_list();
	$form['ledger']['policy']['settings']['accounts'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Accounts'),
		'#options' => $account_options,
	);
	if ($json['ledger']['policy']['settings']['accounts']) { $form['ledger']['policy']['settings']['accounts']['#default_value'] = $json['ledger']['policy']['settings']['accounts']; }

	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$form['ledger']['policy']['settings']['ws'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Work Status'),
		'#options' => $ws_options,
	);
	if ($json['ledger']['policy']['settings']['ws']) { $form['ledger']['policy']['settings']['ws']['#default_value'] = $json['ledger']['policy']['settings']['ws']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_policy_definition_form_submit($form, $form_state) {
	$definition_node = node_load($form_state['definition_nid']);
	$json = sirius_jsonfield_util_node_json_get($definition_node);
	$json['ledger']['policy']['settings'] = $form_state['values']['ledger']['policy']['settings'];
	sirius_jsonfield_util_node_json_set($definition_node, $json);
	node_save($definition_node);
	drupal_set_message("Policy updated.");
}


function sirius_ledger_node_tab_policy_definition_exim_access($definition_node) { return sirius_ledger_node_tab_policy_definition_access($definition_node); }

function sirius_ledger_node_tab_policy_definition_exim($definition_node) { return drupal_get_form('sirius_json_definition_exim_form', $definition_node->nid, 'ALL'); }


function sirius_ledger_policy_schedule_options() {
	return array(
		'month' => 'Monthly',
		'semimonth' => "Semi-Monthly",
		'hour' => 'Hourly',
	);
}

function sirius_ledger_policy_trigger_options($schedule_key) {
	if ($schedule_key == 'hour') {
		return array(
			'hour' => 'On Hour',
		);
	} else {
		return array(
			'manual' => 'Manual',
			'cron' => 'Auto',
			'payment' => 'Payment'
		);
	}
}

function sirius_ledger_node_tab_policy_definition_rates_access($definition_node) {
	return sirius_ledger_node_tab_policy_definition_access($definition_node);
}

function sirius_ledger_node_tab_policy_definition_rates($definition_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_definition_rates_form', $definition_node);
}

function sirius_ledger_node_tab_policy_definition_rates_form($form, &$form_state, $definition_node) {
	$form_state['definition_nid'] = $definition_node->nid;
	$json = sirius_jsonfield_util_node_json_get($definition_node);

	$account_options = sirius_ledger_policy_accounts($json);
	$ms_level_options = sirius_ledger_ms_level_options();
	$schedule_options = sirius_ledger_policy_schedule_options();
	$schedules = sirius_ledger_policy_schedules($json);

	if (!$schedules) {
		drupal_set_message("Please select at least one schedule on the settings page.", 'warning');
		return array();
	}

	$form['#tree'] = TRUE;
	$table_open_html = '<table><tr><th rowspan="2">Member Status</th>';
	foreach ($account_options as $account_nid => $account_name) {
		if ($account_nid == 'pay') {
			$table_open_html .= '<th>' . $account_name . '</th>';
		} else {
			$table_open_html .= '<th colspan="2">' . $account_name . '</th>';
		}
	}
	$table_open_html .= '</tr>';
	$table_open_html .= '<tr>';
	foreach ($account_options as $account_nid => $account_name) {
		if ($account_nid == 'pay') {
			$table_open_html .= '<th>Rate</th>';
		} else {
			$table_open_html .= '<th>Rate</th><th>Triggers</th>';
		}
	}
	$table_open_html .= '</tr>';

	foreach ($schedules as $schedule_key => $schedule_name) {
		$form['ledger']['policy']['rates'][$schedule_key]['intro'] = array('#markup' => '<h2>' . t('Rates: ') . $schedule_name . '</h2>');

		$form['ledger']['policy']['rates'][$schedule_key]['table'] = array('#markup' => $table_open_html);
		foreach ($ms_level_options as $ms_level_code => $ms_level_name) {
			$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code]['tr'] = array('#markup' => '<tr><td>' . $ms_level_name . '</td>');
			foreach ($account_options as $account_nid => $account_name) {
				$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate'] = array(
					'#type' => 'textfield',
					'#size' => 5,
					'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate'],
					'#prefix' => '<td>',
					'#suffix' => '</td>',
				);

				if ($account_nid != 'pay') {
					$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['triggers'] = array(
						'#type' => 'checkboxes',
						'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['triggers'],
						'#options' => sirius_ledger_policy_trigger_options($schedule_key),
						'#prefix' => '<td>',
						'#suffix' => '</td>',
					);
				}
			}
			$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code]['trc'] = array('#markup' => '</tr>');
		}
		$form['ledger']['policy']['rates'][$schedule_key]['tablec'] = array('#markup' => '</table>');
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_policy_definition_rates_form_submit($form, $form_state) {
	$definition_node = node_load($form_state['definition_nid']);
	$json = sirius_jsonfield_util_node_json_get($definition_node);
	$json['ledger']['policy']['rates'] = $form_state['values']['ledger']['policy']['rates'];
	sirius_jsonfield_util_node_json_set($definition_node, $json);
	node_save($definition_node);
	drupal_set_message("Policy updated.");
}

function sirius_ledger_policy_accounts($policy_json) {
	$account_options = array('pay' => t('Pay Rate'));
	$account_options += sirius_ledger_account_list();

	if (!$policy_json['ledger']['policy']['settings']['accounts']) { return array(); }
	$nids = array_filter(array_values($policy_json['ledger']['policy']['settings']['accounts']));
	$result = array();
	foreach ($nids as $nid) { $result[$nid] = $account_options[$nid]; }
	return $result;
}

function sirius_ledger_policy_schedules($policy_json) {
	$schedule_options = sirius_ledger_policy_schedule_options();

	if (!$policy_json['ledger']['policy']['settings']['schedule']) { return array(); }
	$keys = array_filter(array_values($policy_json['ledger']['policy']['settings']['schedule']));
	$result = array();
	foreach ($keys as $key) { $result[$key] = $schedule_options[$key]; }
	return $result;
}


function sirius_ledger_node_tab_policy_definition_monthly_access($definition_node) {
	return sirius_ledger_node_tab_policy_definition_access($definition_node);
}

function sirius_ledger_node_tab_policy_definition_monthly($definition_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_definition_monthly_form', $definition_node);
}

function sirius_ledger_node_tab_policy_definition_monthly_form($form, &$form_state, $definition_node) {
	$form_state['definition_nid'] = $definition_node->nid;

	$json = sirius_jsonfield_util_node_json_get($definition_node);

	$form['#tree'] = TRUE;

	$form['ledger']['policy']['monthly']['monthly_charge_day'] = array(
		'#type' => 'textfield',
		'#title' => t('Monthly Charge Day'),
		'#size' => 2,
		'#description' => t('For monthly charges, on which day of the month should the charge be entered?')
	);
	if ($json['ledger']['policy']['monthly']['monthly_charge_day']) { $form['ledger']['policy']['monthly']['monthly_charge_day']['#default_value'] = $json['ledger']['policy']['monthly']['monthly_charge_day']; }

	$form['ledger']['policy']['monthly']['semimonthly_charge_day_1'] = array(
		'#type' => 'textfield',
		'#title' => t('Semi-Monthly Charge Day 1'),
		'#size' => 2,
		'#description' => t('For sem-monthly charges, on which day of the month should the first charge be entered?')
	);
	if ($json['ledger']['policy']['monthly']['semimonthly_charge_day_1']) { $form['ledger']['policy']['monthly']['semimonthly_charge_day_1']['#default_value'] = $json['ledger']['policy']['monthly']['semimonthly_charge_day_1']; }

	$form['ledger']['policy']['monthly']['semimonthly_charge_day_2'] = array(
		'#type' => 'textfield',
		'#title' => t('Semi-Monthly Charge Day 2'),
		'#size' => 2,
		'#description' => t('For monthly charges, on which day of the month should the second charge be entered?')
	);
	if ($json['ledger']['policy']['monthly']['semimonthly_charge_day_2']) { $form['ledger']['policy']['monthly']['semimonthly_charge_day_2']['#default_value'] = $json['ledger']['policy']['monthly']['semimonthly_charge_day_2']; }

	$form['ledger']['policy']['monthly']['skip_months'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Omit monthly charges on the following months'),
		'#options' => array(
			1 => t('January'),
			2 => t('February'),
			3 => t('March'),
			4 => t('April'),
			5 => t('May'),
			6 => t('June'),
			7 => t('July'),
			8 => t('August'),
			9 => t('September'),
			10 => t('October'),
			11 => t('November'),
			12 => t('December'),
		),
	);
	if ($json['ledger']['policy']['monthly']['skip_months']) { $form['ledger']['policy']['monthly']['skip_months']['#default_value'] = $json['ledger']['policy']['monthly']['skip_months']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_policy_definition_monthly_form_submit($form, $form_state) {
	$definition_node = node_load($form_state['definition_nid']);
	$json = sirius_jsonfield_util_node_json_get($definition_node);
	$json['ledger']['policy']['monthly'] = $form_state['values']['ledger']['policy']['monthly'];
	sirius_jsonfield_util_node_json_set($definition_node, $json);
	node_save($definition_node);
	drupal_set_message("Policy updated.");
}
