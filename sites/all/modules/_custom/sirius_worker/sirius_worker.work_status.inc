<?php

function sirius_worker_work_status_set_possible(&$worker_node, $work_status_tid, $reason, $attachments, $json = array()) {
	if ($json['feed']) {
		$old_work_status_term = taxonomy_term_load($worker_node->field_sirius_work_status['und'][0]['tid']);
		$old_work_status_json = sirius_json_get($old_work_status_term);
		if ($old_work_status_json['feed']['locked'] == 'Yes') {
			return array('success' => FALSE, 'msg' => "Unable to change work status from [" . $old_work_status_term->name . "] because that work status is feed-locked.");
		}
	}
	return array('success' => TRUE);
}

function sirius_worker_work_status_set(&$worker_node, $work_status_tid, $reason, $attachments, $json = array()) {
	$worker_node = node_load($worker_node->nid);
	if ($worker_node->field_sirius_work_status['und'][0]['tid'] == $work_status_tid) {
		return array('success' => TRUE, 'msg' => 'Work status unchanged.');
	}

	$possible = sirius_worker_work_status_set_possible($worker_node, $work_status_tid, $reason, $attachments, $json);
	if (!$possible['success']) { return $possible; }

	$worker_node->field_sirius_work_status['und'][0]['tid'] = $work_status_tid;
	try {
		node_save($worker_node);
	} catch (exception $e) {
		return array('success' => FALSE, 'msg' => "Save failed: " . $e->getMessage());
	}

	$work_status_term = taxonomy_term_load($work_status_tid);
	$category = 'worker:work_status';
	$type = 'update';
	$message = $worker_node->title . ' has been assigned the new work status ' . $work_status_term->name;
	$summary = $reason;
	$handler_nids = array($worker_node->nid);
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);

	return array('success' => TRUE, 'msg' => 'Work status updated.');
}