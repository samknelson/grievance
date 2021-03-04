<?php

function sirius_event_participant_register_user($event_nid, $uid, $participant_role_tid, $participant_status_tid, $json = NULL) {
  $user_full = user_load($uid);
  $contact_nid = $user_full->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) { return array('success' => FALSE, 'msg' => 'That user does not have an associated contact.'); }
  return sirius_event_participant_register($event_nid, $contact_nid, $participant_role_tid, $participant_status_tid, $json);
}

function sirius_event_participant_register($event_nid, $contact_nid, $participant_role_tid, $participant_status_tid, $json = NULL) {
	$participant_node = sirius_event_participant_get($event_nid, $contact_nid);
	if ($participant_node) {
		return array('success' => FALSE, 'msg' => 'That contact is already registered for the event.', 'code' => 1, 'participant_node' => $participant_node);
  }

	$domain_nid = sirius_domain_nid();
	$participant_node = new stdClass();
	$participant_node->type = 'sirius_event_participant';
	$participant_node->no_recurse = TRUE;
	$participant_node->language = $user->language;
	$participant_node->uid = $user->uid;
	$participant_node->status = 1;
	$participant_node->promote = 0;
	$participant_node->comment = 0;
	$participant_node->title = $form_state['values']['title'];
	$participant_node->field_sirius_event['und'][0]['target_id'] = $event_nid;
	$participant_node->field_sirius_contact['und'][0]['target_id'] = $contact_nid;
	$participant_node->field_sirius_event_prole['und'][0]['tid'] = $participant_role_tid;
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $participant_status_tid;
	$participant_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s');
	if ($json) { sirius_jsonfield_util_node_json_set($participant_node, $json); }

	$sql = "select title from node where nid = :nid";
	$stmt = sirius_sql_query($sql, array(':nid' => $event_nid));
	$hr = $stmt->fetchAssoc();
	$participant_node->title = $hr['title'];

	$sql = "select title from node where nid = :nid";
	$stmt = sirius_sql_query($sql, array(':nid' => $contact_nid));
	$hr = $stmt->fetchAssoc();
	$participant_node->title .= ': ' . $hr['title'];
	sirius_jsonfield_log($participant_node, 'registered', 'Participant registered.');
	node_save($participant_node);

	$msg = "Participant created.";
	if ($json['notification']['medium']) {
		$contact_node = node_load($contact_nid);
		$contact_email = $contact_node->field_sirius_email['und'][0]['value'];
		$contact_phone = $contact_node->field_sirius_phone['und'][0]['value'];

		if (in_array($json['notification']['medium'], array('sms', 'both')) && $contact_phone) {
			$params = array();
      $params['sms'] = $json['notification']['sms'];
      $params['to'] = $contact_phone;
      $result = sirius_sms_send($params);
      if ($result['success']) { $msg .= " SMS sent to: $contact_phone."; } else { $msg .= " SMS to $contact_phone failed: " . $result['msg']; }
    }

		if (in_array($json['notification']['medium'], array('email', 'both')) && $contact_email) {
			$params = array();
      $params['body'] = $json['notification']['email_body'];
      $params['subject'] = $json['notification']['email_subject'];
      $params['to'] = $contact_email;
      $result = sirius_email_send($params);
      if ($result['success']) { $msg .= " Email sent to: $contact_email."; } else { $msg .= " Email to $contact_email failed: " . $result['msg']; }
    }
	}

	return array('success' => TRUE, 'msg' => $msg, 'nid' => $participant_node->nid);
}

function sirius_event_participant_get($event_nid, $contact_nid) {
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
	$query->fieldCondition('field_sirius_event', 'target_id', $event_nid);
	$query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
  $stmt = $query->execute();
  if (!$stmt['node']) { return; }
  return node_load(array_keys($stmt['node'])[0]);
}

function sirius_event_register_widgets() {
	$info_all = &drupal_static(__FUNCTION__);
	if (!$info_all) {
		$info_all = sirius_event_register_widget_info_all();
	}

	$widgets = array();
	foreach ($info_all as $key => $info) {
		$widgets[$key] = $info['title'];
	}

	return $widgets;
}

function sirius_event_register_widget_info($key) {
	$info_all = &drupal_static(__FUNCTION__);
	if (!$info_all) {
		$info_all = sirius_event_register_widget_info_all();
	}

	return $info_all[$key];
}

function sirius_event_register_widget_info_all() {
	$info['user_select'] = array('title' => 'User Select');
	$info['user_byname'] = array('title' => 'User by Name');

	$info['user_self'] = array(
		'title' => 'Self Registration',
		'config_callback' => 'sirius_event_register_user_self_config_form'
	);

	$info['worker_byname'] = array('title' => 'Worker by Name');
	$info['contact_bulk'] = array('title' => 'Contact Bulk');
	$info['bullpen_signin'] = array('title' => 'Bullpen Signin');
	$info['intake'] = array(
		'title' => 'Intake',
		'config_callback' => 'sirius_event_register_intake_config_form'
	);

	return $info;
}