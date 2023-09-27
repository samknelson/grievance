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

	if ($action == 'purge') {
		$count = report_cache_purge($report_type, $report_id);
		drupal_set_message(t('Purged report [@report_id]; @count records deleted.', array('@report_id' => $report_id, '@count' => $count)));
	} elseif ($action == 'download') {
		report_cache_download($report_type, $report_id, $info);
	} elseif ($action == 'preview') {
		$render = report_cache_preview($report_type, $report_id, $info);
	} elseif ($action == 'render') {
		$render = report_cache_render($report_type, $report_id, $info, $_REQUEST['callback-name']);
	} elseif ($action == 'operations') {
		$render = report_cache_operations($report_type, $report_id, $info, $_REQUEST['callback-name']);
	} 

	if ($render) {
		drupal_set_title(t("Reports: ") . $info['name'] . ': ' . $report_id);
 		sirius_backlink_add("sirius/reports/$report_type", array('name' => 'Report List'));
		return $render;
	}

	drupal_set_title(t("Reports: ") . $info['name']);

	$sql_args = array();
	$sql = "select distinct report_id from report_cache ";
	$sql .= "where report_type = :report_type ";
	$sql_args[':report_type'] = "$report_type :: PARAMETERS";
	$sql .= "and report_domain_nid = :domain_nid ";
	$sql_args[':domain_nid'] = $domain_nid;
	$sql .= "order by report_id desc ";
	$stmt = db_query($sql, $sql_args);
	$report_ids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$report_ids[] = $hr['report_id'];
	}
	
	if (!$report_ids) {
		drupal_set_message(t('No reports of type @report_type have been generated yet.', array('@report_type' => $report_type)));
	}

	$header = array();
	$header[] = t('Report ID');
	if ($info['title_callback']) { $header[] = t('Title'); }
	$header[] = t('Tools');

	$attributes = array();

	$rows = array();
	foreach ($report_ids as $tmp_report_id) {

		$tools = l(t('Preview'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $tmp_report_id, 'action' => 'preview')));
		$tools .= l(t('Download'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $tmp_report_id, 'action' => 'download')));
		if ($info['render_callbacks']) {
			foreach ($info['render_callbacks'] as $name => $callback) {
				$tools .= l(
					$name, 
					"sirius/reports/$report_type", 
					array(
						'attributes' => array('class' => array('tool-button')), 
						'query' => array(
							'report_id' => $tmp_report_id, 
							'action' => 'render',
							'callback-name' => $name,
						)
					)
				);
			}
		}

		if ($info['operations']) {
			$tools .= l(t('Operations'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $tmp_report_id, 'action' => 'operations')));
		}
		$tools .= l(t('Purge'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $tmp_report_id, 'action' => 'purge')));

		$row = array();
		$row[] = $tmp_report_id;
		if ($info['title_callback']) { 
			$row[] = $info['title_callback']($report_type, $tmp_report_id);
		}

		$row[] = $tools;
		$rows[] = $row;
	}

	// $html .= '<h2>' . $info['name'] . '</h2>';
	$html .= $info['description'];

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));

	if ($preview) {
		$html .= '<hr>';
		$html .= $preview;
	}

	$path = $info['path'];
	if (!$path) { 
		$path = 'sirius/reports/' . $report_type . '/form';
	}
	$html .= l(t('Generate a new report'), $path, array('attributes' => array('class' => array('tool-button'))));

	sirius_backlink_add('sirius/reports', array('name' => 'All Reports'));
	return $html;
}

function report_cache_render_meta($parameters) {
	$html = '<div class="report_cache_meta">';
	if ($parameters['meta']['instance']['uuid']) {
		$instance = report_cache_instance($report_type, $parameters['meta']['instance']['uuid']);
		if ($instance) {
			$html .= sirius_fakerender_field('Instance:', $instance['title']);
		}
	}
	if ($parameters['meta']['user']) {
		$html .= sirius_fakerender_field('Generated by:', $parameters['meta']['user']['name'] . ' [' . $parameters['meta']['user']['mail'] . ']');
	}
	if ($parameters['meta']['ts']) {
		$html .= sirius_fakerender_field('Date:', date('l, F d Y - g:ia', $parameters['meta']['ts']['completed']));
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
		$hr = unserialize($hr['report_data']);
		$row = array();
		foreach ($fields as $name => $prompt) {
			$row[] = strip_tags($hr[$name]);
		}
		fputcsv($fp, $row);
	}
	
	exit();
}



