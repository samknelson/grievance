<?php

function sirius_edls_node_tab_sheet_sts_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_view', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_sts($sheet_node) {
	sirius_require('sirius_edls', 'sirius_edls.report_sts.inc');
	$instance = Sirius_Edls_Report_Sts::getInstance();

	$employer_nid = $sheet_node->field_grievance_shop['und'][0]['target_id'];
	$date = date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
	$ts_now = time();

	$crews = sirius_edls_crews($sheet_node, 'uuid');
	$assignments = sirius_edls_assignment_fetch_sheet($sheet_node);

	$header = array();
	$header[] = t('Worker');
	$header[] = t('Next Shift Start');
	$header[] = t('Time From Shift Start');
	$header[] = t('Time From Now');

	$html = '';
	foreach ($crews as $crew_uuid => $crew) {
		$html .= '<h2>' . sirius_edls_crew_title($crew) . '</h2>';
		$rows = array();
		foreach ($assignments[$crew_uuid] as $assignment) {
      $worker_full = sirius_edls_worker_fetch($employer_nid, $assignment['worker_id'], $date);
      $curr_ts = $instance->start_ts($worker_full['assignments']['curr']);
      $next_ts = $instance->start_ts($worker_full['assignments']['next']);

      if ($curr_ts && $next_ts) {
      	$date_render = date('Y-m-d H:i', $next_ts);

      	$interval1 = $next_ts - $curr_ts;
      	$interval2 = $next_ts - $ts_now;
      	if ($interval2 < 0) { $interval2 = 0; }

      	$interval1_render = format_interval($interval1, 2);
      	$interval2_render = format_interval($interval2, 2);

      	if ($interval1 < 60*60*12) {
      		$color = 'red';
      	} else if ($interval1 < 60*60*24) {
      		$color = 'yellow';
      	} else {
      		$color = 'green';
      	}
      } else {
      	$date_render = '[no next shift]';
      	$interval_render = '';
      	$color = 'grey';
      }

      $row = array();
      $row[] = $assignment['worker_name'];
      $row[] = $date_render;
      $row[] = array(
      	'data' => $interval1_render, 
      	'class' => array($color),
      );
      $row[] = $interval2_render;
      $rows[] = $row;
		}

		// $html .= sirius_table_render($header, $rows, array('attributes' => array('class' => array('sirius_table_align', 'sirius_contact_relationships'))));
	}

	return $html;
}