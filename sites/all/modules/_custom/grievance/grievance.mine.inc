<?php

function grievance_views_post_execute_grievances_mine(&$view) {
	if (isset($view->result)) {
		foreach ($view->result as &$result) {
			$grievance_nid = $result->nid;

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
				$html .= t('First&nbps;Call:&nbsp;') . date('m/d/Y', $ts);
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

	    $step_1_status_tids = array(29,90,91,92);
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
}

