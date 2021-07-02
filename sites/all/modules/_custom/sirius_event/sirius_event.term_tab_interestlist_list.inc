<?php

function sirius_event_term_tab_interestlist_list_access($event_type_term) {
	if (!sirius_event_interestlist_enabled()) { return FALSE; }
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }
	if (!sirius_event_interestlist_enabled_byterm($event_type_term)) { return FALSE; }
	return FALSE;
}

function sirius_event_term_tab_interestlist_list($event_type_term) {
	sirius_backlink_add('sirius/event/interestlists', array('name' => 'All Interest Lists'));
	return drupal_get_form('sirius_event_term_tab_interestlist_list_form', $event_type_term);
}

function sirius_event_term_tab_interestlist_list_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;

	$interestlist = sirius_event_interestlist_list($event_type_term, array('include_event_registrations' => TRUE));
	$pkeys = array();
	foreach ($interestlist as $record) {
		if ($record['register']) {
			foreach ($record['register'] as $pkey => $ignore) { 
				$pkeys[$pkey]++;
			}
		}
	}
	$pkeys = array_keys($pkeys);

	$pstatus_lookup = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
	$prole_lookup = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');

	$header = array();
	$header[] = t('Signup Date');
	$header[] = t('Name');
	$header[] = t('Contact');
	foreach ($pkeys as $pkey) {
		list ($prole_tid, $pstatus_tid) = explode('|', $pkey);
		$header[] = $prole_lookup[$prole_tid] . ': ' . $pstatus_lookup[$pstatus_tid];
	}
	$header[] = t('Comments');
	$header[] = t('Tools');

	$options = array();
	foreach ($interestlist as $record) {
		$tools = '';
		if ($record['worker_nid']) { $tools .= l('Worker', 'node/' . $record['worker_nid'], array('attributes' => array('class' => array('tool-button')))); }

		$title = $record['contact_title'];
		if ($record['worker_ws_name']) { $title .= '<br />' . $record['worker_ws_name']; }
		if ($record['worker_id']) { $title .= '<br />' . $record['worker_id']; }

		$contact_info = sirius_phone_render($record['contact_phone']) . '<br />' . sirius_email_render($record['contact_email']);

		$comments = $record['interestlist_json']['interestlist']['signup']['comments'];
		if ($record['worker_ms_name']) {
			if ($comments) { $comments .= '<hr>'; }
			$comments .= '(Member status: ' . $record['worker_ms_name'] . ')'; }

		$row = array();
		$row[] = date('Y-m-d\<\b\r \/\>h:i a', $record['interestlist_signup_ts']);
		$row[] = $title;
		$row[] = $contact_info;
		foreach ($pkeys as $pkey) {
			$count = $record['register'][$pkey]['count'];
			if (!$count) { $count = 0; }
			$last_ts = $record['register'][$pkey]['last_ts'];
			$register_string = "$count records";
			if ($last_ts) { $register_string .= '<br />' . date('Y-m-d', $last_ts); }
			$row[] = $register_string;
		}
		$row[] = $comments;
		$row[] = $tools;

		$options[$record['interestlist_nid']] = $row; 
	}

	if (!$options) {
		drupal_set_message('Nobody has signed up for this interest list.', 'warning');
		return $form;
	}

	$form['filters'] = array(
		'#title' => t('Filters'),
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		// '#collapsed' => TRUE,
	);

	$ms_level_options = sirius_ledger_ms_level_options();
	if ($ms_level_options) {
		$form['filters']['ms_level'] = array(
			'#title' => t('Member Status'),
			'#options' => $ms_level_options,
			'#type' => 'checkboxes',
			// '#prefix' => "<div class=\"checkboxes-inline\">",
			// '#suffix' => '</div><br clear="left" />',
			// '#multiple' => TRUE,
			// '#attributes' => array('class' => array('sirius_twocol_select')),
			// '#multiple' => TRUE,
			// '#type' => 'select',
			// '#type' => 'multiselect',
	    // '#multiple' => TRUE,
	    // '#size' => 15,
	    // '#required' => TRUE,
	    // '#name' => 'ms_level',
		);
	}

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Filter'),
	);

	$form['interestlist'] = array(
		'#type' => 'tableselect',
		'#title' => 'Signups',
		'#header' => $header,
		'#options' => $options,
	);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			NULL => t('-- Select --'),
			'csv' => t('Export to Spreadsheet'),
			'cancel' => t('Cancel interestlist signups'),
			'bulk' => t('Create a bulk message'),
			'event' => t('Register or invite to an event'),
		),
	);

	$media = sirius_bulk_media();
  $form["medium"] = array(
    '#title' => t('Medum'),
		'#type' => 'select',
		'#options' => $media,
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'bulk'))),
  );

	$event_options = sirius_event_upcoming();
	$form['event_nid'] = array(
		'#type' => 'select',
		'#title' => 'Event',
		'#options' => $event_options,
		'#states' => array('visible' => array(':input[name="action"]' => array('value' => 'event'))),
	);

	$form['comments'] = array(
		'#type' => 'textarea',
		'#title' => 'Comments',
		'#states' => array('visible' => array(':input[name="action"]' => array('value' => 'cancel'))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_interestlist_list_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$action = $form_state['values']['action'];

	if (!$action) { 
		$form_state['rebuild'] = TRUE;
		return;
	}

	// Handle redirect to register
	if ($action == 'event') {
		$event_nid = $form_state['values']['event_nid'];
		if (!$event_nid) { 
			drupal_set_message("Please select an event.", 'error');
			return;
		}

		$interestlist_nids = array_filter(array_values($form_state['values']['interestlist']));
		$interestlist_nodes = node_load_multiple($interestlist_nids);
		foreach ($interestlist_nodes as $interestlist_node) {
			$contact_node = sirius_log_handler($interestlist_node, 'sirius_contact');
			if (!$contact_node) { continue; }
			$worker_node = sirius_worker_get_by_contact_nid($contact_node->nid);
			if (!$worker_node) { continue; }
			$worker_nids[] = $worker_node->nid;
		}
		if (!$worker_nids) { 
			drupal_set_message("Please select at least one worker", 'error');
			return;
		}

		$widget = 'worker_byname';
		$result = sirius_worker_stupid_event_widget_finder($event_nid, $widget);
		if (!$result['success']) {
			drupal_set_message($result['msg'], 'error');
		} else {
			$path = 'node/' . $event_nid . '/sirius_event_participants_register';
			$query = array(
				'delta' => $result['delta'],
				'widget' => $widget,
				'worker_nids' => join(',', $worker_nids),
			);
			drupal_goto($path, array('query' => $query));
		}
	}


	if ($action == 'csv') {
		$interestlist = sirius_event_interestlist_list($event_type_term, array());

		$fp = fopen('php://output', 'w');
		$filename = 'interestlist_' . $event_type_term->name;
		$filename = strtolower($filename);
		$filename = preg_replace('/[^a-z0-9_]/', '_', $filename);
		$filename = preg_replace('/_+/', '_', $filename);
		$filename .= '.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		$header = array(
			'NID',
			'Date',
			'Comments',
			'Contact NID',
			'Name',
			'Contact Phone',
			'Contact Email',
			'Worker NID',
			'Worker ID',
			'Work Status',
			'Member Status',
		);
		fputcsv($fp, $header);
		foreach ($interestlist as $record) {
			$row = array();
			$row[] = $record['interestlist_nid'];
			$row[] = date('Y-m-d h:i a', $record['interestlist_signup_ts']);
			$row[] = $record['interestlist_json']['interestlist']['signup']['comments'];
			$row[] = $record['contact_nid'];
			$row[] = $record['contact_title'];
			$row[] = $record['contact_phone'];
			$row[] = $record['contact_email'];
			$row[] = $record['worker_nid'];
			$row[] = $record['worker_id'];
			$row[] = $record['worker_ws_name'];
			$row[] = $record['worker_ms_name'];
			fputcsv($fp, $row);
		}
		fclose($fp);
		exit();
	}

	$batch_params = array();

	if ($action == 'bulk') {
		$bulk_node = sirius_bulk_create('Interest List Announcement - ' . $event_type_term->name, $form_state['values']['medium']);
		$batch_params['bulk_nid'] = $bulk_node->nid;
		drupal_set_message("Bulk message created. " . l('Manage', 'node/' . $bulk_node->nid . '/sirius_bulk_medium', array('attributes' => array('class' => array('tool-button')))));
	}
	$batch_params['comments'] = $form_state['values']['comments'];

	$interestlist_nids = array_filter(array_values($form_state['values']['interestlist']));
	$interestlist_chunks = array_chunk($interestlist_nids, 100);
	$operations = array();
	foreach ($interestlist_chunks as $chunk) {
		$operations[] = array(
			'sirius_event_term_tab_interestlist_list_batch',
			array($chunk, $action, $batch_params),
		);
	}
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_event_term_tab_interestlist_list_batch_finished',
		'file' => drupal_get_path('module', 'sirius_event') . '/sirius_event.term_tab_interestlist_list.inc',
  );
  batch_set($batch);
}

function sirius_event_term_tab_interestlist_list_batch($interestlist_nids, $action, $batch_params, &$context) {
	$context['results']['action'] = $action;
	$context['results']['batch_params'] = $batch_params;
	$interestlist_nodes = node_load_multiple($interestlist_nids);

	foreach ($interestlist_nodes as $interestlist_nid => $interestlist_node) {
		++$context['results']['count'];

		if ($action == 'cancel') {
			$result = sirius_event_interestlist_cancel_impl($interestlist_node, array('interestlist' => array('cancel' => array('comments' => $batch_params['comments']))));
		} else if ($action == 'bulk') {
			$contact_node = sirius_log_handler($interestlist_node, 'sirius_contact');
			$result = sirius_bulk_recipient_add($batch_params['bulk_nid'], $contact_node->nid);
		}

		if ($result['success']) {
			++$context['results']['count_good'];
		} else {
			++$context['results']['count_bad'];
			drupal_set_message($result['msg'], 'warning');
		}
	}
}

function sirius_event_term_tab_interestlist_list_batch_finished($success, $results, $operations) {
	drupal_set_message(
		t(
			'Attempted <strong>@action</strong> on @count records with @count_good success and @count_bad failures.', 
			array(
				'@action' => $results['action'],
				'@count' => number_format($results['count']), 
				'@count_bad' => number_format($results['count_bad']),
				'@count_good' => number_format($results['count_good']),
			)
		)
	);
}
