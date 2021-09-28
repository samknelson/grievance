<?php

function sirius_employer_node_tab_labproj_view_access($labproj_node) {
	if ($labproj_node->field_sirius_category['und'][0]['value'] != 'labproj') { return FALSE; }

	// If it's an employer, must be *this* employer
	if (!user_access('sirius employer admin')) {
		global $user;
		$my_employer_node = sirius_employer_get($user->uid);
		if (!$my_employer_node) { return FALSE; }
		$labproj_employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
		if ($my_employer_node->nid != $labproj_employer_node->nid) { return FALSE; }
	}

	return TRUE; 
}

function sirius_employer_node_tab_labproj_view($labproj_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_employer') . '/css/labproj.css', 'file');

	$form_state['labproj_nid'] = $labproj_node->nid;

	$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
	$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
	$group_options = sirius_dispatch_group_list_active($industry_tids);
	$labproj_json = sirius_json_get($labproj_node);
	$status = sirius_employer_labproj_get_status($labproj_node);
	$status_options = sirius_employer_labproj_status_options();

	$job_group_nids = array();
	if (sirius_employer_labproj_is_detail_by_group($labproj_json)) {
		if ($labproj_json['settings']['job_groups']) {
			$job_group_nids = array_filter(array_values($labproj_json['settings']['job_groups']));
		}
	}
	list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);

	$last_day_of_month = date('t', strtotime("$year-$month-15"));

	$header = array();
	$header[] = 'Date';
	$header[] = 'Total';
	foreach ($job_group_nids as $job_group_nid) {
		$header[] = $group_options[$job_group_nid];
	}

	$rows = array();

	$total_row = array();
	$total_row[] = '<strong>Total</strong>';
	$total_row[] = '<strong>' . $labproj_json['totals']['total'] . '</strong>';
	foreach ($job_group_nids as $job_group_nid) {
		$total_row[] = '<strong>' . $labproj_json['totals']['group_totals'][$job_group_nid] . '</strong>';
	}

	$max_row = array();
	$max_row[] = '<strong>Max</strong>';
	$max_row[] = '<strong>' . $labproj_json['totals']['max'] . '</strong>';
	foreach ($job_group_nids as $job_group_nid) {
		$max_row[] = '<strong>' . $labproj_json['totals']['group_maxes'][$job_group_nid] . '</strong>';
	}

	$rows[] = $total_row;
	$rows[] = $max_row;

	for ($day = 1; $day <= $last_day_of_month; ++$day) {
		$daystr = date('D', strtotime("$year-$month-$day"));
		$class = array();
		if ($daystr == 'Sat' || $daystr == 'Sun') { $class[] = 'weekend'; }

		$row = array();
		$row[] = $daystr . " $month/$day/$year";
		$total_render = $labproj_json['days'][$day]['total'];
		if ($total_render >= $labproj_json['totals']['max']) { $total_render = '<strong>' . $total_render . '</strong>'; }
		$row[] = $total_render;
		foreach ($job_group_nids as $job_group_nid) {
			$total_render = $labproj_json['days'][$day]['groups'][$job_group_nid];
			if ($total_render >= $labproj_json['totals']['group_maxes'][$job_group_nid]) { $total_render = '<strong>' . $total_render . '</strong>'; }
			$row[] = $total_render;
		}
		$rows[] = array('class' => $class, 'data' => $row);
	}

	$rows[] = $total_row;
	$rows[] = $max_row;

	$html = '';
	$html .= sirius_fakerender_field('Status', $status_options[$status]);
	$html .= sirius_fakerender_field('Comments', sirius_text_to_html($labproj_node->field_sirius_summary['und'][0]['value']));
	$html .= theme_table(array('header' => $header, 'attributes' => array(), 'rows' => $rows));

	return $html;
}
