<?php

function sirius_training_node_tab_worker_classes($worker_node) {
	sirius_esignature_init();
	drupal_add_css(drupal_get_path('module', 'sirius_training') . '/css/training.css');

	// Get the class types
	$event_type_terms = sirius_event_get_class_types();
	if (!$event_type_terms) {
		drupal_set_message("No event types have been designated as classes.", 'error');
		return '';
	}

	$prole_terms = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'full');
	$pstatus_terms = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full');

	// Get the contact node
	$events_by_type = sirius_event_participants_by_contact($worker_node->field_sirius_contact['und'][0]['target_id'], array_keys($event_type_terms));

	$rows = array();
	$summary_rows = array();
	$total_hours = array();
	foreach ($events_by_type as $event_type_tid => $records) {
		$total_hours[$event_type_tid] = array();
		$event_type_term = $event_type_terms[$event_type_tid];
		$total_hours[$event_type_tid]['status'] = array();
		foreach ($records as $record) {
			$event_node = $record['event_node'];
			$participant_node = $record['participant_node'];
			$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

			$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
			$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
			$prole_term = $prole_terms[$prole_tid];
			$pstatus_term = $pstatus_terms[$pstatus_tid];

			$total_hours[$event_type_tid]['status'][] = $pstatus_term->name;

			$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date', 'Y-m-d');

			$sub_rows = array();
			$subtotal_hours = array();
			foreach ($dates as $delta => $date) {
				$which = 'attendance_' . $delta;
				$duration_default = strtotime($event_node->field_sirius_daterepeat['und'][$delta]['value2']) - strtotime($event_node->field_sirius_daterepeat['und'][$delta]['value']);
				$duration = $participant_json[$which]['data']['hours'];
				if (!isset($duration)) {
					$duration_render = sirius_event_duration_format_hours($duration_default) . ' (est.)';
					$total_hours[$event_type_tid]['total'] += $duration_default;
					$total_hours[$event_type_tid]['est'] += $duration_default;
					$subtotal_hours['total'] += $duration_default;
					$subtotal_hours['est'] += $duration_default;
				} else {
					$duration_render = sirius_event_duration_format_hours($duration);
					$total_hours[$event_type_tid]['total'] += $duration;
					$total_hours[$event_type_tid]['confirmed'] += $duration;
					$subtotal_hours['total'] += $duration;
					$subtotal_hours['confirmed'] += $duration;
				}
				$esignature = sirius_jsonfield_util_esignature_get($participant_json, $which, NULL);
				$sub_row = array();
				$sub_row[] = $date . '<br /> ' . $duration_render;
				$sub_row[] = sirius_esignature_render($esignature, '', $participant_node->nid, $which);
				$sub_row[] = sirius_text_to_html($participant_json[$which]['data']['comments']);
				$sub_rows[] = $sub_row;
			}

			$tools = l('Event', 'node/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));
			$tools .= ' ';
			$tools .= l('Participant', 'node/' . $participant_node->nid . '/edit', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));

			$name_render = $event_type_term->name;
			if ($event_node->title != $event_type_term->name) { $name_render .= '<br />' . $event_node->title; }
			if ($subtotal_hours['total'] > 0) {
				$name_render .= '<br />' . sirius_event_duration_format_hours($subtotal_hours['total']);
				if ($subtotal_hours['est']) {
					$name_render .= ' (' . sirius_event_duration_format_hours($subtotal_hours['est']) . ' est.)';
				}
			}

			if ($participant_json['comments']) {
				$name_render .= '<br /><br />';
				$name_render .= sirius_text_to_html($participant_json['comments']);
			}

			$row = array();
			$row[] = $name_render;
			$row[] = $prole_term->name . '<br />' .$pstatus_term->name;
			$row[] = theme_table(array('rows' => $sub_rows, 'header' => NULL, 'attributes' => array()));
			$row[] = $tools;			
			$rows[] = $row;
		}

		$summary_row = array();
		$summary_row[] = $event_type_term->name;

		$duration_render = sirius_event_duration_format_hours($total_hours[$event_type_tid]['total']);
		if ($total_hours[$event_type_tid]['est']) {
			$duration_render .= ' (' . sirius_event_duration_format_hours($total_hours[$event_type_tid]['est']) . ' est.)';
		}
		$summary_row[] = join(' / ', array_unique($total_hours[$event_type_tid]['status']));
		$summary_row[] = $duration_render;
		$summary_rows[] = $summary_row;
	}

	$summary_header = array(
		'Class',
		'Status',
		'Hours',
	);
	$html .= '<h2>' . t('Class Summary') . '</h2>';
	$html .= theme_table(array('rows' => $summary_rows, 'header' => $summary_header, 'attributes' => array('class' => 'sirius_training_event_list')));

	$header = array(
		'Class',
		'Status',
		'Attendance',
		'Tools'
	);

	$html .= '<h2>' . t('Attendance Details') . '</h2>';
	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => 'sirius_training_event_list')));
	return $html;
}
