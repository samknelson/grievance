<?php

function sirius_trust_policy_benefits_offered($policy_node, $options = array()) {
	$policy_json = sirius_json_get($policy_node);
	if (!isset($policy_json['benefits']['enabled'])) { return array(); }

	$benefit_nids = array_filter(array_values($policy_json['benefits']['enabled']));

	if ($options['electable']) {
		$electable_benefit_nids = array();
		if ($policy_json['elections']['benefits']['electable']) {
			$electable_benefit_nids = array_filter(array_values($policy_json['elections']['benefits']['electable']));
		}
		$benefit_nids = array_intersect($benefit_nids, $electable_benefit_nids);
	}

	$result = node_load_multiple($benefit_nids);
	$context = array('policy_node' => $policy_node, 'options' => $options);
	drupal_alter('sirius_trust_policy_benefits_offered', $result, $context);
	return $result;

}

function sirius_trust_policy_benefits_offered_options($policy_node) {
	$benefits = sirius_trust_policy_benefits_offered($policy_node);
	if (!$benefits) { return array(); }

	$result = array();
	foreach ($benefits as $benefit_nid => $benefit_node) {
		$result[$benefit_nid] = $benefit_node->title;
	}
	return $result;
}

function sirius_trust_policy_election_types($policy_node) {
	$policy_json = sirius_json_get($policy_node);
	if (!isset($policy_json['elections']['types']['electable'])) { return array(); }
	$lookup = sirius_taxonomy_load('sirius_trust_election_type', 'tid', 'name');
	$tids = array_filter(array_values($policy_json['elections']['types']['electable']));
	$result = array();
	foreach ($tids as $tid) { $result[$tid] = $lookup[$tid]; }
	return $result;
}

