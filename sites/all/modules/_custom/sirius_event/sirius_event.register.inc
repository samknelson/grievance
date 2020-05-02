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

	sirius_jsonfield_log($participant_node, 'registered', 'Participant registered.');
	node_save($participant_node);

	return array('success' => TRUE, 'msg' => 'Participant created.', 'nid' => $participant_node->nid);
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
