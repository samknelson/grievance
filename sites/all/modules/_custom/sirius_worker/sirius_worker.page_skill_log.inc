<?php

function sirius_node_tab_worker_skill_log($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_skill_log_form', $worker_node);
}

function sirius_node_tab_worker_skill_log_form($form, &$form_state, $worker_node) {
	$form += sirius_log_render_form($worker_node->nid, 'worker:skill');
	unset($form['log_wrapper']['#collapsed']);
	unset($form['log_wrapper']['#collapsible']);
	return $form;
}
