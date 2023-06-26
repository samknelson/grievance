<?php

function sirius_node_tab_worker_work_status_history($parent_node) {
	$history_key = 'worker:work_status';
	$history = sirius_worker_history($parent_node, $history_key);

	$html = '';

	if (!$history || $_REQUEST['recompute']) {
		$result = sirius_worker_history_calc($parent_node, $history_key);
		if ($result['success']) {
			drupal_set_message("History recomputed: " . $result['msg']);
		} else {
			drupal_set_message("History recompute failed: " . $result['msg'], 'warning');
		}
	}

	$html .= sirius_worker_history_render($parent_node, $history_key);

	$log = sirius_minilog_render_html();
	if ($log) {
		$html .= '<hr>';
		$html .= '<h2>' . t('Recalc Log') . '</h2>';
		$html .= $log;
	}

	$html .= l('Recompute Now', current_path(), array('query' => array('recompute' => TRUE), 'attributes' => array('class' => array('tool-button'))));

	return $html;
}

