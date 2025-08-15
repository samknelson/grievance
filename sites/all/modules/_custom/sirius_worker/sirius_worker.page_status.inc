<?php

function sirius_node_tab_worker_status_access($worker_node) {
	return Sirius_Worker_Access::getInstance()->access('view status', array('worker_node' => $worker_node));
}

function sirius_node_tab_worker_status($worker_node) {
	$statuses = sirius_taxonomy_load('sirius_work_status', 'tid', 'name', TRUE);

	$status_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];
	$status_term = taxonomy_term_load($status_tid);
	if ($status_term) {
		$html .= sirius_fakerender_field('Work Status:', $status_term->name);
		$html .= '<hr>';
	}

  $html .= sirius_worker_render_industries($worker_node);

  return $html;
}
