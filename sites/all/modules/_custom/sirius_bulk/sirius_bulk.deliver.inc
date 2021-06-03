<?php

/**
 * Cron function -- deliver all pending messages
 */

function sirius_bulk_deliver($force = FALSE) {
	// Don't run too often
	$last_ts = variable_get("sirius_bulk_last_ts", 0);
	$delay = 60;
	if (!$force && time() < $last_ts + $delay) {
		sirius_minilog("sirius_bulk_deliver: too soon to run.");
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
  	sirius_minilog("About to deliver: " . $bulk_node->title);

		$old_domain_nid = sirius_domain_nid();
		$new_domain_nid = $bulk_node->field_sirius_domain['und'][0]['target_id'];
		if ($new_domain_nid != $old_domain_nid) {
			$new_domain = sirius_domain($new_domain_nid);
			sirius_minilog('Switched into domain: ' . $new_domain->title);
		}

  	$result = sirius_bulk_deliver_batch($bulk_node, $force);

		if ($new_domain_nid != $old_domain_nid) {
			$old_domain = sirius_domain($old_domain_nid);
			sirius_minilog('Switched back into domain: ' . $old_domain->title);
		}

  	sirius_minilog("Delivered: " . $bulk_node->title . ': ' . $result['msg']);
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
		$json = sirius_jsonfield_util_node_json_get($bulk_node);
		$do_not_disurb_id = $json['do_not_disturb']['id'];
		if (!$do_not_disurb_id) { $do_not_disurb_id = 'default'; }
		$ok = sirius_do_not_disturb_check($do_not_disurb_id);
		// $ok = sirius_dispatch_notify_schedule_ok();
		if (!$ok['success']) { return $ok; }
	}

	// Get the recipients, both "add" and "add-manual".
	$batch_size = variable_get("sirius_bulk_batch_$medium", 50);
	$queue = array();
	foreach (array('add', 'add-manual') as $type) {
		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_log_handler', 'target_id', $bulk_node->nid);
	  $query->fieldCondition('field_sirius_category', 'value', 'bulk:queue');
		$query->fieldCondition('field_sirius_type', 'value', $type);
	  $query->range(0, $batch_size);
	  $stmt = $query->execute();
	  if ($stmt['node']) { $queue[$type] = array_keys($stmt['node']); }
	}

	// If there are no recipients, the bulk delivery is complete.
	if (!$queue['add'] && !$queue['add-manual']) {
  	$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'sent';
  	$bulk_node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d H:i:s');
  	node_save($bulk_node);
  	return array('success' => TRUE, 'msg' => 'No more pending recipients. Delivery complete.');
  }

  if (!$queue['add']) {
  	return array('success' => TRUE, 'msg' => 'There are no more pending recipients, but there are still some recipients requiring manual confirmation. Message delivery will continue.');
  }

  $count_success = $count_error = $count = 0;
  foreach ($queue['add'] as $log_nid) {
  	$result = sirius_bulk_deliver_entry($log_nid);
  	++$count; 
  	if ($result['success']) { ++$count_success; } else { ++$count_error; }
  	sirius_minilog("Delivery to recipient #$log_nid via " . $result['medium'] . ": " . $result['to'] . ': ' . $result['msg']);
  }

	return array('success' => TRUE, 'msg' => "Attempted delivery of $count messages, with $count_success success and $count_error error.");;
}

function sirius_bulk_deliver_entry($log_nid) {
	// Get the log node
	$log_node = node_load($log_nid);
	if (!$log_node) { return array('success' => FALSE, 'msg' => 'No queue entry.'); }
	if ($log_node->field_sirius_category['und'][0]['value'] != 'bulk:queue') { return  array('success' => FALSE, 'msg' => 'Not a queue entry.'); }
	if ($log_node->field_sirius_type['und'][0]['value'] != 'add') { return array('success' => FALSE, 'msg' => 'Not a pending queue entry.'); }

	// Get the bulk and contact nodes
	$handler_nids = sirius_fieldvals($log_node, 'field_sirius_log_handler', 'target_id');
	$handler_nodes = node_load_multiple($handler_nids);
	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_bulk') { $bulk_node = $handler_node; }
		if ($handler_node->type == 'sirius_contact') { $contact_node = $handler_node; }
	}
	if (!$bulk_node) { return array('success' => FALSE, 'msg' => 'No bulk node.'); }
	if (!$contact_node) { 
		$result = array('success' => FALSE, 'msg' => 'No contact node'); 
	} else {
		// Get the medium
		$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
		if ($medium == 'choice') {
			$worker_node = sirius_worker_get_by_contact_nid($contact_node->nid);
			if ($worker_node) {
				$medium = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
			}
		}
		if (!$medium || $medium == 'choice' || $medium == 'none') {
			$medium = 'email';
		}

		// Get the recipient
		if ($medium == 'email') { $to = sirius_email_format($contact_node->field_sirius_email['und'][0]['value']); }
		else { $to = sirius_phone_format($contact_node->field_sirius_phone['und'][0]['value']); }

		$result = sirius_bulk_deliver_impl($bulk_node, $medium, $to);
		$result['to'] = $to;
		$result['medium'] = $medium;
	}

	if ($result['success']) {
		$log_node->field_sirius_type['und'][0]['value'] = 'sent';
	} else {
		$log_node->field_sirius_type['und'][0]['value'] = 'error';
	}
	$log_node->field_sirius_summary['und'][0]['value'] .= $result['msg'];
	node_save($log_node);

	return $result;
}

function sirius_bulk_deliver_impl($bulk_node, $medium, $to, $is_test = FALSE) {
	$json = sirius_json_get($bulk_node); 

	if (!$to) { return array('success' => FALSE, 'msg' => 'No recipient.'); }
	$params = array();
	$params['handler_nids'] = array($bulk_node->nid);
	$params['to'] = $to;
	$params['subject'] = $bulk_node->title;
	$params['body'] = $bulk_node->body['und'][0]['value'];
	$params['sms'] = $bulk_node->field_sirius_sms['und'][0]['value'];
	$params['voice'] = $json['voice']['text']['human'];
	$params['voice_machine'] = $json['voice']['text']['machine'];
	$params['callerid_nid'] = $bulk_node->field_sirius_callerids['und'][0]['target_id'];

	if ($bulk_node->field_sirius_audio_public['und'][0]['uri']) { 
		$params['phonetree']['play']['url'] = file_create_url($bulk_node->field_sirius_audio_public['und'][0]['uri']);
	}
	if ($bulk_node->field_sirius_audio_public['und'][1]['uri']) { 
		$params['phonetree']['play']['url_machine'] = file_create_url($bulk_node->field_sirius_audio_public['und'][1]['uri']);
	}

	$params['phonetree']['disable'] = TRUE;
	$params['amd']['MachineDetection'] = 'DetectMessageEnd';

	if (!$is_test) {
		$params['X-Sirius-Id'] = "UNIQUE/bulk/" . $bulk_node->nid . '/' . $to;
	}

	if ($medium == 'email') {
		return sirius_email_send($params);
	} else if ($medium == 'voice') {
		if ($json['voice']['type'] == 'flow') {
			// @todo: Wrap in a sirius() function, rather than bypassing sirius and handing directly to Twilio
			$flow_sid = $json['voice']['flow']['sid'];
			sirius_twilio_outgoing_flow(
				$flow_sid, 
				$to, 
				$bulk_node->field_sirius_callerids['und'][0]['target_id'], 
				array(), 
				$bulk_node->nid
			);
		} else {
			return sirius_voice_send($params);
		}
	} else {
		return sirius_sms_send($params);
	}
}