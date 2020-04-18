<?php

function sirius_training_node_tab_worker_level_access($worker_node) {
	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}

function sirius_training_node_tab_worker_level($worker_node) {
	$is_admin = user_access('sirius training admin');

	if ($is_admin) {
		$ms_tid = $_REQUEST['ms_tid'];
		$graduate = $_REQUEST['graduate'];
		if ($ms_tid && $graduate) {
			return sirius_training_node_tab_worker_graduate($worker_node, $ms_tid, $graduate);
		}
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
			if ($payrate_skilled) { $html .= sirius_fakerender_field('Payrate (Skilled):', '$' . number_format($level['payrate_skilled'], 2)); }
			if ($payrate_unskilled) { $html .= sirius_fakerender_field('Payrate (Unskilled):', '$' . number_format($level['payrate_unskilled'], 2)); }
		} else {
			$html .= sirius_fakerender_field('Level:', t('(no level)'));
		}

		if ($is_admin) {
			$html .= '<strong>' . t('Change level to:') . '</strong> ';
			foreach ($levels as $level) {
				$html .= l($level['title'], current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('ms_tid' => $ms_tid, 'graduate' => $level['code'])));
				$html .= ' ';
			}
		}

		$html .= '<br /><br />';
	}

	return $html;
}

