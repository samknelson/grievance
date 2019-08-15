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