<?php

function sirius_steward_page_list() {
	$stewards = sirius_steward_search();

	if (!$stewards) { return t('No stewards found.'); }

	$rows = array();
	foreach ($stewards as $steward_nid => $steward_node) {
		$tools = array();
		$tools[] = l('View', "node/$steward_nid", array('attributes' => array('class' => array('tool-button'))));
		$tools[] = l('Checkin', "node/$steward_nid/sirius_worker_steward/checkin", array('attributes' => array('class' => array('tool-button'))));



		$row = array();
		$row[] = $steward_node->title;
		$row[] = sirius_node_title($steward_node->field_grievance_shop['und'][0]['target_id']);
		$row[] = join('', $tools);
		$rows[] = $row;
	}

	$header = array();
	$header[] = t('Steward');
	$header[] = t('Employer');
	$header[] = t('Tools');

	return sirius_table_render($header, $rows);
}