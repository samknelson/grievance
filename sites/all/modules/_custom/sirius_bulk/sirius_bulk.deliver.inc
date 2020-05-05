<?php

/**
 * Cron function -- deliver all pending messages
 */

function sirius_bulk_deliver($force = FALSE) {
	// Don't run too often
	$last_ts = variable_get("sirius_bulk_last_ts", 0);
	$delay = 60;
	if (!$force && time() < $last_ts + $delay) {
		return array('success' => FALSE, 'msg' => 'Too soon to run.');
	}
	variable_set("sirius_bulk_last_ts", time());

	// Get any pending nodes
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_bulk');
  $query->fieldCondition('field_sirius_bulk_status', 'value', 'pending');
  $query->fieldCondition('field_sirius_datetime', 'value', gmdate('Y-m-d H:i:s'), '<=');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array('success' => TRUE, 'msg' => 'No nodes with status "pending" and a send date before now.'); }
  $bulk_nids = array_keys($stmt['node']);
  $bulk_nodes = node_load_multiple($bulk_nids);

  $count = 0;
  foreach ($bulk_nodes as $bulk_nid => $bulk_node) {
  	++$count;
  	sirius_bulk_deliver_batch($bulk_node, $force);
  }
  return array('success' => TRUE, 'msg' => "Delivered the next batch on $count bulk node(s).");
}

/**
 * Deliver the next batch for a specific message
 */

function sirius_bulk_deliver_batch($bulk_node, $force = FALSE) {
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];

	// Don't run too often
	$last_ts = variable_get("sirius_bulk_last_ts_$medium", 0);
	$delay = variable_get("sirius_bulk_delay_$medium", 300);
	if (!$force && time() < $last_ts + $delay) {
		return array('success' => FALSE, 'msg' => 'Too soon to run the next batch.');
	}
	variable_set("sirius_bulk_last_ts_$medium", time());

	// Don't send messages at night
	if (!$force) {
		$ok = sirius_dispatch_notify_schedule_ok();
		if (!$ok['success']) { return $ok; }
	}

	// Get the recipients
	$batch_size = variable_get("sirius_bulk_batch_$medium", 50);
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $bulk_node->nid);
  $query->fieldCondition('field_sirius_category', 'value', 'bulk:queue');
	$query->fieldCondition('field_sirius_type', 'value', 'add');
  $query->range(0, $batch_size);
  $stmt = $query->execute();
  if (!$stmt['node']) { 
  	$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'sent';
  	$bulk_node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d H:i:s');
  	node_save($bulk_node);
  	return array('success' => TRUE, 'msg' => 'No more pending recipients. Delivery complete.');
  }
  $log_nids = array_keys($stmt['node']);

  foreach ($log_nids as $log_nid) {
  	sirius_bulk_deliver_entry($log_nid);
  }


	return array('success' => TRUE, 'msg' => 'Yay.');
}

function sirius_bulk_deliver_entry($log_nid) {
	$log_node = node_load($log_nid);
	if (!$log_node) { return; }
	if ($log_node->field_sirius_category['und'][0]['value'] != 'bulk:queue') { return; }
	if ($log_node->field_sirius_type['und'][0]['value'] != 'add') { return; }

	$bulk_nid = $log_node->field_sirius_log_handler['und'][0]['target_id'];
	if (!$bulk_nid) { return; }
	$bulk_node = node_load($bulk_nid);
	if (!$bulk_node) { return; }
	if ($bulk_node->type != 'sirius_bulk') { return; }

	$to = $log_node->field_sirius_notes['und'][0]['value'];

	$result = sirius_bulk_deliver_impl($bulk_node, $to);

	if ($result['success']) {
		$log_node->field_sirius_type['und'][0]['value'] = 'sent';
	} else {
		$log_node->field_sirius_type['und'][0]['value'] = 'error';
	}
	$log_node->field_sirius_summary['und'][0]['value'] .= $result['msg'];
	node_save($log_node);

	return $result;
}

function sirius_bulk_deliver_impl($bulk_node, $to_input, $is_test = FALSE) {
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$params = array();
	$params['handler_nids'] = array($bulk_node->nid);

	if (is_numeric($to_input)) {
		$worker_node = node_load($to_input);
		$params['handler_nids'][] = $worker_node->nid;
		$params['worker_nid'] = $worker_node->nid;
	}

	//
	// Figure out the medium.
	//

	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	if ($medium == 'choice') {
		if ($worker_node) {
			$medium = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
		} else if (preg_match('/@/', $to_input)) {
			$medium = 'email';
		} else {
			$medium = 'sms';
		}
	}
	if (!$medium || $medium == 'choice' || $medium == 'none') {
		$medium = 'email';
	}

	//
	// Figure out the recipient
	//

	if ($worker_node) {
		if ($medium == 'email') {
			$to = $worker_node->field_sirius_email['und'][0]['value'];
		} else {
			$to = $worker_node->field_sirius_phone['und'][0]['value'];
		}
	} else {
		$to = $to_input;
	}
	$params['to'] = $to;

	$params['subject'] = $bulk_node->title;
	$params['body'] = $bulk_node->body['und'][0]['value'];
	// $params['sms'] = $bulk_node->title . "\n\n" . drupal_html_to_text($bulk_node->body['und'][0]['value']);
	$params['sms'] = $bulk_node->title . "\n\n" . $bulk_node->field_sirius_sms['und'][0]['value'];
	// $params['voice'] = drupal_html_to_text($bulk_node->body['und'][0]['value']);
	$params['voice'] = $bulk_node->title . "\n\n" . $bulk_node->field_sirius_voice['und'][0]['value'];

	if (!$is_test) {
		$params['X-Sirius-Id'] = "UNIQUE/bulk/" . $bulk_node->nid . '/' . $to_input;
	}

	if ($medium == 'email') {
		return sirius_email_send($params);
	} else if ($medium == 'voice') {
		return sirius_voice_send($params);
	} else {
		return sirius_sms_send($params);
	}
}