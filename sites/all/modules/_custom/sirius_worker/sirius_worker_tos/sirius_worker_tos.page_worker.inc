<?php

function sirius_worker_tos_node_tab_worker_access($worker_node) {
	return Sirius_Worker_Tos_Access::getInstance()->access('worker_view', array('worker_node' => $worker_node));
}

function sirius_worker_tos_node_tab_worker($worker_node) {
	return drupal_get_form('sirius_worker_tos_worker_form', $worker_node);
}
