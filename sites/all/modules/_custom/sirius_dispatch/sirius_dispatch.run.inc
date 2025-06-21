<?php

function sirius_dispatch_run_all($mode = 'test', $force = FALSE, $options = array()) {
	// Too soon?
	$last_run = variable_get('sirius_dispatch_run_last_ts', 0);
	$delay = variable_get('sirius_dispatch_run_poll_delay', 300);
	if ($force && (time() - $last_run < $delay)) {
    return array('success' => TRUE, 'msg' => 'Too soon to run.');
	}
	variable_set('sirius_dispatch_run_last_ts', time());

	// Find all running jobs
	watchdog('sirius_dispatch', 'Runing all jobs.');
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch_job');
  $query->fieldCondition('field_sirius_dispatch_job_status', 'value', 'Running');
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if (!$stmt['node']) {
  	return array('success' => TRUE, 'msg' => 'No running jobs found.');
  }

  // Run all jobs
  $job_nids = array_keys($stmt['node']);
  $count = $count_success = $count_error = 0;
  foreach ($job_nids as $job_nid) {
    ++$count;
  	$job_node = node_load($job_nid);
  	$result = sirius_dispatch_run($job_node, $mode, $force, $options);
    if ($result['success']) { ++$count_success; } else { ++$count_error; }
  }

  return array('success' => TRUE, $msg => "Polled $count jobs with $count_success success and $count_error errors.", 'count' => $count);
}

function sirius_dispatch_run($job_node, $mode = 'test', $force = FALSE, $options = array()) {
  sirius_minilog("** Poll starting for: " . $job_node->title . ' (' . $job_node->nid . ')');

  // Switch into the job's domain
  $old_domain_nid = sirius_domain_nid();
  if (!$old_domain_nid) { $old_domain_nid = -1; }

  $domain_nid = $job_node->field_sirius_domain['und'][0]['target_id'];
  if (!$domain_nid) { $domain_nid = -1; }

  if ($domain_nid != $old_domain_nid) { sirius_domain($domain_nid); }

  // Close jobs after the report date
  $result = sirius_dispatch_run_check_report_date($job_node, $mode, $force, $options);

	// Expire dispatches
  if ($result['success']) { $result = sirius_dispatch_run_expire($job_node, $mode, $force, $options); }

	// Create dispatches
	if ($result['success']) { $result = sirius_dispatch_run_create_dispatches($job_node, $mode, $force, $options); }

	// Send notifications
	if ($result['success']) { $result = sirius_dispatch_run_notify($job_node, $mode, $force, $options); }

	// Are we filled?
	if ($result['success']) { $result = sirius_dispatch_run_finalize($job_node, $mode, $force, $options); }

  // Switch out of the job's domain
  if ($domain_nid != $old_domain_nid) { sirius_domain($old_domain_nid); }

  sirius_minilog("** Poll complete for: " . $job_node->title . ' (' . $job_node->nid . ')');

	return array('success' => TRUE, 'info' => $info);
}

function sirius_dispatch_run_check_report_date($job_node, $mode = 'test', $force = FALSE, $options = array()) {
  $report_ts = sirius_date_ts($job_node, 'field_sirius_datetime');

  if ($report_ts + 30*60*60 >= time()) {
    $msg = 'It is not necessary to close the job, because the report date has not yet passed.';
    sirius_minilog($msg);
    return array('success' => TRUE, 'msg' => $msg);
  }

  $job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Filled';
  $job_node->field_sirius_summary['und'][0]['value'] = 'Closing because report date passed on ' . date('c');

  $msg = "Report date has passed. Closing the job.";
  sirius_minilog($msg);

  if ($mode == 'live') {
    node_save($job_node);
  } else {
    sirius_minilog("(Skipping because mode = $mode)");
  }

  return array('success' => FALSE, 'msg' => $msg);
}


function sirius_dispatch_run_error($job_node, $msg, $mode = 'test', $force = FALSE, $options = array()) {
  if ($mode == 'live') {
  	sirius_log('job:run', 'error', $msg, '', '', array($job_node->nid));
  	$job_node->field_sirius_summary['und'][0]['value'] = $msg;
  	$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Unfilled';
  	node_save($job_node);
  } else {
    sirius_minilog($msg, 'warning');
    sirius_minilog("(Skipping because mode = $mode)");
  }
	return;
}

function sirius_dispatch_run_finalize($job_node, $mode = 'test', $force = FALSE, $options = array()) {
	// How many do we need?
	$count_we_need = $job_node->field_sirius_count['und'][0]['value'];

	// How many are requested, notified, or accepted?
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('accepted', 'accepted_secondary'), 'in');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $count_we_have = 0;
  if ($stmt['node']) {
  	$count_we_have = count(array_keys($stmt['node']));
  }

  if ($count_we_have < $count_we_need) {
  	// do nothing
    $msg = "The job is not ready to finalize, because we have $count_we_have accepted dispatches and need $count_we_need.";
    sirius_minilog($msg);
  	return array('success' => TRUE, 'msg' => $msg);
  }

  if ($count_we_have > $count_we_need) {
		$msg = "We need $count_we_need dispatches, but there are already $count_we_have dispatches with status accepted. This job is oversubscribed.";
	  sirius_dispatch_run_error($job_node, $msg, $mode, $force, $options);
		return array('success' => FALSE, 'msg' => $msg);
	}

	if ($count_we_have == $count_we_need) {
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Filled';
		$job_node->field_sirius_summary['und'][0]['value'] = 'Filled on ' . date('c');

    $msg = "There are $count_we_have dispatches with status accepted. Setting job status to Filled.";
    sirius_minilog($msg);

    if ($mode == 'live') {
      node_save($job_node);
    } else {
      sirius_minilog("(Skipping because mode = $mode)");
    }

    return array('success' => TRUE, 'msg' => $msg);
  }

  return array('success' => TRUE, 'msg' => "Unreachable code.");
}

function sirius_dispatch_run_notify($job_node, $mode = 'test', $force = FALSE, $options = array()) {
	// Make sure we're ok to send automatic notifications
  $job_type_term = taxonomy_term_load($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
  $job_type_json = sirius_json_get($job_type_term);
  $do_not_disturb_id = $job_type_json['run']['do_not_disturb']['id'];
  if (!$do_not_disturb_id) { $do_not_disturb_id = 'default'; }
  $result = sirius_do_not_disturb_check($do_not_disturb_id);
	if (!$result['success']) {
    $msg = "Do not disturb check failed: " . $result['msg'];
    sirius_minilog($msg);
		return array('success' => TRUE, 'msg' => $msg);
	}

	// Get the dispatches to notify
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_dispatch_status', 'value', 'requested');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  if (!$stmt['node']) {
		$msg = "There are no dispatches with status 'requested'. No notifications will be sent.";
    sirius_minilog($msg);
		return array('success' => TRUE, 'msg' => $msg);
  }

  $nids = array_keys($stmt['node']);
  $attempted = 0;
  $count = 0;
  $failed = 0;
  foreach ($nids as $nid) {
  	$dispatch_node = node_load($nid);
  
    if ($mode == 'live') {
      $result = sirius_dispatch_notify($dispatch_node);
    } else {
      $result = array('success' => FALSE, 'msg' => "(Skipping notification because mode = $mode)");
    }

    ++$attempted;
    if ($result['success']) {
      ++$count;
    } else {
      ++$failed;
      sirius_minilog('Notification failed: ' . $result['msg']);
    }
  }
	$msg = "Attempted $attempted notifications. Sent $count, with $failed failures.";
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_run_create_dispatches($job_node, $mode = 'test', $force = FALSE, $options = array()) {
  $policy = sirius_dispatch_job_policy($job_node);

  // Never create dispatches for CBNs.  Let's not even bother cluttering up the log.
  if ($policy['run']['fan'] != 'Yes') {
    $msg = 'New dispatches are not created for call-by-names. (They are generally created at the time that the job is created.)';
    sirius_minilog($msg);
    return array('success' => TRUE, 'msg' => $msg);
  }

	// How many do we need?
	$count_we_need = $job_node->field_sirius_count['und'][0]['value'];

  // Job settings
  $batch_max = $job_node->field_sirius_batch_max['und'][0]['value'];
  if (!$batch_max) { $batch_max = variable_get('sirius_dispatch_run_create_count', 25); }
  $batch_min = $job_node->field_sirius_batch_min['und'][0]['value'];
  if (!$batch_min) { $batch_min = 1; }
  $batch_ratio = $job_node->field_sirius_batch_ratio['und'][0]['value'];
  if (!$batch_ratio) { $batch_ratio = 1; }

	// How many are requested, notified, or accepted?
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('requested', 'notified', 'accepted', 'accepted_secondary'), 'in');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $count_we_have = 0;
  if ($stmt['node']) {
  	$count_we_have = count(array_keys($stmt['node']));
  }

	// We don't have enough
	$count_to_create = $count_we_need - $count_we_have;
  $count_to_create = $count_to_create * $batch_ratio;
  if ($count_to_create > $batch_max) { $count_to_create = $batch_max; }
  if ($count_to_create < $batch_min) { $count_to_create = $batch_min; }
  if ($count_to_create >= 1) {
  	$workers = sirius_dispatch_elig_fetch(
      $job_node, 
      'dispatch', 
      $count_to_create,
      array('cbn' => 'No', 'set_eligibility_cache' => TRUE)
    );

  	// Create all
  	$created = 0;
  	foreach ($workers as $worker) {
  		$worker_nid = $worker['nid'];
  		sirius_dispatch_actions_create($job_node, $worker_nid, 'No');
  		++$created;
  	}

    // There are not enough dispatchable workers -- error
    if (count($workers) < $count_to_create) {
      $msg = "There are not enough dispatchable workers.";
      sirius_minilog($msg);
      sirius_dispatch_run_error($job_node, $msg, $mode, $force, $options);
      return array('success' => FALSE, 'msg' => $msg);
    }

  	// All done
  	$msg = "Created $created dispatches.";
  }
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_run_expire($job_node, $mode = 'test', $force = FALSE, $options = array()) {
	$timelimit = $job_node->field_sirius_timelimit['und'][0]['value'];
	if ($timelimit < 1) {
		$msg = "Cannot expire dispatches: time limit is less than 1 minute.";
    sirius_minilog($msg);
    sirius_dispatch_run_error($job_node, $msg, $mode, $force, $options);
		return array('success' => FALSE, 'msg' => $msg);
	}

	$cutoff = time() - (60*$timelimit);
	$cutoff_str = date('Y-m-d H:i:s', $cutoff);

	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_dispatch_status', 'value', 'notified');
  $query->fieldCondition('field_sirius_datetime', 'value', $cutoff_str, '<');;
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $expired = 0;
  if ($stmt['node']) {
  	$dispatch_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($dispatch_nodes as $dispatch_node) {
  		sirius_dispatch_expire($dispatch_node);
  		++$expired;
  	}
  }

  $msg = "Expired $expired dispatches which were notified prior to $cutoff_str";
	return array('success' => TRUE, 'msg' => $msg, 'count' => $expired);
}

function sirius_dispatch_run_admin() {
  $form = array();

  $var = 'sirius_dispatch_run_poll_delay';
  $form[$var] = array(
    '#type' => 'textfield',
    '#title' => t('Poll Delay (in seconds)'),
    '#size' => 6,
    '#default_value' => variable_get($var, 300),
    '#description' => t("The frequency with which a running dispatch will be polled."),
  );

  $var = 'sirius_dispatch_run_create_count';
  $form[$var] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Dispatches Created Per Poll'),
    '#size' => 6,
    '#default_value' => variable_get($var, 25),
    '#description' => t("Maximum number of dispatches that will be created each time a running job is polled"),
  );

  $form["sirius_dispatch_log_level"] = array(
    '#type' => 'select',
    '#title' => 'Log Level',
    '#options' => array(
      'standard' => t('Standard - only log errors'),
      'debug' => t('Debug - log everything'),
    ),
    '#default_value' => variable_get('sirius_dispatch_log_level', 'standard'),
  );

  return system_settings_form($form);
}