<?php

function sirius_trust_eligibility_plugin_info_all() {
	$info_all = &drupal_static(__FUNCTION__, NULL);
	if (!isset($info_all)) {
		$info_all = module_invoke_all('sirius_trust_eligibility_plugin_info');
		foreach ($info_all as $key => $record) {
			$info_all[$key]['key'] = $key;
			if (!$info_all[$key]['execution_cost']) { $info_all[$key]['execution_cost'] = 0; }
		}
	}

	ksort($info_all);

	return $info_all;
}

function sirius_trust_eligibility_plugin_info($plugin_key) {
	$info_all = sirius_trust_eligibility_plugin_info_all();
	return $info_all[$plugin_key];
}

function sirius_trust_eligibility_plugin_check_all($policy_node, $benefit_node, $subscriber_worker_node, $relationship_node, $ts, $shortcut = FALSE) {
	$plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
	if (!$plugins) { return array('success' => FALSE, 'plugins' => array()); }

	$result = array('success' => TRUE);

	// Sort by execution cost, so we run the cheapest check first
	$plugin_info_all = sirius_trust_eligibility_plugin_info_all();
	usort($plugins, function($a, $b) use ($plugins, $plugin_info_all) {
		return $plugin_info_all[$a]['execution_cost'] <=> $plugin_info_all[$b]['execution_cost'];
	});

	foreach ($plugins as $plugin_key) {
		$sub_result = sirius_trust_eligibility_plugin_check($policy_node, $benefit_node, $subscriber_worker_node, $relationship_node, $ts, $plugin_key);
		if (!$sub_result['success']) {
			$result['success'] = FALSE;
			if ($shortcut) { break; }
		}

		$result['plugins'][$plugin_key] = $sub_result;
	}

	return $result;
}

function sirius_trust_eligibility_plugin_check($policy_node, $benefit_node, $subscriber_worker_node, $relationship_node, $ts, $plugin_key) {
	if (!$ts) { $ts = time(); }
	sirius_trust_eligibility_plugin_load($plugin_key);
	$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	$settings = sirius_trust_eligibility_plugin_settings($policy_node, $benefit_node, $plugin_key);
	$callback = $plugin_info['callbacks']['check'];
	if (!$callback) { return array('success' => FALSE, 'msg' => 'Plugin callback not specified.'); }

	$params = array();
	$params['subscriber_worker_node'] = $subscriber_worker_node;
	$params['policy_node'] = $policy_node;
	$params['benefit_node'] = $benefit_node;
	$params['relationship_node'] = $relationship_node;
	$params['ts'] = $ts;
	$params['plugin_info'] = $plugin_info;

	return $callback($settings, $params);
}

function sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node) {
	$policy_json = sirius_json_get($policy_node);
	if (!isset($policy_json['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled'])) { return array(); }
	return array_filter(array_values($policy_json['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled']));
}
/**
 * Implements hook_sirius_trust_eligibility_plugin_info
 */

function sirius_trust_sirius_trust_eligibility_plugin_info() {
	$items = array();

	$items['election'] = array(
		'title' => 'Election Required',
		'description' => 'To be eligible for this benefit, the member or dependent must have an election on file. The election must have been made before the date of benefit commencement, and the end date on the election must be either empty or after the date of the benefit commencement.',
		'file' => 'sirius_trust.eligibility_plugin_election.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'check' => 'sirius_trust_eligibility_plugin_election_check',
		),
		'execution_cost' => 3,
	);

	$items['universal'] = array(
		'title' => 'Universal Eligibility',
		'description' => 'Universal eligibility indicates that there are no special eligibility requirements. All workers in the system will be eligibile for benefits with universal eligibility. In general, this plugin should not be combined with other plugins; it should be the only plugin for a given benefit.',
		'file' => 'sirius_trust.eligibility_plugin_universal.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'settings' => 'sirius_trust_eligibility_plugin_universal_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_universal_check',
		),
		'execution_cost' => -10,
	);

	$items['ws'] = array(
		'title' => 'Work Status',
		'description' => 'Work status eligibility indicates that a dependent or subscriber must have a specific work status, or must be an an active member.',
		'file' => 'sirius_trust.eligibility_plugin_ws.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'settings' => 'sirius_trust_eligibility_plugin_ws_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_ws_check',
		),
		'execution_cost' => 5,
	);

	$items['ms'] = array(
		'title' => 'Member Status',
		'description' => 'Member Status eligibility indicates that a dependent or subscriber must have a specific member status.',
		'file' => 'sirius_trust.eligibility_plugin_ms.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'settings' => 'sirius_trust_eligibility_plugin_ms_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_ms_check',
		),
		'execution_cost' => 5,
	);

	$items['reltype'] = array(
		'title' => 'Relationship Type',
		'description' => 'Eligibility for this benefit is restricted to only certain relationship types.',
		'file' => 'sirius_trust.eligibility_plugin_reltype.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'settings' => 'sirius_trust_eligibility_plugin_reltype_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_reltype_check',
		),
		'execution_cost' => -7,
	);

	$items['employer'] = array(
		'title' => 'Employer',
		'description' => 'Eligibility for this benefit is restricted based on whether the subscriber was employed at the time, and whether the employer offered the benefit.',
		'file' => 'sirius_trust.eligibility_plugin_employer.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			// 'benefit_settings' => 'sirius_trust_eligibility_plugin_employer_benefit_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_employer_check',
		),
		'execution_cost' => 7,
	);

	$items['hours'] = array(
		'title' => 'Hours',
		'description' => 'Eligibility for this benefit is restricted based on the subscriber\'s hours.',
		'file' => 'sirius_trust.eligibility_plugin_hours.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'settings' => 'sirius_trust_eligibility_plugin_hours_settings_form',
			'check' => 'sirius_trust_eligibility_plugin_hours_check',
		),
		'execution_cost' => 2,
	);

	$items['relationship_active'] = array(
		'title' => 'Relationship Active',
		'description' => 'The relationship must be active at the time that the eligibility is checked.',
		'file' => 'sirius_trust.eligibility_plugin_relationship_active.inc',
		'module' => 'sirius_trust',
		'callbacks' => array(
			'check' => 'sirius_trust_eligibility_plugin_relationship_active_check',
		),
		'execution_cost' => -7,
	);

	return $items;
}

function sirius_trust_eligibility_plugin_load($plugin_key) {
	$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	if ($plugin_info['file']) { sirius_require($plugin_info['module'], $plugin_info['file']); }
}

function sirius_trust_eligibility_plugin_settings($policy_node, $benefit_node, $plugin_key) {
	$policy_json = sirius_json_get($policy_node);
	return $policy_json['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings'];
}
