<?php

function sirius_event_page_participant_list($event_node, $prole_tid, $rule) {
	return drupal_get_form('sirius_event_page_participant_list_form', $event_node, $prole_tid, $rule);
}

function sirius_event_page_participant_list_form($form, &$form_state, $event_node, $prole_tid, $rule) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['prole_tid'] = $prole_tid;
	$form_state['rule'] = $rule;

	/*
	$form['filters'] = array(
		'#title' => t('Filters'),
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['filters']['sort'] = array(
		'#type' => 'select',
		'#options' => array(
			'register_date' => t('Registration Date'),
		),
		'#default_value' => 'register_date',
	);
	*/

	// Get the list of pstatuses that we care about
	$participant_nids = sirius_event_participant_list_by_rule($event_node->nid, $rule, 100000, 0);
	if (count($participant_nids) > 1000) {
		drupal_set_message("Limiting output to the first 1000 records.", 'warning');
		$participant_nids = array_slice($participant_nids, 0, 1000);
	}
	$participant_nodes = node_load_multiple($participant_nids);

	// Sections enabled?
	$section_enabled = sirius_event_section_enabled($event_node);

	// Lookup prole and pstatus names
	$prole_lookup = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');
	$pstatus_lookup = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
	$ms_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$level_lookup = sirius_training_levels_fetch_all();

  $form['action'] = array(
  	'#title' => t('Action'),
  	'#type' => 'select',
  	'#options' => array(
  		NULL => t('-- Select --'),
  		'pstatus' => 'Change Status',
  	),
  );

  $form['pstatus_tid'] = array(
  	'#title' => t('Status'),
  	'#type' => 'select',
  	'#options' => array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tid),
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'pstatus'))),
  );

	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
		$contact_node = node_load($contact_nid);
		$worker_node = sirius_worker_get_by_contact_nid($contact_nid);
		$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

		// Name of the contact
		$title = $contact_node->title;

		// Registration date
		$date = '';
		if ($participant_node->field_sirius_datetime['und'][0]['value']) {
			$date = sirius_date_date($participant_node, 'field_sirius_datetime', 'Y-m-d H:i');
		}

		// Details - Member Status
		$details = '';
		$levels = sirius_training_levels_fetch_worker($worker_node);
		$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
		$ms_names = array();
		foreach ($ms_tids as $ms_tid) {
			$name = $ms_lookup[$ms_tid];
			if ($levels[$ms_tid]) {
				$name .= ' (' . $level_lookup[$ms_tid][$levels[$ms_tid]['code']]['title'] . ')';
			}
			$ms_names[] = $name;
		}
		if ($ms_names) {
			if ($details) { $details .= '<br />'; }
			$details .= join('<br />', $ms_names);
		}

		// Details - Worker ID
		if ($worker_node->field_sirius_id['und'][0]['value']) {
			if ($details) { $details .= '<br />'; }
			$details .= 'ID: ' . $worker_node->field_sirius_id['und'][0]['value'];
		}

		// Tools
		$tools = l('Participant', 'node/' . $participant_node->nid . '/sirius_event_participant_comments', array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Contact', 'node/' . $contact_node->nid, array('attributes' => array('class' => array('tool-button'))));
		if ($worker_node) { $tools .= l('Worker', 'node/' . $worker_node->nid . '/sirius_worker_events', array('attributes' => array('class' => array('tool-button')))); }

		$row = array();
		$row[] = $title;
		$row[] = $date;
		$row[] = $pstatus_lookup[$participant_node->field_sirius_event_pstatus['und'][0]['tid']];
		if ($section_enabled) {
			$section = sirius_event_section_get_from_participant($event_node, $participant_node);
			$row[] = $section['title'];
		}
		$row[] = $participant_json['comments'];
		$row[] = $details;
		$row[] = $tools;
		$rows[$participant_nid] = $row;
	}

	$header = array();
	$header[] = t('Title');
	$header[] = t('Registered');
	$header[] = t('Status');
	if ($section_enabled) { 
		$header[] = t('Section');
	} 
	$header[] = t('Comments');
	$header[] = t('Details');
	$header[] = t('Tools');

	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

  $form['participants'] = array(
    '#type' => 'tableselect',
    '#title' => 'Current Locks',
    '#header' => $header,
    '#options' => $rows,
  );

	// $html .= views_embed_view('sirius_event_participants', 'block_1', $event_node->nid, $prole);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_event_page_participant_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$event_nid = $form_state['event_nid'];

	$participant_nids = array();
	if ($form_state['values']['participants']) { $participant_nids = array_filter(array_values($form_state['values']['participants'])); }

	$action = $form_state['values']['action'];
	if ($action == 'pstatus') {
		if (!$participant_nids) { 
			drupal_set_message("Please select at least one participant.", 'warning');
			return;
		}
		$pstatus_tid = $form_state['values']['pstatus_tid'];
		if (!$pstatus_tid) {
			drupal_set_message("Please select a status.", 'warning');
			return;
		}

		$count = $count_success = $count_failure = 0;
		foreach ($participant_nids as $participant_nid) {
			++$count;
			$participant_node = node_load($participant_nid);
			$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
			$result = sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid);
			if ($result['success']) { 
				++$count_success;
			} else {
				++$count_failure;
				drupal_set_message($result['msg'], 'warning');
			}
		}
		drupal_set_message("Set status for $count participants with $count_success success and $count_failure failures.");
	}
}