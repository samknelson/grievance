<?php

function sirius_trust_election_wizard_benefits_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$employer_node = sirius_trust_election_wizard_get_employer($data);
	$policy_nid = $policy_node->nid;
	$policy_json = sirius_json_get($policy_node);
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	$benefits = sirius_trust_policy_benefits_offered($policy_node, array('electable' => TRUE, 'worker_node' => $worker_node));
	// $benefits = array();

	$form['prefix'] = array('#markup' => $policy_json['elections']['benefits']['prefix']);

	$access = Sirius_Trust_Access::getInstance();
	$access_context = array('election_wizard_node' => $wizard_node, 'current_step' => $current_step);

	// Make a list of benefits that I am eligible for, broken out by type
	$benefit_options_by_type = array();
	$eligibility_results_by_benefit = array();

	$params = array();
	$params['policy_node'] = $policy_node;
	$params['subscriber_worker_node'] = $worker_node;
	$params['relationship_node'] = NULL;
	$params['ts'] = time();
	$params['shortcut'] = FALSE;
	$params['scan_type'] = 'benefit_start';
	$params['employer_node'] = $employer_node;
	$params['skip_plugins'] = array('cardcheck', 'election');
	$params['from_benefit_nids'] = array_keys($benefits);
	if ($data['addr']['change'] == 'Yes') { $params['addr'] = $data['addr']['new']; }
	$params['is_election'] = TRUE;

	$automatic_benefits = node_load_multiple(array_keys(sirius_trust_policy_benefits_offered($policy_node, array('electable' => FALSE, 'worker_node' => $worker_node))));
	$all_benefits = $benefits + $automatic_benefits;

	foreach ($all_benefits as $benefit_nid => $benefit_node) {
		$params['benefit_node'] = $benefit_node;
		$eligibility_results_by_benefit[$benefit_nid] = sirius_trust_eligibility_plugin_check_all($params);
	}

	foreach ($benefits as $benefit_nid => $benefit_node) {
		if (!$eligibility_results_by_benefit[$benefit_nid]['success']) { continue; }
		$benefit_type_tid = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
		$benefit_options_by_type[$benefit_type_tid][$benefit_nid] = $benefit_node->title;
	}

	// Sort by the benefit type order. There's probably a way to write cleaner code.
	$benefit_types = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'name');
	$benefit_type_weights = array();
	$weight = 0;
	foreach ($benefit_types as $tid => $name) { $benefit_type_weights[$tid] = $weight++; }
	uksort($benefit_options_by_type, function($a, $b) use ($benefit_type_weights) {
		return $benefit_type_weights[$a] <=> $benefit_type_weights[$b];
	});


	foreach ($benefit_options_by_type as $benefit_type_tid => $benefit_options) {
		$form['benefits']['by_type'][$benefit_type_tid] = array(
			'#type' => 'select',
			'#title' => sirius_term_title($benefit_type_tid),
			'#options' => array(NULL => t('I waive this benefit')) + $benefit_options,
			'#default_value' => $data['benefits']['by_type'][$benefit_type_tid],
		);
	}

	if (!$benefit_options_by_type) {
		$form['benefits']['by_type'] = array(
			'#markup' => '<p>' . '<em>' . t('No electable benefits are available.') . '</em>' . '</p>',
		);
	}

	$form['suffix'] = array('#markup' => $policy_json['elections']['benefits']['suffix']);

	// For staff, show me the reason that I am or am not eligible.
	if ($access->access('staff', $access_context)) {
		$rows = array();

		foreach ($all_benefits as $benefit_nid => $benefit_node) {
			$result = $eligibility_results_by_benefit[$benefit_nid];

			if (!$result['plugins']) {
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
				$row[] = $benefit_node->title;
				$row[] = 'OVERALL';
				$row[] = 'No';
				$row[] = 'There are no eligibility plugins for this benefit.';
				$row[] = join('', $tools);
				$rows[] = array('data' => $row, 'class' => array($class, 'summary'));
				continue;
			} 

			$subrows = array();
			$summary_eligible = TRUE;
			$summary_failed = array();
			foreach ($result['plugins'] as $plugin_key => $sub_result) {
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

				$msg = $sub_result['msg'];
				if ($sub_result['success']) {
					if ($sub_result['warning']) {
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
					$summary_failed[] = $plugin_info['name'];
				}
				if ($sub_result['tools']) { $tools = array_merge($tools, $sub_result['tools']); }

				$row = array();
				$row[] = '';
				$row[] = $plugin_info['name'];
				$row[] = $eligible;
				$row[] = $sub_result['msg'];
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
			$row[] = $benefit_node->title;
			$row[] = 'OVERALL';
			$row[] = $eligible;
			$row[] = $msg;
			$row[] = join('', $tools);
			$rows[] = array('data' => $row, 'class' => array($class, 'summary'));

			foreach ($subrows as $row) { $rows[] = $row; }
		}

		$header = array(
			'Benefit',
			'Eligibility Test',
			'Result',
			'Notes',
			'Tools',
		);

		$form['eligibility_details'] = array(
			'#type' => 'fieldset',
			'#title' => 'Staff Only - Eligibility Details',
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

		$form['eligibility_details']['details'] = array('#markup' => sirius_table_render($header, $rows, array('class' => array('sirius_table_align'))));
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_benefits_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['benefits'] = $form_state['values']['benefits'];

	//
	// Based on the elected benefits, we're now going to compute which of my relationships is eligible for which benefit.
	//

	// Get the worker
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	if (!$worker_contact_nid) { drupal_set_message("No contact for this worker", 'error'); return; }
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	if (!$policy_node) { drupal_set_message("No policy found", 'error'); return; }
	$policy_json = sirius_json_get($policy_node);

	// All my relationships
  $params = array();
  $params['active'] = 'Yes';
	$params['contact_nid'] = $worker_contact_nid;
	$relationship_nodes = sirius_contact_relationships($params);

	// Get the elected benefits
	$tmp = $form_state['values']['benefits']['by_type'];
	if (!$tmp) { $tmp = array(); }
	$benefit_nids = array();
	foreach ($tmp as $key => $benefit_nid) { $benefit_nids[] = $benefit_nid; }
	$benefit_nids = array_filter(array_unique($benefit_nids));
	$benefits = node_load_multiple($benefit_nids);

	//
  // Add any "non-electable" benefits for which we are eligible
  //

	$automatic_benefit_nids = array_keys(sirius_trust_policy_benefits_offered($policy_node, array('electable' => FALSE, 'worker_node' => $worker_node)));

	foreach ($automatic_benefit_nids as $benefit_nid) {
		$benefit_node = node_load($benefit_nid);

		$params = array();
		$params['policy_node'] = $policy_node;
		$params['subscriber_worker_node'] = $worker_node;
		$params['relationship_node'] = NULL;
		$params['ts'] = time();
		$params['shortcut'] = FALSE;
		$params['scan_type'] = 'benefit_start';
		$params['employer_node'] = $employer_node;
		$params['skip_plugins'] = array('cardcheck', 'election');
		$params['benefit_node'] = $benefit_node;
		$params['from_benefit_nids'] = $benefit_nids;
		if ($data['addr']['change'] == 'Yes') { $params['addr'] = $data['addr']['new']; }
		$params['is_election'] = TRUE;

		if ($data['addr']['change'] == 'Yes') {
			$params['addr'] = $data['addr']['new'];
		}

		$result = sirius_trust_eligibility_plugin_check_all($params);

		if ($result['plugins']) {
			foreach ($result['plugins'] as $plugin_key => $sub_result) {
				$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
				$msg = $sub_result['msg'];
				if (!$result['success']) {
					// drupal_set_message("... Automatic benefit " . $benefit_node->title . " failed eligibility plugin: $plugin_info[name]: $sub_result[msg]", 'warning');
				}
			}
		}
		if (!$result['success']) { continue; }

		$benefit_nids[] = $benefit_nid;
  }

  $data['benefits']['all'] = $benefit_nids;

	// Eligibility params
	$params = array();
	$params['policy_node'] = $policy_node;
	$params['subscriber_worker_node'] = $worker_node;
	$params['employer_node'] = $employer_node;
	$params['ts'] = time();
	$params['shortcut'] = FALSE;
	$params['scan_type'] = 'benefit_start';
	if ($data['addr']['change'] == 'Yes') { $params['addr'] = $data['addr']['new']; }
	$params['skip_plugins'] = array('cardcheck', 'election');

	$data['dependents'] = array();
	$data['dependents']['eligibility'] = array();
	foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
		$params['relationship_node'] = $relationship_node;
		$at_least_one = FALSE;
		foreach ($benefits as $benefit_nid => $benefit_node) {
			$params['benefit_node'] = $benefit_node;
			$data['dependents']['eligibility'][$relationship_nid][$benefit_nid] = sirius_trust_eligibility_plugin_check_all($params);
			if ($data['dependents']['eligibility'][$relationship_nid][$benefit_nid]['success']) {
				// drupal_set_message($relationship_node->title . " is eligible for " . $benefit_node->title);
				$at_least_one = TRUE;
			} else {
				// drupal_set_message($relationship_node->title . " IS NOT eligible for " . $benefit_node->title);
			}
			$data['dependents']['eligibility'][$relationship_nid]['at_least_one'] = $at_least_one;
		}
	}

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_benefits_render($info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$form['prefix'] = array('#markup' => $policy_json['elections']['benefits']['prefix']);

	if (!$data['benefits']['by_type']) { $data['benefits']['by_type'] = array(); }
	foreach ($data['benefits']['by_type'] as $type_tid => $benefit_nid) {
		$type_name = sirius_term_title($type_tid);
		if ($benefit_nid) {
			$benefit_name = sirius_node_title($benefit_nid);
		} else {
			$benefit_name = 'I waive this benefit';
		}

		$html .= sirius_fakerender_field($type_name . ' Benefit:', $benefit_name);
	}
	$html .= $policy_json['elections']['benefits']['suffix'];

	return $html;
}