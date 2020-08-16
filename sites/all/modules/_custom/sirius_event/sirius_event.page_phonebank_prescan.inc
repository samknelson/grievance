<?php

function sirius_node_tab_event_phonebank_prescan_access($event_node) {
	return sirius_node_tab_event_phonebank_settings_access($event_node);
}

function sirius_node_tab_event_phonebank_prescan($event_node) {
	return drupal_get_form('sirius_node_tab_event_phonebank_prescan_form', $event_node);
}

function sirius_node_tab_event_phonebank_prescan_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$tocall_prole_tid = $event_type_json['phonebank']['prole_tid'];
	$tocall_prole_term = taxonomy_term_load($tocall_prole_tid);
	$tocall_pstatus_tid = $event_json['phonebank']['pstatus_tid'];
	$tocall_pstatus_term = taxonomy_term_load($tocall_pstatus_tid);


	$participants = sirius_event_phonebank_call_list($event_node->nid);
	$count = number_format(count($participants));
	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . "There are <strong>$count</strong> participants in this phone bank with the role <strong>" . $tocall_prole_term->name . "</strong> and the status <strong>" . $tocall_pstatus_term->name . "</strong>.<br /><br />The phone number for each of these participants will be scanned, and those with an invalid phone number will be placed in the status selected below. Scanning <strong>will not cause the recipient's phone to ring</strong>. Scanning <strong>may result in charges</strong> from the telephony service provider." . '</div>',
	);


	$form['pstatus_tid'] = array(
		'#type' => 'select',
		'#title' => t('Invalid Number Status'),
		'#options' => $pstatus_options,
		'#description' => t('Each phone number in the bank will be scanned. Participants with an invalid phone number (e.g. no carrier) will be placed in this status.'),
		'#required' => TRUE,
	);
	if ($event_type_json['phonebank']['prescan']['pstatus_tid']) { $form['pstatus_tid']['#default_value'] = $event_type_json['phonebank']['prescan']['pstatus_tid']; }

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, execute the prescan now',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_phonebank_prescan_form_submit($form, &$form_state) {
	$pstatus_tid = $form_state['values']['pstatus_tid'];

	$participant_nids = sirius_event_phonebank_call_list($form_state['event_nid']);
	$participant_chunks = array_chunk($participant_nids, 100);
	$operations = array();
	$count = 0;
	foreach ($participant_chunks as $chunk) {
		++$count;
		$operations[] = array(
			'sirius_node_tab_event_phonebank_prescan_batch',
			array($chunk, $pstatus_tid),
		);
	}
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_node_tab_event_phonebank_prescan_batch_finished',
  );
  batch_set($batch);
}

function sirius_node_tab_event_phonebank_prescan_batch($participant_nids, $pstatus_tid, &$context) {
	$context['results']['pstatus_tid'] = $pstatus_tid;
	foreach ($participant_nids as $participant_nid) {
		++$context['results']['count'];
		// Under development
	}
}

function sirius_node_tab_event_phonebank_prescan_batch_finished($success, $results, $operations) {
	drupal_set_message(t('Scanned @count participants.', array('@count' => number_format($results['count']))));
	drupal_set_message("Feature under development.");
}


function sirius_event_phonebank_call_list($event_nid) {
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$pstatus_tid = $form_state['values']['pstatus_tid'];
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$tocall_prole_tid = $event_type_json['phonebank']['prole_tid'];
	$tocall_pstatus_tid = $event_json['phonebank']['pstatus_tid'];

	$rule = array(
		'participants' => array(
			"$tocall_prole_tid|$tocall_pstatus_tid",
		),
	);
	return sirius_event_participant_list_by_rule($event_node->nid, $rule);
}