<?php

function sirius_node_tab_event_phonebank_call_access($event_node) {
	if (!sirius_node_tab_event_phonebank_access($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$caller_prole_tids = array_values($type_json['phonebank']['caller_prole_tids']);
	if (!in_array($prole_tid, $caller_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_phonebank_call($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	$active = sirius_phonebank_active($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['phonebank']['prole_tid'];
	$pstatus_tid = $event_json['phonebank']['pstatus_tid'];
	if (!$prole_tid || !$pstatus_tid) {
		return 'Please go to the settings page and select the role and status to call.';
	}

	$mode = $_REQUEST['mode'];
	if (!$mode) {
		// Release locks on this page, because we're not in the caller interface
		global $user;
		$lock_type = sirius_event_phonebank_lock_type($event_node);
		sirius_lock_release_by_uid($lock_type, $user->uid);

		if (!$active['success']) { drupal_set_message($active['msg']); }

		$html .= $event_json['phonebank']['caller_intro'];
		$html .= '<br />';
		$html .= sirius_fakerender_field('Current Time:', date('l, F d Y - g:ia'));
		$html .= '<br />';
		$html .= sirius_dispatch_view_render_field($event_node, 'field_sirius_daterepeat', 'Phone Bank Open');
		$html .= '<br />';

		$html .= l(t('Dry Run'), current_path(),  array('query' => array('mode' => 'test'), 'attributes' => array('class' => array('tool-button'))));
		if ($active['success']) { 
			$html .= l(t('Start Making Calls'), current_path(),  array('query' => array('mode' => 'live'), 'attributes' => array('class' => array('tool-button'))));
		}

		return $html;
	}

	// Get the participant
	if ($mode == 'live' && $active['success']) {
		$phone = sirius_event_phonebank_next_number($event_node, $prole_tid, $pstatus_tid);
		if (!$phone) { return 'There are no more calls to make!'; }
	} 

	if ($mode == 'direct') {
		$phone = sirius_phone_format($_REQUEST['phone']);
		if (!$phone) { return 'Called with "direct" mode, but no phone number was specified.'; }

		$lock_type = sirius_event_phonebank_lock_type($event_node);
	  $result = sirius_lock_obtain($lock_type, $phone);
	  if (!$result['success']) { return "Unable to obtain a lock: " . $result['msg']; }
		$participant_nodes = sirius_event_phonebank_participants_by_phone($event_node, $prole_tid, $phone);
		if (!$participant_nodes) { return "No event participant found with the phone number [$phone]."; }
	}

	return drupal_get_form('sirius_node_tab_event_phonebank_call_form', $event_node->nid, $phone, $mode);
}

