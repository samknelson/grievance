<?php

function sirius_node_tab_dispatch_job_events_access($job_node) {
	// Job must be associated with at least one event (the "root" event)
	if (!$job_node->field_sirius_events['und'][0]['target_id']) { return FALSE; }

	// Staff can view any job
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employer can view only their own shop's jobs.
	global $user;
	$user_full = user_load($user->uid);
	if (user_access('sirius dispatch employer')) {
		if ($job_node->field_grievance_shop['und'][0]['target_id'] && 
			($user_full->field_grievance_shops['und'][0]['target_id'] == $job_node->field_grievance_shop['und'][0]['target_id'])) {
			return TRUE;
		}
	}

	// Oh well.
	return FALSE;
}

function sirius_node_tab_dispatch_job_events($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_events_form', $job_node);
}

function sirius_node_tab_dispatch_job_events_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;

	$root_event_node = node_load($job_node->field_sirius_events['und'][0]['target_id']);
	$root_job_node = sirius_event_bullpen_job($root_event_node->nid);
	$root_employer_node = node_load($root_job_node->field_grievance_shop['und'][0]['target_id']);
	$root_facility_node = node_load($root_job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$root_title = $root_employer_node->title . ': ' . sirius_date_date($root_event_node, 'field_sirius_daterepeat', 'Y-m-d h:i a') . ': ' . $root_facility_node->title;

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Main Event: ') . l($root_title, 'node/' . $root_event_node->nid . '/sirius_event_info') . '<br /><br />' . '</div>',
	);

	$event_type_tid = $root_event_node->field_sirius_event_type['und'][0]['tid'];
	$start_date = date('Y-m-d 00:00:00', strtotime($root_event_node->field_sirius_daterepeat['und'][0]['value']));
	$end_date = date('Y-m-d 23:59:59', strtotime($root_event_node->field_sirius_daterepeat['und'][0]['value']));
	$display_date = date('l, F d, Y', strtotime($root_event_node->field_sirius_daterepeat['und'][0]['value']));

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_event');
	$query->propertyCondition('nid', $root_event_node->nid, '!=');
	$query->fieldCondition('field_sirius_event_type', 'tid', $event_type_tid);
	$query->fieldCondition('field_sirius_daterepeat', 'value', $start_date, '>=');
	$query->fieldCondition('field_sirius_daterepeat', 'value', $end_date, '<=');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		$form['err'] = array('#markup' => t('There are no other events ocurring on <strong>@date</strong>.', array('@date' => $display_date)) . '<br /><br />');
	} else {
		$form['err'] = array('#markup' => t('The following additional events occur on <strong>@date</strong>. Select which events which should be included in this job.', array('@date' => $display_date)));
		$event_nodes = node_load_multiple(array_keys($stmt['node']));
		$current_event_nids = sirius_fieldvals($job_node, 'field_sirius_events', 'target_id');

		foreach ($event_nodes as $event_nid => $event_node) {
			$job_node = sirius_event_bullpen_job($event_node->nid);
			$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
			$facility_node = node_load($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
			$title = $employer_node->title . ': ' . sirius_date_date($event_node, 'field_sirius_daterepeat', 'Y-m-d h:i a') . ': ' . $facility_node->title;
			$event_options[$event_nid] = $title;
		}

		$form['event_nids'] = array(
			'#title' => t('Additional Events'),
			'#type' => 'checkboxes',
			'#options' => $event_options,
			'#default_value' => $current_event_nids,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_node_tab_dispatch_job_events_form_submit($form, &$form_state) {
	$job_node = node_load($form_state['job_nid']);

	$events = array($job_node->field_sirius_events['und'][0]);
	if ($form_state['values']['event_nids']) {
		foreach (array_values($form_state['values']['event_nids']) as $event_nid) {
			$events[] = array('target_id' => $event_nid);
		}
	}
	$job_node->field_sirius_events['und'] = $events;
	node_save($job_node);
	drupal_set_message(t('Events updated.'));
}