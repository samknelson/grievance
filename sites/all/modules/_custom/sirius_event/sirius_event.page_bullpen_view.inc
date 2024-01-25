<?php

function sirius_node_tab_event_bullpen_view_access($event_node) {
  return sirius_event_bullpen_enabled($event_node);
}

function sirius_node_tab_event_bullpen_date_format($ts) {
  $render = date('g:i A, l, F j Y T', $ts);
  $interval = time() - $ts;
  if ($interval > 0) {
    $render .= ' (' . format_interval($interval, 2) . ' ago)';
  } else {
    $render .= ' (in ' . format_interval(-1 * $interval, 2) . ')';
  }

  return $render;
}

function sirius_node_tab_event_bullpen_view($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	$ts = sirius_event_bullpen_ts($event_node);

  global $user;
  if ($user->uid == 1) { 
    drupal_set_message("<pre>" . print_r($event_node->field_sirius_daterepeat, 1) . '</pre>');

    $tmp = strtotime($event_node->field_sirius_daterepeat['und'][0]['value'] . ' ' . $event_node->field_sirius_daterepeat['und'][0]['timezone_db']);
    drupal_set_message("ts $tmp");
    drupal_set_message('<pre>' . print_r($user, 1) . '</pre>');
  }

  $host_job_node = sirius_event_bullpen_job($event_node->nid);
  $employer_node = node_load($host_job_node->field_grievance_shop['und'][0]['target_id']);
  $facility_node = node_load($host_job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
  $hall_node = node_load($host_job_node->field_sirius_dispatch_hall['und'][0]['target_id']);

  $html .= '<h2>' . t('Bullpen Information') . '</h2>';
  if ($employer_node) { $html .= sirius_fakerender_field('Host Employer:', $employer_node->title); }
  if ($facility_node) { $html .= sirius_fakerender_field('Facility:', $facility_node->title); }
  if ($hall_node) { $html .= sirius_fakerender_field('Hall:', $hall_node->title); }
  if ($host_job_node->field_sirius_address_notes['und'][0]['value']) { $html .= sirius_fakerender_field('Report Location:', $host_job_node->field_sirius_address_notes['und'][0]['value']); }

  $html .= '<br />';
  $html .= sirius_fakerender_field('Bullpen Starts:', '<strong>' . sirius_node_tab_event_bullpen_date_format($ts['signin_opens_self']) . '</strong>');
  $html .= sirius_fakerender_field('Self Sign-in Opens:', sirius_node_tab_event_bullpen_date_format($ts['signin_opens_self']));
  $html .= sirius_fakerender_field('General Sign-in Opens:', sirius_node_tab_event_bullpen_date_format($ts['signin_opens_others']));
  $html .= sirius_fakerender_field('Sign-in Closes:', sirius_node_tab_event_bullpen_date_format($ts['signin_closes']));
  $html .= '<br />';

  // $html .= sirius_fakerender_field('Time Now:', date($datestr, time()));

  $html .= sirius_dispatch_view_render_field($host_job_node, 'field_sirius_member_status', t('Member Statuses'));
  $html .= render(field_view_field('node', $event_node, 'body', 'full'));

  $job_nodes = sirius_event_bullpen_job($event_node->nid, TRUE);
  if ($job_nodes) {
    $is_first = TRUE;
    foreach ($job_nodes as $job_nid => $job_node) {
      $html .= '<br />';
      if ($is_first) { $title = 'Host Job'; } else { $title = 'Additional Job'; }
      $is_first = FALSE;

      $employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
      $employer_node = node_load($employer_nid);
      $title .= ': ' . $employer_node->title; 

      $html .= '<strong>' . $title . '</strong><br />';

      $html .= render(field_view_field('node', $job_node, 'field_sirius_skill', 'full'));
      $html .= render(field_view_field('node', $job_node, 'field_grievance_departments', 'full'));

      $paths = array(
        'node/' . $job_node->nid . '/sirius_dispatch_job_manage' => 'Manage',
        'node/' . $job_node->nid . '/sirius_dispatch_job_bullpen' => 'Create Dispatches',
        'node/' . $job_node->nid . '/sirius_dispatch_job_dispatches' => 'Accept or Decline Dispatches',
      );
      $found = FALSE;
      foreach ($paths as $path => $title) {
        if (drupal_valid_path($path)) {
          $found = TRUE;
          $html .= l($title, $path, array('attributes' => array('class' => array('tool-button')))) . ' ';
        }
      }
      if ($found) {
        $count_yes = $job_node->field_sirius_count_yes['und'][0]['value'];
        if (!$count_yes) { $count_yes = 0; }
        $count_no = $job_node->field_sirius_count_no['und'][0]['value'];
        if (!$count_no) { $count_no = 0; }
        $count_notified = $job_node->field_sirius_count_notified['und'][0]['value'];
        if (!$count_notified) { $count_notified = 0; }

        $html .= ' ' . "Notified: $count_notified / Accepted: $count_yes / Declined: $count_no<br />";
      }
    }
  }


	return $html;
}
