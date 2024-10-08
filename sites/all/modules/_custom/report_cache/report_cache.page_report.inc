<?php

function report_cache_page_report($report_type) {
	drupal_add_css(drupal_get_path('module', 'report_cache') . '/css/report_cache.css');

	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
	$report_id = $_REQUEST['report_id'];
	$action = $_REQUEST['action'];

	if ($report_id) {
		$sql = "select report_domain_nid from report_cache where report_type = :report_type and report_id = :report_id ";
		$stmt = db_query($sql, array(':report_type' => "$report_type :: PARAMETERS", ':report_id' => $report_id));
		$hr = $stmt->fetchAssoc();
		if ($hr) {
			$report_domain_nid = $hr['report_domain_nid'];
			if ($report_domain_nid != $domain_nid) {
				return 'Access denied.';
			}
		} else {
			$report_id = NULL;
		}
	}

	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }
	$fields = $info['fields'];

	if ($info['description']) {

	}

	sirius_require('report_cache', 'report_cache.page_report_list.inc');
	if ($action == 'purge') {
		$count = report_cache_purge($report_type, $report_id);
		drupal_set_message(t('Purged report [@report_id]; @count records deleted.', array('@report_id' => $report_id, '@count' => $count)));
		$render = report_cache_page_report_list($report_type, $info);
	} elseif ($action == 'download') {
		report_cache_download($report_type, $report_id, $info);
	} elseif ($action == 'preview') {
		$render = report_cache_preview($report_type, $report_id, $info);
	} elseif ($action == 'preview_modal') {
		$render = report_cache_preview_modal($report_type, $report_id, $info);
	} elseif ($action == 'render') {
		$render = report_cache_render($report_type, $report_id, $info, $_REQUEST['callback-name']);
	} elseif ($action == 'operations') {
		$render = report_cache_operations($report_type, $report_id, $info, $_REQUEST['callback-name']);
	} else {
		$render = report_cache_page_report_list($report_type, $info);
	}

	$title = t("Reports: ") . $info['name'];
	if ($report_id) { $title .= ": $report_id"; }
	drupal_set_title($title);

	if ($report_id) {
		sirius_backlink_add("sirius/reports/$report_type", array('name' => 'Report List'));
	} else {
		sirius_backlink_add('sirius/reports', array('name' => 'All Reports'));
	}
	return $render;
}

function report_cache_render_meta($parameters) {
	$html = '<div class="report_cache_meta">';
	if ($parameters['meta']['instance']['uuid']) {
		$instance = report_cache_instance($parameters['report_type'], $parameters['meta']['instance']['uuid']);
		if ($instance) {
			$html .= sirius_fakerender_field('Instance:', $instance['title']);
		}
	}
	if ($parameters['meta']['user']) {
		$html .= sirius_fakerender_field('Generated by:', $parameters['meta']['user']['name'] . ' [' . $parameters['meta']['user']['mail'] . ']');
	}
	if ($parameters['meta']['ts']['completed']) {
		$html .= sirius_fakerender_field('Date:', date('l, F d Y - g:ia T', $parameters['meta']['ts']['completed']));
	} else if ($parameters['meta']['ts']['started']) {
		$html .= sirius_fakerender_field('Started:', date('l, F d Y - g:ia T', $parameters['meta']['ts']['started']));
	}

	if ($parameters['meta']['ts']['duration']) {
		$html .= sirius_fakerender_field('Run time:', format_interval($parameters['meta']['ts']['duration']));
	}
	$html .= '</div>';
	return $html;
}

function report_cache_download($report_type, $report_id, $info) {
	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
	if ($info['download_callback']) {
		$callback = $info['download_callback'];
		return $callback($report_type, $report_id, $info);
	}

	$fields = $info['fields'];

	$filename = $report_type . '_' . $report_id;
	$filename = preg_replace('/[^a-zA-Z0-9_]/', '_', $filename);
	$filename = preg_replace('/_+/', '_', $filename);
	$filename = strtolower($filename);
	$filename = $filename . '.csv';

	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	$fp = fopen('php://output', 'w');
	fputcsv($fp, $fields);

	$query = db_select('report_cache');
	$query->fields('report_cache');
	$query->condition('report_type', $report_type);
	$query->condition('report_id', $report_id);
	$query->condition('report_domain_nid', $domain_nid);
	$stmt = $query->execute();

	while ($hr = $stmt->fetchAssoc()) {
		$hr = $info['generator']->row_decode($hr['report_data']);
		$row = array();
		foreach ($fields as $name => $prompt) {
			// A one-off hack. Don't ask.
			if ($report_type == 'grievance-report-open-summary-with-log') {
				$hr[$name] = preg_replace('/\<br \/\>/', ', ', $hr[$name]);
			}
			$row[] = strip_tags($hr[$name]);
		}
		fputcsv($fp, $row);
	}
	
	exit();
}



