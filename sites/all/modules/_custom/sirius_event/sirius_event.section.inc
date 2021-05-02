<?php

function sirius_event_section_enabled_tid($event_type_tid) { return sirius_event_section_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_section_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['section']['enabled'] == 'Yes';
}


function sirius_event_section_enabled_nid($event_nid) { return sirius_event_section_enabled(node_load($event_nid)); }

function sirius_event_section_enabled($event_node) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['section']['enabled'] == 'Yes';
}

function sirius_event_section_title($section, $empty_title = '(new section)') {
	if ($section['title']) { $title = $section['title']; }
	if (!$title && ($section['ts'] > 1)) { $title = sirius_event_section_render_ts($section['ts']); }
	if (!$title) { $title = $section['uuid']; }
	if (!$title) { $title = $empty_title; }

	return $title;
}

function sirius_event_section_options($event_node, $counts = NULL, &$options_attributes = NULL) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$limit = $event_json['section']['limit'];
	$section_list = $event_json['section_list'];
	if (!$section_list) { $section_list = array(); }
	uksort($section_list, function($a, $b) use ($section_list) { return $section_list[$a]['weight'] <=> $section_list[$b]['weight']; });
	$options = array(NULL => t('-- Select --'));
	foreach ($section_list as $section_uuid => $section) {
		$prompt = sirius_event_section_title($section);
		$section_limit = $section['limit'];
		if (!$section_limit) { $section_limit = $limit; }
		if ($counts) {
			$count = $counts[$section_uuid];
			if (!$count) { $count = 0; }
			// $prompt .= " (" . $count . ' / ' . $section_limit . ")";
			if ($count >= $section_limit) { 
				$options_attributes[$section_uuid] = array('disabled' => TRUE);
				$prompt .= ' [FULL]';
			} 
		}
		$options[$section_uuid] = $prompt;
	}

	return $options;
}

/**
 * Which pstatus_tids are NOT excluded from the counts?
 */

function sirius_event_section_pstatus_tids($prole_tids) {
  // What statuses are we interested in? 
  $all_pstatus_tids = sirius_event_participant_role_status_tids($prole_tids);
  $all_pstatus_terms = taxonomy_term_load_multiple(array_keys($all_pstatus_tids));
  $pstatus_tids = array();
  foreach ($all_pstatus_terms as $pstatus_tid => $pstatus_term) {
  	$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
  	if ($pstatus_json['section']['exclude'] == 'Yes') { continue; }
  	$pstatus_tids[] = $pstatus_tid;
  }
  return $pstatus_tids;
}

function sirius_event_section_counts($event_node) {
  // What role(s) are we interested in?
  $event_json = sirius_jsonfield_util_node_json_get($event_node);
  $event_section_json = $event_json['section'];
  if (!$event_section_json['prole_tids']) { return array(); }
  $prole_tids = array_filter(array_values($event_section_json['prole_tids']));
  if (!$prole_tids) { return array(); }

  $pstatus_tids = sirius_event_section_pstatus_tids($prole_tids);
  if (!$pstatus_tids) { return array(); }

  // Write a query
	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select json_unquote(json_extract(field_sirius_json_value, '\$.section.uuid')) as section_uuid, ";
	$sql .= "count(node.nid) as participant_count ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and (";
	$delta = 0;
	foreach ($prole_tids as $prole_tid) {
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " field_sirius_event_prole_tid = :prole_tid_$delta ";
		$sql_args[":prole_tid_$delta"] = $prole_tid;
		++$delta;
	}
	$sql .= ') ';
	$sql .= "and (";
	$delta = 0;
	foreach ($pstatus_tids as $pstatus_tid) {
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " field_sirius_event_pstatus_tid = :pstatus_tid_$delta ";
		$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
		++$delta;
	}
	$sql .= ') ';
	$sql .= "and json_extract(field_sirius_json_value, '\$.section.uuid') is not null ";
	$sql .= "group by section_uuid ";
	$sql .= "limit 1000 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$counts = array();
	while ($hr = $stmt->fetchAssoc()) {
		$counts[$hr['section_uuid']] = $hr['participant_count'];
	}
	return $counts;
}

function sirius_event_section_participants($event_node) {
  // What role(s) are we interested in?
  $event_json = sirius_jsonfield_util_node_json_get($event_node);
  $event_section_json = $event_json['section'];
  if (!$event_section_json['prole_tids']) { return array(); }
  $prole_tids = array_filter(array_values($event_section_json['prole_tids']));
  if (!$prole_tids) { return array(); }

  $pstatus_tids = sirius_event_section_pstatus_tids($prole_tids);
  if (!$pstatus_tids) { return array(); }

  // Look up term names, to make life easier on client code
  $pstatus_names = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
  $prole_names = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');

  // Write a query
	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "contact_node.title as contact_title, "; 
	$sql .= "field_sirius_event_pstatus_tid as pstatus_tid, ";
	$sql .= "field_sirius_event_prole_tid as prole_tid, ";
	$sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.section.uuid')) as section_uuid, ";
	$sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.comments')) as participant_comments, ";
	$sql .= "worker_node.nid as worker_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_contact worker_contact on worker_contact.entity_type = 'node' and worker_contact.bundle = 'sirius_worker' and worker_contact.field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = worker_contact.entity_id ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and (";
	$delta = 0;
	foreach ($prole_tids as $prole_tid) {
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " field_sirius_event_prole_tid = :prole_tid_$delta ";
		$sql_args[":prole_tid_$delta"] = $prole_tid;
		++$delta;
	}
	$sql .= ') ';
	$sql .= "and (";
	$delta = 0;
	foreach ($pstatus_tids as $pstatus_tid) {
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " field_sirius_event_pstatus_tid = :pstatus_tid_$delta ";
		$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
		++$delta;
	}
	$sql .= ') ';
	$sql .= "order by case when section_uuid is null then 99999999999 when section_uuid = 0 then 99999999999 else section_uuid end, contact_title ";
	// $sql .= "and json_extract(field_sirius_json_value, '\$.section.ts') is not null ";
	$sql .= "limit 1000 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$participants = array();
	while ($hr = $stmt->fetchAssoc()) {
		$hr['pstatus_name'] = $pstatus_names[$hr['pstatus_tid']];
		$hr['prole_name'] = $prole_names[$hr['prole_tid']];
		$participants[$hr['participant_nid']] = $hr;
	}

	/*
	uksort($participants, function ($a, $b) use ($participants) { 
		$a_ts = $participants[$a]['section_ts'];
		$b_ts = $participants[$b]['section_ts'];
		$a_name = $participants[$a]['contact_title'];
		$a_name = $participants[$a]['contact_title'];
		if (!$a_ts) { return 1; }
		if (!$b_ts) { return -1; }
		if ($a_ts > $b_ts) { return 1; }
		return -1; 
	});
	*/
	return $participants;
}

function sirius_event_section_render_ts($ts) {
	if (!$ts) { return '[no assigned section]'; }
	return date('l, F j, g:i A', $ts);
}

function sirius_event_section_assign($participant_nid, $uuid, $options = array()) {
	$participant_node = node_load($participant_nid);
	if (!$participant_node) { return array('success' => FALSE, 'msg' => t('Participant node not found.')); }
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	if (!$participant_json['section']) { $participant_json['section'] = array(); }
	$old_uuid = $participant_json['section']['uuid'];
	if ($uuid == $old_uuid) { return array('success' => TRUE, 'msg' => 'Section assignment was not changed', 'changed' => FALSE); }

	if (!$options['allow_oversubscribe']) {
		$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
		$event_json = sirius_jsonfield_util_node_json_get($event_node);
		$counts = sirius_event_section_counts($event_node);
		$count = $counts[$uuid];
		$section_limit = $event_json['section_list'][$uuid]['limit'];
		if (($section_limit > 0) && ($count >= $section_limit)) {
			return array('success' => FALSE, 'msg' => 'The requested section is full.');
		}
	}

	$participant_json['section']['uuid'] = $uuid;
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);

	$msg = "Section assignment saved.";

	if ($options['notify']) {
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
		$event_json = sirius_jsonfield_util_node_json_get($event_node);
		$email = $contact_node->field_sirius_email['und'][0]['value'];
		$phone = $contact_node->field_sirius_phone['und'][0]['value'];
		$section_alias = sirius_event_section_alias($event_node);

		$old_section = $event_json['section_list'][$old_uuid];
		$new_section = $event_json['section_list'][$uuid];

    $params = array();
    $params['subject'] = "$section_alias Assignment Changed: " . $event_node->title;
    $params['body'] .= "Your $section_alias assignment for " . $event_node->title . " has been changed.<br /><br />";
    $params['body'] .= $options['notify']['comments'];
    $params['body'] .= "<br /><br />";
    $params['body'] .= "Old $section_alias: " . sirius_event_section_title($old_section, '(no section)') . '<br />';
    $params['body'] .= "New $section_alias: <strong>" . sirius_event_section_title($new_section, '(no section)') . '</strong><br />';
    $params['sms'] .= "Your $section_alias assignment for " . $event_node->title . " has been changed.\n";
    $params['sms'] .= $options['notify']['comments'];
    $params['sms'] .= "\n\n";
    $params['sms'] .= "Old $section_alias: " . sirius_event_section_title($old_section, '(no section)') . "\n\n";
    $params['sms'] .= "New $section_alias: " . sirius_event_section_title($new_section, '(no section)');

    if ($options['notify']['medium'] == 'email' || $options['notify']['medium'] == 'both') {
	    $params['to'] = $email;
    	$result = sirius_email_send($params);
    	if ($result['success']) { $msg .= " Notified by email: $email."; } else { $msg .= " Email notification failed: $result[msg]"; }
    }

    if ($options['notify']['medium'] == 'sms' || $options['notify']['medium'] == 'both') {
	    $params['to'] = $phone;
    	$result = sirius_sms_send($params);
    	if ($result['success']) { $msg .= " Notified by SMS: $phone."; } else { $msg .= " SMS notification failed: $result[msg]"; }
    }
	}

	node_save($participant_node);

	return array('success' => TRUE, 'msg' => $msg, 'changed' => TRUE);

	drupal_set_message("Your selection has been saved.");
}

function sirius_event_section_get_from_participant($event_node, $participant_node) {
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	if (!$participant_json || !$participant_json['section']) { return NULL; }
	$uuid = $participant_json['section']['uuid'];
	if (!$uuid) { return NULL; }

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['section_list'][$uuid];
}

function sirius_event_section_alias($event_node) {
	$cache = &drupal_static(__FUNCTION__);
	if (!isset($cache)) { $cache = array(); }

	if (!$cache[$event_node->nid]) {
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
		$alias = $event_type_json['section']['section_alias'];
		if (!$alias) { $alias = 'Section'; }
		$cache[$event_node->nid] = $alias;
	}

	return $cache[$event_node->nid];
}
