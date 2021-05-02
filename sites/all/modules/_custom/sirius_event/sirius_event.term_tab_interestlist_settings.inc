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
