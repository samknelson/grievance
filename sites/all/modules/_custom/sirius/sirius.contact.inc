<?php

function sirius_node_access_contact($node, $op, $account) {
	return user_access('sirius admin');
}

function sirius_contact_sync_from_worker($worker_node) {
	return;

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

	$field_names = array('field_sirius_name', 'field_sirius_email', 'field_sirius_phone', 'field_sirius_address', 'field_sirius_phone_alt', 'field_sirius_phone_fax');
	foreach ($field_names as $field_name) {
		if (sirius_field_changed($worker_node, $contact_node, $field_name)) {
			// drupal_set_message("Difference between <pre>" . print_r($worker_node->{$field_name}, 1) . "</pre> and <pre>" . print_r($contact_node->{$field_name}, 1) . '</pre>');
			$contact_node->{$field_name} = $worker_node->{$field_name};
			$need_save = TRUE;
		}
	}

	if ($need_save) {
		node_object_prepare($contact_node);
		$contact_node = node_submit($contact_node);
		node_save($contact_node);
	}

	if ($worker_node->field_sirius_contact['und'][0]['target_id'] != $contact_node->nid) {
		$worker_node->field_sirius_contact['und'][0]['target_id'] = $contact_node->nid; 
	}

	return $contact_node;
}

function sirius_contact_sync_from_user($account) {
	return; 
	
	$need_save = FALSE;

	watchdog('debug', "Synccing from " . $account->name);

	if ($account->field_sirius_worker['und'][0]['target_id']) {
		$worker_node = node_load($account->field_sirius_worker['und'][0]['target_id']);
		$contact_node = sirius_contact_sync_from_worker($worker_node);
	}
	else {
		if ($account->field_sirius_contact['und'][0]['target_id']) {
		} else {

		}
	}
}