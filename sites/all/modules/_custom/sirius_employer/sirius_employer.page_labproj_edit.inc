<?php

function sirius_employer_node_tab_labproj_edit_access($labproj_node) {
	if ($labproj_node->field_sirius_category['und'][0]['value'] != 'labproj') { return FALSE; }
	if (!in_array($labproj_node->field_sirius_type['und'][0]['value'], array('draft', 'active'))) { return FALSE; }

	// Employers can access their own page only
	if (!user_access('sirius employer admin')) { 
		$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
		global $user;
		$user_full = user_load($user->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] != $employer_node->nid) { return FALSE; }
	}

	return TRUE; 
}

function sirius_employer_node_tab_labproj_edit($labproj_node) {
	return drupal_get_form('sirius_employer_node_tab_labproj_edit_form', $labproj_node);
}

function sirius_employer_node_tab_labproj_edit_form($form, &$form_state, $labproj_node) {
	$form_state['labproj_nid'] = $labproj_node->nid;

	$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
	$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
	$labproj_json = sirius_json_get($labproj_node);
	$status = sirius_employer_labproj_get_status($labproj_node);
	$status_options = sirius_employer_labproj_status_options();

	list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);

	$params = array();
	$ts = strtotime("$year-$month-01");
	$params['industry_tids'] = $industry_tids;
	$params['interval'] = array(
		'start_ts' => strtotime("-1 day", $ts),
		'end_ts' => strtotime("+1 month", $ts),
	);
	$params['nids_only'] = TRUE;
	$group_options = sirius_dispatch_group_search($params);

	$detail_by_group = sirius_employer_labproj_is_detail_by_group($labproj_json);
	$job_group_nids = array();
	if ($detail_by_group) {
		if ($labproj_json['settings']['job_groups']) {
			$job_group_nids = array_filter(array_values($labproj_json['settings']['job_groups']));
		}
		if (!$job_group_nids) { $detail_by_group = FALSE; }
	}

	list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);

	$last_day_of_month = date('t', strtotime("$year-$month-15"));

	$form['#tree'] = TRUE;

	$table_open_html = '<table>';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>' . t('Date') . '</th>';
	if (!$detail_by_group) { $table_open_html .= '<th>' . t('Total') . '</th>'; }
	foreach ($job_group_nids as $job_group_nid) {
		$table_open_html .= '<th>' . $group_options[$job_group_nid] . '</th>';
	}
	$table_open_html .= '</tr>';
	$form['table_open'] = array('#markup' => $table_open_html);
	for ($day = 1; $day <= $last_day_of_month; ++$day) {
		$form['days'][$day]['row_open'] = array('#markup' => '<tr><td>' . date('D', strtotime("$year-$month-$day")) . " $month/$day/$year" . '</td>');

		if (!$detail_by_group) {
			$form['days'][$day]['total'] = array(
				'#type' => 'textfield',
				'#size' => 4,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value' => $labproj_json['days'][$day]['total'],
			);
		}

		foreach ($job_group_nids as $job_group_nid) {
			$form['days'][$day]['groups'][$job_group_nid] = array(
				'#type' => 'textfield',
				'#size' => 4,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value' => $labproj_json['days'][$day]['groups'][$job_group_nid],
			);
		}
		$form['days'][$day]['row_close'] = array('#markup' => '</tr>');
	}
	$form['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_employer_node_tab_labproj_edit_form_validate($form, &$form_state) {
	foreach ($form_state['values']['days'] as $day => $day_record) {
		if ($day_record['total'] && (
			!is_numeric($day_record['total']) || 
			($day_record['total'] < 0) || 
			(round($day_record['total']) != $day_record['total'])
		)) {
			form_set_error("days][$day][total", 'Please enter a valid number of workers.');
		}

		if ($day_record['groups']) {
			foreach ($day_record['groups'] as $group_nid => $value) {
				if ($value && (
					!is_numeric($value) || 
					($value < 0) || 
					(round($value) != $value)
					)) {
					form_set_error("days][$day][groups][$group_nid", 'Please enter a valid number of workers.');
				}
			}
		}
	}
}

function sirius_employer_node_tab_labproj_edit_form_submit($form, &$form_state) {
	$labproj_node = node_load($form_state['labproj_nid']);

	$result = sirius_employer_labproj_edit($labproj_node, $form_state['values']['days']);
	if (!$result['success']) {
		$form_state['rebuild'] = TRUE;
		drupal_set_message($result['msg'], 'error');
		return;
	}

	drupal_set_message("Labor projection saved.");
}