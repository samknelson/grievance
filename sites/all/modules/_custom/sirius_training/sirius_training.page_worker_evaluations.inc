<?php

function sirius_training_node_tab_worker_evaluations_access($worker_node) {
	// Evaluations must be enabled
	if (sirius_domain_variable_get('sirius_training_evaluations_enabled', '') != 'Yes') { return FALSE; }

	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}

function sirius_training_node_tab_worker_evaluations($worker_node) {
	$links = array();
	$links['accepted'] 	= array('path' => current_path(), 'title' => 'Accepted', 	'query' => array('status' => NULL));
	$links['draft'] 		= array('path' => current_path(), 'title' => 'Draft', 		'query' => array('status' => 'draft'));
	$links['rejected'] 	= array('path' => current_path(), 'title' => 'Rejected', 	'query' => array('status' => 'rejected'));
	$links['all'] 			= array('path' => current_path(), 'title' => 'All', 			'query' => array('status' => 'all'));
	$html .= sirius_quicknav($links);

	$status = $_REQUEST['status'];
	if (!$status) { $status = 'accepted'; }
	else if ($status == 'all') { $status = NULL; }

	$evaluation_nodes = sirius_training_evaluations_fetch_by_worker($worker_node->nid, $status);

	if ($evaluation_nodes) {
		foreach ($evaluation_nodes as $evaluation_node) {
			$html .= sirius_training_evaluations_render($evaluation_node, array('hide_worker' => TRUE));
		}
	} else {
		$html .= '<p>There are no evaluations for this worker.</p>';
	}

	$html .= l('Add an Evaluation', 'sirius/training/evaluation/' . $worker_node->field_sirius_id['und'][0]['value'], array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
	return $html;
}
