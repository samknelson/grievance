<?php

function sirius_worker_work_status_set($worker_node, $work_status_tid, $reason, $attachments) {
	$work_status_term = taxonomy_term_load($work_status_tid);

	if ($worker_node->field_sirius_work_status['und'][0]['tid'] != $work_status_tid) {
		$worker_node->field_sirius_work_status['und'][0]['tid'] = $work_status_tid;
		$need_save = TRUE;
	}

	if ($need_save) {
		node_save($worker_node);
	}

	$category = 'worker:work_status';
	$type = 'update';
	$message = $worker_node->title . ' has been assigned the new work status ' . $work_status_term->name;
	$summary = $reason;
	$handler_nids = array($worker_node->nid);
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}