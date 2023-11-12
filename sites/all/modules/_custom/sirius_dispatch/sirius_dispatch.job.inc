<?php

function sirius_dispatch_job_recompute_from_dispatch($dispatch_node, $is_delete = FALSE) {
	$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
	if (!$job_nid) { return; }
	$job_node = node_load($job_nid);
	if (!$job_node) { return; }

	// Fetch from the database
	$sql = "select field_sirius_dispatch_status_value as status, field_data_field_sirius_dispatch_status.entity_id as nid ";
	$sql .= "from field_data_field_sirius_dispatch_status, field_data_field_sirius_dispatch_job ";
	$sql .= "where field_data_field_sirius_dispatch_status.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_job.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_status.entity_id = field_data_field_sirius_dispatch_job.entity_id ";
	$sql .= "and field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id = :job_nid ";
	$stmt = db_query($sql, array(':job_nid' => $job_nid));
	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['nid']] = $hr['status'];
	}

	// Update with the current node, since it might not be written to the database yet
	if ($is_delete) {
		unset($dispatches[$dispatch_node->nid]);
	} else {
		$dispatches[$dispatch_node->nid] = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	}

	// Count
	$count_notified = 0;
	$count_yes = 0;
	$count_no = 0;
	foreach ($dispatches as $ignore => $status) {
		if ($status == 'notified') { ++$count_notified; }
		if ($status == 'accepted' || $status == 'accepted_secondary' || $status == 'resigned' || $status == 'vlri' || $status == 'terminated') { ++$count_yes; }
		if ($status == 'declined' || $status == 'notification failed' || $status == 'expired') { ++$count_no; }
	}

	$need_save = FALSE;

	if ($job_node->field_sirius_count_yes['und'][0]['value'] != $count_yes) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_yes['und'][0]['value'] = $count_yes;
	}

	if ($job_node->field_sirius_count_no['und'][0]['value'] != $count_no) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_no['und'][0]['value'] = $count_no;
	}

	if ($job_node->field_sirius_count_notified['und'][0]['value'] != $count_notified) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_notified['und'][0]['value'] = $count_notified;
	}

	if ($need_save) {
		node_save($job_node);
	}
}

function sirius_dispatch_job_workerform_ajax($form, &$form_state) {
	return $form['fieldset_workers']['ajax'];
}

function sirius_dispatch_job_workerform($form, &$form_state, $states = NULL) {
	// Job node
	$job_node = $form_state['job_node'];
	$job_policy = sirius_dispatch_job_policy($job_node);

	// Can we create dispatches for ineligible workers? Usually we can, but some job types prohibit this.  In particular, you
	// can't create a dispatch on a rollover job for anybody who's not rollover.
	$must_be_eligible = ($job_policy['workers']['must_be_eligible'] == 'Yes');

	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$member_status_abbrv = array();
	foreach ($member_status_terms as $tid => $term) {
  	if ($term->field_sirius_name_short['und'][0]['value']) {
  		$term_name = $term->field_sirius_name_short['und'][0]['value'];
  	} else {
  		$term_name = $term->name;
  	}
  	$member_status_abbrv[$tid] = $term_name;
	}

	// drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.worker_filter.js', 'file');
	$form['fieldset_workers']  = array(
		'#type' => 'fieldset',
		'#title' => 'Requested Workers',
		'#states' => $states,
	);

	// Filter ID
	$form['fieldset_workers']['worker_id'] = array(
		'#type' => 'textfield',
		'#title' => 'Filter by member name or ID. Enter * to see all dispatchable workers.',
		'#size' => 20,
		'#states' => $states,
		'#prefix' => '<div class="container-inline">',
	);

	$form['fieldset_workers']['go'] = array(
		'#type' => 'button',
		'#value' => 'Filter',
    '#ajax' => array(
      'callback' => 'sirius_dispatch_job_workerform_ajax',
      'wrapper' => 'workers_filtered',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    // '#limit_validation_errors' => array(),
		'#suffix' => '</div>',
	);

	//
	// Which workers should we show? It's complicated, because we want to show some that are not dispatchable
	// 

	// We're searching for this worker
	$search_worker_id = $form_state['values']['fieldset_workers']['worker_id'];

	// We've selected these workers
	$selected_workers = array();
	if ($form_state['values']['fieldset_workers']['ajax']['workers']) {
		foreach ($form_state['values']['fieldset_workers']['ajax']['workers'] as $nid => $value) { 
			if ($value) { 
				$selected_workers[$nid] = TRUE;
			}
		}
	}

	// Find any dispatchable workers
	$dispatchable_workers = array();
	if ($search_worker_id || $selected_workers) { 
		$dispatchable_workers = sirius_dispatch_elig_fetch($job_node, 'name');
	}

	// Start with any dispatchable workers who are already checked, and who match on name or ID
	$workers = array();
	$found_id = FALSE;
	foreach ($dispatchable_workers as $nid => $worker) {
		$is_good = FALSE; 
		if ($selected_workers[$nid]) { $is_good = TRUE; }
		if ($worker['id'] == $search_worker_id) { $is_good = TRUE; $found_id = TRUE; }
		if ($search_worker_id == '*') { $is_good = TRUE; }
		if (stripos($worker['family'], $search_worker_id) !== FALSE) { $is_good = TRUE; }
		if (stripos($worker['given'], $search_worker_id) !== FALSE) { $is_good = TRUE; }
		if ($is_good) { $workers[$nid] = $worker; }
	}

	// Next, if we're searching by ID, and if the worker is non-dispatchable, add them with an explanation.
	$non_dispatchable_workers = array();
	foreach ($selected_workers as $nid => $ignore) {
		if (!$workers[$nid]) {
			$non_dispatchable_workers[$nid] = TRUE;
		}
	}
	if ($search_worker_id > 0 && !$found_id) {
		$nid = sirius_find_nid_by_id($search_worker_id, 'sirius_worker');
		if ($nid) { $non_dispatchable_workers[$nid] = TRUE; }
	}
	foreach ($non_dispatchable_workers as $nid => $ignore) {
		$sql = "select field_data_field_sirius_id.entity_id, field_sirius_id_value, field_sirius_name_family, field_sirius_name_given ";
		$sql .= "from field_data_field_sirius_id, field_data_field_sirius_name ";
		$sql .= "where field_data_field_sirius_id.entity_id = :nid ";
		$sql .= "and field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_id.entity_id = field_data_field_sirius_name.entity_id ";
		$sql .= "order by field_data_field_sirius_id.entity_id desc limit 1 ";
		$stmt = db_query($sql, array(':nid' => $nid));
		$hr = $stmt->fetchAssoc();
		if ($hr['entity_id']) {
			$workers[$nid] = array();
			$workers[$nid]['id'] = $hr['field_sirius_id_value'];
			$workers[$nid]['family'] = $hr['field_sirius_name_family'];
			$workers[$nid]['given'] = $hr['field_sirius_name_given'];
			$result = sirius_dispatch_elig_explain($job_node, $nid);
			if (!$result['success']) {
				$workers[$nid]['msg'] = $result['msg'];	
			}
			
		}
	}

  $worker_options = array();
  foreach ($workers as $worker_nid => $worker) {
  	$tid = $worker['member_status_tid'];
  	$worker_options[$worker_nid] = $worker['family'] . ', ' . $worker['given'];
  	if ($tid) { $worker_options[$worker_nid] .= ' [' . $member_status_abbrv[$tid] . ']'; }
  	if ($worker['eba'] == 'Yes') { $worker_options[$worker_nid] .= ' [EBA]'; }
  	if ($worker['msg']) { $worker_options[$worker_nid] .= '<br><span class="msg">' . $worker['msg'] . '</span>'; }
  }

  $form['fieldset_workers']['ajax'] = array(
  	'#type' => 'fieldset',
  	'#prefix' => '<div id="workers_filtered">',
  	'#suffix' => '</div>',
  );

  /*
	$form['fieldset_workers']['ajax']['debug'] = array(
		'#type' => 'markup',
		'#markup' => 'NONCE: ' . rand(0, 10000) . ' / JOB: ' . $job_node->title . ' / ' . ' count: ' . count($dispatchable_workers) . ' / DEBUG: ' . $debug,
	);
	*/

  if ($worker_options) {
		$form['fieldset_workers']['ajax']['workers'] = array(
			'#type' => 'checkboxes',
			'#options' => $worker_options,
			'#title' => 'Workers',
			'#prefix' => "<div class=\"checkboxes-inline\">",
			'#suffix' => '</div><div style="clear: left;">&nbsp;</div></div>',
			'#states' => $states,
		);
		if ($must_be_eligible) {
			foreach ($non_dispatchable_workers as $nid => $ignore) {
				if ($worker_options[$nid]) {
					$form['fieldset_workers']['ajax']['workers'][$nid]['#disabled'] = TRUE;
				}
			}
		}
	} else if ($search_worker_id || $selected_workers) {
		$form['fieldset_workers']['ajax']['workers'] = array(
			'#type' => 'markup',
			'#markup' => t('No workers match your criteria.'),
			'#title' => 'Workers',
		);
	}

	return $form;	
}

function sirius_dispatch_layoff_expired_jobs($mode = 'test') {
	$date = gmdate('Y-m-d H:i:s');

	$mode = "test";
	sirius_minilog("[This feature is still under development, so forcing mode = $mode]");

	$sql = "select distinct node.nid from node ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = job_node.nid ";
	$sql .= "left join field_data_field_sirius_datetime_completed on field_data_field_sirius_datetime_completed.entity_type = 'node' and field_data_field_sirius_datetime_completed.entity_id = job_node.nid ";
	$sql .= "where node.type = 'sirius_dispatch' ";
	$sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
	$sql .= "and field_sirius_datetime_value is not null ";
	$sql .= "and field_sirius_datetime_value <= field_sirius_datetime_completed_value ";
	$sql .= "and field_sirius_datetime_completed_value <= :date ";
	$sql .= "order by node.nid desc limit 1000 ";
	
	$stmt = db_query($sql, array(':date' => $date));
	$dispatch_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatch_nids[] = $hr['nid'];
	}

	$count = $count_success = $count_failure = 0;
	foreach ($dispatch_nids as $dispatch_nid) {
		++$count;
		$dispatch_node = node_load($dispatch_nid);
		sirius_minilog("Laying off: <a href=\"/node/$dispatch_nid\">Dispatch #$dispatch_nid</a>");

		if ($mode == 'live') {
			$result = sirius_dispatch_terminate(
				$dispatch_node, 
				'The job\'s end date has passed.'
			);
			if ($result['success']) {
				++$count_success;
				sirius_minilog("-- Layoff succeeded");
			} else {
				++$count_failure;
				$last_failure_msg = $result['msg'];
				sirius_minilog("-- Layoff failed: " . $result['msg'], 'warning');
			}
		} else {
			sirius_minilog("-- Skipping layoff because mode = $mode"); 
		}

	}

	$msg = "Cleaned up $count dispatches, with $count_success success and $count_failure errors. ";
	if ($last_failure_msg) { $msg .= " The last error message was: $last_failure_message."; }
	return array('success' => TRUE, 'msg' => $msg, 'count' => $count_success);
}


function sirius_dispatch_layoff_secondary_without_primary($mode = 'test') {
	//$mode = "test";
	//sirius_minilog("[This feature is still under development, so forcing mode = $mode]");

	// Get the job types
	$job_types = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'full');
	foreach ($job_types as $job_type_tid => $job_type_term) {
		$job_type_json = sirius_json_get($job_type_term);
		if ($job_type_json['workers']['layoff_secondary_without_primary'] != 'Yes') {
			unset($job_types[$job_type_tid]);
			continue;
		}
		sirius_minilog("Adding job type: " . $job_type_term->name);
	}
	if (!$job_types) {
		return array('success' => TRUE, 'msg' => 'No job types have the flag "layoff_secondary_without_primary" set to "Yes".');
	}

	// Get all secondary dispatches for these job types
	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql = "select distinct node.nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_sirius_dispatch_job_type on field_data_field_sirius_dispatch_job_type.entity_type = 'node' and field_data_field_sirius_dispatch_job_type.entity_id = job_node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = worker_node.nid ";

	$sql .= "where node.type = 'sirius_dispatch' ";

	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}

	$sql .= "and field_sirius_dispatch_status_value = 'accepted_secondary' ";
	$sql .= "and field_grievance_shop_target_id is null ";

	$sql .= "and field_sirius_dispatch_job_type_tid in (:job_type_tids) ";
	$sql_args[':job_type_tids'] = array_keys($job_types);
	
	$stmt = db_query($sql, $sql_args);
	$dispatch_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatch_nids[] = $hr['nid'];
	}
	if (!$dispatch_nids) {
		return array('success' => TRUE, 'msg' => 'No secondary dispatches found for these job types where the worker does not have a primary dispatch.');
	}

	$count = $count_success = $count_failure = 0;
	foreach ($dispatch_nids as $dispatch_nid) {
		++$count;
		$dispatch_node = node_load($dispatch_nid);
		sirius_minilog("Laying off: <a href=\"/node/$dispatch_nid\">Dispatch #$dispatch_nid</a>");

		if ($mode == 'live') {
			$result = sirius_dispatch_terminate(
				$dispatch_node, 
				'Laying off a secondary dispatch because there is no current primary dispatch.'
			);
			if ($result['success']) {
				++$count_success;
				sirius_minilog("-- Layoff succeeded");
			} else {
				++$count_failure;
				$last_failure_msg = $result['msg'];
				sirius_minilog("-- Layoff failed: " . $result['msg'], 'warning');
			}
		} else {
			sirius_minilog("-- Skipping layoff because mode = $mode"); 
		}

	}

	$msg = "Cleaned up $count dispatches, with $count_success success and $count_failure errors. ";
	if ($last_failure_msg) { $msg .= " The last error message was: $last_failure_message."; }
	return array('success' => TRUE, 'msg' => $msg, 'count' => $count_success);
}


function sirius_dispatch_job_counts($job_nid) {
	$sql = "select field_sirius_dispatch_status_value as status, count(distinct node.nid) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "where field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id = :job_nid ";
	$sql .= "group by field_sirius_dispatch_status_value order by field_sirius_dispatch_status_value ";
	$stmt = sirius_sql_query($sql, array(':job_nid' => $job_nid));

	$counts = array();
	while ($hr = $stmt->fetchAssoc()) {
		$counts[$hr['status']] = $hr['c'];
	}

	return $counts;
}

function sirius_dispatch_job_counts_render($job_nid) {
	$counts = sirius_dispatch_job_counts($job_nid);
	$status_options = sirius_dispatch_status_options();

	$total_options = array(
		'accepted' => array(
			'title' => 'Accepted',
			'color' => 'green',
			'statuses' => array('accepted', 'accepted_secondary', 'resigned', 'vlri', 'terminated'),
		),

		'requested' => array(
			'title' => 'Requested',
			'color' => 'yellow',
			'statuses' => array('notified', 'requested'),
		),

		'declined' => array(
			'title' => 'Declined',
			'color' => 'grey',
			'statuses' => array('declined', 'notification failed', 'expired'),
		),
	);

	$row = $header = array();
	foreach ($total_options as $key => $record) {
		$total = 0;
		$details = array();
		foreach ($record['statuses'] as $status) {
			$count = $counts[$status];
			if (!$count) { continue; }

			$total += $count;
			$details[] = $count . ' ' . $status_options[$status];
		}

		// if (!$total) { continue; }
		$data = '<strong>' . $total . '</strong>';
		if ($details) { $data .= ' (' . join(', ', $details) . ')'; }
		$row[] = array('data' => $data, 'class' => array($record['color']), 'style' => 'width: 33%;');
		$header[] = $record['title'];
	}
	$rows[] = $row;

	return sirius_table_render($header, $rows);
}
