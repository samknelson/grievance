<?php

function sirius_backlink_add($path = NULL, $link = NULL) {
	$links = &drupal_static(__FUNCTION__);
  if (!isset($links)) { $links = array(); }
  if (!$path) { return $links; }
  $links[$path] = $link;
  return $links;
}

function sirius_backlinks_render() {
	$links = sirius_backlink_add();

	$node = menu_get_object('node');
	if ($node && is_object($node)) { $links += sirius_backlinks_from_node($node); }

	$term = menu_get_object('taxonomy_term', 2);
	if ($term && is_object($term)) { $links += sirius_backlinks_from_term($term); }

	if (!$links) { return; }

	$html = '';
	foreach ($links as $path => $link) { 
		if (drupal_valid_path($path)) {
			$html .= l("&lArr; " . $link['name'], $path, array('html' => TRUE, 'attributes' => array('title' => $link['title'], 'class' => array('backlink'))));
		}
	}

	return $html;
}

function sirius_backlinks_from_term($term) {
	return array();
}

function sirius_backlinks_from_node($node) {
	$links = array();

	$contact_nid = $node->field_sirius_contact['und'][0]['target_id'];
	if ($contact_nid) { $worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE); }
	if ($contact_nid && !$worker_nid) {
		if ($node->type == 'sirius_event_participant') 			{ $path = "node/$contact_nid"; }
		else 																								{ $path = ""; }

		if ($path) { $links[$path] = array('name' => 'Contact', 'title' => sirius_backlink_title($contact_nid)); }
	}

	if (!$worker_nid) { $worker_nid = $node->field_sirius_worker['und'][0]['target_id']; }
	if ($worker_nid) {
		if ($node->type == 'sirius_payperiod') 										{ $path = "node/$worker_nid/sirius_worker_hours"; }
		else if ($node->type == 'sirius_dispatch') 								{ $path = "node/$worker_nid/sirius_worker_dispatches"; }
		else if ($node->type == 'sirius_event_participant') 			{ $path = "node/$worker_nid/sirius_worker_events"; }
		else if ($node->type == 'sirius_worker')									{ $path = ""; }
		else 																											{ $path = "node/$worker_nid"; }

		if ($path) { $links[$path] = array('name' => 'Worker', 'title' => sirius_backlink_title($worker_nid)); }
	}

	$job_nid = $node->field_sirius_dispatch_job['und'][0]['target_id'];
	if ($job_nid) {
		$path = "node/$job_nid/sirius_dispatch_job_dispatches";
		if ($path) { $links[$path] = array('name' => 'Job', 'title' => sirius_backlink_title($job_nid)); }
	}

	$employer_nid = $node->field_grievance_shop['und'][0]['target_id'];
	if ($employer_nid) {
		if ($node->type == 'sirius_payperiod') 			{ 
			/*
			$ts = sirius_date_ts($node, 'field_sirius_datetime');
			$date = gmdate('Y-m', $ts);
			list ($year, $month) = explode('-', $date);
			$path = "node/$employer_nid/sirius_employer_hours/$year/$month";
			*/
			$path = '';
		}
		else if ($node->type == 'sirius_worker') 		{ $path = ''; }
		else 																				{ $path = "node/$employer_nid"; }

		if ($path) { $links[$path] = array('name' => 'Employer', 'title' => sirius_backlink_title($employer_nid)); }
	}

	$event_nid = $node->field_sirius_event['und'][0]['target_id'];
	if ($event_nid) {
		$path = "node/$event_nid/sirius_event_info";
		if ($path) { $links[$path] = array('name' => 'Event', 'title' => sirius_backlink_title($event_nid)); }
	}

	$payer_nid = $node->field_sirius_payer['und'][0]['target_id'];
	if ($payer_nid) {
		$path = "node/$payer_nid";
		if ($path) { $links[$path] = array('name' => 'Payer', 'title' => sirius_backlink_title($payer_nid)); }
	}

	if ($node->type == 'sirius_log') {
		$handler_nids = sirius_fieldvals($node, 'field_sirius_log_handler', 'target_id');
		$sql = 'select nid, type, title from node where nid in (:handler_nids)';
		$sql_args = array(':handler_nids' => $handler_nids);
		$stmt = sirius_sql_query($sql, $sql_args);
		$log_type = $node->field_sirius_type['und'][0]['value'];
		$log_category = $node->field_sirius_category['und'][0]['value'];
		while ($hr = $stmt->fetchAssoc()) {
			$handler_type = $hr['type'];
			$handler_nid = $hr['nid'];
			if ($handler_type == 'sirius_event' && $log_category == 'ledger:auth')									{ $path = "node/$handler_nid/sirius_event_auths"; }
			else if ($handler_type == 'sirius_event') 																							{ $path = "node/$handler_nid/sirius_event_info"; }
			else if ($handler_type == 'sirius_worker' && $log_category == 'worker:certification') 	{ $path = "node/$handler_nid/sirius_worker_tc/certifications"; }
			else if ($handler_type == 'sirius_json_definition') 																		{ $path = ''; }
			else 																																										{ $path = "node/$handler_nid/${handler_type}_log"; }
			$name = sirius_content_type_name($handler_type);
			$links[$path] = array('name' => $name, 'title' => $hr['title']);
		}
	}

	return $links;
}

function sirius_backlink_title($nid) {
	$sql = "select title from node where nid = :nid";
	$sql_args = array(':nid' => $nid);
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	return $hr['title'];
}