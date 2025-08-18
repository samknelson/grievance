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

function sirius_worker_work_status_set(&$worker_node, $work_status_tid, $reason, $attachments, $json = array(), $ts = NULL) {
	$history_key = 'worker:work_status';
	if (!$ts) { $ts = time(); }

	$possible = sirius_worker_work_status_set_possible($worker_node, $work_status_tid, $reason, $attachments, $json);
	if (!$possible['success']) { return $possible; }

	$work_status_term = taxonomy_term_load($work_status_tid);
  $args = array($worker_node, $work_status_term, $reason, $attachments, $json, $ts);
  drupal_alter('sirius_worker_work_status_set', $args);

	$worker_node = node_load($worker_node->nid);

	$old_work_status_tid = sirius_worker_history_asof($worker_node, $history_key, $ts);
	if (!$old_work_status_tid) { $old_work_status_tid = $worker_node->field_sirius_work_status['und'][0]['tid']; }
	if ($old_work_status_tid == $work_status_tid) {
		return array('success' => TRUE, 'msg' => 'Work status unchanged.');
	}

	// @todo: This SHOULD BE unnecessary, because it will be computed from the history. But I'm saving here just in case.
	$worker_node->field_sirius_work_status['und'][0]['tid'] = $work_status_tid;
	try {
		node_save($worker_node);
	} catch (exception $e) {
		return array('success' => FALSE, 'msg' => "Save failed: " . $e->getMessage());
	}

	$type = 'update';
	$message = $worker_node->title . ' has been assigned the new work status ' . $work_status_term->name;
	$summary = $reason;
	$handler_nids = array($worker_node->nid);
	$json = array();
	$json['change']['old_value'] = $old_work_status_tid;
	$json['change']['new_value'] = $work_status_tid;
	$json['change']['ts'] = $ts;

	sirius_log($history_key, $type, $message, $summary, $notes, $handler_nids, $attachments, $json);

	return array('success' => TRUE, 'msg' => 'Work status updated.');
}

function sirius_worker_work_status_history_callback(&$parent_node, $history) {
	if ($history['last']['new_value']) {
  	$parent_node->field_sirius_work_status['und'][0]['tid'] = $history['last']['new_value'];
  } else {
  	unset($parent_node->field_sirius_work_status['und']);
  }
}

/*
function sirius_worker_work_status_set_month(&$worker_node, $ws_tid, $reason, $attachments, $json, $date) {
	$start_date = date("Y-m-1 00:00:00", strtotime($date));
	$start_ts = strtotime($start_date);
	$end_ts = strtotime('+1 month', $start_ts);

	drupal_set_message("Settings " . sirius_term_title($ws_tid) . " for the month of $start_date");

  $history_key = 'worker:work_status';
  $old_ws_tid = sirius_worker_history_asof($worker_node, $history_key, $start_ts);
  if (!$old_ws_tid) { $old_ws_tid = sirius_taxonomy_lookup_by_anything('sirius_work_status', 'unknown'); }

  if ($old_ws_tid == $ws_tid) {
  	drupal_set_message("Status unchanged.");
  	return array('success' => TRUE, 'msg' => 'Work status unchanged.');
  } else {
    drupal_set_message("Changing from " . sirius_term_title($old_ws_tid) . " to " . sirius_term_title($ws_tid));

		drupal_set_message("A) Setting " . sirius_term_title($ws_tid) . " as of " . date('Y-m-d H:i:s', $start_ts));
    $result = sirius_worker_work_status_set($worker_node, $ws_tid, $reason, $attachments, $json, $start_ts);
    sirius_worker_history_calc($worker_node, $history_key);
    if (!$result['success']) { return $result; }

		drupal_set_message("B) Setting " . sirius_term_title($old_ws_tid) . " as of " . date('Y-m-d H:i:s', $start_ts));
    $result = sirius_worker_work_status_set($worker_node, $old_ws_tid, $reason, $attachments, $json, $end_ts);
    sirius_worker_history_calc($worker_node, $history_key);
    if (!$result['success']) { return $result; }
  }

	return array('success' => TRUE);
}
*/
