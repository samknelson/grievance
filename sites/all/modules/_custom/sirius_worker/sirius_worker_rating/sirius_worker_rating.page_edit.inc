<?php

function sirius_worker_rating_node_tab_edit_access($rating_node) {
	return Sirius_Worker_rating_Access::getInstance()->access('edit', array('rating_node' => $rating_node));
}

function sirius_worker_rating_node_tab_edit($rating_node) {
	return drupal_get_form('sirius_worker_rating_form', $rating_node);
}
