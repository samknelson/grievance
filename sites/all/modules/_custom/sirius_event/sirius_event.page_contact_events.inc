<?php

function sirius_event_node_tab_contact_events($contact_node) {
	$by_type = sirius_event_participants_by_contact($contact_node->nid);
	if (!$by_type) { return 'This contact is not registered for any events.'; }

	$pstatus_lookup = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
	$prole_lookup = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');
	$type_lookup = sirius_taxonomy_load('sirius_event_type', 'tid', 'name');

	$records = array();
	foreach ($by_type as $type_tid => $type_records) {
		foreach ($type_records as $record) {
			$records[] = $record;
		}
	}

	usort($records, function($a, $b) {
		return strcmp(
			$b['event_node']->field_sirius_daterepeat['und'][0]['value'],
			$a['event_node']->field_sirius_daterepeat['und'][0]['value']
		);
	});

	$rows = array();
	foreach ($records as $record) {
		$tools = '';
		$tools .= l('Event', 'node/' . $record['event_node']->nid . '/sirius_event_info', ['attributes' => ['class' => ['tool-button']]]);
		$tools .= l('Participant', 'node/' . $record['participant_node']->nid , ['attributes' => ['class' => ['tool-button']]]);

		$title = $record['event_node']->title;
		$type_name = $type_lookup[$record['event_node']->field_sirius_event_type['und'][0]['tid']];
		if ($type_name != $title) { $title = $type_name . '<br />' . $title; }

		$ts = sirius_date_ts($record['event_node'], 'field_sirius_daterepeat', 0, 'value');
		$datestr = date('l, F j, Y\<\b\r \/\>g:i a', $ts);

		$row = array();
		$row[] = $datestr;
		$row[] = $title;
		$row[] = $prole_lookup[$record['participant_node']->field_sirius_event_prole['und'][0]['tid']];
		$row[] = $pstatus_lookup[$record['participant_node']->field_sirius_event_pstatus['und'][0]['tid']];
		$row[] = $tools;

		$rows[] = $row;
	}

	$header = array(
		t('Date'),
		t('Event'),
		t('Role'),
		t('Status'),
		t('Tools'),
	);

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}