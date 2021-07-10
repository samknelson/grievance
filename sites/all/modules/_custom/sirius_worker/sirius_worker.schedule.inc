<?php

function sirius_worker_schedule($worker_nid) {
	// Lots of lookups. Let's cache them.
	$lookups = &drupal_static(__FUNCTION__);
	$ts_now = time();

	// Get the contact
	$contact_nid = sirius_contact_get_nid_from_worker($worker_nid);
	if (!$contact_nid) { return array(); }

	// Results array
	$schedule = array();

	// Event Schedule. 
	// @todo: This loads all the event and participant nodes for all the contacts. We should probably refactor to use a raw SQL. sirius_event_participants_by_contact() is very expensive.

	$events_by_type = sirius_event_participants_by_contact($contact_nid);
	if ($events_by_type) {
		if (!isset($lookups['prole_terms'])) { $lookups['prole_terms'] = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'full'); }
		if (!isset($lookups['pstatus_terms'])) { $lookups['pstatus_terms'] = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full'); }

		foreach ($events_by_type as $event_type_tid => $records) {
			foreach ($records as $delta => $record) {
				$pstatus_tid = $record['participant_node']->field_sirius_event_pstatus['und'][0]['tid'];
				$json = sirius_json_get($lookups['pstatus_terms'][$pstatus_tid]);
				if ($json['schedule']['display']['active'] != 'Yes') { continue; }

				if (!$record['event_node']->field_sirius_daterepeat['und'][0]) { continue; }
				foreach ($record['event_node']->field_sirius_daterepeat['und'] as $delta => $d) {
					$ts1 = sirius_date_ts($record['event_node'], 'field_sirius_daterepeat', $delta, 'value');
					$ts2 =  sirius_date_ts($record['event_node'], 'field_sirius_daterepeat', $delta, 'value2');

					$is_good = ($ts1 >= $ts_now || $tw2 >= $ts_now);
					if (!$is_good) { continue; }

					$schedule_entry = array();
					$schedule_entry['type'] = 'Event';
					$schedule_entry['title'] = $json['schedule']['display']['title'];
					$schedule_entry['title_short'] = $json['schedule']['display']['title_short'];
					$schedule_entry['handler_nid'] = $record['event_node']->nid;
					$schedule_entry['handler_title'] = $record['event_node']->title;
					$schedule_entry['start_ts'] = $ts1; 
					$schedule_entry['start_date'] = date('Y-m-d H:i:s', $ts1);
					if ($ts2 > 0) { 
						$schedule_entry['end_ts'] = $ts2; 
						$schedule_entry['end_date'] = date('Y-m-d H:i:s', $ts2);
					}
					$schedule[$ts1] = $schedule_entry;
				}
			}
		}
	}

	// Time-off requests
	// @todo: Big query, probably should optimize

	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'tor');
	$query->fieldCondition('field_sirius_log_handler', 'target_id', $worker_nid);
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  $tor_nodes = node_load_multiple(array_keys($stmt['node']));
	if ($tor_nodes) {
		foreach ($tor_nodes as $tor_nid => $tor_node) {
			$json = sirius_jsonfield_util_term_json_get($tor_node);
			$ts1 = strtotime($json['tor']['start_date']);
			$ts2 = strtotime($json['tor']['end_date']);
			$is_good = ($ts1 >= $ts_now || $tw2 >= $ts_now);
			if (!$is_good) { continue; }

			$schedule_entry = array();
			$schedule_entry['type'] = 'TOR';
			$schedule_entry['title'] = 'Time-Off Request: ' . $json['tor']['status'];
			$schedule_entry['title_short'] = 'TOR';
			$schedule_entry['handler_nid'] = $tor_node->nid;
			$schedule_entry['handler_title'] = $json['tor']['summary'];
			$schedule_entry['start_ts'] = $ts1; 
			$schedule_entry['start_date'] = date('Y-m-d H:i:s', $ts1);
			$schedule_entry['end_ts'] = $ts2; 
			$schedule_entry['end_date'] = date('Y-m-d H:i:s', $ts2);
			$schedule_entry['path'] = "node/$worker_nid/sirius_worker_schedule/tor/view/" . $tor_node->nid;
			$schedule[$ts1] = $schedule_entry;
		}
	}

	ksort($schedule);
	return $schedule;
}

function sirius_worker_schedule_render($schedule, $format = 'full') {
	$html = "<div class=\"sirius_worker_schedule sirius_worker_schedule_$format\">";
	if ($format == 'full') { $html .= sirius_worker_schedule_render_full($schedule); }
	else if ($format == 'tiny') { $html .=  sirius_worker_schedule_render_tiny($schedule); }
	else { $html .=  'Unknown format: $format'; }
	$html .= '</div>';
	return $html;
}

function sirius_worker_schedule_render_tiny($schedule) {
	$shorts = array();
	foreach ($schedule as $ts => $schedule_entry) {
		$shorts[$schedule_entry['title_short']]++;
	}

	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');

	$html = '<a href="#" class="sirius_popup_trigger">' . join(',', array_keys($shorts)) . '</a>';
	$html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
	$html .= '<div style="display:none" class="sirius_popup_wrap">';
	$html .= '<div class="sirius_popup_contents">';
	$html .= sirius_worker_schedule_render_full($schedule);
	$html .= '</div>';
	$html .= '<div class="sirius_popup_close"></div>';
	$html .= '</div>';
	return $html;
}

function sirius_worker_schedule_render_full($schedule) {
	if (!$schedule) { return $html .= 'The schedule is empty.'; }

	$rows = array();
	foreach ($schedule as $ts => $schedule_entry) {
		$tools = '';
		$path = $schedule_entry['path'];
		if (!$path) { $path = 'node/' . $schedule_entry['handler_nid']; }
		if (drupal_valid_path($path)) { $tools .= l('View', $path, array('attributes' => array('class' => array('tool-button')))); }

		$start_ts = $schedule_entry['start_ts'];
		$end_ts = $schedule_entry['end_ts'];

		$start_date = date('Y-m-d', $start_ts);
		$end_date = date('Y-m-d', $end_ts);

		if ($start_date == $end_date) {
			$datestr = $start_date;
		} else {
			$datestr .= "$start_date - $end_date";
		}

		$row = array();
		$row[] = $datestr;
		$row[] = $schedule_entry['type'];
		$row[] = $schedule_entry['title'];
		$row[] = $schedule_entry['handler_title'];
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array(
		'Date',
		'Type',
		'Status',
		'Details',
		'Tools',
	);

	return theme_table(array('attributes' => array(), 'header' => $header, 'rows' => $rows));
}