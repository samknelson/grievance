<?php

function sirius_training_node_tab_worker($worker_node) {
	drupal_goto(current_path() . '/overview');
}

function sirius_training_node_tab_worker_overview($worker_node) {
	$ms_tid = $_REQUEST['ms_tid'];
	$graduate = $_REQUEST['graduate'];
	if ($ms_tid && $graduate) {
		return sirius_training_node_tab_worker_graduate($worker_node, $ms_tid, $graduate);
	}

	$all_levels = sirius_training_levels_fetch_all();
	$worker_levels = sirius_training_levels_fetch_worker($worker_node);

	foreach ($all_levels as $ms_tid => $levels) {
		$ms_term = taxonomy_term_load($ms_tid);
		$level_code = $worker_levels[$ms_tid]['code'];
		$level = $levels[$level_code];
		$worker_level = $worker_levels[$ms_tid];

		$html .= '<strong>' . $ms_term->name . '</strong><br />';
		if ($worker_level) {
			$html .= sirius_fakerender_field('Level:', $level['title']);
			$html .= sirius_fakerender_field('Date:', date('Y-m-d H:i:s', $worker_level['ts'])); 
		} else {
			$html .= sirius_fakerender_field('Level:', t('(no level)'));
		}

		$html .= '<strong>' . t('Change level to:') . '</strong> ';
		foreach ($levels as $level) {
			$html .= l($level['title'], current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('ms_tid' => $ms_tid, 'graduate' => $level['code'])));
			$html .= ' ';
		}

		$html .= '<br /><br />';
	}

	return $html;
}

