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

	if (!$plugin_info || !$plugin_info['instance']) { return array('success' => FALSE, 'msg' => 'Plugin callback not specified.'); }

	$settings = sirius_trust_eligibility_plugin_settings($policy_node, $benefit_node, $plugin_key);

	$params = array();
	$params['subscriber_worker_node'] = $subscriber_worker_node;
	$params['policy_node'] = $policy_node;
	$params['benefit_node'] = $benefit_node;
	$params['relationship_node'] = $relationship_node;
	$params['ts'] = $ts;
	$params['plugin_info'] = $plugin_info;

	return $plugin_info['instance']->check($settings, $params);
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
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_universal.inc');
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_election.inc');
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_reltype.inc');
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_ws.inc');
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_ms.inc');
	sirius_require('sirius_trust', 'sirius_trust.eligibility_plugin_geog.inc');

  $items = array();
  Sirius_Trust_Elgibility_Plugin_Universal::getInstance()->setInfo($items);
  Sirius_Trust_Elgibility_Plugin_Election::getInstance()->setInfo($items);
  Sirius_Trust_Elgibility_Plugin_Reltype::getInstance()->setInfo($items);
  Sirius_Trust_Elgibility_Plugin_WS::getInstance()->setInfo($items);
  Sirius_Trust_Elgibility_Plugin_MS::getInstance()->setInfo($items);
  Sirius_Trust_Elgibility_Plugin_Geog::getInstance()->setInfo($items);
  return $items;

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
















class Sirius_Trust_Elgibility_Plugin extends Sirius_Singleton {
	public $type = 'root';
	public $name = 'Root';
	public $description = 'Root eligibility plugin; must be overridden.';

	public function form($json) {
		return array();
	}

	public function form_submit(&$json) {
		// Handle configuration form submit (i.e. extra calculations)
	}

	public function check($settings, $params) {
		return array('success' => FALSE, 'msg' => 'Root eligibility check called.');
	}
}
