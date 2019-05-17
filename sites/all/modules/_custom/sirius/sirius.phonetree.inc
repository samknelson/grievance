<?php

function sirius_tts_title($node) {
	if ($node->field_sirius_name_tts['und'][0]['value']) {
		return $node->field_sirius_name_tts['und'][0]['value'];
	}
	return $node->title;
}

function sirius_tts_name($term) {
	if ($term->field_sirius_name_tts['und'][0]['value']) {
		return $term->field_sirius_name_tts['und'][0]['value'];
	}
	return $term->name;
}

/**
 * This is the root handler for all incoming calls, and all callbacks for existing calls.
 */

function sirius_phonetree($call_sid, $context) {
	$digits = $context['digits'];
	$handler = $context['handler'];

	// If we have an outgoing message, just say it.
	if ($context['outgoing_msg']) {
		$msg = $context['outgoing_msg'];
		unset($context['outgoing_msg']);
		$msg = "This is an automated call from " . variable_get('sirius_twilio_site_name_tts', '') . ". " . $msg;
		$msg .= " Enter star to hear this message again. Enter 0 to return to the main menu.  Enter 9 to be connected with the operator. ";
		$msg .= "Message repeates. ";
		$msg .= "This is an automated call from " . variable_get('sirius_twilio_site_name_tts', '') . ". " . $msg;
		$msg .= " Enter star to hear this message again. Enter 0 to return to the main menu.  Enter 9 to be connected with the operator. ";
		$msg .= "Message repeates. ";
		$msg .= "This is an automated call from " . variable_get('sirius_twilio_site_name_tts', '') . ". " . $msg;
		$msg .= " Enter star to hear this message again. Enter 0 to return to the main menu.  Enter 9 to be connected with the operator. ";
		sirius_twilio_outgoing_voice_say($call_sid, $context, $msg, 1);
	}

	// Enter 9 to be connected with an operator.
	if ($digits == 9) {
		sirius_phonetree_forward_to_operator($call_sid, $context); 
	} 

	// Enter * to hear this message again.
	if ($digits == '*') {
		sirius_twilio_outgoing_voice_repeat($call_sid, $context);
	} 

	// All other actions require a login.
	$worker_nid = $context['worker_nid'];
	if (!$worker_nid) {
		sirius_phonetree_worker_login($call_sid, $context);
	}

	// Ok, we're logged in. What should we do?
	if ($handler == 'dispatch_list') {
		return sirius_phonetree_dispatch_list($call_sid, $context);
	} else if ($handler == 'dispatch') {
		return sirius_phonetree_dispatch($call_sid, $context);
	} else if ($handler == 'bullpen') {
		return sirius_phonetree_bullpen($call_sid, $context);
	} else if ($handler == 'dispatch_status') {
		return sirius_phonetree_dispatch_status($call_sid, $context);
	} else if ($handler == 'dispatch_current') {
		return sirius_phonetree_dispatch_current($call_sid, $context);
	} else {
		return sirius_phonetree_worker($call_sid, $context);
	}
}

/**
 * Login
 */

function sirius_phonetree_worker_login($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];

	if (!$expect) {

		$text .= "You have reached " . variable_get('sirius_twilio_site_name_tts', '') . ". Please enter your worker I D. "; 
		$text .= "You may enter 9 at any time to be connected with the operator. Enter star to hear this message again. ";
		$context['expect'] = 'worker_nid';
		sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 6);

	} else if ($expect == 'worker_nid') {

		$worker_node = sirius_worker_get_by_id($digits);
		if (!$worker_node || $worker_node->type != 'sirius_worker') {
			sirius_twilio_outgoing_voice_repeat($call_sid, $context, 'That is not a valid worker ID. ');
		} 

		$aat_required = $worker_node->field_sirius_aat_required['und'][0]['value'];

		if ($aat_required != 'Yes') {
			$context['worker_nid'] = $worker_node->nid;
			$context['expect'] = $context['digits'] = $context['handler'] = NULL;
			sirius_phonetree($call_sid, $context);
		}

		// Ask AAT
		$context['candidate_worker_nid'] = $worker_node->nid;
		$context['expect'] = 'aat';
		sirius_twilio_outgoing_voice_say($call_sid, $context, "Please enter your passcode.", 7);

	} else if ($expect == 'aat') {

		$worker_nid = $context['candidate_worker_nid'];
		$worker_node = node_load($worker_nid);
		$aat = $digits;
		$aat_target = sirius_phone_keypad($worker_node->field_sirius_aat['und'][0]['value']);
		if (!$aat || ($aat != $aat_target)) {
	
			// Not a good AAT
			sirius_twilio_outgoing_voice_repeat($call_sid, $context, 'That is not the correct access code. ');

		} else {

			// Worker handler
			$context['expect'] = $context['digits'] = $context['handler'] = NULL;
			$context['worker_nid'] = $worker_node->nid;
			sirius_phonetree($call_sid, $context);

		}
	} 
}

function sirius_phonetree_worker($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];

	if ($digits == 1) {
		$context['digits'] = NULL;
		$context['handler'] = 'dispatch_list';
		sirius_phonetree($call_sid, $context);
	} else if ($digits == 2) {
		$context['digits'] = NULL;
		$context['handler'] = 'bullpen';
		sirius_phonetree($call_sid, $context);
	} else if ($digits == 3) {
		$context['digits'] = NULL;
		$context['handler'] = 'dispatch_status';
		sirius_phonetree($call_sid, $context);
	} else if ($digits == 4) {
		$context['digits'] = NULL;
		$context['handler'] = 'dispatch_current';
		sirius_phonetree($call_sid, $context);
	} 

	$worker_node = node_load($context['worker_nid']);
	$status_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	$text = "Hi, " . $worker_node->field_sirius_name['und'][0]['given'] . "!\n";
	if ($status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$text .= "Enter 1 to hear your dispatches.\n";
	}
	$text .= "Enter 2 to hear the bullpen list.\n";
	if ($status_tid != sirius_get_special_tid('sirius_worker_dispatch_status|employed')) {
		$text .= "Enter 3 to sign in or out of the out-of-work list.\n";
	}
	if ($status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|employed')) {
		$text .= "Enter 4 to hear your current dispatch.\n";
	}

	$text .= "Enter 9 at any time to be connected with the operator. ";
	$text .= "Enter star to hear this message again. ";

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 1);
}

function sirius_phonetree_bullpen($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	$worker_nid = $context['worker_nid'];
	$worker_node = node_load($worker_nid);
	$worker_member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');

	if ($digits == '8') {
		$context['digits'] = $context['handler'] = NULL;
		sirius_phonetree($call_sid, $context);
	}

	// Get the bullpens
	if ($worker_member_status_tids) {
	  $query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_dispatch_job');
		$query->fieldCondition('field_sirius_dispatch_job_type', 'tid', sirius_get_special_tid('sirius_dispatch_job_type|BULL'));
	  $query->fieldCondition('field_sirius_datetime', 'value', gmdate('Y-m-d H:i:s', strtotime('-1 days')), '>=');
	  $query->fieldCondition('field_sirius_member_status', 'tid', $worker_member_status_tids, 'in');
	  $query->fieldOrderBy('field_sirius_datetime', 'value');
	  $stmt = $query->execute();
	  if ($stmt['node']) {
	  	$bullpen_nodes = node_load_multiple(array_keys($stmt['node']));
	  }
	}

	// Bullpen list
	if (!$bullpen_nodes) {
		$text = "There are no upcoming bullpens.";
	} else {
		$i = 0;
		$count = count($bullpen_nodes);
		$text .= "There are $count upcoming bullpens. ";
		foreach ($bullpen_nodes as $bullpen_node) {
			++$i;
			$employer_node = node_load($bullpen_node->field_grievance_shop['und'][0]['target_id']);
			$facility_node = node_load($bullpen_node->field_sirius_dispatch_facility['und'][0]['target_id']);
			// $ts = strtotime($bullpen_node->field_sirius_datetime['und'][0]['value']);
			$ts = sirius_date_ts($bullpen_node, 'field_sirius_datetime');
			$text .= "Bullpen number $i:\n";
			$text .= sirius_tts_title($employer_node) . " on " . date('l, F d ', $ts) . ":\n\n";
			$member_status_tids = sirius_fieldvals($bullpen_node, 'field_sirius_member_status', 'tid');
			if ($member_status_tids) {

				$member_status_terms = taxonomy_term_load_multiple($member_status_tids);
				$text .= "This bullpen is open to the following lists: ";
				foreach ($member_status_terms as $tid => $term) {
					$text .= $term->name . ":\n\n";
				}
			}
			$text .= "Report to " . $facility_node->title . " at " . date('h:i a', $ts) . " : \n";
		}

		$text .= "Press star to hear this list again.\n";
	}
	$text .= 'Enter 8 to return to the previous menu. ';

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 1);
}


function sirius_phonetree_dispatch_status($call_sid, $context) {

	$expect = $context['expect'];
	$digits = $context['digits'];
	$worker_nid = $context['worker_nid'];
	$worker_node = node_load($worker_nid);
	$current_status_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	$current_status_term = taxonomy_term_load($current_status_tid);

	if ($current_status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$new_status_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
	} else {
		$new_status_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
	}

	$possible = sirius_dispatch_worker_dispstatus_set_possible($worker_node, $new_status_tid, sirius_worker_get_user($worker_node->nid));

	if ($digits == '8') {

		$context['digits'] = $context['handler'] = NULL;
		sirius_phonetree($call_sid, $context);

	} else if ($digits == 1) {

		$result = sirius_dispatch_worker_dispstatus_set(
			$worker_node, 
			$new_status_tid, 
			sirius_worker_get_user($worker_node->nid), 
			'Request via automated phone tree'
		);
		$current_status_tid = $new_status_tid;
		$current_status_term = taxonomy_term_load($current_status_tid);

		if ($result['success'] == FALSE) {

			sirius_twilio_outgoing_voice_repeat($call_sid, $context, "Your request failed: " . $result['msg']);

		} else {

			if ($new_status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
				$text = "You have been placed on the \"available\" list, and you will receive notifications about new dispatches. ";
			} else {
				$text = "You have been removed from the \"available\" list, and you will not receive notifications about new dispatches. ";
			}
		}
	} else if ($digits) {

		sirius_twilio_outgoing_voice_repeat($call_sid, $context, "I'm sorry, that is not a valid entry. ");

	}

	$text .= "Your current dispatch status is: " . $current_status_term->name . ". ";
	if ($possible['success']) {
		if ($new_status_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
			$text .= "Enter 1 to be placed on the \"available\" list for upcoming dispatches. ";
		} else {
			$text .= "Enter 1 to be removed from the \"available\" list for upcoming dispatches. ";
		}
	} else {
		$text .= "If you wish to change your dispatch status, please contact the office. ";
	}
	$text .= "Enter 8 to return to the previous menu. Enter 9 to be connected with an operator. Enter star to hear this message again. ";
	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 1);
}

function sirius_phonetree_dispatch_list($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	$worker_nid = $context['worker_nid'];
	$worker_node = node_load($worker_nid);

	// Fetch pending dispatches
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

	if ($digits == 8) {

		// Switch to NULL handler
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$context['expect'] = $context['handler'] = $context['digits'] = NULL;
		return sirius_phonetree($call_sid, $context);

	} else if ($digits) {

		$idx = $digits;

		$idx = preg_replace('/^1/', '', $idx);
		$dispatch_node = $dispatch_nodes[$idx];
		if ($dispatch_node) {

			// Switch to the dispatch handler
			$context['expect'] = NULL;
			$context['digits'] = NULL;
			$context['dispatch_nid'] = $dispatch_node->nid;
			$context['handler'] = 'dispatch';
			return sirius_phonetree($call_sid, $context);

		} else {

			// Invalid entry, just try again.
			sirius_twilio_outgoing_voice_repeat($call_sid, $context, "I'm sorry, that is not a valid entry. ");

		}
	}

  if (!$dispatch_nodes) {

  	// No dispatches.
		$text .= "There are no pending dispatches for you. ";

  } else {

  	foreach ($dispatch_nodes as $idx => $dispatch_node) {

  		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
			$text .= "You have a dispatch from " . sirius_tts_title($employer_node) . ": ... \n";
			$text .= "To select this dispatch, enter 1 $idx. . . .\n";
		}

  }

	$text .= "Enter 0 8 to return to the previous menu.\n";
	$text .= "Enter 0 9 to be connected with an operator.\n";
	$text .= "Enter star to hear this message again.\n";

  sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 2);
}

/**
 * Dispatch handler
 */

function sirius_phonetree_dispatch($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	$worker_nid = $context['worker_nid'];
	$worker_node = node_load($worker_nid);
	$dispatch_nid = $context['dispatch_nid'];
	$dispatch_node = node_load($dispatch_nid);

	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
	$job_type_term = taxonomy_term_load($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
	$facility_node = node_load($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$hall_node = node_load($job_node->field_sirius_dispatch_hall['und'][0]['target_id']);

	$tokens = array(
		'dispatch_node' => $dispatch_node,
		'job_node' => $job_node,
		'worker_node' => $worker_node,
	);
	$tokens['sirius']['dispatch']['job']['body_text'] = drupal_html_to_text($job_node->body['und'][0]['value']);

	$gather_digits = FALSE;
	if (!isset($digits) || $digits == 0) {

		$text = "Teamsters 6 3 1 is announcing a job from " . sirius_tts_title($employer_node) . "\n";
		$text .= "The call type is " . sirius_tts_name($job_type_term) . "\n";
		$text .= "The report date is " . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F d') . ' at ' . sirius_date_date($job_node, 'field_sirius_datetime', 'g i a') . "\n";
		$text .= "Press 1 for more details. ";
		$text .= "Press 2 to accept. ";
		$text .= "Press 3 to decline. ";
		$text .= "Press 8 to return to the previous menu. ";
		$text .= "Press 9 to be connected with the operator. ";
		$text .= "Press star to hear this message again.";

		$gather_digits = 1;
	} 

	else if ($digits == 1) {
		$text .= "The employer is " . sirius_tts_title($employer_node) . "\n";
		if ($facility_node) {
			$text .= "The facility is " .  sirius_tts_title($facility_node) . "\n";
		}
		// $text .= drupal_html_to_text($job_node->field_sirius_notes['und'][0]['value']) . "\n";
		$text .= "Press 1 for more details. ";
		$text .= "Press 2 to accept. ";
		$text .= "Press 3 to decline. ";
		$text .= "Press 8 to return to the previous menu. ";
		$text .= "Press 9 to be connected with the operator. ";
		$text .= "Press star to hear this message again.";

		$gather_digits = 1;
	} 

	else if ($digits == 2) {
		$result = sirius_dispatch_accept($dispatch_node);
  	if ($result['success']) { 
			$text = "The dispatch has been accepted. ";
			$text .= "The report time is " . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F d \a\t h:i a') . ". ";
			$text .= "Please report to: " . sirius_tts_title($facility_node) . ". ";
			if ($hall_node) { $text .= sirius_tts_title($hall_node) . ". "; }
			if ($job_node->field_sirius_address_notes['und'][0]['value']) { $text .= $job_node->field_sirius_address_notes['und'][0]['value'] . ". "; }
			$text .= "Press 8 to return to the previous menu. ";
			$text .= "Press 9 to be connected with the operator. ";
			$text .= "Press star to hear this message again.";
			$gather_digits = 1;
  	} else {
			$text = "We were unable to process your request: " . $result['msg'];
			$text .= "Press 8 to return to the previous menu. ";
			$text .= "Press 9 to be connected with the operator. ";
			$text .= "Press star to hear this message again.";
			$gather_digits = 1;
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
		$context['expect'] = $context['digits'] = $context['handler'] = $context['dispatch_nid'] = NULL;
		return sirius_phonetree($call_sid, $context);
	}

	else {
		$text = sirius_dispatch_message_text('voice', 'dispatch_notify_unknown', $tokens);
		$gather_digits = 1;
	}

	sirius_twilio_outgoing_voice_say($call_sid, $context, $text, $gather_digits);
}

/**
 * Current dispatch
 */

function sirius_phonetree_dispatch_current($call_sid, $context) {
	$expect = $context['expect'];
	$digits = $context['digits'];
	$worker_nid = $context['worker_nid'];
	$worker_node = node_load($worker_nid);
	$facility_node = node_load($job_node->field_sirius_facility['und'][0]['target_id']);

	if ($worker_node) {
		$dispatch_node = sirius_dispatch_get_current($worker_nid);
	}
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_type_term = taxonomy_term_load($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);

	if (!$dispatch_node) {
		$text = "You do not have a current dispatch.\n";
	} else {
		$text = "You are currently on a dispatch from " . sirius_tts_title($employer_node) . "\n";
		$text .= "The call type is " . sirius_tts_name($job_type_term) . "\n";
		$text .= "The report date is " . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F d') . ' at ' . sirius_date_date($job_node, 'field_sirius_datetime', 'g i a') . "\n";
		// $text .= drupal_html_to_text($job_node->field_sirius_notes['und'][0]['value']) . "\n";
	}

	if ($dispatch_node) {
		$possible = sirius_dispatch_resign_possible($dispatch_node);
		if ($possible['success']) {
			$text .= "Enter 6 to voluntary layoff this dispatch.\n";
		}
	}

	if ($digits == 8) {

		// Switch to NULL handler
		$context['expect'] = $context['handler'] = $context['digits'] = NULL;
		return sirius_phonetree($call_sid, $context);

	} else if ($digits == 6) {

		$possible = sirius_dispatch_resign($dispatch_node);
		if ($possible['success']) {
			$text = "You have gone on voluntary layoff for this dispatch.\n";
		} else {
			$text = "Voluntary layoff failed: " . $possible['msg'] . ".\n";
		}

	} else if ($digits) {

		// Invalid entry, just try again.
		sirius_twilio_outgoing_voice_repeat($call_sid, $context, "I'm sorry, that is not a valid entry. ");

	}

	$text .= "Enter 8 to return to the previous menu.\n";
	$text .= "Enter 9 to be connected with an operator.\n";
	$text .= "Enter star to hear this message again.\n";

  sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 2);
}


/**
 * Forwward to Operator
 */

function sirius_phonetree_forward_to_operator($call_sid, $context) {
	$status = variable_get('sirius_voice_forward_to_operator_status', 'open');
	if ($status == 'closed') {
		$which = variable_get('sirius_voice_forward_to_operator_disabled_which', 1);
		$text = variable_get("sirius_voice_forward_to_operator_disabled_$which", "We're sorry, the office is closed right now.");
		$text .= ": ";
		$text .= "Enter 8 to return to the previous menu.\n";
		$text .= "Enter star to hear this message again.\n";
		$context['expect'] = $context['handler'] = $context['digits'] = NULL;
	  sirius_twilio_outgoing_voice_say($call_sid, $context, $text, 1);
	}
	sirius_twilio_outgoing_voice_dial($call_sid, $context, "Please wait while we connect you with an operator.", variable_get('sirius_twilio_operator', ''));
}
