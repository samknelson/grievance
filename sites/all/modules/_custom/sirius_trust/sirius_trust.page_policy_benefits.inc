<?php


function sirius_trust_node_tab_policy_benefits_access($policy_node) {
	return $policy_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy';
}

function sirius_trust_node_tab_policy_benefits($policy_node) {
	// The "Benefits" tab has two levels of dynamic menus. For a given policy, we need to be able to navigate to:
	// - The individual benefit
	// - The individual eligibility criterion for the benefit
	// I.e. "Basic Package 2021 > Dental > Hours Eligilbity"
	// 
	// We can't use standard node tabs because both levels are dynamic (we don't want a menu router entry for every single possible combination). 
	// Hence the monstrosity below.

	sirius_require('sirius_trust', 'sirius_trust.page_policy_benefits_offered.inc');
	sirius_require('sirius_trust', 'sirius_trust.page_policy_benefit_eligibility.inc');
	sirius_require('sirius_trust', 'sirius_trust.page_policy_benefit_eligibility_plugin.inc');

	$benefit_nid = sirius_node_tab_arg(0);
	$benefit_node = node_load($benefit_nid);
	if ($benefit_node && !sirius_trust_node_tab_policy_benefit_eligibility_access($policy_node, $benefit_node)) { return 'Access denied.'; }

	$plugin_key = sirius_node_tab_arg(1);
	if ($plugin_key && !sirius_trust_node_tab_policy_benefit_eligibility_plugin_access($policy_node, $benefit_node, $plugin_key)) { return 'Access denied.'; }

	$html = sirius_trust_node_tab_policy_benefits_quicknav($policy_node, $benefit_node);

	if (!$benefit_node) {
		$renderable = drupal_get_form('sirius_trust_node_tab_policy_benefits_offered_form', $policy_node);
		$html .= drupal_render($renderable);
	} else if (!$plugin_key) {
		$renderable = drupal_get_form('sirius_trust_node_tab_policy_benefit_eligibility_form', $policy_node, $benefit_node);
		$html .= drupal_render($renderable);
	} else {
		$renderable = drupal_get_form('sirius_trust_node_tab_policy_benefit_eligibility_plugin_form', $policy_node, $benefit_node, $plugin_key);
		$html .= drupal_render($renderable);
	}
	return $html;
}

function sirius_trust_node_tab_policy_benefits_quicknav($policy_node, $my_benefit_node) {
	// Main menu
	$offered_benefits = sirius_trust_policy_benefits_offered($policy_node);
	if (!$offered_benefits) { return ''; }

	$base_path = 'node/' . $policy_node->nid . '/sirius_trust_policy_benefits';

  $links = array();

  $link = array();
  $link['title'] = 'Benefits Offered';
  $link['path'] = $base_path;
  $links[] = $link;

	foreach ($offered_benefits as $benefit_nid => $benefit_node) {
		$sub_path = "$base_path/$benefit_nid";

		$link = array();
	  $link['title'] = $benefit_node->title;
	  $link['path'] = $sub_path;
	  if ($benefit_nid == $my_benefit_node->nid) { $link['current'] = TRUE; }
	  $links[] = $link;
	}

	$html = sirius_quicknav($links);

	// Sub menu
	if (!$my_benefit_node) { return $html; }

	$enabled_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $my_benefit_node);
	if (!$enabled_plugins) { return $html; }

  $sub_links = array();
  $sub_base_path = $base_path . '/' . $my_benefit_node->nid;

  $sub_link = array();
  $sub_link['title'] = 'Enabled Eligibility Plugins';
  $sub_link['path'] = $sub_base_path;
  $sub_links[] = $sub_link;

  foreach ($enabled_plugins as $plugin_key) {
  	$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	  $sub_link['title'] = $plugin_info['title'];
	  $sub_link['path'] = "$sub_base_path/$plugin_key";
	  $sub_links[] = $sub_link;
  }

	$html .= sirius_quicknav($sub_links);

	return $html;
}
