<?php

function report_cache_page_report($report_type) {
	
	$report_id = $_REQUEST['report_id'];
	$action = $_REQUEST['action'];

	$report_type_infos = module_invoke_all('report_cache_info');
	$report_type_info = $report_type_infos[$report_type];
	$fields = $report_type_info['fields'];
	drupal_set_title(t('Report - @report_type', array('@report_type' => $report_type_info['name'])));


	if ($action == 'purge') {
		$count = report_cache_purge($report_type, $report_id);
		drupal_set_message(t('Purged report [@report_id]; @count records deleted.', array('@report_id' => $report_id, '@count' => $count)));
	} elseif ($action == 'download') {
		report_cache_download($report_type, $report_id, $report_type_info);
	} elseif ($action == 'preview') {
		$preview .= report_cache_preview($report_type, $report_id, $report_type_info);
	} elseif ($action == 'render') {
		$render .= report_cache_render($report_type, $report_id, $report_type_info, $_REQUEST['callback-name']);
	}

	if ($render) {
		$html = '<div class="report-cache-backlink">' . '[' . l(t('Back to report list'), "admin/reports/report-cache/$report_type", array('query' => array('report_id' => $tmp_report_id, 'action' => 'preview'))) . ']' . '</div>';
		$html .= $render;
		return $html;
	}

	$sql = "select distinct report_id from report_cache where report_type = :report_type order by report_id desc";
	$stmt = db_query($sql, array(':report_type' => $report_type));
	$report_ids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$report_ids[] = $hr['report_id'];
	}
	
	if (!$report_ids) {
		drupal_set_message(t('No reports of type @report_type have been generated yet.', array('@report_type' => $report_type)));
	}

	$header = array();
	$header[] = t('Report ID');
	$header[] = t('Preview');
	$header[] = t('Download');
	if ($report_type_info['render_callbacks']) {
		foreach ($report_type_info['render_callbacks'] as $name => $callback) {
			$header[] = $name;
		}
	}
	$header[] = t('Purge');

	$attributes = array();

	$rows = array();
	foreach ($report_ids as $tmp_report_id) {
		$row = array();

		$row[] = $tmp_report_id;
		$row[] = l(t('Preview'), "admin/reports/report-cache/$report_type", array('query' => array('report_id' => $tmp_report_id, 'action' => 'preview')));
		$row[] = l(t('Download'), "admin/reports/report-cache/$report_type", array('query' => array('report_id' => $tmp_report_id, 'action' => 'download')));
		if ($report_type_info['render_callbacks']) {
			foreach ($report_type_info['render_callbacks'] as $name => $callback) {
				$row[] = l(
					$name, 
					"admin/reports/report-cache/$report_type", 
					array(
						'query' => array(
							'report_id' => $tmp_report_id, 
							'action' => 'render',
							'callback-name' => $name,
						)
					)
				);
			}
		}
		$row[] = l(t('Purge'), "admin/reports/report-cache/$report_type", array('query' => array('report_id' => $tmp_report_id, 'action' => 'purge')));

		$rows[] = $row;
	}

	$result .= '<h2>' . t('All Reports of Type: @report_type', array('@report_type' => $report_type)) . '</h2>';

	$result .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));

	if ($preview) {
		$result .= '<hr>';
		$result .= $preview;
	}

	$result .= l(t('Generate a new report'), $report_type_info['path']);
	
	return $result;
}


function report_cache_preview($report_type, $report_id, $report_type_info) {
	$fields = $report_type_info['fields'];

	$result = '';

	$result .= "<h2>$report_id Preview</h2>";

	$header = array_values($fields);
	$query = db_select('report_cache');
	$query->fields('report_cache');
	$query->condition('report_id', $report_id);
	$query->condition('report_type', $report_type);
	$query = $query->extend('PagerDefault');
	$query->limit(100);
	
	$stmt = $query->execute();
	
	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$hr = unserialize($hr['report_data']);
		$row = array();
		foreach ($fields as $name => $prompt) {
			$row[] = $hr[$name];
		}
		$rows[] = $row;
	}
	
	$result .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	$result .= theme('pager', array('tags' => array()));

	return $result;
}

function report_cache_render($report_type, $report_id, $report_type_info, $callback_name) {
	$callback = $report_type_info['render_callbacks'][$callback_name];
	if (!$callback) { 
		drupal_set_message(t('Unable to find callback named "@name"', array('@name' => $callback_name)), 'error');
		return NULL;
	}

	return $callback($report_type, $report_id);
}




function report_cache_download($report_type, $report_id, $report_type_info) {

	if ($report_type_info['download_callback']) {
		$callback = $report_type_info['download_callback'];
		return $callback($report_type, $report_id, $report_type_info);
	}

	$fields = $report_type_info['fields'];

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
	$query->condition('report_id', $report_id);
	$stmt = $query->execute();
	
	while ($hr = $stmt->fetchAssoc()) {
		$hr = unserialize($hr['report_data']);
		$row = array();
		foreach ($fields as $name => $prompt) {
			$row[] = $hr[$name];
		}
		fputcsv($fp, $row);
	}
	
	exit();
}



