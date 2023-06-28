<?php

/**
 * The "Three Strikes" rule.
 *
 * If you are signed in ("not employed / not available"), and you decline (or allow to expire) at least one dispatch
 * in each of three consecutive days, then you are automatically signed out ("not employed / not available").
 */

function sirius_dispatch_threestrikes_admin() {
  $form = array();

  $hours = array();
  for ($i=0; $i<=24; ++$i) {
  	$hours[$i] = $i;
  }

  $minutes = array();
  for ($i=0; $i<=59; ++$i) {
  	$minutes[$i] = str_pad($i, 2, '0', STR_PAD_LEFT);
  }

	$form['sirius_dispatch_threestrikes_enable'] = array(
		'#type' => 'select',
		'#title' => 'Activate the three-strikes rule?',
		'#options' => array(
			'No' => 'No, do not run the three-strikes rule on this site',
			'Yes' => 'Yes, the three-strikes rule is active on this site.',
		),
		'#default_value' => variable_get('sirius_dispatch_threestrikes_enable', 'No'),
	);

	$form['sirius_dispatch_threestrikes_start_hr'] = array(
		'#type' => 'select',
		'#title' => 'Three strikes scan should start every day at this hour:',
		'#options' => $hours,
		'#default_value' => variable_get('sirius_dispatch_threestrikes_start_hr', 1),
	);

	$form['sirius_dispatch_threestrikes_start_min'] = array(
		'#type' => 'select',
		'#title' => 'Three strikes scan should start every day at this minute:',
		'#options' => $minutes,
		'#default_value' => variable_get('sirius_dispatch_threestrikes_start_min', 30),
	);

	$form['sirius_dispatch_threestrikes_worker_notification_subject'] = array(
		'#type' => 'textfield',
		'#title' => 'Email notification subject',
		'#description' => 'Subject of the email which is sent to the worker',
		'#default_value' => variable_get('sirius_dispatch_threestrikes_worker_notification_subject', ''),
	);

	$form['sirius_dispatch_threestrikes_worker_notification_body'] = array(
		'#type' => 'textarea',
		'#title' => 'Email notification body',
		'#description' => 'Body of the email which is sent to the worker. May contain HTML.',
		'#default_value' => variable_get('sirius_dispatch_threestrikes_worker_notification_body', ''),
	);

	$form['sirius_dispatch_threestrikes_worker_notification_sms'] = array(
		'#type' => 'textarea',
		'#title' => 'SMS notification body',
		'#description' => 'Body of the SMS which is sent to the worker. No HTML is permitted.',
		'#default_value' => variable_get('sirius_dispatch_threestrikes_worker_notification_sms', ''),
	);

  return system_settings_form($form);
}

function sirius_dispatch_threestrikes_cron() {
	// Enabled?
	$enabled = variable_get('sirius_dispatch_threestrikes_enable', 'No');
	if ($enabled != 'Yes') { return; }
	
	// Run once a day
	$today = date('Y-m-d');
	$last_run = variable_get('sirius_dispatch_threestrikes_last_date', 0);
	if ($last_run == $today) { return; }

	// Don't run before a specific time
	$hour = variable_get('sirius_dispatch_threestrikes_start_hr', 1);
	$min = variable_get('sirius_dispatch_threestrikes_start_min', 30);
	if (time() < strtotime("$today $hour:$min")) { return; }

	// Run the scan
	$result = sirius_dispatch_threestrikes_scan();
	watchdog('sirius_dispatch_threestrikes', "Ran a threestrikes scan. Looked at " . $result['count_scanned'] . " workers, signed out " . $result['count_signed_out'] . ".");
	variable_set('sirius_dispatch_threestrikes_last_date', $today);
}

function sirius_dispatch_threestrikes_page() {
	return drupal_get_form('sirius_dispatch_threestrikes_form');
}

function sirius_dispatch_threestrikes_form($form, &$form_state) {
	$form['action'] = array(
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'worker' => t('Run Three-Strikes on a single worker'),
			'all' => t('Run Three-Strikes on all workers'),
		),
		'#title' => t('Action'),
		'#required' => TRUE,
	);

	$form['worker_nid'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => 'Enter the worker node ID (NID)',
		'#states' => array(
	  	'visible' => array(
	  		array(':input[name="action"]' => array('value' => 'worker')),
	  	)
		),
	);

	$form['worker_force'] = array(
		'#type' => 'select',
		'#title' => t('FORCE a three-strikes block for this worker, no matter what'),
		'#options' => array(
			'No' => t('No, run the usual three-strikes test on this worker.'),
			'Yes' => t('Yes, this worker should have a three-strikes block placed immediately.'),
		),
		'#states' => array(
	  	'visible' => array(
	  		array(':input[name="action"]' => array('value' => 'worker')),
	  	)
		),
	);

	$form['is_test'] = array(
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			TRUE => t('Test mode'),
			FALSE => t('Live mode'),
		),
		'#title' => t('Mode'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	if ($form_state['results']) {
		$form['results'] = array(
			'#type' => 'markup',
			'#markup' => $form_state['results'],
		);
	}

	return $form;
}

function sirius_dispatch_threestrikes_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$action = $form_state['values']['action'];
	$worker_nid = $form_state['values']['worker_nid'];
	$is_test = $form_state['values']['is_test'];
	$is_force = $form_state['values']['worker_force'];
	unset($form_state['results']);

	if ($is_test) {
		drupal_set_message("Operating in TEST mode.");
	} else {
		drupal_set_message("Operating in LIVE mode.");
	}

	if ($is_force) {
		drupal_set_message("Forcing a three-strikes block without checking for declined dispatches.");
	}

	if ($action == 'worker') {
		$result = sirius_dispatch_threestrikes_scan_worker($worker_nid, $is_test, $is_force);
		drupal_set_message("Scanning a single worker: <pre>" . $result['msg'] . '</pre>');
	} else {
		$result = sirius_dispatch_threestrikes_scan($is_test);

		$rows = array();
		foreach ($result['results'] as $hr) {
			$row = array();
			$row[] = 'Worker #' . l($hr['worker_nid'], 'node/' . $hr['worker_nid']);
			$row[] = '<pre>' . $hr['msg'] . '</pre>';
			if ($hr['signed_out']) {
				$row[] = 'SIGNED OUT';
			} else {
				$row[] = 'unchanged';
			}
			$rows[] = $row;
		}

		$header = array(
			'Worker',
			'Scan',
			'Result'
		);
		$form_state['results'] = '<div class="sirius_form_results">' . 
			'<hr>' . '<h2>' . 'Results' . '</h2>' .
			theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array())) .
			'</div>';

			drupal_set_message(t("Scanned <strong>@count_scanned</strong> workers, signed out <strong>@count_signed_out</strong>. The details are below.", array('@count_scanned' => $result['count_scanned'], '@count_signed_out' => $result['count_signed_out'])));
	}
}

function sirius_dispatch_threestrikes_scan($is_test = FALSE) {
	// Fetch the currently signed-in workers
	$available_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
	if (!$available_tid) { return array('success' => FALSE, 'msg' => 'Configuration error: no Not Employed / Available TID'); }
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_worker');
  $query->fieldCondition('field_sirius_worker_dispstatus', 'tid', $available_tid);
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if (!$stmt['node']) {
 		return array('success' => TRUE, 'msg' => 'There are no workers on the Out Of Work list (nobody Not Employed / Available.)');
  }
  $worker_nids = array_keys($stmt['node']);
  $msg = '';
  $results = array();
  $count_scanned = 0;
  $count_signed_out = 0;
  foreach ($worker_nids as $worker_nid) {
		$result = sirius_dispatch_threestrikes_scan_worker($worker_nid, $is_test);
		$result['worker_nid'] = $worker_nid;
		$results[] = $result;
		++$count_scanned;
		if ($result['signed_out']) {
			++$count_signed_out;
		}
  }

  return array(
  	'success' => TRUE, 
  	'msg' => "Scanned $count_scanned workers, signed out $count_signed_out.",
  	'results' => $results,
  	'count_scanned' => $count_scanned,
  	'count_signed_out' => $count_signed_out
  );
}

function sirius_dispatch_threestrikes_scan_worker($worker_nid, $is_test = FALSE, $is_force = FALSE) {
	if ($is_force) {
		$msg = "Skipping scan for worker #<a href=\"/node/$worker_nid\">$worker_nid</a> (forcing three-strikes):\n";
		$msg .= "The administrator is forcing a three-strikes block on this worker.";
	} else {
		$msg = "Scanning worker #<a href=\"/node/$worker_nid\">$worker_nid</a>:\n";
		for ($day_start = 1; $day_start <=3; ++$day_start) {
			$result = sirius_dispatch_threestrikes_scan_worker_day($worker_nid, $day_start);
			$msg .= $result['msg'] . "\n";
			if (!$result['dispatch_nid']) {
				$msg .= "This worker will NOT be signed out.";
				return array('success' => TRUE, 'msg' => $msg, 'signed_out' => FALSE);
			} 
		}
		$msg .= "Dispatches were declined on three consecutive days, so this worker will be signed out.";
	}

	$worker_node = node_load($worker_nid);
	$tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
	if (!$is_test) {
		sirius_dispatch_worker_dispstatus_set($worker_node, $tid, 1, $msg);
		sirius_dispatch_worker_dispstatus_block_set($worker_node, sirius_get_special_tid('sirius_dispatch_sib|threestrikes'), $msg);
		sirius_worker_notify(
			$worker_node, 
			array(
				'subject' => variable_get('sirius_dispatch_threestrikes_worker_notification_subject', ''),
				'body' => variable_get('sirius_dispatch_threestrikes_worker_notification_body', ''),
				'sms' => variable_get('sirius_dispatch_threestrikes_worker_notification_sms', ''),
			)
		);

	}
	return array('success' => TRUE, 'msg' => $msg, 'signed_out' => TRUE);
}

function sirius_dispatch_threestrikes_scan_worker_day($worker_nid, $day_start) {
	$today_ts = strtotime(date('Y-m-d'));
	$day_end = $day_start-1;
	$start_ts = strtotime("-$day_start days", $today_ts);
	$end_ts = strtotime("-$day_end days", $today_ts);

	// Search for dispatches
	// - Where the dispatch status is "declined" or "expired"
	// - Where the job status is "filled" or "archived"
	// - Where the dispatch is created between start_ts and end_ts
	// Do in SQL for efficiency

	$tables = array(
		'field_data_field_sirius_dispatch_status',
		'field_data_field_sirius_worker',
		'field_data_field_sirius_dispatch_job',
	);
	$sql .= "select max(node.nid) as nid from node ";
	foreach ($tables as $table) {
		$sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
	}

	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_sirius_dispatch_job_status on field_data_field_sirius_dispatch_job_status.entity_type = 'node' and field_data_field_sirius_dispatch_job_status.entity_id = job_node.nid ";

	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
	$sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = field_grievance_shop_target_id ";
	$sql .= "left join field_data_field_sirius_dispatch_3sen on field_data_field_sirius_dispatch_3sen.entity_type = 'taxonomy_term' and field_data_field_sirius_dispatch_3sen.entity_id = field_sirius_industry_tid ";

	$sql .= "where node.type = 'sirius_dispatch' ";
	$sql .= "and field_sirius_dispatch_status_value in ('expired', 'declined') ";
	$sql .= "and field_sirius_dispatch_job_status_value in ('Filled', 'Archived') ";
	$sql .= "and field_sirius_dispatch_3sen_value = 'Yes' ";
	$sql .= "and node.created >= :start_ts ";
	$sql .= "and node.created < :end_ts ";
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$args[':start_ts'] = $start_ts;
	$args[':end_ts'] = $end_ts;
	$args[':worker_nid'] = $worker_nid;

	$stmt = db_query($sql, $args);
	$hr = $stmt->fetchAssoc();
	$msg = "On " . date('Y-m-d', $start_ts) . ", ";
	if ($hr['nid']) {
		$nid = $hr['nid'];
		$msg .= "at least one dispatch, #<a href=\"/node/$nid\">$nid</a> was declined";
	} else {
		$msg .= "no dispatches were declined.";
	}
	return array('success' => TRUE, 'dispatch_nid' => $hr['nid'], 'msg' => $msg);
}