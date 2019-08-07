<?php

function grievance_home() {
	drupal_goto('grievances/mine/');
}

function grievance_all() {
  return views_embed_view('grievances', 'page');
}

function grievance_mine_assigned_today() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'assigned_today');
}

function grievance_mine() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'upcoming');
}

function grievance_mine_expanded() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
  return views_embed_view($view_name, 'page_3', 'upcoming');
}

function grievance_mine_all() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, NULL, 'show_warnings');
}

function grievance_mine_reminder_30() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'reminder_30');
}

function grievance_mine_reminder_60() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'reminder_60');
}

function grievance_mine_reminder_90() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'reminder_90');
}

function grievance_mine_reminder_no_first_call() {
	$view_name = variable_get('grievance_mine_view_name', 'grievances_mine');
	$display_name = variable_get('grievance_mine_view_display', 'page_1');
  return views_embed_view($view_name, $display_name, 'reminder_no_first_call');
}

function grievance_views_query_alter_grievances_mine(&$view, &$query) {
	if (isset($view->args) && ($view->args[0] == 'upcoming')) {
		$query->where[1]['conditions'][] = array(
			'field' => 'field_grievance_alert_date_value',
			'operator' => 'is not null',
		);
	}
}

function grievance_views_post_execute_grievances_mine(&$view) {
	if (!isset($view->result)) { return; }

	//
	// "Upcoming" doesn't compute reminders
	//

	$reminders_enabled = TRUE;
	if ($view->name != 'grievances_mine') { $reminders_enabled = FALSE; }
	if ($view->args[0] == 'upcoming') { $reminders_enabled = FALSE; }

	//
	// Array of reminders.  We're doing something a little weird here -- we're iterating through the whole list and
	// removing entries which are *not* reminders.  Yes that's cumbersome and @todo may need to be cached some day.
	// 

	$reminders = array();
	if ($reminders_enabled) {
		$reminders = array(
			'reminder_30' => array(
				'title' => t('no log entries for 30 days'),
				'no_match' => array(),
				'count' => 0,
				'path' => 'grievances/mine/reminder-30',
			),
			'reminder_60' => array(
				'title' => t('no log entries for 60 days'),
				'no_match' => array(),
				'count' => 0,
				'path' => 'grievances/mine/reminder-60',
			),
			'reminder_90' => array(
				'title' => t('no log entries for 90 days'),
				'no_match' => array(),
				'count' => 0,
				'path' => 'grievances/mine/reminder-90',
			),
			'reminder_no_first_call' => array(
				'title' => t('no first call'),
				'no_match' => array(),
				'count' => 0,
				'path' => 'grievances/mine/reminder-no-first-call',
			),
			'assigned_today' => array(
				'title' => t('assigned today'),
				'no_match' => array(),
				'count' => 0,
				'path' => 'grievances/mine/assigned-today',
			),
		);
	}
	$total = 0;

	// 
	// Look up various TID's
	//

  $first_call_tid = variable_get('grievance_log_first_call_type', '');
  $filed_status_tid = variable_get('grievance_log_filed_status', '');
  $filed_status_tid_2 = variable_get('grievance_log_filed_status_2', '');
  $log_assignment_type = variable_get('grievance_log_assignment_type', '');

	foreach ($view->result as $delta => &$result) {
		++$total;
		$grievance_nid = $result->nid;

		if ($view->name == 'grievances_mine') {
			if ($reminders_enabled) {
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
				// Assigned today
				//

				$cutoff = date('Y-m-d');
			  $query = new EntityFieldQuery();
			  $query->entityCondition('entity_type', 'node');
			  $query->entityCondition('bundle', 'grievance_log');
			  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
			  $query->fieldCondition('field_grievance_status_date', 'value', $cutoff, '>=');
			  $query->fieldCondition('field_grievance_log_type', 'tid', $log_assignment_type);
			  $stmt = $query->execute();
			  if (!$stmt['node']) {
			  	$reminders["assigned_today"]['no_match'][] = $delta;
			  }
			}
		}

		//
		// Replace "comments" with the "Processed" date for the node, and the first call date
		//

	  $html = '';
		$ts = grievance_log_status_date($grievance_nid, array($filed_status_tid, $filed_status_tid_2));
	  if ($ts) {
			$html .= variable_get('grievances_mine_processed_title', 'Processed') . ':&nbsp;' . date('Y-m-d', $ts);
	  }

		$ts = grievance_log_entry_date($grievance_nid, $first_call_tid);
	  if ($ts) {
	  	if ($html) { $html .= '<br />'; }
			$html .= variable_get('grievances_mine_first_call_title', 'First&nbsp;Call') . ':&nbsp;' . date('Y-m-d', $ts);
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

	  $grievance_log_step_1_status_display = variable_get('grievance_log_step_1_status_display', NULL);
	  if ($grievance_log_step_1_status_display != 'No') {
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
	}

	//
	// If we have any to remove, remove them.  Make sure to reverse the array first so indices are correct.
	//

	if ($reminders_enabled) {
		$msgs = array();
		foreach ($reminders as $type => &$entry) {
			$entry['count'] = $total - count($entry['no_match']);

			if ($entry['count']) {
				$msgs[] = t(
					'Warning: there are <strong>@count</strong> grievances with <a href="@url">@title</a>.', 
					array(
						'@count' => $entry['count'], 
						'@title' => $entry['title'], 
						'@url' => url($entry['path']),
					)
				);
			}
		}

		if ($view->args[0] != 'upcoming') {
			$entry = $reminders[$view->args[0]];
			if ($entry && $entry['no_match']) {
				$to_remove = array_reverse($entry['no_match']);
				foreach ($to_remove as $delta) {
					unset($view->result[$delta]);
				}
			}
		}

		if ($view->args[1] == 'show_warnings') {
			foreach ($msgs as $msg) { drupal_set_message($msg, 'warning'); }
		}
	}
}
