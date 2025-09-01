<?php

function sirius_trust_dashboard_wbs_data($prefs, $options = array()) {
	$rows = array();

	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(); }

	$tmp = sirius_trust_worker_benefits($worker_node);
	if (!$tmp) { return array(); }

	$data = array();
	foreach ($tmp as $wb_nid => $wb_node) {
		$record = array();
		$record['wb_node'] = $wb_node;
		$record['benefit_node'] = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
		$record['type_term'] = taxonomy_term_load($record['benefit_node']->field_sirius_trust_benefit_type['und'][0]['tid']);
		$record['benefit_json'] = sirius_json_get($record['benefit_node']);
		$data[$wb_nid] = $record;
	}

	// Break out medical, dental, vision, other
	$breakout_data = array();
	foreach ($data as $wb_nid => $record) {
		if ($record['benefit_json']['dashboard']['standalone'] == 'Yes') {
			$breakout_data[$wb_nid] = $record;
			unset($data[$wb_nid]);
		}
	}

	$dashboard_items = array();

	// Breakouts
	foreach ($breakout_data as $wb_nid => $record) {
		$title = $record['type_term']->name . ' Benefit';
		$html = '<div style="padding-bottom: 5px;"><strong>' . $record['benefit_node']->title . '</strong></div>';
		$html .= sirius_richtext_handle_input($record['benefit_json']['dashboard']['details']['description']);
		$dashboard_items[] = array(
			'title' => $title,
			'details' => $html,
			'tools' => l('View Details', 'node/' . $record['benefit_node']->nid, array('attributes' => array('class' => array('tool-button')))),
		);
	}

	if ($data) {
		$html = '';
		foreach ($data as $wb_nid => $record) {
			$html .= '<div style="padding-bottom: 5px;">';
			$html .= '<strong>' . $record['type_term']->name . ':</strong><br />';
			$html .= $record['benefit_node']->title;
			$html .= '</div>';
		}

		$title = 'Other Benefits';

		$tools = l(
			'View Details',
			'node/' . $worker_node->nid . '/sirius_trust_worker_benefits',
			array('attributes' => array('class' => array('tool-button')))
		);

		$dashboard_items[] = array(
			'tools' => $tools,
			'details' => $html,
			'title' => $title,
		);
	}

	return $dashboard_items;
}
