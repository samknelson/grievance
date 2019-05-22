<?php

/**********************************************************************************
 * SMS
 **********************************************************************************/

function sirius_dispatch_handle_sms($dispatch_node, $msg, $phone) {
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);

	$tokens = array(
		'dispatch_node' => $dispatch_node,
		'job_node' => $job_node,
		'worker_node' => $worker_node,

	);
	$tokens['sirius']['dispatch']['job']['body_text'] = drupal_html_to_text($job_node->body['und'][0]['value']);

	$msg = trim($msg);

  list($nid, $action) = preg_split('/\s+/', $msg);
  $action = trim(strtolower($action));

	// Requested details
  if ($action == 'info') {
		$text = sirius_dispatch_message_text('sms', 'dispatch_notify_details', $tokens);
		return sirius_twilio_outgoing_sms($phone, $text, $dispatch_node->nid);
  } 

  // Accept
  if ($action == 'accept') {
  	$result = sirius_dispatch_accept($dispatch_node);
  	if ($result['success']) {
			$text = sirius_dispatch_message_text('sms', 'dispatch_accept_confirm', $tokens);
  	} else {
	  	$tokens['sirius']['dispatch']['err'] = $result['msg'];
			$text = sirius_dispatch_message_text('sms', 'dispatch_error', $tokens);
		}
		return sirius_twilio_outgoing_sms($phone, $text, $dispatch_node->nid);
  }

  // Decline
  if ($action == 'decline') {
  	$result = sirius_dispatch_decline($dispatch_node);
  	if ($result['success']) {
			$text = sirius_dispatch_message_text('sms', 'dispatch_decline_confirm', $tokens);
  	} else {
	  	$tokens['sirius']['dispatch']['err'] = $result['msg'];
			$text = sirius_dispatch_message_text('sms', 'dispatch_error', $tokens);
		}
		return sirius_twilio_outgoing_sms($phone, $text, $dispatch_node->nid);
  }

  // Unknown response
	$text = sirius_dispatch_message_text('sms', 'dispatch_notify_unknown', $tokens);
	$result = sirius_twilio_outgoing_sms($phone, $text, $dispatch_node->nid);
	return $result;
}
