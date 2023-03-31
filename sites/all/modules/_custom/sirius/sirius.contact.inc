<?php

function sirius_node_access_contact($node, $op, $account) {
	return user_access('sirius admin');
}

function sirius_contact_sync_from_worker($worker_node) {
	$need_save = FALSE;
	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	if ($contact_nid) {
		$contact_node = node_load($contact_nid);
	}

	if (!$contact_node) {
		$contact_node = new stdClass();
		$contact_node->type = 'sirius_contact';
		$contact_node->no_recurse = TRUE;
		$contact_node->language = $worker_node->language;
		$contact_node->uid = $worker_node->uid;
		$contact_node->status = 1;
		$contact_node->promote = 0;
		$contact_node->comment = 0;
		$contact_node->title = $worker_node->title;
		$need_save = TRUE;
	}

	$field_names = array('field_sirius_name', 'field_sirius_email', 'field_sirius_phone', 'field_sirius_address', 'field_sirius_phone_alt', 'field_sirius_phone_fax', 'field_sirius_domain');
	foreach ($field_names as $field_name) {
		if (sirius_field_changed($worker_node, $contact_node, $field_name)) {
			$contact_node->{$field_name} = $worker_node->{$field_name};
			$need_save = TRUE;
		}
	}

	if ($need_save) {
		node_object_prepare($contact_node);
		$contact_node = node_submit($contact_node);
		node_save($contact_node);
	}

	if ($worker_node->field_sirius_contact['und'][0]['target_id'] != $contact_node->nid) { $worker_node->field_sirius_contact['und'][0]['target_id'] = $contact_node->nid; }

	return $contact_node;
}

function sirius_contact_sync_from_user($account) {
	if ($account->field_sirius_worker['und'][0]['target_id']) {
		// If the user references a worker, then that worker and the user have the same contact.
		$worker_node = node_load($account->field_sirius_worker['und'][0]['target_id']);
		$contact_node = sirius_contact_sync_from_worker($worker_node);
	} else {
		$need_save = FALSE;
		if ($account->field_sirius_contact['und'][0]['target_id']) {
			$contact_node = node_load($account->field_sirius_contact['und'][0]['target_id']);
		}

		if (!$contact_node) {
			$contact_node = new stdClass();
			$contact_node->type = 'sirius_contact';
			$contact_node->no_recurse = TRUE;
			$contact_node->language = 'und';
			$contact_node->uid = $account->uid;
			$contact_node->status = 1;
			$contact_node->promote = 0;
			$contact_node->comment = 0;
			$contact_node->title = $account->name;
			$need_save = TRUE;
		}

		if ($contact_node->field_sirius_email['und'][0]['value'] != $account->mail) { $contact_node->field_sirius_email['und'][0]['value'] = $account->mail; $need_save = TRUE; }

		$name_parts = preg_split('/[\s\.]+/', $account->name);
		$family = array_pop($name_parts);
		$given = array_shift($name_parts);
		$middle = join(' ', $name_parts);
		if ($family != $contact_node->field_sirius_name['und'][0]['family']) { $contact_node->field_sirius_name['und'][0]['family'] = $family; $need_save = TRUE; }
		if ($middle != $contact_node->field_sirius_name['und'][0]['middle']) { $contact_node->field_sirius_name['und'][0]['middle'] = $middle; $need_save = TRUE; }
		if ($given  != $contact_node->field_sirius_name['und'][0]['given'])  { $contact_node->field_sirius_name['und'][0]['given']  = $given;  $need_save = TRUE; }

		$phone = $account->field_grievance_phone['und'][0]['value'];
		if ($phone != $contact_node->field_sirius_phone['und'][0]['value']) { $contact_node->field_sirius_phone['und'][0]['value'] = $phone; $need_save = TRUE; }

		$phone_alt = $account->field_grievance_phone_off['und'][0]['value'];
		if ($phone_alt != $contact_node->field_sirius_phone_off['und'][0]['value']) { $contact_node->field_sirius_phone_off['und'][0]['value'] = $phone_alt; $need_save = TRUE; }

		$field_names = array('field_sirius_domain');
		foreach ($field_names as $field_name) {
			if (sirius_field_changed($account, $contact_node, $field_name)) {
				$contact_node->{$field_name} = $account->{$field_name};
				$need_save = TRUE;
			}
		}

		if ($need_save) { node_save($contact_node); }
	}

	if ($account->field_sirius_contact['und'][0]['target_id'] != $contact_node->nid) { $account->field_sirius_contact['und'][0]['target_id'] = $contact_node->nid; }

	return $contact_node;
}

function sirius_contact_nid_get_from_user($uid = NULL) {
	if (!$uid) { 
		global $user;
		$uid = $user->uid;
	}
	if (!$uid) { return NULL; }

	$sql = "select field_sirius_contact_target_id from field_data_field_sirius_contact ";
	$sql .= "where entity_type = 'user' ";
	$sql .= "and entity_id = :uid";
	$stmt = db_query($sql, array(':uid' => $uid));
	$hr = $stmt->fetchAssoc();
	return $hr['field_sirius_contact_target_id'];
}

function sirius_contact_sirius_jsonfield_form_definitions() {
	return array();
}

function sirius_contact_get_nid_from_worker($worker_nid) {
	$sql = "select field_sirius_contact_target_id from field_data_field_sirius_contact ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_worker' ";
	$sql .= "and entity_id = :worker_nid";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
	$hr = $stmt->fetchAssoc();
	return $hr['field_sirius_contact_target_id'];
}

function sirius_contact_list_sources() {
	$sources_by_domain =& drupal_static(__FUNCTION__, array());

	$domain_nid = sirius_domain_nid();
	if (!isset($sources_by_domain[$domain_nid])) {
	$sql_args = array();
		$sql = "select distinct field_sirius_source_value as source from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_source on field_data_field_sirius_source.entity_type = 'node' and field_data_field_sirius_source.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_contact' ";
		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		}
		$sql .= "order by field_sirius_source_value ";
		$stmt = db_query($sql, $sql_args);
		$sources_by_domain[$domain_nid] = array();
		while ($hr = $stmt->fetchAssoc()) {
			if ($hr['source']) { $sources_by_domain[$domain_nid][] = $hr['source']; }
		}
	}

	return $sources_by_domain[$domain_nid];
}

function sirius_contact_tag_add($contact_node, $tid) {
	$current_tids = sirius_fieldvals($contact_node, 'field_sirius_contact_tags', 'tid');
	if (in_array($tid, $current_tids)) { return array('success' => TRUE, 'msg' => 'Contact already had the tag.'); }

	$contact_node->field_sirius_contact_tags['und'][] = array('tid' => $tid); 
	node_save($contact_node);
	return array('success' => TRUE, 'msg' => 'Contact saved.');
}

function sirius_contact_tag_remove($contact_node, $tid) {
	$current_tids = sirius_fieldvals($contact_node, 'field_sirius_contact_tags', 'tid');
	if (!in_array($tid, $current_tids)) { return array('success' => TRUE, 'msg' => 'Contact already did not have the tag.'); }

	$new_value = array();
	foreach ($current_tids as $candidate_tid) {
		if ($candidate_tid == $tid) { continue; }
		$new_value[] = array('tid' => $candidate_tid); 
	}

	if ($new_value) 	{ $contact_node->field_sirius_contact_tags['und'] = $new_value; } 
	else 							{ unset($contact_node->field_sirius_contact_tags['und']); } 
	node_save($contact_node);
	return array('success' => TRUE, 'msg' => 'Contact saved.');
}

/**
 * Prevent deletion of a contact if it is referenced by a user or a worker, unless $node->no_recurse
 */

function sirius_contact_can_delete($contact_node) {
	if ($contact_node->type != 'sirius_contact') {
		return array('success' => FALSE, 'msg' => 'Not a contact node.');
	}

	$sql = "select 1 from field_data_field_sirius_contact ";
	$sql .= "where entity_type = 'user' ";
	$sql .= "and field_sirius_contact_target_id = :contact_nid ";
	$sql_args = array(':contact_nid' => $contact_node->nid);
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	if ($hr) {
		return array('success' => FALSE, 'msg' => 'This contact is still referenced by a user. The user must be deleted before the contact.');
	}

	$sql = "select 1 from field_data_field_sirius_contact ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_worker' ";
	$sql .= "and field_sirius_contact_target_id = :contact_nid ";
	$sql_args = array(':contact_nid' => $contact_node->nid);
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	if ($hr) { 
		return array('success' => FALSE, 'msg' => 'This contact is still referenced by a worker. The worker must be deleted before the contact.');
	}

	return array('success' => TRUE);
}

function sirius_contact_autocomplete($string, $s2 = NULL, $s3 = NULL, $s4 = NULL) {
	// If the string has a slash, it's not passed by the autocomplete function.
	// See https://www.drupal.org/project/drupal/issues/93854
	// So we use this monstrosity.
	if (isset($s2)) { $string .= "/$s2"; }
	if (isset($s3)) { $string .= "/$s3"; }
	if (isset($s4)) { $string .= "/$s4"; }

  $matches = array();
	if (strlen($string) < 3) { 
		drupal_json_output($matches);
		return;
	}

	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql_args[':match'] = '%' . $string . '%';
	$sql = "select nid, title from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_contact' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	$sql .= "and title like :match ";
	$sql .= "order by title limit 10 ";
	$stmt = sirius_sql_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$label = check_plain($hr['title']) . ' (' . $hr['nid'] . ')';
		$matches[$label] = $hr['title'];
	}
  drupal_json_output($matches);
}

function sirius_contact_entity_add($entity_node, $contact_nid, $type_tids) {
	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
	$entity_json['contacts'][$contact_nid]['type_tids'] = $type_tids;
	sirius_jsonfield_util_node_json_set($entity_node, $entity_json);
	node_save($entity_node);
}

function sirius_contact_entity_remove($entity_node, $contact_nid) {
	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
	unset($entity_json['contacts'][$contact_nid]); 
	sirius_jsonfield_util_node_json_set($entity_node, $entity_json);
	node_save($entity_node);
}

function sirius_contact_relationship_node_presave($relationship_node) {
	// Relationship is active only if it started before now, and it either hasn't ended yet, or it ended after now.
	$start_ts = sirius_fieldval($relationship_node, 'field_sirius_date_start', 'ts');
	$end_ts = sirius_fieldval($relationship_node, 'field_sirius_date_end', 'ts');
	$now_ts = time();

	if ($start_ts <= $now_ts && ($end_ts < 1 || $end_ts >= $now_ts)) {
		$relationship_node->field_sirius_active['und'][0]['value'] = 'Yes';
	} else {
		$relationship_node->field_sirius_active['und'][0]['value'] = 'No';
	}

	// Never change a value that already exists
	if ($relationship_node->field_sirius_count['und'][0]['value']) { return; }

	// Load all our existing relationships
	$contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
	$target_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
	if (!$contact_nid || !$target_nid) { return; }
	$relationships = sirius_contact_relationships(array('contact_nid' => $contact_nid));
	$by_seq = $by_target = array();
	foreach ($relationships as $other_relationship_nid => $other_relationship_node) {
		if ($other_relationship_nid == $relationship_node->nid) { continue; }
		$other_relationship_seq = $other_relationship_node->field_sirius_count['und'][0]['value'];
		if (!$other_relationship_seq) { continue; }
		$by_seq[$other_relationship_seq] = TRUE;
		$other_target_nid = $other_relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		if (!$other_target_nid) { continue; }
		$by_target[$other_target_nid] = $other_relationship_seq;
	}

	if ($by_target[$target_nid]) {
		// If there's another relationship with the same target, then we use that sequence.
		$relationship_node->field_sirius_count['und'][0]['value'] = $by_target[$target_nid];
	} else {
		// Assign a new sequence number. This is not my most elegant code.
		for ($i=1; $i<1000; ++$i) {
			if ($by_seq[$i]) { continue; }
			$relationship_node->field_sirius_count['und'][0]['value'] = $i;
			break;
		}
	}
}

function sirius_contact_relationship_node_form_alter(&$form, &$form_state, $form_id) {
	// Admins can see the full form
	if (user_access('sirius admin')) {
		drupal_set_message("Administrative user: Full form is displayed. Please use caution.", 'warning');
		return;
	}

	// "Active" field is computed, so don't edit manually
	if ($form['field_sirius_active']) { $form['field_sirius_active']['#access'] = FALSE; }

	// New record or existing record?
	$relationship_node = $form['#entity'];
	if ($relationship_node && $relationship_node->nid) {
		$is_edit = TRUE;
	}

	// You can never change the contacts
	$form['field_sirius_contact']['#disabled'] = TRUE;
	$form['field_sirius_contact_alt']['#disabled'] = TRUE;

	if ($is_edit) {
		// Existing record: You can't change the type, from, or to
		$form['field_sirius_contact_reltype']['#access'] = FALSE;
	} else {
		// New record: Relationship type is required
		$form['field_sirius_contact_reltype']['#required'] = TRUE;
		$form['field_sirius_contact_reltype']['und']['#required'] = TRUE;

		// Include some confirmation text
		$contact_node = node_load($_REQUEST['sirius_contact']);
		$other_contact_node = node_load($_REQUEST['sirius_contact_alt']);
		if (!$contact_node || !$other_contact_node) {
			drupal_set_message('Must receive valid contact nodes.', 'error');
			$form['#access'] = FALSE;
			return;
		}

		drupal_set_message("Creating a relationship FROM " . $contact_node->title . " TO " . $other_contact_node->title, 'warning');
	}
}

function sirius_contact_relationship_node_validate($node, $form, &$form_state) {
	$contact_nid = $node->field_sirius_contact['und'][0]['target_id'];
	$contact_node = node_load($contact_nid);
	$other_contact_nid = $node->field_sirius_contact_alt['und'][0]['target_id'];
	$other_contact_node = node_load($other_contact_nid);

	if (!$contact_nid) { form_set_error('field_sirius_contact', t('Please enter a contact.')); return; }
	if (!$contact_node) { form_set_error('field_sirius_contact', t('Contact not found.')); return; }
	if (!$other_contact_nid) { form_set_error('field_sirius_contact_alt', t('Please enter a second contact')); return; }
	if (!$other_contact_node) { form_set_error('field_sirius_contact_alt', t('Contact not found.')); return; }

	if ($contact_nid == $other_contact_nid) { form_set_error('field_sirius_contact_alt', t('The two contacts are the same.')); return; }

	// End date must be after start date
	$ts1 = strtotime($node->field_sirius_date_start['und'][0]['value']);
	$ts2 = strtotime($node->field_sirius_date_end['und'][0]['value']);
	if ($ts1 && $ts2 && $ts2 < $ts1) { form_set_error('field_sirius_date_end', t('End date must be after start date.')); return; }

	// Relationship type is required
	$reltype_tid = $node->field_sirius_contact_reltype['und'][0]['tid'];
	if (!$reltype_tid) { form_set_error('field_sirius_contact_reltype', t('Please select a relationship type.')); return; }

	// Can only have one spouse at a time
	$reltype_term = taxonomy_term_load($reltype_tid);
	$count = $reltype_term->field_sirius_count['und'][0]['value'];
	if ($count) {
		$params['active'] = 'Yes';
		$params['contact_nid'] = $contact_nid;
		$params['reltype_tid'] = $reltype_tid;
		$params['nids_only'] = TRUE;
		$params['exclude_nid'] = $node->nid;
		$relationships = sirius_contact_relationships($params);
		if ($count <= count($relationships)) {
			form_set_error('field_sirius_contact_reltype', "Only $count relationships of type " . $reltype_term->name . " are permitted."); return;
		}
	}
}

function sirius_contact_relationships($params) {
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_contact_relationship');
  if ($params['contact_nid']) {
	  $query->fieldCondition('field_sirius_contact', 'target_id', $params['contact_nid']);
  }
  if ($params['contact_alt_nid']) {
	  $query->fieldCondition('field_sirius_contact_alt', 'target_id', $params['contact_alt_nid']);
  }
  if ($params['reltype_tid']) {
	  $query->fieldCondition('field_sirius_contact_reltype', 'tid', $params['reltype_tid']);
  }
  if ($params['reltype_tids']) {
	  $query->fieldCondition('field_sirius_contact_reltype', 'tid', $params['reltype_tids'], 'in');
  }
  if ($params['exclude_nid']) {
	  $query->propertyCondition('nid', $params['exclude_nid'], '!=');
  }
  if ($params['active']) {
	  $query->fieldCondition('field_sirius_active', 'value', $params['active']);
  }
  $query->propertyOrderBy('created', 'desc');
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $query->range(0, $limit);
  // $query->fieldOrderBy('field_sirius_date_start', 'value', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }

  if ($params['nids_only']) {
  	return array_keys($stmt['node']);
  }

  // Can't use fieldOrderBy because it eliminates nulls
  $nodes = node_load_multiple(array_keys($stmt['node']));
  uksort($nodes, function($a, $b) use ($nodes) { 
  	$a_date = $nodes[$a]->field_sirius_date_start['und'][0]['value'];
  	$b_date = $nodes[$b]->field_sirius_date_start['und'][0]['value'];
  	return strcmp($b_date, $a_date);
  });

  return $nodes;
}