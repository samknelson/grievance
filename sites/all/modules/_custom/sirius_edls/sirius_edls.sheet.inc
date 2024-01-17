<?php

function sirius_edls_sheet_title($sheet_node) {
	$e = sirius_node_title($sheet_node->field_grievance_shop['und'][0]['target_id']);
	if (!$e) { $x = '[no employer]'; }

	$d = date('m/d/Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));

	$j = $sheet_node->field_sirius_job_number['und'][0]['value'];

	return "$e $d - #$j";
}

function sirius_edls_sheet_status_options() {
	return array(
		'draft' => t('Draft'),
		'request' => t('Requested'),
		'lock' => t('Scheduled'),
		'trash' => t('Discarded'),
	);
}

function sirius_edls_sheet_clone($old_sheet_node, $options = array()) {
	$date = $options['date'];
	if (!$date) {  return array('success' => FALSE, 'msg' => "Please enter the date for the new sheet."); }

	// Create the new object
	global $user;
	$sheet_node = new stdClass();
	$sheet_node->type = 'sirius_edls_sheet';
	$sheet_node->title = 'EDSL Sheet';
	$sheet_node->language = LANGUAGE_NONE;
	$sheet_node->uid = $user->uid;
	$sheet_node->status = 1;
	$sheet_node->promote = 0;
	$sheet_node->comment = 0;

	// Copy all fields
	foreach ($old_sheet_node as $key => $value) {
		if (!preg_match('/field_/', $key)) { continue; }
		$sheet_node->{$key} = $old_sheet_node->{$key};
	}

	// Set the start date and status
	$sheet_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($date));
	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = 'draft';

	// Give each crew a new UUID
	$sheet_json = sirius_json_get($sheet_node);
	$crew_uuid_map = array();
	$old_crews = $sheet_json['edls']['crews'];
	$new_crews = array();
	foreach ($old_crews as $old_crew_uuid => $crew) {
		$new_crew_uuid = uuid_generate();
		$crew['uuid'] = $new_crew_uuid;
		$new_crews[$new_crew_uuid] = $crew;
		$crew_uuid_map[$old_crew_uuid] = $new_crew_uuid;
	}
	$sheet_json['edls']['crews'] = $new_crews;
	sirius_json_set($sheet_node, $sheet_json);

	// Save the node
	$sheet_node->title = sirius_edls_sheet_title($sheet_node);
	$sheet_node = node_submit($sheet_node);
	node_object_prepare($sheet_node);
	node_save($sheet_node);

	// Assign all workers. Failure is a warning
	$warnings = array();
	$assignments = sirius_edls_assignment_fetch_sheet($old_sheet_node);

	foreach ($assignments as $old_crew_uuid => $records) {
		foreach ($records as $assignment) {
			$result = sirius_edls_assign(
				$date, 
				$assignment['worker_id'], 
				$sheet_node->nid, 
				$crew_uuid_map[$old_crew_uuid],
				$assignment['assignment_extra']
			);

			if (!$result['success']) { $warnings[] = "Unable to assign $assignment[worker_name]: " . $result['msg']; }
		}
	}

	return array(
		'success' => TRUE, 
		'warnings' => $warnings,
		'sheet_node' => $sheet_node
	);
}

function sirius_edls_sheet_status_set($sheet_node, $new_status) {
	$access = sirius_access_instance('edls');
	if (!$access->access("sheet_status_$new_status", array('sheet_node' => $sheet_node))) { return array('success' => FALSE, 'msg' => 'Access Denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

	// If new status is "trash", then unassign everybody
	if ($new_status == 'trash') {
		$assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
		foreach ($assignments as $crew_nid => $records) {
			foreach ($records as $position => $assignment) {
				sirius_edls_unassign($date, $assignment['worker_id'], $sheet_node->nid);
			}
		}
	}

	// Queue any pending notifications
	sirius_edls_notify_queue($sheet_node, $new_status);

	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = $new_status;
	node_save($sheet_node);
	return array('success' => TRUE);
}


function sirius_edls_sheet_notes_set($sheet_node, $notes) {
	$notes = substr($notes, 0, 10000);
	$sheet_json = sirius_json_get($sheet_node);
	$sheet_json['edls']['notes'] = $notes;
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);
}

function sirius_edls_sheet_notes_get($sheet_node) {
	$sheet_json = sirius_json_get($sheet_node);
	return $sheet_json['edls']['notes'];
}

function sirius_edls_sheet_notes_render($sheet_node, $options = array()) {
	$html = '';
	if (!$options['hide_div']) { $html .= '<div class="sirius_edls_notes">'; }
	$html .= sirius_text_to_html(sirius_edls_sheet_notes_get($sheet_node));
	if (!$options['hide_div']) { $html .= '</div>'; }
	return $html;
}

function sirius_edls_sheet_export($sheet_node) {
	$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
	$sheet_vid = $sheet_node->vid;
	$sheet_nid = $sheet_node->nid;
	$status_options = sirius_edls_sheet_status_options();
	$sheet_json = sirius_json_get($sheet_node);
	$ms_names = sirius_edls_ms_list($employer_node);

	$crews = sirius_edls_crews($sheet_node, 'uuid');
	if (!$crews) { $crews = array(aray()); }
	if ($options['fetch_workers_from_json']) {
		$assignments = $sheet_json['edls']['assignments'];
		if (!$assignments) { $assignments = array(); }
	} else {
		$assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
	}

	$notifications_by_id = sirius_edls_notify_fetch_sheet($sheet_node);
	$notifications = array();
	foreach ($notifications_by_id as $notification_id => $notification) {
		$assignment_id = $notification['assignment_id'];
		$notifications[$assignment_id] = $notification;
	}

	$filename = "sheet_${sheet_nid}_${sheet_vid}.csv";
	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	// print '<pre>';
	$fp = fopen('php://output', 'w');

	$sheet_fields = array();
	$sheet_fields['sheet_version'] = sirius_sheet_render_version($sheet_node, array('text_only' => TRUE));
	$sheet_fields['sheet_status'] = $status_options[$sheet_node->field_sirius_edls_sheet_status['und'][0]['value']];
	$sheet_fields['sheet_employer'] = $employer_node->title;
	$sheet_fields['sheet_supervisor'] = sirius_edls_supervisor_render($sheet_node, array('name_only' => TRUE));
	$sheet_fields['sheet_date'] = date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
	$sheet_fields['sheet_event']  = sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']);
	$sheet_fields['sheet_event_status'] = sirius_text_to_html($sheet_json['edls']['sheet']['show_status']);
	$sheet_fields['sheet_department'] = sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']);
	$sheet_fields['sheet_job'] = sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']);
	$sheet_fields['sheet_facility'] = sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$sheet_fields['sheet_hall'] = sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']);
	$sheet_fields['sheet_count']  = $sheet_node->field_sirius_count['und'][0]['value'];
	$sheet_fields['sheet_notes'] = sirius_edls_sheet_notes_render($sheet_node, array('hide_div' => TRUE));

	foreach ($crews as $crew_uuid => $crew) {
		$crew_fields = array();
		$crew_fields['crew_uuid'] = $crew['uuid'];
		$crew_fields['crew_name'] = $crew['name'];
		$crew_fields['crew_task'] = $crew['task'];
		$crew_fields['crew_count'] = $crew['count'];
		$crew_fields['crew_start_time'] = $crew['start_time'];
		$crew_fields['crew_end_time'] = $crew['end_time'];
		$crew_fields['crew_checkin_location'] = $crew['checkin_location'];

		if (!$assignments[$crew_uuid]) { $assignments[$crew_uuid] = array(array()); }
		foreach ($assignments[$crew_uuid] as $assignment) {
			$assignment_fields = array();
			$assignment_fields['assignment_id'] = $assignment['assignment_id'];
			$assignment_fields['assignment_date'] = $assignment['assignment_date'];
			$assignment_fields['worker_id'] = $assignment['worker_id'];
			$assignment_fields['worker_name'] = $assignment['worker_name'];
			$assignment_fields['worker_ms'] = $ms_names[$assignment['worker_ms']];
			$assignment_fields['assignment_extra'] = sirius_edls_assignment_render_extra($assignment);

			$notification = $notifications[$assignment['assignment_id']];
			$notification_fields = array();
			$notification_fields['notification_id'] = $notification['notification_id'];
			$notification_fields['notification_status'] = $notification['notification_status'];
			$notification_fields['notification_uuid'] = $notification['notification_uuid'];

			$row = array_merge($sheet_fields, $crew_fields, $assignment_fields, $notification_fields);
			if (!$header) {
				fputcsv($fp, array_keys($row));
				$header = TRUE;
			}
			$row = array_values($row);
			foreach ($row as $delta => $val) { 
				$row[$delta] = html_entity_decode(strip_tags($val));
			}
			fputcsv($fp, $row);
		}
	}
	
	fclose($fp);
	drupal_exit();
}