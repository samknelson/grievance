<?php

function sirius_ledger_node_tab_policy_definition_rate_quicknav($policy_node) {
	if (!sirius_domain_component_enabled('trust')) { return ''; }
	$election_types = sirius_trust_policy_election_types($policy_node);
	if (!$election_types) { return ''; }

	$item = menu_get_item();
	$href = $item['href'];

	$links = array();
	$links[] = array('path' => $href, 'title' => 'Default');

	foreach ($election_types as $election_type_tid => $election_type_name) {
		$links[] = array('path' => "$href/$election_type_tid", 'title' => $election_type_name);
	}

	return sirius_quicknav($links);
}

function sirius_ledger_node_tab_policy_definition_rates_access($policy_node) {
	return sirius_ledger_node_tab_policy_definition_access($policy_node);
}

function sirius_ledger_node_tab_policy_definition_rates($policy_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_definition_rates_form', $policy_node);
}

function sirius_ledger_node_tab_policy_definition_rates_form($form, &$form_state, $policy_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$json = sirius_jsonfield_util_node_json_get($policy_node);

	$account_options = sirius_ledger_account_list();
	$ms_level_options = sirius_ledger_ms_level_options();
	$schedule_options = sirius_ledger_policy_schedule_options();
	$schedules = sirius_ledger_policy_schedules($json);
	$trigger_options = sirius_ledger_policy_trigger_options();

	$election_type_tid = sirius_node_tab_arg(0);
	$form_state['election_type_tid'] = $election_type_tid;

	if (!$schedules) { return sirius_form_error("Please select at least one schedule on the settings page."); }
	$form['quicknav'] = array('#markup' => sirius_ledger_node_tab_policy_definition_rate_quicknav($policy_node));

	$form['#tree'] = TRUE;

	if (!$election_type_tid) { $table_open_html = '<table><tr><th rowspan="2">Member Status</th>'; }
	else { $table_open_html = '<table><tr><th>Member Status</th>'; }
	
	foreach ($account_options as $account_nid => $account_name) {
		if ($election_type_tid && $account_nid == 'pay') { continue; }

		if ($election_type_tid) { $table_open_html .= '<th>' . $account_name . '</th>'; }
		else { $table_open_html .= '<th colspan="2">' . $account_name . '</th>'; }
	}
	$table_open_html .= '</tr>';

	if (!$election_type_tid) {
		$table_open_html .= '<tr>';
		foreach ($account_options as $account_nid => $account_name) {
			if ($account_nid == 'pay') {
				$table_open_html .= '<th>' . t('Regular') . '</th>';
				$table_open_html .= '<th>' . t('Skilled') . '</th>';
			} else {
				$table_open_html .= '<th>' . t('Rate') . '</th>';
				$table_open_html .= '<th>' . t('Triggers') . '</th>';
			}
		}
		$table_open_html .= '</tr>';
	}

	foreach ($schedules as $schedule_key => $schedule_name) {
		$form['ledger']['policy']['rates'][$schedule_key]['intro'] = array('#markup' => '<h2>' . t('Rates: ') . $schedule_name . '</h2>');

		$form['ledger']['policy']['rates'][$schedule_key]['table'] = array('#markup' => $table_open_html);
		foreach ($ms_level_options as $ms_level_code => $ms_level_name) {
			$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code]['tr'] = array('#markup' => '<tr><td>' . $ms_level_name . '</td>');
			foreach ($account_options as $account_nid => $account_name) {
				if ($election_type_tid && $account_nid == 'pay') { continue; }

				if ($election_type_tid) {
					$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['election_rates'][$election_type_tid]['rate'] = array(
						'#type' => 'textfield',
						'#size' => 5,
						'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['election_rates'][$election_type_tid]['rate'],
						'#prefix' => '<td>',
						'#suffix' => '</td>',
					);
				} else {
					$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate'] = array(
						'#type' => 'textfield',
						'#size' => 5,
						'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate'],
						'#prefix' => '<td>',
						'#suffix' => '</td>',
					);
				}

				if ($account_nid == 'pay') {
					$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate_skilled'] = array(
						'#type' => 'textfield',
						'#size' => 5,
						'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['rate_skilled'],
						'#prefix' => '<td>',
						'#suffix' => '</td>',
					);
				}

				if ($account_nid != 'pay' && !$election_type_tid) {
					$form['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['triggers'] = array(
						'#type' => 'checkboxes',
						//'#default_value' => $json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['triggers'],
						'#options' => sirius_ledger_policy_trigger_options($schedule_key),
						'#prefix' => '<td>',
						'#suffix' => '</td>',
						'#default_value' => sirius_form_array_default_value($json['ledger']['policy']['rates'][$schedule_key][$ms_level_code][$account_nid]['triggers']),
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
	$policy_node = node_load($form_state['policy_nid']);
	$json = sirius_jsonfield_util_node_json_get($policy_node);
	_sirius_ledger_node_tab_policy_definition_rates_merge($json['ledger']['policy']['rates'], $form_state['values']['ledger']['policy']['rates']);
	sirius_jsonfield_util_node_json_set($policy_node, $json);
	node_save($policy_node);
	drupal_set_message("Policy updated.");
}

function _sirius_ledger_node_tab_policy_definition_rates_merge(&$json1, $json2) {
	foreach ($json2 as $key => $value) {
		if (!is_array($value)) {
			$json1[$key] = $json2[$key];
		} else {
			_sirius_ledger_node_tab_policy_definition_rates_merge($json1[$key], $json2[$key]);
		}
	}
}