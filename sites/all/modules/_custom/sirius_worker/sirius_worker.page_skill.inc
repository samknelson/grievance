<?php

function sirius_node_tab_worker_skill_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_skill($worker_node) {
	drupal_goto(current_path() . '/view');
}

function sirius_node_tab_worker_skill_view($worker_node) {
 	drupal_add_css(drupal_get_path('module', 'sirius_worker') . '/css/skills.css');

 	if ($worker_node->field_sirius_skill['und']) {
		$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
		$rows = array();
		$header = array(t('Skill'), t('Expires'));
		$now_ts = time();
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = $skill_terms[$skill_tid];

			$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
			$classes = array();
			if (!$expires_ts || $expires_ts > time() + 60*60*24*365*50) {
				$expires = sirius_domain_variable_get('sirius_worker_skill_never_expires_text', 'Never Expires');
			} else if ($expires_ts < $now_ts) {
				$expires = t('Expired on @date', array('@date' => date('m/d/Y', $expires_ts))) . '</em>';
				$classes[] = 'sirius_worker_skill_expired';
			} else {
				$expires = t('Expires: @date', array('@date' => date('m/d/Y', $expires_ts)));
				$expires .= ' (' . format_interval($expires_ts - time(), 2) . t(' remaining') . ')';
			}

			$row = array();
			$row[] = $skill_term->name;
			$row[] = $expires;
			$rows[] = array('data' => $row, 'class' => $classes);
		}
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('sirius_data_table'))));
	} else {
		$html .= t('@name does not currently have any skills registered.', array('@name' => $worker_node->title));
	}

	return $html;
}
