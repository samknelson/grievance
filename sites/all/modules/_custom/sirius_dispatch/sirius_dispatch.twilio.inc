<?php

use Twilio\Rest\Client;
use Twilio\TwiML;

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

/**********************************************************************************
 * Voice
 **********************************************************************************/

/**
 * This is the root handler for all incoming calls, and all callbacks for existing calls.
 * - node: The last node in the log with this call_sid
 * - digits: The numbers entered by the caller
 * - call_sid: The SID of the call
 */

function sirius_dispatch_handle_voice($call_sid, $context) {
	$handler_nid = $context['handler_nid'];
	if ($handler_nid) {
		$handler_node = node_load($handler_nid);
	}
	$digits = $context['digits'];

	if ($digits == 9) {
		sirius_twilio_outgoing_voice_dial($call_sid, $context, "Please wait while we connect you with an operator.", variable_get('sirius_twilio_operator', ''));
	} else if ($handler_node && $handler_node->type == 'sirius_dispatch') {
		return sirius_dispatch_handle_voice_dispatch($call_sid, $context, $handler_node);
	} else if ($handler_node && $handler_node->type == 'sirius_worker') {
		return sirius_dispatch_handle_voice_worker($call_sid, $context, $handler_node);
	} else {
		return sirius_dispatch_handle_voice_null($call_sid, $context);
	}
}

/**
 * Root handler
 */

function sirius_dispatch_handle_voice_null($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	if ($digits == '*') { $digits = NULL; }

	// Get the bullpens
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch_job');
	$query->fieldCondition('field_sirius_dispatch_job_type', 'tid', sirius_get_special_tid('sirius_dispatch_job_type|BULL'));
  $query->fieldCondition('field_sirius_datetime', 'value', date('Y-m-d H:i:s', strtotime('-1 days')), '>=');
  $query->fieldOrderBy('field_sirius_datetime', 'value');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$bullpen_nodes = node_load_multiple(array_keys($stmt['node']));
  }

  if ($bullpen_nodes) {
		$root_menu = "Enter 1 to hear your dispatches. Enter 2 for the bullpen list. Enter 9 to be connected to an operator. ";
	} else {
		$root_menu = "Enter 1 to hear your dispatches. Enter 9 to be connected to an operator.";
	}


	if (!$expect) {

		if (!$digits) {

			// Root menu
			$text = "You have reached Teamster 6 3 1. "; 
			$text .= $root_menu;
			$gather_digits = 1;

		} else if ($digits == 1) {

			if ($context['worker_nid']) {

				// They've already authenticated themselves, just pass them to the worker handler
				$worker_node = node_load($context['worker_nid']);
				$context['expect'] = NULL;
				$context['digits'] = NULL;
				$context['handler_nid'] = $worker_node->nid;
				return sirius_dispatch_handle_voice_worker($call_sid, $context, $worker_node);

			} else {

				// Ask worker NID
				$text = "Please enter your worker ID.";
				$context['expect'] = 'worker_nid';
				$gather_digits = 6;
			}

		} else if ($digits == 2) {

			// Bullpen list
			if (!$bullpen_nodes) {
				$text = "There are no upcoming bullpens. ";
			} else {
				$i = 0;
				foreach ($bullpen_nodes as $bullpen_node) {
					++$i;
					$employer_node = node_load($bullpen_node->field_grievance_shop['und'][0]['target_id']);
					$facility_node = node_load($bullpen_node->field_sirius_dispatch_facility['und'][0]['target_id']);
					$ts = strtotime($bullpen_node->field_sirius_datetime['und'][0]['value']);
					$text .= "Bullpen number $i:\n";
					$text .= $employer_node->title . " on " . date('l, F d ', $ts) . ":\n\n";
					$text .= "Report to " . $facility_node->title . " at " . date('h:i a', $ts) . " : \n";
				}
			}
			$text .= "\nMain Menu\n";
			$text .= $root_menu;
			$gather_digits = 1;

		} else {

			// Error
			$text = "I'm sorry, I don't understand that response.";
			$gather_digits = 1;

		}

	} else if ($expect == 'worker_nid') {

		$worker_node = sirius_worker_get_by_id($digits);

		if (!$worker_node || $worker_node->type != 'sirius_worker') {

			// Not a worker
			$text = "That is not a valid worker ID. " . $root_menu;
			$context['expect'] = NULL;
			$gather_digits = 1;

		} else {

			// Ask AAT
			$text .= "Please enter your six-digit passcode.";
			$context['candidate_worker_nid'] = $worker_node->nid;
			$context['expect'] = 'aat';
			$gather_digits = 6;
		}

	} else if ($expect == 'aat') {

		$worker_nid = $context['candidate_worker_nid'];
		$worker_node = node_load($worker_nid);
		$aat = $digits;
		if (!$aat || $worker_node->field_sirius_aat['und'][0]['value'] != $aat) {
	
			// Not a good AAT
			$text = "The access code is not correct. " . $root_menu;
			$context['expect'] = NULL;
			$context['candidate_worker_nid'] = NULL;
			$gather_digits = 1;

		} else {

			// Worker handler
			$context['expect'] = NULL;
			$context['digits'] = NULL;
			$context['worker_nid'] = $worker_node->nid;
			$context['handler_nid'] = $worker_node->nid;
			return sirius_dispatch_handle_voice_worker($call_sid, $context, $worker_node);
		}

	} 

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, $gather_digits);
}

/**
 * Worker Handler
 */

function sirius_dispatch_handle_voice_worker($call_sid, $context, $worker_node) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	if ($digits == '*') { $digits = NULL; }
	if ($context['intro']) {
		$text = $context['intro'];
		$context['intro'] = NULL;
	}

	// For now, all a worker can do is respond to dispatches.
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('requested', 'notified', 'notification failed', 'expired'), 'in');
  $query->fieldCondition('field_sirius_worker', 'target_id', $worker_node->nid);
  $query->propertyCondition('created', time()-(60*60*24*10), '>=');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  if ($stmt['node']) {
		$dispatch_nodes = array_values(node_load_multiple(array_keys($stmt['node'])));
  }

	if (!$expect) {
	  if (!$dispatch_nodes) {

	  	// No dispatches.
			$text .= "There are no pending dispatches for you. ";
			$text .= "Enter 8 to return to the previous menu.\n";
			$text .= "Enter 9 to be connected with an operator.\n";
			$gather_digits = 1;

	  } else {

	  	foreach ($dispatch_nodes as $idx => $dispatch_node) {

	  		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
				$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
				$text .= "You have a dispatch from " . $employer_node->title . ": ... \n";
				$text .= "To select this dispatch, enter 1 $idx. . . .\n";
			}

			$text .= "Enter 0 8 to return to the previous menu.\n";
			$text .= "Enter 0 9 to be connected with an operator.\n";
			$text .= "Enter * to hear this message again.\n";
			$gather_digits = 2;

	  }
		$context['expect'] = 'dispatch_idx';

	} else if ($expect == 'dispatch_idx') {

		if ($digits == 8) {

			// Switch to NULL handler
			$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
			$context['expect'] = NULL;
			$context['handler_nid'] = NULL;
			$context['digits'] = NULL;
			return sirius_dispatch_handle_voice_null($call_sid, $context);

		} else {

			$idx = $digits;

			$idx = preg_replace('/^1/', '', $idx);
			$dispatch_node = $dispatch_nodes[$idx];
			if ($dispatch_node) {

				// Switch to the dispatch handler
				$context['expect'] = NULL;
				$context['digits'] = NULL;
				$context['handler_nid'] = $dispatch_node->nid;
				return sirius_dispatch_handle_voice_dispatch($call_sid, $context, $dispatch_node);

			} else {

				// Invalid entry, just try again.
				$context['expect'] = NULL;
				$context['digits'] = NULL;
				$context['intro'] = "I'm sorry, that is not a valid entry. ";
				return sirius_dispatch_handle_voice_worker($call_sid, $context, $worker_node);
			}
		}
	}

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, $gather_digits);
}

/**
 * Dispatch handler
 */

function sirius_dispatch_handle_voice_dispatch($call_sid, $context, $dispatch_node) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	if ($digits == '*') { $digits = NULL; }


	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);
	$tokens = array(
		'dispatch_node' => $dispatch_node,
		'job_node' => $job_node,
		'worker_node' => $worker_node,
	);
	$tokens['sirius']['dispatch']['job']['body_text'] = drupal_html_to_text($job_node->body['und'][0]['value']);

	$gather_digits = FALSE;
	if (!isset($digits) || $digits == 0) {
		$text = sirius_dispatch_message_text('voice', 'dispatch_notify', $tokens);
		$gather_digits = 1;
	} 

	else if ($digits == 1) {
		$text = sirius_dispatch_message_text('voice', 'dispatch_notify_details', $tokens);
		$gather_digits = 1;
	} 

	else if ($digits == 2) {
  	if (sirius_dispatch_accept($dispatch_node)) { 
			$text = sirius_dispatch_message_text('voice', 'dispatch_accept_confirm', $tokens);
  	} else {
			$text = sirius_dispatch_message_text('voice', 'dispatch_error', $tokens);
		}
	} 

	else if ($digits == 3) {
  	if (sirius_dispatch_decline($dispatch_node)) { 
			$text = sirius_dispatch_message_text('voice', 'dispatch_decline_confirm', $tokens);
  	} else {
			$text = sirius_dispatch_message_text('voice', 'dispatch_error', $tokens);
		}
	}

	else if ($digits == 8) {

		// Switch to worker handler
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$context['expect'] = NULL;
		$context['digits'] = NULL;
		$context['handler_nid'] = $worker_node->nid;
		return sirius_dispatch_handle_voice_worker($call_sid, $context, $worker_node);
	}

	else if ($digits == 9) {
		$text = sirius_dispatch_message_text('voice', 'dispatch_forward_to_operator', $tokens);
		// Forward to operator -- need a dial.
	}

	/*
	else if ($digits == 9) {
		$text = sirius_dispatch_message_text('voice', 'dispatch_callback_requested', $tokens);
	}
	*/

	else {
		$text = sirius_dispatch_message_text('voice', 'dispatch_notify_unknown', $tokens);
		$gather_digits = 1;
	}

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, $gather_digits);
}
