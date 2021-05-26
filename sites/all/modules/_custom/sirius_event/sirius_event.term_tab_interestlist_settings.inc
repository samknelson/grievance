<?php

function sirius_event_term_tab_interestlist_settings_access($event_type_term) {
	if (!sirius_event_interestlist_enabled()) { return FALSE; }
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }
	if (!sirius_event_interestlist_enabled_byterm($event_type_term)) { return FALSE; }
	return FALSE;
}

function sirius_event_term_tab_interestlist_settings($event_type_term) {
	sirius_backlink_add('sirius/event/interestlists', array('name' => 'All Interest Lists'));
	return drupal_get_form('sirius_event_term_tab_interestlist_settings_form', $event_type_term);
}

function sirius_event_term_tab_interestlist_settings_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$form['#tree'] = TRUE;

	$form['interestlist']['settings']['enabled'] = array(
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $event_type_json['interestlist']['settings']['enabled'],
		'#description' => t('Are interest lists enabled for this event type?'),
	);

	$paccess_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
	if ($paccess_options) {
		$form['interestlist']['settings']['auto_cancel'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Auto-Cancel'),
			'#description' => t('When a contact is added to for an event of this type with one of the selected participant roles and statuses, then they are automatically removed from the interest list.'),
			'#options' => $paccess_options,
		);
		if ($event_type_json['interestlist']['settings']['auto_cancel']) { $form['interestlist']['settings']['auto_cancel']['#default_value'] = $event_type_json['interestlist']['settings']['auto_cancel']; }
	}

	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	if ($ms_options) {
		$form['interestlist']['settings']['self_register']['ms_tids'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Self-Sign-Up Member Statuses'),
			'#description' => t('Restrict self-registration to the following member statuses. If no member statuses are selected, than any worker with appropriate permissions can sign up for the interest list.'),
			'#options' => $ms_options,
		);
		if ($event_type_json['interestlist']['settings']['self_register']['ms_tids']) {
			$form['interestlist']['settings']['self_register']['ms_tids']['#default_value'] = $event_type_json['interestlist']['settings']['self_register']['ms_tids'];
		}
	}

	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	if ($ws_options) {
		// field_sirius_dispatch_available
		// field_sirius_member_active

		$ws_terms = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
		$dispatch_available_names = array();
		$active_member_names = array();
		foreach ($ws_terms as $ws_tid => $ws_term) {
			if ($ws_term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') { $dispatch_available_names[] = $ws_term->name; }
			if ($ws_term->field_sirius_member_active['und'][0]['value'] == 'Yes') { $active_member_names[] = $ws_term->name; }
		}

		$dispatch_available_name = join(' / ', $dispatch_available_names);
		if (!$dispatch_available_name) { $dispatch_available_name = '[no work statuses]'; }

		$active_member_name = join(' / , $active_member_names);
		if (!$active_member_name) { $active_member_name = '[no work statuses]'; }

		$form['interestlist']['settings']['self_register']['ws_dispatch_available'] = array(
			'#type' => 'select',
			'#title' => t('Restrict to "Dispatch Available" work statuses?'),
			'#description' => t('If "Yes", then only workers with one of the work statuses flagged as "dispatch available" can sign up for the interest list.'),
			'#options' => array(
				NULL => t('No'),
				'Yes' => ('Yes'),
			),
			'#default_value' => $form['interestlist']['settings']['self_register']['ws_dispatch_available'],
			'#prefix' => "<p>The following work statuses \"Dispatch Available:\" <strong>$dispatch_available_name</strong></p>",
		);

		$form['interestlist']['settings']['self_register']['ws_member_active'] = array(
			'#type' => 'select',
			'#title' => t('Restrict to "Active Member" work statuses?'),
			'#description' => t('If "Yes", then only workers with one of the work statuses flagged as "active member" can sign up for the interest list.'),
			'#options' => array(
				NULL => t('No'),
				'Yes' => ('Yes'),
			),
			'#default_value' => $form['interestlist']['settings']['self_register']['ws_dispatch_available'],
			'#prefix' => "<p>The following work stauses are flagged \"Active Member:\" <strong>$active_member_name</strong></p>",
		);

		$form['interestlist']['settings']['self_register']['ws_tids'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Restrict to Specific Work Statuses'),
			'#description' => t('Restrict self-registration to the following work statuses. If no work statuses are selected, than any worker with appropriate permissions can sign up for the interest list.'),
			'#options' => $ws_options,
		);
		if ($event_type_json['interestlist']['settings']['self_register']['ws_tids']) {
			$form['interestlist']['settings']['self_register']['ws_tids']['#default_value'] = $event_type_json['interestlist']['settings']['self_register']['ws_tids'];
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_interestlist_settings_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$event_type_json['interestlist']['settings'] = $form_state['values']['interestlist']['settings'];
	sirius_jsonfield_util_term_json_set($event_type_term, $event_type_json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}
