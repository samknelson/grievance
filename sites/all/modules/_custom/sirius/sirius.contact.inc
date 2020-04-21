<?php

function sirius_node_access_contact($node, $op, $account) {
	return user_access('sirius admin');
}

function sirius_contact_sync_from_worker($worker_node) {
	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

	$need_save = FALSE;

	if ($contact_nid) {
		$contact_node = node_load($contact_nid);
	} else {
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
		} else {
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