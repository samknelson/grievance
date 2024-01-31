<?php

function sirius_teamsters631_node_tab_dispatch_oneoff_end_date_correct_form($form, &$form_state, $dispatch_node) {
	$form_state['dispatch_nid'] = $dispatch_node->nid;

	$form['intro'] = array(
		'#markup' => '<p>' . 'One-off end date correction tool: for dispatches that do not have an end date, search the log for events such as "terminate" or "accept", and set the end date to the date of the first such event.' . '</p>'
	);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => 'Test',
			'live' => 'Live',
		),
		'#description' => t('Test mode will perform all calculations, but will not update the dispatch.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Run the End Date Correction Script'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_teamsters631_node_tab_dispatch_oneoff_end_date_correct_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$dispatch_node = node_load($form_state['dispatch_nid']);
	$result = sirius_teamsters631_oneoff_end_date_correct($dispatch_node, $form_state['values']['mode']);
	if ($result['success']) {
		drupal_set_message("End dates corrected.");
	} else {
		drupal_set_message("Error: " . $result['msg'], 'warning');
	}

	return;
}

function sirius_teamsters631_oneoff_end_date_correct($dispatch_node, $mode = 'test') {
	sirius_minilog("Starting end date correction for: " . $dispatch_node->title . ' (' . $dispatch_node->nid . ')');

	if (isset($dispatch_node->field_sirius_date_start['und'][0]['value']) && isset($dispatch_node->field_sirius_date_end['und'][0]['value'])) {
		sirius_minilog('- Start and end dates were already set. Data correction not necessary.');
		return array('success' => TRUE);
	}

	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	sirius_minilog("- Current status: $status");

	if (!in_array($status, array('accepted', 'accepted_secondary', 'resigned', 'terminated', 'vlri'))) {
		sirius_minilog('- Based on the status, the dispatch was never accepted. Skipping.');
		return array('success' => TRUE);
	}

	$need_save = FALSE;

	if (!$dispatch_node->field_sirius_date_start['und'][0]['value']) {
		sirius_minilog('- Start date is empty. Attempting to set the start date.');

		$sql_args = array();
		$sql = "select created from node ";
		$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_log' ";

		$sql .= "and field_sirius_log_handler_target_id = :dispatch_nid ";
		$sql_args[':dispatch_nid'] = $dispatch_node->nid;

		$sql .= "and field_sirius_category_value in (:categories) ";
		$sql_args[':categories'] = array('dispatch:accept');
		$sql .= "and field_sirius_type_value in ('complete', 'accept') ";
		$sql .= "order by created limit 1 ";

		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$ts_accepted = $hr['created'];
		if ($ts_accepted) { sirius_minilog('- Dispatch was accepted on ' . date('Y-m-d', $ts_accepted)); }

		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$ts_report = sirius_fieldval($job_node, 'field_sirius_datetime', 'ts');
		sirius_minilog('- Report date was ' . date('Y-m-d', $ts_report));

		$ts_start = NULL;
		if ($ts_accepted) {
			if ($ts_report > $ts_accepted) {
				$ts_start = $ts_report;
			} else {
				$ts_start = $ts_accepted;
			}
		}

		if ($ts_start) {
			sirius_minilog('- Using start date: ' . date('Y-m-d', $ts_start));
			$dispatch_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d', $ts_start);
			$need_save = TRUE;
		}
	}

	$current_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if (!$dispatch_node->field_sirius_date_end['und'][0]['value'] && 
		in_array($status, array('resigned', 'terminated', 'vlri'))) {
		sirius_minilog('- End date is empty. Attempting to set the end date.');

		$sql_args = array();
		$sql = "select created from node ";
		$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_log' ";

		$sql .= "and field_sirius_log_handler_target_id = :dispatch_nid ";
		$sql_args[':dispatch_nid'] = $dispatch_node->nid;

		$sql .= "and field_sirius_category_value in (:categories) ";
		$sql_args[':categories'] = array(
			'dispatch:resign',
			'dispatch:rollover',
			'dispatch:terminate',
			'dispatch:vlri',
		);
		$sql .= "and field_sirius_type_value in ('complete', 'accept') ";
		$sql .= "order by created limit 1 ";

		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$ts_terminated = $hr['created'];
		if ($ts_terminated) { sirius_minilog('- Dispatch was completed on ' . date('Y-m-d', $ts_terminated)); }

		if ($ts_terminated) {
			sirius_minilog('- Using end date: ' . date('Y-m-d', $ts_terminated));
			$dispatch_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d', $ts_terminated);
			$need_save = TRUE;
		}
	}
	/*
dispatch:accept
dispatch:create
dispatch:decline
dispatch:expire
dispatch:notify
dispatch:resign
dispatch:rollover
dispatch:signin
dispatch:signout
dispatch:terminate
dispatch:undecline
dispatch:vlri
dispatch_job:manual
	*/

	if ($need_save) {
		if ($mode != 'live') {
			sirius_minilog("- [Skipping node save because mode = $mode]");
		} else {
			node_save($dispatch_node);
			sirius_minilog("- Dispatch node saved");
		}
	}

	return array('success' => TRUE);
}



