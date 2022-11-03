<?php

function sirius_trust_node_tab_worker_eligibility($worker_node) {
	return drupal_get_form('sirius_trust_node_tab_worker_eligibility_form', $worker_node);
}

function sirius_trust_node_tab_worker_eligibility_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

  $params = array();
	$params['active'] = 'Yes';
	$params['contact_nid'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
	$relationship_nodes = sirius_contact_relationships($params);
	if ($relationship_nodes) {
		$dependent_options = array();
		$dependent_options[NULL] = t('[self]');
		foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
			$relationship_type_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
			$relationship_type_term = taxonomy_term_load($relationship_type_tid);
			$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
			$dependent_worker_node = sirius_worker_get_by_contact_nid($dependent_contact_nid);
			$dependent_options[$relationship_nid] = '(' . $relationship_type_term->name . ' to) ' . $dependent_worker_node->title;
		}

		$form['relationship_nid'] = array(
			'#type' => 'select',
			'#title' => t('Dependent'),
			'#options' => $dependent_options,
		);
	}

	$policies = sirius_ledger_policy_list();
	$policy_options = array(NULL => t('-- Select --'));
	foreach ($policies as $policy_nid => $policy_node) { $policy_options[$policy_nid] = $policy_node->title; }
	$form['policy_nid'] = array(
	  '#title' => t('Policy'),
		'#type' => 'select',
		'#options' => $policy_options,
		'#required' => TRUE,
	);

	$form['date'] = array(
	  '#title' => t('As Of'),
		'#type' => 'date_popup',
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#required' => TRUE,
		'#date_format' => 'Y-m-d',
		'#default_value' => date('Y-m-d'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr><h2>' . t('Results') . '</h2>' . $form_state['results']);
	}

	return $form;
}

function sirius_trust_node_tab_worker_eligibility_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$worker_node = node_load($form_state['worker_nid']);

	$policy_nid = $form_state['values']['policy_nid'];
	$policy_node = node_load($policy_nid);

	$ts = strtotime($form_state['values']['date']);

	$relationship_node = node_load($form_state['values']['relationship_nid']);
	if ($relationship_node) {
		$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$dependent_worker_node = sirius_worker_get_by_contact_nid($dependent_contact_nid);
	} else {
		$dependent_worker_node = $worker_node;
	}

	$benefits = sirius_trust_benefits();
	$results = array();

	$rows = array();
	foreach ($benefits as $benefit_nid => $benefit_node) {
		$eligibility_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);

		if (!$eligibility_plugins) {
			$paths = array(
				"node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid" => 'Eligibility Settings',
			);
			$tools = array();
			foreach ($paths as $path => $label) {
				if (drupal_valid_path($path)) {
					$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}
			$class = 'ineligible';
			$row = array();
			$row[] = $worker_node->title;
			$row[] = $dependent_worker_node->title;
			$row[] = $benefit_node->title;
			$row[] = 'No';
			$row[] = 'There are no eligibility plugins for this benefit.';
			$row[] = join('', $tools);
			$rows[] = array('data' => $row, 'class' => array($class, 'summary'));
			continue;
		}

		$subrows = array();
		$summary_eligible = TRUE;
		$summary_failed = array();
		foreach ($eligibility_plugins as $plugin_key) {
			$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);

			$paths = array();
			if ($plugin_info['callbacks']['settings']) { $paths["node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid/$plugin_key"] = 'Settings'; }

			$tools = array();
			foreach ($paths as $path => $label) {
				if (drupal_valid_path($path)) {
					$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}

			$class = 'eligible';
			$result = sirius_trust_eligibility_plugin_check($policy_node, $benefit_node, $worker_node, $relationship_node, $ts, $plugin_key);

			$msg = $result['msg'];
			if ($result['success']) {
				if ($result['warning']) {
					$class = 'warning';
					$eligible = 'Warning';
					$summary_warning = TRUE;
				} else {
					$class = 'green';
					$eligible = 'Yes';
				}
			} else {
				$class = 'red';
				$eligible = 'No';
				$summary_eligible = FALSE;
				$summary_failed[] = $plugin_info['title'];
			}
			if ($result['tools']) { $tools = array_merge($tools, $result['tools']); }

			$row = array();
			$row[] = '';
			$row[] = '';
			$row[] = $plugin_info['title'];
			$row[] = $eligible;
			$row[] = $result['msg'];
			$row[] = join('', $tools);
			$subrows[] = array('data' => $row, 'class' => array($class));
		}

		if ($summary_eligible) {
			if ($summary_warning) {
				$class = 'warning';
				$eligible = 'Warning';
				$msg = 'The member or dependent is eligible for this benefit, but is at risk of losing eligibility.';
			} else {
				$class = 'green';
				$eligible = 'Yes';
				$msg = 'The member or dependent is eligible for this benefit.';
			}
		} else {
			$class = 'red';
			$eligible = 'No';
			$msg = 'Failed tests: ' . join(', ', $summary_failed);
		}

		$paths = array(
			"node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid" => 'Eligibility Settings',
		);
		$tools = array();
		foreach ($paths as $path => $label) {
			if (drupal_valid_path($path)) {
				$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}

		$row = array();
		$row[] = $worker_node->title;
		$row[] = $dependent_worker_node->title;
		$row[] = $benefit_node->title;
		$row[] = $eligible;
		$row[] = $msg;
		$row[] = join('', $tools);
		$rows[] = array('data' => $row, 'class' => array($class, 'summary'));

		foreach ($subrows as $row) { $rows[] = $row; }
	}

	$header = array(
		'Subscriber',
		'Dependent',
		'Benefit<br />Eligibility Test',
		'Result',
		'Notes',
		'Tools',
	);

	$form_state['results'] = sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}
