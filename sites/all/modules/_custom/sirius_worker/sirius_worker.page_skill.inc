<?php

function sirius_node_tab_worker_skill_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_skill($worker_node) {
	drupal_goto(current_path() . '/view');
}

function sirius_node_tab_worker_skill_view($worker_node) {
	if ($worker_node->field_sirius_skill['und']) {
		$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
		$rows = array();
		$header = array(t('Skill'), t('Expires'));
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = $skill_terms[$skill_tid];

			$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
			if (!$expires_ts || $expires_ts > time() + 60*60*24*365*50) {
				$expires = t('Never Expires');
			} else {
				$expires = t('Expires: @date', array('@date' => date('m/d/Y', $expires_ts)));
				$expires .= ' (' . format_interval($expires_ts - time(), 2) . t(' remaining') . ')';
			}

			$row = array();
			$row[] = $skill_term->name;
			$row[] = $expires;
			$rows[] = $row;
		}
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('sirius_data_table'))));
	} else {
		$html .= t('@name does not currently have any skills registered.', array('@name' => $worker_node->title));
	}

	return $html;
}
