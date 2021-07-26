<?php

function sirius_worker_term_tab_schedule_tor_view_access($tor_node) {
	if ($tor_node->field_sirius_category['und'][0]['value'] != 'tor') { return FALSE; }

	return sirius_worker_schedule_tor_access('view', NULL, $tor_node);
}

function sirius_worker_node_tab_schedule_tor_view($tor_node) {
	$entity_node = sirius_log_handler($tor_node, 'sirius_worker');
	if (!$entity_node) {
		drupal_set_message("Time-off request is not associated with a worker.", 'error');
		return '';
	}

	$tor_json = sirius_json_get($tor_node);

	$tor_type_term = taxonomy_term_load($tor_json['tor']['type_tid']);

	$html .= sirius_fakerender_field('Worker', $entity_node->title);

	$dispatch_nid = $tor_json['tor']['dispatch_nid'];
	if ($dispatch_nid) { 
		$dispatch_node = node_load($dispatch_nid);
		if (drupal_valid_path("node/$dispatch_nid/sirius_dispatch_worker_view")) {
			$html .= sirius_fakerender_field('Dispatch', l($dispatch_node->title, "node/$dispatch_nid/sirius_dispatch_worker_view"));
		} else {
			$html .= sirius_fakerender_field('Dispatch', $dispatch_node->title);
		}

		$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
		$job_node = node_load($job_nid);
		if (drupal_valid_path("node/$job_nid")) {
			$html .= sirius_fakerender_field('Job', l($job_node->title, "node/$job_nid"));
		} else {
			$html .= sirius_fakerender_field('Job', $job_node->title);
		}

		$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
		$employer_node = node_load($employer_nid);
		if (drupal_valid_path("node/$employer_nid")) {
			$html .= sirius_fakerender_field('Employer', l($employer_node->title, "node/$employer_nid"));
		} else {
			$html .= sirius_fakerender_field('Employer', $employer_node->title);
		}
	}

	$status_options = sirius_worker_schedule_tor_status_options();
	$html .= sirius_fakerender_field('Type', $tor_type_term->name);
	$html .= sirius_fakerender_field('Status', $status_options[$tor_json['tor']['status']]);
	$html .= sirius_fakerender_field('Start Date', $tor_json['tor']['start_date']);
	$html .= sirius_fakerender_field('End Date', $tor_json['tor']['end_date']);
	$html .= sirius_fakerender_field('Reason for Absence', sirius_text_to_html(sirius_fieldval($tor_node, 'field_sirius_summary')));

	if (sirius_worker_schedule_tor_access('admin', NULL, $tor_node)) {
		$comment = sirius_text_to_html($tor_json['tor']['comment']);
		if ($comment) { $html .= sirius_fakerender_field('Comment', $comment); }
	}

	$html .= drupal_render(field_view_field('node', $tor_node, 'field_sirius_attachments', 'full'));

	$html .= '<br /><br />';

	$html .= sirius_fakerender_field('Created On', date('l, F j, y \a\t g:i A', $tor_node->created));
	$tor_user = user_load($tor_node->uid);
	$html .= sirius_fakerender_field('Created By', $tor_user->name);

	/*
	if (sirius_worker_schedule_tor_access('admin', NULL, $tor_node)) {
		$html .= '<hr>';
		$html .= sirius_jsonfield_log_render($tor_node);
	}
	*/

	return $html;
}
