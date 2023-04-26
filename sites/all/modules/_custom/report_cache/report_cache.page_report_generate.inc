<?php

function report_cache_page_report_generate($report_type) {
	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }

	if (!$info) { return 'Report type not found.'; }
	if ($info['path']) { drupal_goto($info['path']); }
	if (!$info['form_callback']) { return 'Generation callback not found.'; }
	drupal_set_title("Reports: " . $info['name']);
	sirius_backlink_add('sirius/reports', array('name' => 'All Reports'));

	return drupal_get_form($info['form_callback'], $report_type);
}
