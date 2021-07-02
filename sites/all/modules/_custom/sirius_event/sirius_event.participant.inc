<?php

function sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid) {
	return array('success' => TRUE);	
}

function sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid, $notify = array()) {
	$possible = sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid);
	if (!$possible['success']) { return $possible; }

	if (!$prole_tid) { $prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid']; }
	if (!$pstatus_tid) { $pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid']; }

	if ($prole_tid == $participant_node->field_sirius_event_prole['und'][0]['tid'] && 
			$pstatus_tid == $participant_node->field_sirius_event_pstatus['und'][0]['tid']) {
		return array('success' => TRUE, 'msg' => 'No changes.');
	}

	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);

	$participant_node->field_sirius_event_prole['und'][0]['tid'] = $prole_tid;
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;

	global $user;
	static $term_cache;
	if (!$term_cache[$prole_tid]) { $term_cache[$prole_tid] = taxonomy_term_load($prole_tid); }
	if (!$term_cache[$pstatus_tid]) { $term_cache[$pstatus_tid] = taxonomy_term_load($pstatus_tid); }
	sirius_jsonfield_log($participant_node, 'status', "Set prole = " . $term_cache[$prole_tid]->name . ", pstatus = " . $term_cache[$pstatus_tid]->name);
	node_save($participant_node);

	$msg = 'Participant status changed. ';

	$notify_medium = $notify['medium'];
	if ($notify_medium) {
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$pstatus_term = taxonomy_term_load($pstatus_tid);
		$prole_term = taxonomy_term_load($prole_tid);
		$notify_body = $notify['body'];
	  global $base_url;
	  $url = $base_url . '/node/' . $event_node->nid . '/sirius_event_info';

	  if ($notify_medium == 'sms' || $notify_medium == 'both') {
			$phone = $contact_node->field_sirius_phone['und'][0]['value'];
			if (!$phone) { 
				$msg .= "Unable to notify by SMS: the contact has no phone number listed.";
			} else {
		    $params = array();
		    $params['sms'] = "Status update for " . $event_node->title . ": Role is " . $prole_term->name . " and status is " . $pstatus_term->name;
		    $params['sms'] .= "\n";
		    $params['sms'] .= $url;
		    if ($notify_body) {
		    	$params['sms'] .= "\n" . $notify_body;
		    }

	      $params['to'] = $phone;
	      $result = sirius_sms_send($params);
				if ($result['success']) { 
					$msg .= " Notified by SMS: $phone.";
					sirius_jsonfield_log($participant_node, "notify", "Notified by SMS: $phone: $params[sms]");
				} else {
					sirius_jsonfield_log($participant_node, "notify_fail", "Failed to notify by SMS: $phone: $result[msg]");
					$msg .= " " . $result['msg'];
				}
	    }
	  }

	  if ($notify_medium == 'email' || $notify_medium == 'both') {
			$email = $contact_node->field_sirius_email['und'][0]['value'];
			if (!$email) { 
				$msg .= "Unable to notify by Email: the contact has no email address listed.";
			} else {
		    $params = array();
		    $params['subject'] = "Status update for: " . $event_node->title;
		    $params['body'] = "Status update for " . $event_node->title . ": Role is " . $prole_term->name . " and status is " . $pstatus_term->name;
		    $params['body'] .= "<br /><br />";
		    $params['body'] .= "<a href=\"$url\">$url</a>";
		    if ($notify_body) {
		    	$params['body'] .= "<br /><br />" . $notify_body;
		    }

	      $params['to'] = $email;
	      $result = sirius_email_send($params);
				if ($result['success']) { 
					$msg .= " Notified by email: $email.";
					sirius_jsonfield_log($participant_node, "notify", "Notified by email: $email: $params[subject] $params[body]");
				} else {
					sirius_jsonfield_log($participant_node, "notify_fail", "Failed to notify by email: $email: $params[subject] $params[body]");
					$msg .= " " . $result['msg'];
				}
	    }
	    node_save($participant_node);
	  }
	}

	sirius_interestlist_auto_cancel($event_type_term, $participant_node);
	sirius_event_class_auto_skill($participant_node);

	return array('success' => TRUE, 'msg' => $msg);
}

/**
 * Return the full list of participant and events for a contact
 *
 * $result[participant_type_tid][0]['participant_node']
 * $result[participant_type_tid][0]['event_node']
 * $result[participant_type_tid][1]['participant_node']
 * $result[participant_type_tid][1]['event_node']
 *
 * @todo: This function loads too many things, especially when filtering by an event type and a contact has many participant records.
 */

function sirius_event_participants_by_contact($contact_nid, $event_type_tids = NULL) {
	// Get the full participant list for this contact
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
  $query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $participant_nids = array();
  if ($stmt['node']) { $participant_nids = array_keys($stmt['node']); }
  $participant_nodes = node_load_multiple($participant_nids);

  // By type
  $by_type = array();
  foreach ($participant_nodes as $participant_nid => $participant_node) {
  	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
  	$event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  	if ($event_type_tids && !in_array($event_type_tid, $event_type_tids)) { continue; }
		$by_type[$event_type_tid][] = array(
			'participant_node' => $participant_node,
			'event_node' => $event_node,
		);
  }

  return $by_type;
}

function sirius_event_participant_list_by_rule($event_nid, $rule, $limit = 0, $offset = 0) {
	$sql_args = array();
	$sql_args[':event_nid'] = $event_nid;
	$sql = "select node.nid as participant_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	if ($rule['participants']) {
		$sql .= "and ( ";
		$delta = 0;
		foreach ($rule['participants'] as $tmp) {
			list ($prole_tid, $pstatus_tid) = explode('|', $tmp);
			if ($delta > 0) { $sql .= ' or '; }
			$sql .= " (field_sirius_event_prole_tid = :prole_tid_$delta and field_sirius_event_pstatus_tid = :pstatus_tid_$delta ) ";
			$sql_args[":prole_tid_$delta"] = $prole_tid;
			$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
			++$delta;
		}
		$sql .= ') ';
	}
	if ($limit) { $sql .= "limit $limit offset $offset "; }

	$stmt = db_query($sql, $sql_args);
	$participant_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participant_nids[] = $hr['participant_nid'];
	}
	return $participant_nids;
}

function sirius_event_participant_summary($event_nid, $prole_tid = NULL) {
	$sql_args = array();
	$sql_args[':event_nid'] = $event_nid;
	$sql_args[':prole_tid'] = $prole_tid;
	$sql = "select field_sirius_event_pstatus_tid, count(distinct node.nid) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and field_sirius_event_prole_tid = :prole_tid ";
	$sql .= "group by field_sirius_event_pstatus_tid ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$data[$hr['field_sirius_event_pstatus_tid']] = $hr['c'];
	}
	return $data;
}

function sirius_event_participant_nid_from_contact($event_nid, $contact_nid) {
  if (!$event_nid || !$contact_nid) { return NULL; }

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
  $query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
  $query->fieldCondition('field_sirius_event', 'target_id', $event_nid);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  return array_keys($stmt['node'])[0];
}

function sirius_event_participant_delete_from_contact($event_nid, $contact_nid) {
  if (!$event_nid) { return array('success' => FALSE, 'msg' => 'No event.'); }
  if (!$contact_nid) { return array('success' => FALSE, 'msg' => 'No contact.'); }
  $participant_nid = sirius_event_participant_nid_from_contact($event_nid, $contact_nid);
  if (!$participant_nid) { return array('success' => FALSE, 'msg' => 'No participant record for this contact and event.'); }
  node_delete($participant_nid);
  return array('success' => TRUE, 'msg' => 'Participant record deleted.');
}

function sirius_event_participant_mypstatus_options($pstatus_json) {
	if (!$pstatus_json['mypstatus']['options']) { return array(); }
	$pstatus_tids = array_filter(array_values($pstatus_json['mypstatus']['options']));
	if (!$pstatus_tids) { return array(); }
	$sql = "select tid, name from taxonomy_term_data where tid in (:pstatus_tids) order by name";
	$stmt = sirius_sql_query($sql, array(':pstatus_tids' => $pstatus_tids));
	$options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$options[$hr['tid']] = $hr['name'];
	}
	return $options;
}

function sirius_event_participants_widgets() {
	$info_all = &drupal_static(__FUNCTION__);
	if (!$info_all) {
		$info_all = sirius_event_participants_widget_info_all();
	}

	$widgets = array();
	foreach ($info_all as $key => $info) {
		$widgets[$key] = $info['title'];
	}

	return $widgets;
}

function sirius_event_participants_widget_info($key) {
	$info_all = &drupal_static(__FUNCTION__);
	if (!$info_all) {
		$info_all = sirius_event_participants_widget_info_all();
	}

	return $info_all[$key];
}

function sirius_event_participants_widget_info_all() {
	$info = array();
	$info['list'] = array('title' => 'Roster');
	$info['reports'] = array('title' => 'Download reports');
	$info['summary'] = array('title' => 'View a Summary');
	$info['attendance'] = array('title' => 'Attendance Sheets');
	$info['certifications'] = array('title' => 'Certifications');
	$info['mightysearch_lite'] = array('title' => 'Advanced Search Lite');
	$info['mightysearch'] = array(
		'title' => 'Advanced Search', 
		'config_callback' => 'sirius_event_partcipant_mightysearch_config_form'
	);

	/*
	$info['intake'] = array(
		'title' => 'Intake',
		'config_callback' => 'sirius_event_register_intake_config_form'
	);
	*/

	return $info;
}