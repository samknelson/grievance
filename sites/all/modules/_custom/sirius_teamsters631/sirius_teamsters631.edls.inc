<?php

/**
 * Implements hook_sirius_edls_notify_pre_send_alter
 */

function sirius_teamsters631_sirius_edls_notify_pre_send_alter(&$context) {
	$employer_json = sirius_json_get($context['employer_node']);

	// Set the "From"
	$context['notify_params']['sms_from'] = $employer_json['edls']['teamsters631']['twilio']['phone'];

	// Change the client
	sirius_teamsters631_edls_twilio_client_get($context['employer_node']);
}

/**
 * Implements hook_sirius_edls_notify_post_send_alter
 */

function sirius_teamsters631_sirius_edls_notify_post_send_alter(&$context) {
	// Restore
	sirius_teamsters631_edls_twilio_client_get(NULL);
}

function sirius_teamsters631_edls_twilio_client_get($employer_node) {
	// Cache all Twilio connections by employer NID
	$cache = &drupal_static(__FUNCTION__);
	if (!isset($cache)) { $cache = array(); }

	// Remember our default client
	if (!isset($cache['DEFAULT'])) {
		$cache['DEFAULT'] = sirius_twilio_client_get();
	}

	// Which client do we want?
	if ($employer_node) {
		$cache_key = $employer_node->nid;
	} else {
		$cache_key = 'DEFAULT';
	}

	// Connect if necessary
	if (!isset($cache[$cache_key])) {
		$employer_json = sirius_json_get($employer_node);

    $account_sid = $employer_json['edls']['teamsters631']['twilio']['account_sid'];
    $account_token = $employer_json['edls']['teamsters631']['twilio']['account_token'];
    
    if ($account_sid) {
			$cache[$cache_key] = sirius_twilio_client_connect(array(
				'provider' => sirius_twilio_provider_get(), 
				'account_sid' => $account_sid,
				'account_token' => $account_token,
			));
		} else {
			$cache[$cache_key] = $cache['DEFAULT'];
		}
	}

	return sirius_twilio_client_get($cache[$cache_key]);
}

function sirius_teamsters631_edls_apprentice_fetch($mode = 'test') {

	$employers = sirius_edls_employer_list();

	$count = $count_employers = $count_added = $count_removed = 0;

	foreach ($employers as $employer_nid => $employer_name) {
		$change_count = 0;
		sirius_minilog("Fetching apprentices for $employer_name");
		$employer_node = node_load($employer_nid);
		if (!$employer_node) {
			sirius_minilog("... $employer_name not found.", 'warning');
			continue;
		}

		$employer_id = $employer_node->field_sirius_id['und'][0]['value'];
		if (!$employer_id) {
			sirius_minilog("... $employer_name does not have an id (field_sirius_id).", 'warning');
			continue;
		}

		$result = sirius_service_client_request('sirius_teamsters631_webservice', 'apprentice_list', array('employer_id' => $employer_id));
		if (!$result['success']) {
			sirius_minilog("... Apprentice fetch for $employer_name failed: " . $result['msg']);
			continue;
		}

		$fetch_result = $result['data'];
		if (!$fetch_result) { 
			sirius_minilog("... Apprentice fetch for $employer_name did not return the expected variable ('data')", 'warning');
			continue;
		}

		if (!$fetch_result['success']) { 
			sirius_minilog("... Apprentice fetch for $employer_name returned an error result: " . $fetch_result['msg'], 'warning');
			continue;
		}

		// Our new, or currently dispatched, apprentices
		$new_records = $fetch_result['data'];

		// Outsourced worker IDs are always prefixed with an "O", and member status "APP"
		foreach ($new_records as $delta => $record) {
			$new_records[$delta]['worker_id'] = 'O' . $new_records[$delta]['worker_id'];
			$new_records[$delta]['worker_ms'] = 'APP';
		}

		// Our old, currently configured workers
		$old_records = sirius_edls_outsource_list($employer_nid);

		// Index our lists by worker_id for easy access
		$new_records_by_id = array();
		$old_records_by_id = array();
		foreach ($new_records as $record) { $new_records_by_id[$record['worker_id']] = $record; }
		foreach ($old_records as $record) { $old_records_by_id[$record['worker_id']] = $record; }

		// Our "good" records, what we will eventually write to the database
		$good_records_by_id = $old_records_by_id;

		// Remove any old apprentices who aren't on the new list
		foreach ($good_records_by_id as $worker_id => $record) {
			if ($record['worker_ms'] != 'APP') { continue; }
			if ($new_records_by_id[$worker_id]) { continue; }

			sirius_minilog("... Removing: $record[worker_name] ($worker_id)");

			unset($good_records_by_id[$worker_id]);
			++$count_removed;
			++$change_count;
		}

		// Add any new apprentices who aren't on the old list
		foreach ($new_records_by_id as $worker_id => $record) {
			if ($good_records_by_id[$worker_id]) { continue; }

			sirius_minilog("... Adding: $record[worker_name] ($worker_id)");

			$good_records_by_id[$worker_id] = $record;
			++$count_added;
			++$change_count;
		}

		$count += $change_count;

		// No changes, no db write necessary.
		if (!$change_count) {
			sirius_minilog("... No new or stale apprentices found.");
			continue;
		}

		// Test mode, no db write necessary
		if ($mode != 'live') {
			sirius_minilog("Skipping database update because mode = $mode.");
			continue;
		}

		// Write to the database
		$good_records = array_values($good_records_by_id);
		$result = sirius_edls_outsource_set_all($employer_node, $good_records);
		if (!$result['success']) {
			sirius_minilog("... Database write for $employer_name failed: " . $result['msg'], 'warning');
		}
	}

	sirius_minilog("Apprentice fetch complete. Processed $count_employers employers. Removed $count_removed stale apprentices; added $count_added new apprentices, for a total of $count updates.");

	return array('success' => TRUE, 'count' => $count);
}