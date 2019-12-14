<?php

function sirius_node_tab_event_bullpen_view_access($event_node) {
	return sirius_node_tab_event_bullpen_access($event_node);
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

  global $user;
  $user_full = user_load($user->uid);
  $contact_node = node_load($user_full->field_sirius_contact['und'][0]['target_id']);
  if ($contact_node) {
    $possible = sirius_event_bullpen_selfsignin_possible($event_node);
    if ($possible['success'] && $_REQUEST['selfsignin']) {
      drupal_set_message("Self sign-in.");
    }


    $html .= '<h2>' . t('Sign-in Status') . '</h2>';
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'sirius_event_participant');
    $query->fieldCondition('field_sirius_contact', 'target_id', $contact_node->nid);
    $query->fieldCondition('field_sirius_event', 'target_id', $event_node->nid);
    $stmt = $query->execute();
    if ($stmt['node']) {
      $participant_node = node_load(array_keys($stmt['node']));
      $pstatus_term = taxonomy_term_load($participant_node->field_sirius_event_pstatus['und'][0]['tid']);
      $prole_term = taxonomy_term_load($participant_node->field_sirius_event_prole['und'][0]['tid']);
      $html .= sirius_fakerender_field('Participant Role:', $prole_term->name);
      $html .= sirius_fakerender_field('Participant Status:', $prole_term->name);
    } else {
      if (!$possible['success']) {
        $html .= sirius_fakerender_field('Self sign-in:', "Not available - " . $possible['msg']);
      } else {
        $html .= sirius_fakerender_field('Self sign-in:', l(t('Sign-in Now'), current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('selfsignin' => TRUE))));
      }

    }
    // Self sign-in possible?
  }

  $job_nodes = sirius_event_bullpen_job($event_node->nid, TRUE);
  if ($job_nodes) {
    $html .= '<h2>' . t('Employers') . '</h2>';
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


function sirius_event_bullpen_selfsignin_possible($event_node) {
  // Self sign-in must be open
  $ts = sirius_event_bullpen_ts($event_node);
  $datestr = 'g:i A, l, F j';
  if ($ts['signin_opens_self'] > time()) {
    // return array('success' => FALSE, 'msg' => 'Self sign-in opens on ' . date($datestr, $ts['signin_opens_self']));
  }

  // Must have a worker
  $worker_node = sirius_worker_get();
  if (!$worker_node) {
    return array('success' => FALSE, 'msg' => 'There is no worker associated with your user.');
  }

  // Must have a contact
  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
  if (!$contact_node) {
    return array('success' => FALSE, 'msg' => 'There is no contact associated with your user.');
  }

  // Must be eligible for the host job
  $job_node = sirius_event_bullpen_job($event_node->nid);
  $result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_node->nid, array('ignore_skill' => TRUE, 'ignore_department' => TRUE, 'ignore_dispatch_status' => TRUE, 'ignore_event' => TRUE));
  if (!$result['success']) {
    return array('success' => FALSE, 'msg' => $result['msg']);
  }

  // Must be employed by the current employer
  $current_employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
  if ($current_employer_nid != $job_node->field_grievance_shop['und'][0]['target_id']) {
    return array('success' => FALSE, 'msg' => "You can only sign yourself into bullpens offered by your current employer.");
  }

  // Must be EBA on the correct date
  /*
  if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes') {
    return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
  }
  $found = FALSE;
  $eba_dates = sirius_fieldvals($worker_node, 'field_sirius_dispatch_eba_dates');
  $job_date = date('Y-m-d', strtotime(sirius_fieldval($job_node, 'field_sirius_datetime')));
  foreach ($eba_dates as $eba_date) {
    $eba_date = date('Y-m-d', strtotime($eba_date));
    if ($eba_date == $job_date) {
      $found = TRUE;
      break;
    }
  }
  if (!$found) {
    // return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
  }
  */

  return array('success' => TRUE, 'msg' => 'Self sign-in here.');

}