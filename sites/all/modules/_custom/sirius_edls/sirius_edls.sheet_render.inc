<?php

function sirius_edls_sheet_render($sheet_node, $options = array()) {
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', array('type' => 'file'));

	$sheet_vid = $sheet_node->vid;
	$sheet_nid = $sheet_node->nid;
	$status_options = sirius_edls_sheet_status_options();
	$sheet_json = sirius_json_get($sheet_node);

	$html = '';
	$html .= sirius_fakerender_field('Version:', "$sheet_nid-$sheet_vid");
	$html .= sirius_fakerender_field('Status:', $status_options[$sheet_node->field_sirius_edls_sheet_status['und'][0]['value']]);
	$html .= sirius_fakerender_field('Employer:', sirius_node_title($sheet_node->field_grievance_shop['und'][0]['target_id']));
	$html .= sirius_fakerender_field('Supervisor:', sirius_edls_supervisor_render($sheet_node));
	$html .= sirius_fakerender_field('Start Date:', date('l, F j, Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])));


	$html .= sirius_fakerender_field('Event:', sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']));
	$html .= sirius_fakerender_field('Event Status:', sirius_text_to_html($sheet_json['edls']['sheet']['show_status']));
	$html .= sirius_fakerender_field('Department:', sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']));
	$html .= sirius_fakerender_field('Job #:', sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']));
	$html .= sirius_fakerender_field('Facility:', sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']));
	$html .= sirius_fakerender_field('Hall:', sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']));
	$assigned_count = sirius_edls_assignment_count_sheet($sheet_node->field_sirius_date_start['und'][0]['value'], $sheet_nid);
	$html .= sirius_fakerender_field('Count:', $assigned_count . ' / ' . $sheet_node->field_sirius_count['und'][0]['value']);

	if (!$options['hide_crews']) {
		$crews = sirius_edls_crews($sheet_node);
		$html .= '<div class="sirius_edls_crews">';
		foreach ($crews as $crew_uuid => $crew) {
			$html .= sirius_edls_crew_render($sheet_node, $crew);
		}
		$html .= '</div>';
	}

	return $html;
}

function sirius_edls_sheet_list_render($sheet_nodes, $options = array()) {
	$access = sirius_access_instance('edls');

	if (!$sheet_nodes) { return '<p>' . t('No matching sheets found') . '</p>'; }
	$header = array();
	if (!$options['hide_employer']) { $header[] = t('Employer'); }
	if (!$options['hide_date']) { $header[] = t('Date'); }
	if (!$options['hide_job_number']) { $header[] = t('Job #'); }
	if (!$options['hide_supervisor']) { $header[] = t('Supervisor'); }
	if (!$options['hide_department']) { $header[] = t('Department'); }
	if (!$options['hide_facility']) { $header[] = t('Facility') . '<br />' . t('Hall'); }
	if (!$options['hide_count']) { $header[] = t('Count') . '<br />' . t('Assigned'); }
	$header[] = t('Tools');

	$status_options = sirius_edls_sheet_status_options();

	$rows = array();
	foreach ($sheet_nodes as $sheet_nid => $sheet_node) {
		$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

		$row = array();
		$supervisor_name = sirius_edls_supervisor_render($sheet_node);
		$status = $sheet_node->field_sirius_edls_sheet_status['und'][0]['value'];
		if (!$status) { $status = 'draft'; }

		if (!$options['hide_employer']) { $row[] = sirius_node_title($sheet_node->field_grievance_shop['und'][0]['target_id']); }
		if (!$options['hide_date']) { $row[] = date('m/d/Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])); }
		if (!$options['hide_job_number']) {
			$row[] = sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']) .
				'<br />' .
				$status_options[$status];
		}
		if (!$options['hide_supervisor']) { $row[] = $supervisor_name; }
		if (!$options['hide_supervisor']) { $row[] = sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']); }
		if (!$options['hide_job_group']) { $row[] = sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']); }
		if (!$options['hide_facility']) {
			$row[] = sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']) . 
				'<br />' .
				sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']);
		}
		if (!$options['hide_count']) {
			$count = $sheet_node->field_sirius_count['und'][0]['value'];
			$assigned = sirius_edls_assignment_count_sheet($date, $sheet_nid);
			$row[] = "$count<br />$assigned";
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
		else { $class = ''; }
		$rows[] = array('data' => $row, 'class' => array($class));
	}

	return sirius_table_render($header, $rows);
}

