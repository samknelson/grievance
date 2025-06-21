<?php

function sirius_worker_tos_node_tab_edit_access($tos_node) {
	return Sirius_Worker_Tos_Access::getInstance()->access('edit', array('tos_node' => $tos_node));
}

function sirius_worker_tos_node_tab_edit($tos_node) {
	return drupal_get_form('sirius_worker_tos_form', $tos_node);
}
