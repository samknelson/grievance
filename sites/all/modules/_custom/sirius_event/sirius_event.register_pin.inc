<?php

function sirius_event_register_pin_enabled($event_node) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_json_get($event_type_term);
	return ($event_type_json['pin']['enabled'] == 'Yes');
}

function sirius_event_register_pin_possible($event_node) {
	$ts_settings = sirius_event_register_pin_ts($event_node);

  // Self sign-in must be open
  $event_json = sirius_json_get($event_node);
  if ($event_json['pin']['signin_status'] == 'closed') {
  	return array('success' => FALSE, 'msg' => 'Self sign-in is closed.');
  } else if ($event_json['pin']['signin_status'] == 'open') {
  	// Do nothing
  } else {
	  $datestr = 'g:i A, l, F j';
		if (!$ts_settings['event_ts']) {
			return array('success' => FALSE, 'msg' => 'Event date is not set.');
		}
	  if ($ts_settings['signin_opens_ts'] > time()) {
			return array('success' => FALSE, 'msg' => 'Self sign-in opens on ' . date($datestr, $ts_settings['signin_opens_ts']));
	  }
	  if (time() > $ts_settings['signin_closes_ts']) {
			return array('success' => FALSE, 'msg' => 'Self sign-in closes on ' . date($datestr, $ts_settings['signin_closes_ts']));
	  }
	}

  // Must have a worker
  $worker_node = sirius_worker_get();
  if (!$worker_node) {
    return array('success' => FALSE, 'msg' => 'There is no worker associated with your user.');
  }

  // Must have a contact
  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
  if (!$contact_node) {
    return array('success' => FALSE, 'msg' => 'There is no contact associated with your user.');
  }

  return array('success' => TRUE, 'msg' => 'Self sign-in here.');
}

function sirius_event_register_pin_ts($event_node) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_json_get($event_type_term);
	$pin_json = $event_type_json['pin'];

	$signin_opens_delta = $pin_json['signin_opens'];
	if (!$signin_opens_delta) { $signin_opens_delta = 60*30; }

	$signin_grace_delta = $pin_json['signin_grace'];
	if (!$signin_grace_delta) { $signin_grace_delta = 60*30; }

	$event_ts = sirius_fieldval($event_node, 'field_sirius_daterepeat', 'ts');
	$signin_opens_ts = $event_ts - $signin_opens_delta;
	$signin_closes_ts = $event_ts + $signin_grace_delta;

	return array(
		'event_ts' => $event_ts,
		'signin_opens_ts' => $signin_opens_ts,
		'signin_closes_ts' => $signin_closes_ts,
	);
}

function sirius_event_register_pin_settings_access($event_node) {
	if (!sirius_event_register_pin_enabled($event_node)) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_event_register_pin_pin($event_node) {
	$event_json = sirius_json_get($event_node);
	if (!$event_json['pin']['pin']) {
		$pin = '';
		for ($i=0; $i<3; ++$i) {
			$pin .= sirius_aat_word() . ' ';
		}
		$pin = trim($pin);
		$event_json['pin']['pin'] = $pin;
		sirius_json_set($event_node, $event_json);
		node_save($event_node);
	}

	return $event_json['pin']['pin'];
}

function sirius_event_register_pin_register($event_node, $pin_input) {
	$result = sirius_event_register_pin_possible($event_node);
	if (!$result['success']) { return $result; }

  global $user;
  $user_full = user_load($user->uid);
  $contact_nid = $user_full->field_sirius_contact['und'][0]['target_id'];
  $event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
  $event_type_json = sirius_json_get($event_type_term);
	list ($prole_tid, $pstatus_tid) = explode('|', $event_type_json['pin']['register_as']);

  if (!$prole_tid || !$pstatus_tid) {
  	return array('success' => FALSE, 'msg' => 'The event is misconfigured; no prole or pstatus set.');
  }

  $pin_correct = strtolower(sirius_event_register_pin_pin($event_node));
  $pin_input = strtolower(trim($pin_input));
  if ($pin_input != $pin_correct) {
  	return array('success' => FALSE, 'msg' => 'Incorrect PIN.');
  }

	return sirius_event_participant_register($event_node->nid, $contact_nid, $prole_tid, $pstatus_tid);
}
