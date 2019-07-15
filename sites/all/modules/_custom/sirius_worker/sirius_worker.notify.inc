<?php

function sirius_worker_notify($worker_node, $params) {
	global $sirius_suppress_notifications;
	if ($sirius_suppress_notifications) { return array('success' => TRUE, 'msg' => 'Notifications suppressed.'); }
	$params['handler_nids'] = array($worker_node->nid);

	$medium = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
	if (!$medium) { $medium = 'email'; }
	if ($medium == 'email') 	{ $to = $worker_node->field_sirius_email['und'][0]['value']; }
	if ($medium == 'sms') 		{ $to = $worker_node->field_sirius_phone['und'][0]['value']; }
	if ($medium == 'sms_alt') { $to = $worker_node->field_sirius_phone_alt['und'][0]['value']; }

	if (!$to) {
		$msg = "The worker has no notification medium set.";
	  sirius_log('worker:notify', 'error', $msg, '', $params['X-Sirius-Id'], array($worker_node->nid), NULL, json_encode($params));
	  return array('success' => FALSE, 'msg' => $msg);
	}

	$params['to'] = $to;
	if ($medium == 'email') {
		return sirius_email_send($params);
	} else {
		return sirius_sms_send($params);
	}
}