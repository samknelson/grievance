<?php

function grievance_home() {
	drupal_goto('grievances/mine');
}

function grievance_mine() {
  return views_embed_view('grievances_mine', 'page_1');
}

function grievance_all() {
  return views_embed_view('grievances', 'page');
}

function grievance_mine_reminder_30() {
  return views_embed_view('grievances_mine', 'page_1', 'reminder_30');
}

function grievance_mine_reminder_60() {
  return views_embed_view('grievances_mine', 'page_1', 'reminder_60');
}

function grievance_mine_reminder_90() {
  return views_embed_view('grievances_mine', 'page_1', 'reminder_90');
}

function grievance_mine_reminder_no_first_call() {
  return views_embed_view('grievances_mine', 'page_1', 'reminder_no_first_call');
}

function grievance_views_post_execute_grievances_mine(&$view) {
	if (!isset($view->result)) { return; }

	//
	// Array of reminders.  We're doing something a little weird here -- we're iterating through the whole list and
	// removing entries which are *not* reminders.  Yes that's cumbersome and @todo may need to be cached some day.
	// 

	$reminders = array(
		'reminder_30' => array(
			'title' => t('no log entries for 30 days'),
			'no_match' => array(),
			'count' => 0,
			'path' => 'grievances/reminder-30',
		),
		'reminder_60' => array(
			'title' => t('no log entries for 60 days'),
			'no_match' => array(),
			'count' => 0,
			'path' => 'grievances/reminder-60',
		),
		'reminder_90' => array(
			'title' => t('no log entries for 90 days'),
			'no_match' => array(),
			'count' => 0,
			'path' => 'grievances/reminder-90',
		),
		'reminder_no_first_call' => array(
			'title' => t('no first call'),
			'no_match' => array(),
			'count' => 0,
			'path' => 'grievances/reminder-no-first-call',
		),		
	);
	$total = 0;

	foreach ($view->result as $delta => &$result) {
		++$total;
		$grievance_nid = $result->nid;

		// 
		// n-day reminders.  Remove anything if it has a log entry less than n days ago
		//

		foreach (array(30, 60, 90) as $days) {
			$cutoff = date('Y-m-d', strtotime("-$days days"));

		  $query = new EntityFieldQuery();
		  $query->entityCondition('entity_type', 'node');
		  $query->entityCondition('bundle', 'grievance_log');
		  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
		  $query->fieldCondition('field_grievance_status_date', 'value', $cutoff, '>=');
		  $stmt = $query->execute();
		  if ($stmt['node']) {
		  	$reminders["reminder_$days"]['no_match'][] = $delta;
		  }
		}

		//
		// Replace "comments" with the "Processed" date for the node, and the first call date
		//

	  $html = '';
	  $filed_status_tid = variable_get('grievance_log_filed_status', '');
		$ts = grievance_log_status_date($grievance_nid, $filed_status_tid);
	  if ($ts) {
			$html .= t('Processed:&nbsp;') . date('m/d/Y', $ts);
	  }

	  $first_call_tid = variable_get('grievance_log_first_call_type', '');
		$ts = grievance_log_entry_date($grievance_nid, $first_call_tid);
	  if ($ts) {
	  	if ($html) { $html .= '<br />'; }
			$html .= t('First&nbsp;Call:&nbsp;') . date('m/d/Y', $ts);
			$reminders['reminder_no_first_call']['no_match'][] = $delta;
	  }

	  $result->field_field_grievance_comments[0]['rendered']['#markup'] = $html;

	  //
	  // Add a class to the "due date"
	  //

	  if ($result->field_field_grievance_alert_date[0]['raw']['value']) {
	  	$alert_ts = strtotime($result->field_field_grievance_alert_date[0]['raw']['value']);

	  	$class = '';
	    if (time() >= $alert_ts) { $class .= "alert-box-overdue "; }
	    for ($i=1; $i<=5; ++$i) {
	      $days = variable_get('grievance_alert_days_warning' . $i, 0);
	      if ($days) {
	        if (time() >= $alert_ts - $days*24*60*60) {
	          $class .= "alert-box-warning-$i "; 
	        }
	      }
	    }

	    $result->field_field_grievance_alert_date[0]['rendered']['#markup'] = "<span class=\"$class\">" . $result->field_field_grievance_alert_date[0]['rendered']['#markup'] . '</span>';
	  }

    //
    // We also want to show whichever was the most recent from a previous set of statuses
    //

	  $step_1_status_tids = array();
	  for ($i=0; $i<10; ++$i) {
	  	$step_1_status_tids[] = variable_get('grievance_log_step_1_status_' . $i, '');
	  }
    $current_status_tid = $result->field_field_grievance_status[0]['raw']['tid'];
    if (!in_array($current_status_tid, $step_1_status_tids)) {
	    $step_1_entry= grievance_log_status_entry($grievance_nid, $step_1_status_tids, 'desc');
	    if ($step_1_entry) {
	    	$step_1_tid = $step_1_entry->field_grievance_status['und'][0]['tid'];
	    	$step_1_term = taxonomy_term_load($step_1_tid);
	    	$result->field_field_grievance_status[0]['rendered']['#markup'] .= '<br />' . '(' . $step_1_term->name . ')';
	    }
	  }
	}

	//
	// If we have any to remove, remove them.  Make sure to reverse the array first so indices are correct.
	//

	$msgs = array();
	foreach ($reminders as $type => &$entry) {
		$entry['count'] = $total - count($entry['no_match']);

		if ($entry['count']) {
			$msgs[] = t(
				'Warning: there are <strong>@count</strong> messages with <a href="@url">@title</a>.', 
				array(
					'@count' => $entry['count'], 
					'@title' => $entry['title'], 
					'@url' => url($entry['path']),
				)
			);
		}
	}

	if ($view->args[0]) {
		$entry = $reminders[$view->args[0]];
		$to_remove = array_reverse($entry['no_match']);
		foreach ($to_remove as $delta) {
			unset($view->result[$delta]);
		}
	} else {
		foreach ($msgs as $msg) { drupal_set_message($msg, 'warning'); }
	}

	//
	// Just messing around...
	//

	/*
	dpm($view);
	$row = new stdClass();
	$row->nothing_3 = array(array('rendered' => array('#markup' => 'Hello, world.')));
	$view->result[] = $row;
	// unset($view->result[2]->field_field_grievance_status);
	*/
}
