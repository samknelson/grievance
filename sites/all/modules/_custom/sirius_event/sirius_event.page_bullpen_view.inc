<?php

function sirius_node_tab_event_bullpen_view_access($event_node) {
  return sirius_event_bullpen_enabled($event_node);
}

function sirius_node_tab_event_bullpen_view($event_node) {
	$ts = sirius_event_bullpen_ts($event_node);
	$datestr = 'g:i A, l, F j';

  $host_job_node = sirius_event_bullpen_job($event_node->nid);
  $employer_node = node_load($host_job_node->field_grievance_shop['und'][0]['target_id']);
  $facility_node = node_load($host_job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
  $hall_node = node_load($host_job_node->field_sirius_dispatch_hall['und'][0]['target_id']);

  $html .= '<h2>' . t('Bullpen Information') . '</h2>';
  if ($employer_node) { $html .= sirius_fakerender_field('Host Employer:', $employer_node->title); }
  if ($facility_node) { $html .= sirius_fakerender_field('Facility:', $facility_node->title); }
  if ($hall_node) { $html .= sirius_fakerender_field('Hall:', $hall_node->title); }
  if ($host_job_node->field_sirius_address_notes['und'][0]['value']) { $html .= sirius_fakerender_field('Report Location:', $host_job_node->field_sirius_address_notes['und'][0]['value']); }
  $html .= sirius_fakerender_field('EBA Sign-in Opens', date($datestr, $ts['signin_opens_self']));
  $html .= sirius_fakerender_field('General Sign-in Opens:', date($datestr, $ts['signin_opens_others']));
  $html .= sirius_fakerender_field('Sign-in Closes:', date($datestr, $ts['signin_closes']));
  $html .= sirius_fakerender_field('Time Now:', date($datestr));
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
        'node/' . $job_node->nid . '/sirius_dispatch_job_dispatches' => 'Dispatches',
      );
      $found = FALSE;
      foreach ($paths as $path => $title) {
        if (drupal_valid_path($path)) {
          $found = TRUE;
          $html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
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
