<?php

function sirius_dispatch_run_all() {
	// Too soon?
	$last_run = variable_get('sirius_dispatch_run_last_ts', 0);
	$delay = variable_get('sirius_dispatch_run_poll_delay', 300);
	if (time() - $last_run < $delay) {
		return;
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
  	return;
  }

  // Run all jobs
  $job_nids = array_keys($stmt['node']);
  foreach ($job_nids as $job_nid) {
  	$job_node = node_load($job_nid);
  	sirius_dispatch_run($job_node);
  }
}

function sirius_dispatch_run($job_node) {
	// Expire dispatches
	$result = sirius_dispatch_run_expire($job_node);
	if (!$result['success']) { return $result; }

	// Create dispatches
	$result = sirius_dispatch_run_create_dispatches($job_node);
	if (!$result['success']) { return $result; }

	// Send notifications
	$result = sirius_dispatch_run_notify($job_node);
	if (!$result['success']) { return $result; }

	// Are we filled?
	sirius_dispatch_run_finalize($job_node);

	// Log the end
  if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
    sirius_log('job:run', 'poll', 'complete', '', '', array($job_node->nid));
  }

	return array('success' => TRUE);
}

function sirius_dispatch_run_error($job_node, $msg) {
	sirius_log('job:run', 'error', $msg, '', '', array($job_node->nid));
	$job_node->field_sirius_summary['und'][0]['value'] = $msg;
	$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Unfilled';
	node_save($job_node);
	return;
}

function sirius_dispatch_run_finalize($job_node) {
	// How many do we need?
	$count_we_need = $job_node->field_sirius_count['und'][0]['value'];

	// How many are requested, notified, or accepted?
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('accepted'), 'in');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $count_we_have = 0;
  if ($stmt['node']) {
  	$count_we_have = count(array_keys($stmt['node']));
  }

  if ($count_we_have < $count_we_need) {
  	// do nothing
  	return;
  }

  if ($count_we_have > $count_we_need) {
		$msg = "We need $count_we_need dispatches, but there are already $count_we_have dispatches with status accepted. This job is oversubscribed.";
		sirius_dispatch_run_error($job_node, $msg);
		return array('success' => FALSE, 'msg' => $msg);
	}

	if ($count_we_have == $count_we_need) {
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Filled';
		$job_node->field_sirius_summary['und'][0]['value'] = 'Filled on ' . date('c');
		node_save($job_node);
    if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
      sirius_log('job:run', 'complete', "There are $count_we_have dispatches with status accepted. This job is filled.", '', '', array($job_node->nid));
    }
  }
}

function sirius_dispatch_run_notify($job_node) {
	// Make sure we're ok to send automatic notifications
	$result = sirius_dispatch_notify_schedule_ok();
	if (!$result['success']) {
    if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
      sirius_log('job:run', 'notify', $result['msg'], '', '', array($job_node->nid));
    }
		return array('success' => TRUE, 'msg' => $result['msg']);
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
    if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
		  sirius_log('job:run', 'notify', $msg, '', '', array($job_node->nid));
    }
		return array('success' => TRUE, 'msg' => $msg);
  }

  $nids = array_keys($stmt['node']);
  $attempted = 0;
  $count = 0;
  $failed = 0;
  foreach ($nids as $nid) {
  	$dispatch_node = node_load($nid);
  	$result = sirius_dispatch_notify($dispatch_node);

    ++$attempted;
    if ($result['success']) {
      ++$count;
    } else {
      ++$failed;
      if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
        sirius_log('job:run', 'notify', 'Notification failed: ' . $result['msg'], '', '', array($job_node->nid));
      }
    }
  }
	$msg = "Attempted $attempted notifications. Sent $count, with $failed failures.";
  if ($failed) 
  if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
  	sirius_log('job:run', 'notify', $msg, '', '', array($job_node->nid));
  }
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_run_create_dispatches($job_node) {
  // Never create dispatches for CBNs.  Let's not even bother cluttering up the log.
  if ($job_node->field_sirius_dispatch_job_type['und'][0]['tid'] == sirius_get_special_tid('sirius_dispatch_job_type|CBN')) {
    return array('success' => TRUE, 'msg' => 'Dispatches are not created for call-by-names.');
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
  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('notified', 'requested', 'accepted'), 'in');
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $count_we_have = 0;
  if ($stmt['node']) {
  	$count_we_have = count(array_keys($stmt['node']));
  }

  // We have too many -- error
	if ($count_we_need < $count_we_have) {
		$msg = "We need $count_we_need dispatches, but there are already $count_we_have dispatches with status requested, notified, or pending. This job is oversubscribed.";
		sirius_dispatch_run_error($job_node, $msg);
		return array('success' => FALSE, 'msg' => $msg);
	}

	// We have just enough -- success
	if ($count_we_need == $count_we_have) {
		$msg = "There are $count_we_need dispatches already with status requested, notified, or pending. No new dispatches will be created.";
    if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
		  sirius_log('job:run', 'create_dispatches', $msg, '', '', array($job_node->nid));
    }
		return array('success' => TRUE, 'msg' => $msg);
	}

	// We don't have enough
	$count_to_create = $count_we_need - $count_we_have;
  $count_to_create = $count_to_create * $batch_ratio;
  if ($count_to_create > $batch_max) { $count_to_create = $batch_max; }
  if ($count_to_create < $batch_min) { $count_to_create = $batch_min; }
	$workers = sirius_dispatch_job_get_dispatchable_workers(
    $job_node, 
    'dispatch', 
    $count_to_create,
    array('cbn' => 'No')
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
    sirius_dispatch_run_error($job_node, $msg);
    return array('success' => FALSE, 'msg' => $msg);
  }

	// All done
	$msg = "Created $created dispatches.";
  if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
  	sirius_log('job:run', 'create_dispatches', $msg, '', '', array($job_node->nid));
  }
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_run_expire($job_node) {
	$timelimit = $job_node->field_sirius_timelimit['und'][0]['value'];
	if ($timelimit < 1) {
		$msg = "Cannot expire dispatches: time limit is less than 1 minute.";
		sirius_dispatch_run_error($job_node, $msg);
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
  if (variable_get('sirius_dispatch_log_level', '') == 'debug') {
  	sirius_log('job:run', 'expire', $msg, '', '', array($job_node->nid));
  }
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

  $days = array(
  	1 => 'Monday',
  	2 => 'Tuesday',
  	3 => 'Wednesday',
  	4 => 'Thursday',
  	5 => 'Friday',
  	6 => 'Saturday',
  	7 => 'Sunday',
  );

  $hours = array();
  for ($i=0; $i<=24; ++$i) {
  	$hours[$i] = $i;
  }

  $minutes = array();
  for ($i=0; $i<=59; ++$i) {
  	$minutes[$i] = str_pad($i, 2, '0', STR_PAD_LEFT);

  }

  foreach ($days as $day => $day_label) {
  	$form["sirius_dispatch_run_schedule_$day"] = array(
  		'#type' => 'fieldset',
  	);

  	$var = "sirius_dispatch_run_schedule_${day}_enable}";
  	$form["sirius_dispatch_run_schedule_$day"][$var] = array(
  		'#type' => 'select',
  		'#title' => 'Send on ' . $day_label . 's?',
  		'#options' => array('Yes' => 'Yes', 'No' => 'No'),
  		'#default_value' => variable_get($var, 'Yes'),
  		'#prefix' => '<div style="float: left; padding-right: 10px; width: 200px;">',
  		'#suffix' => '</div>',
  	);

  	$var = "sirius_dispatch_run_schedule_${day}_start_hour}";
  	$form["sirius_dispatch_run_schedule_$day"][$var] = array(
  		'#type' => 'select',
  		'#title' => 'Start hour',
  		'#options' => $hours,
  		'#default_value' => variable_get($var, 9),
  		'#prefix' => '<div style="float: left; padding-right: 10px;">',
  		'#suffix' => '</div>',
  	);

  	$var = "sirius_dispatch_run_schedule_${day}_start_minute";
  	$form["sirius_dispatch_run_schedule_$day"][$var] = array(
  		'#type' => 'select',
  		'#title' => 'Start minute',
  		'#options' => $minutes,
  		'#default_value' => variable_get($var, 0),
  		'#prefix' => '<div style="float: left; padding-right: 10px;">',
  		'#suffix' => '</div>',
  	);

  	$var = "sirius_dispatch_run_schedule_${day}_stop_hour}";
  	$form["sirius_dispatch_run_schedule_$day"][$var] = array(
  		'#type' => 'select',
  		'#title' => 'Stop hour',
  		'#options' => $hours,
  		'#default_value' => variable_get($var, 17),
  		'#prefix' => '<div style="float: left; padding-right: 10px;">',
  		'#suffix' => '</div>',
  	);

  	$var = "sirius_dispatch_run_schedule_${day}_stop_minute";
  	$form["sirius_dispatch_run_schedule_$day"][$var] = array(
  		'#type' => 'select',
  		'#title' => 'Stop minute',
  		'#options' => $minutes,
  		'#default_value' => variable_get($var, 0),
  		'#prefix' => '<div style="float: left; padding-right: 10px;">',
  		'#suffix' => '</div>',
  	);
  }

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