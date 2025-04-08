<?php

function sirius_edls_sheet_render($sheet_node, $options = array()) {
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', array('type' => 'file'));

	$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);

	$sheet_vid = $sheet_node->vid;
	$sheet_nid = $sheet_node->nid;
	$status_options = sirius_edls_sheet_status_options();
	$sheet_json = sirius_json_get($sheet_node);

	$col1 = $col2  = '';
	if (!$options['hide_version']) { $col1 .= sirius_fakerender_field('Version:', sirius_sheet_render_version($sheet_node)); }
	if (!$options['hide_status']) { $col1 .= sirius_fakerender_field('Status:', $status_options[$sheet_node->field_sirius_edls_sheet_status['und'][0]['value']]); }
	if (!$options['hide_employer']) { $col1 .= sirius_fakerender_field('Employer:', $employer_node->title); }
	if (!$options['hide_supervisor']) { $col1 .= sirius_fakerender_field('Supervisor:', sirius_edls_supervisor_render($sheet_node)); }
	if (!$options['hide_assignee']) { $col1 .= sirius_fakerender_field('Assignee:', sirius_edls_assignee_render($sheet_node)); }
	if (!$options['hide_date']) { $col1 .= sirius_fakerender_field('Date:', date('l, F j, Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']))); }

	if (!$options['hide_event']) { $col2 .= sirius_fakerender_field('Event:', sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id'])); }
	if (!$options['hide_event_status']) { $col2 .= sirius_fakerender_field('Event Status:', sirius_text_to_html($sheet_json['edls']['sheet']['show_status'])); }
	if (!$options['hide_department']) { $col2 .= sirius_fakerender_field('Department:', sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid'])); }
	if (!$options['hide_job_number']) { $col2 .= sirius_fakerender_field('Job #:', sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value'])); }
	if (!$options['hide_facility']) {
		$col2 .= sirius_fakerender_field('Facility:', sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']));
		$col2 .= sirius_fakerender_field('Hall:', sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']));
	}

	if (!$options['hide_count']) {
		$assigned_count = sirius_edls_assignment_count_sheet($sheet_node->field_sirius_date_start['und'][0]['value'], $sheet_nid);
		$col2 .= sirius_fakerender_field('Count:', $assigned_count . ' / ' . $sheet_node->field_sirius_count['und'][0]['value']);
	}

	$access = sirius_access_instance('edls');
	$can_edit_notifications = $access->access('sheet_notify_edit', array('sheet_node' => $sheet_node));

	$html .= sirius_table_render(
		array(), 
		array(
			array(
				array('style' => 'width: 50%;', 'data' => $col1),
				array('style' => 'width: 50%;', 'data' => $col2),
			),
		)
	);
	$sheet_header_rows = 20;

	if (!$options['hide_notes']) {
		$notes = sirius_edls_sheet_notes_render($sheet_node, array('xhide_div' => TRUE));
		if ($notes) {
			$extra_rows = round(substr_count($notes, 'br')/2) + 2;
			$sheet_header_rows += $extra_rows;
			$html .= sirius_fakerender_field('Notes:', $notes);
			$html .= '<br />';
		}
	}

	if ($sheet_header_rows % 2) { ++$sheet_header_rows; }

	if (!$options['hide_crews']) {
		$crews = sirius_edls_crews($sheet_node, 'uuid');
		if ($options['fetch_workers_from_json']) {
			$sheet_json = sirius_json_get($sheet_node);
			$assignments = $sheet_json['edls']['assignments'];
			if (!$assignments) { $assignments = array(); }
		} else {
			$assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
		}

		$notifications_by_id = sirius_edls_notify_fetch_sheet($sheet_node);
		$notifications = array();
		foreach ($notifications_by_id as $notification_id => $notification) {
			$assignment_id = $notification['assignment_id'];
			if (!$assignment_id) { continue; }
			$notifications[$assignment_id] = $notification;
		}

		// Make our blank pages
		$crew_header_rows = 8;
		$pages = array(array());
		$rows_per_page = $_REQUEST['rows_per_page'];
		if (!$rows_per_page) { $rows_per_page = 62; }
		$page_number = 0;
		$row_number = $sheet_header_rows;
		foreach ($crews as $crew_uuid => $crew) {
			if ($options['crew_uuid'] && ($crew_uuid != $options['crew_uuid'])) { continue; }

			// Make sure each crew has an even number of spots
			$count_even = $crew['count'];
			if ($count_even % 2) { $count_even++; }

			if ($row_number) {
				// Widow control
				$remaining_in_page = $rows_per_page - $row_number;
				// drupal_set_message("Page #$page_number: $remaining_in_page left on page, $count_even left in crew");

				$start_new_page = FALSE;

				// Start a new page if there's only a few spaces left on this page
				if ( ($remaining_in_page < 6+$crew_header_rows) && ($count_even > 6+$crew_header_rows) ) { $start_new_page = TRUE; }

				// Start a new page if a crew can fit on a page
				if ( ($count_even+$crew_header_rows > $remaining_in_page) && ($count_even+$crew_header_rows <= $rows_per_page) ) { $start_new_page = TRUE; }

				if ($start_new_page) { $row_number = 0; ++$page_number; }
			}

			// Start a new crew (jump forward a few rows)
			if ($row_number+$crew_header_rows > $rows_per_page) { $row_number = 0; ++$page_number; }
			$row_number += $crew_header_rows;

			for ($position = 0; $position < $count_even; ++$position) {
				if ($position < $crew['count']) {
					$pages[$page_number][$crew_uuid][] = array('position' => ($position+1));
				} else {
					$pages[$page_number][$crew_uuid][] = array('position' => '');
				}

				++$row_number;
				if ($row_number >= $rows_per_page) { $row_number = 0; ++$page_number; }
			}
		}
		$page_count = count($pages);

		// Put assignments on each page
		$page_number = 0;
		$row_number = 0;
		foreach ($assignments as $crew_uuid => $crew_assignments) {
			// drupal_set_message("Paginating $crew_uuid");
			foreach ($crew_assignments as $assignment) {
				// drupal_set_message("Assigning $assignment[worker_name]");
				$found = FALSE;
				foreach ($pages as $page_number => $page) {
					if (!$page[$crew_uuid]) { continue; }
					foreach ($page[$crew_uuid] as $row_number => $slot) {
						if ($pages[$page_number][$crew_uuid][$row_number]['assignment_id']) { continue; }
						// drupal_set_message("... put $assignment[worker_name] on page $page_number in row $row_number");
						$pages[$page_number][$crew_uuid][$row_number] += $assignment;
						$found = TRUE;
						break;
					}
					if ($found) { break; }
				}
			}
		}

		// Divide into columns
		foreach ($pages as $page_number => $page) {
			foreach ($page as $crew_uuid => $crew_rows) {
				$count = count($crew_rows);
				$col1 = array_slice($crew_rows, 0, $count/2);
				$col2 = array_slice($crew_rows, ($count/2), $count);
				$pages[$page_number][$crew_uuid] = array($col1, $col2);
			}
		}

		$ms_names = sirius_edls_ms_list($employer_node, 'short');
		$html .= '<div class="sirius_edls_crews">';
		foreach ($pages as $page_number => $page) {
			foreach ($page as $crew_uuid => $crew_cols) {
				$crew = $crews[$crew_uuid];

				$html .= '<div class="sirius_edls_crew">';

				$parts = array();
				if ($crew['task']) { $parts[] = $crew['task']; }
				if ($crew['start_time']) { $parts[] = $crew['start_time'] . ' - ' . $crew['end_time']; }
				if ($crew['checkin_location']) { $parts[] = $crew['checkin_location']; }

				if (!$options['hide_supervisor']) {
					if ($crew['supervisor']) {
						$supervisor = user_load($crew['supervisor']);
						$parts[] = $supervisor->name;
					}
				}
				if ($assignments[$crew_uuid]) {
					$assigned = count($assignments[$crew_uuid]);
				} else {
					$assigned = 0;
				}
				$parts[] = "Assigned <strong>$assigned</strong> of <strong>$crew[count]</strong>";

				$html .= '<h2>' . sirius_edls_crew_title($crew) . '</h2>';
				$html .= '<p>' . join(' :: ', $parts) . '</p>';

				$render_rows = array();
				for ($i=0; $i<count($crew_cols[0]); ++$i) {
					$render_row = array();
					for ($c = 0; $c<2; ++$c) {
						$assignment = $crew_cols[$c][$i];
						$assignment_id = $assignment['assignment_id'];
						$render_ms = $assignment['position'] . '.&nbsp;';
						$render_ms .= $ms_names[$assignment['worker_ms']];

						$render = $assignment['worker_name'];
						$render .= sirius_edls_notify_render_status($assignment, $notifications[$assignment_id], array('can_edit_notifications' => $can_edit_notifications));
						$render .= sirius_edls_assignment_render_extra($assignment);

						$render_row[] = array('style' => 'width: 10%;', 'data' => $render_ms);
						$render_row[] = array('style' => 'width: 40%', 'data' => $render);
					}
					$render_rows[] = $render_row;
				}

				$html .= sirius_table_render(NULL, $render_rows, array('class' => array('sirius_table_basic xsirius_table_align')));
				$html .= '</div>';
			}

			$html .= "<div class=\"sirius_edls_page_number\">Page " . ($page_number+1) . " / $page_count</div>";
			if ($page_number < $page_count-1) {
				$html .= '<hr style="page-break-after: always;">';
			}
		}
		$html .= '</div>';
	}

	return $html;
}

function sirius_edls_sheet_list_render($sheet_nodes, $options = array()) {
	$access = sirius_access_instance('edls');

	if (!$sheet_nodes) {
		$msg = $options['empty_msg'];
		if (!$msg) { $msg = t('No matching sheets found'); }
		return '<div class="sirius_edls_sheet_list_empty">' . $msg . '</div>';
	}

	$header = array();
	if (!$options['hide_employer']) { $header[] = t('Employer'); }

	if (!$options['hide_supervisor']) { $header[] = t('Supervisor') . '<br />' . t('Assignee'); }
	if (!$options['hide_date']) { $header[] = t('Date'); }
	if (!$options['hide_event']) { $header[] = t('Event'); }
	if (!$options['hide_department']) { $header[] = t('Department'); }
	if (!$options['hide_job_number']) { $header[] = t('Job #'); }
	if (!$options['hide_facility']) { $header[] = t('Facility') . '<br />' . t('Hall'); }
	if (!$options['hide_count']) { $header[] = t('Count') . '<br />' . t('Assigned'); }
	$header[] = t('Tools');

	$status_options = sirius_edls_sheet_status_options();

	$rows = array();
	$totals = array('count' => 0, 'assigned' => 0);
	foreach ($sheet_nodes as $sheet_nid => $sheet_node) {
		$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

		$row = array();
		$supervisor_name = sirius_edls_supervisor_render($sheet_node);
		$assignee_name = sirius_edls_assignee_render($sheet_node);
		$status = $sheet_node->field_sirius_edls_sheet_status['und'][0]['value'];
		if (!$status) { $status = 'draft'; }

		$sheet_json = sirius_json_get($sheet_node);

		if (!$options['hide_employer']) { $row[] = sirius_node_title($sheet_node->field_grievance_shop['und'][0]['target_id']); }

		if (!$options['hide_supervisor']) {
			$parts = array();
			$parts[] = $supervisor_name;
			$parts[] = $assignee_name;
			$row[] = join('<br />', array_filter($parts));
		}

		if (!$options['hide_date']) { $row[] = date('m/d/Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])); }

		if (!$options['hide_job_group']) { 
			$row[] = sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']) . 
				'<br />' .
				$sheet_json['edls']['sheet']['show_status'];		
		}

		if (!$options['hide_department']) { $row[] = sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']); }

		if (!$options['hide_job_number']) {
			$row[] = sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']) .
				'<br />' .
				$status_options[$status];
		}
		if (!$options['hide_facility']) {
			$row[] = sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']) . 
				'<br />' .
				sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']);
		}
		if (!$options['hide_count']) {
			$assigned = sirius_edls_assignment_count_sheet($date, $sheet_nid);
			if ($status == 'reserved') { 
				$count = $assigned;
			} else {
				$count = $sheet_node->field_sirius_count['und'][0]['value'];
			}
			$row[] = "$count<br />$assigned";
			$totals['count'] += $count;
			$totals['assigned'] += $assigned;
		}

		$tools = array();
		$paths = array(
			"node/$sheet_nid/sirius_edls_sheet_view" => t('View'),
			"node/$sheet_nid/sirius_edls_sheet_edit" => t('Edit'),
			"node/$sheet_nid/sirius_edls_sheet_manage" => t('Manage'),
			"node/$sheet_nid/sirius_edls_sheet_workers" => t('Assign'),
		);
		foreach ($paths as $path => $title) {
			if (drupal_valid_path($path)) {
				$tools[] = l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}
		$row[] = join('', $tools);

		$class = '';
		if ($status == 'draft') { $class = 'grey'; }
		else if ($status == 'request') { $class = 'yellow'; }
		else if ($status == 'lock') { $class = 'green'; }
		else if ($status == 'trash') { $class = 'red'; }
		else if ($status == 'reserved') { $class = 'blue'; }
		else { $class = ''; }
		$rows[] = array('data' => $row, 'class' => array($class));
	}

	$html = sirius_table_render($header, $rows);
	if (!$options['hide_count']) {
		$html .= sirius_fakerender_field(t('Total Count:'), number_format($totals['count'], 0));
		$html .= sirius_fakerender_field(t('Total Assigned:'), number_format($totals['assigned'], 0));
	}
	return $html;
}

function sirius_sheet_render_version($sheet_node, $options = array()) {
	return sirius_sheet_render_version_impl($sheet_node->nid, $sheet_node->vid, $options);
}

function sirius_sheet_render_version_impl($nid, $vid, $options = array()) {
	$html = '';
	if ($options['text_only']) {
		$html .= substr($nid, 0, -4) . '.' . substr($nid, -4) . '::' . substr($vid, 0, -4) . '.' . substr($vid, -4);
	} else {
		$html .= '<span class="sirius_edls_sheet_version">';
		$html .= '<i class="fas fa-history"></i>&nbsp;';
		$html .= substr($nid, 0, -4) . '.' . substr($nid, -4) . '&nbsp;::&nbsp;' . substr($vid, 0, -4) . '.' . substr($vid, -4);
		$html .= '</span>';
	}
	return $html;
}
