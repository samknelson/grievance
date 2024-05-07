<?php

function sirius_node_tab_dispatch_prior_access($dispatch_node) {
	return sirius_node_tab_dispatch_worker_view_access_details($dispatch_node);
}

function sirius_node_tab_dispatch_prior($dispatch_node) {
	$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
	$job_node = node_load($job_nid);

	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);

	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);

	$html = '<h2>' . 'Other dispatches of ' . $worker_node->title . ' to ' . $employer_node->title . '</h2>';

	$params = array();
	$params['employer_nid'] = $employer_nid;
	$params['worker_nid'] = $worker_nid;
	$prior_nodes = sirius_dispatch_search($params);

	if ($prior_nodes) {
		$rows = array();
		foreach ($prior_nodes as $prior_nid => $prior_node) {
			if ($prior_node->nid == $dispatch_node->nid) { continue; }
			$prior_job = node_load($prior_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$type_name = sirius_term_title($prior_job->field_sirius_dispatch_job_type['und'][0]['tid']);

			$ts = sirius_fieldval($prior_job, 'field_sirius_datetime', 'ts');

			$row = array();
			$row[] = date('m/d/Y', $ts);
			$row[] = $type_name;
			$row[] = l('View', "node/$prior_nid/sirius_dispatch_worker_view", array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Date');
		$header[] = t('Job Type');
		$header[] = t('Tools');

		$html .= sirius_table_render($header, $rows);
	} else {
		$html .= '<p>' . t('No other dispatches found.') . '</p>';
	}

	return $html;
}
