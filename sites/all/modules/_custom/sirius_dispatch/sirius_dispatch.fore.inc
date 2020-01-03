<?php

function sirius_dispatch_fore_add_possible($job_node, $worker_node) {
	if ($job_node->field_grievance_shop['und'][0]['target_id'] != $worker_node->field_grievance_shop['und'][0]['target_id']) {
		return array('success' => FALSE, 'msg' => 'The employers for the worker and the job do not match.');
	}

	$json = json_decode($job_node->field_sirius_json['und'][0]['value'], TRUE);
	if ($json['fore'][$worker_node->nid]) {
		return array('success' => FALSE, 'msg' => $worker_node->title . ' is already a foreperson for this job.');
	}
	return array('success' => TRUE);
}

function sirius_dispatch_fore_add($job_node, $worker_node) {
	$result = sirius_dispatch_fore_add_possible($job_node, $worker_node);
	if (!$result['success']) { return $result; }

	$json = json_decode($job_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['fore'][$worker_node->nid] = $worker_node->title;
	$job_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($job_node);

	sirius_log('dispatch:fore', 'add', "Added foreperson " . $worker_node->title, '', '', array($worker_node->nid, $job_node->nid));

	$msg = "Added " . $worker_node->title . " as a foreperson.";
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_fore_remove_possible($job_node, $worker_node) {
	$json = json_decode($job_node->field_sirius_json['und'][0]['value'], TRUE);
	if (!$json['fore'][$worker_node->nid]) {
		return array('success' => FALSE, 'msg' => $worker_node->title . ' is not a foreperson for this job.');
	}
	return array('success' => TRUE);
}

function sirius_dispatch_fore_REMOVE($job_node, $worker_node) {
	$result = sirius_dispatch_fore_remove_possible($job_node, $worker_node);
	if (!$result['success']) { return $result; }

	$json = json_decode($job_node->field_sirius_json['und'][0]['value'], TRUE);
	unset($json['fore'][$worker_node->nid]);
	$job_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($job_node);

	sirius_log('dispatch:fore', 'remove', "Removed foreperson " . $worker_node->title, '', '', array($worker_node->nid, $job_node->nid));

	$msg = "Removed " . $worker_node->title . " as a foreperson.";
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_fore_list($job_node) {
	$json = json_decode($job_node->field_sirius_json['und'][0]['value'], TRUE);
	return $json['fore'];
}