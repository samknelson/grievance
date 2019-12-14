<?php

function sirius_event_register_bullpen_signin_form($form, &$form_state, $event_node, $rule) {

  $form_state['event_nid'] = $event_node->nid;
  $form_state['rule'] = $rule;
  $form_state['participant_role_tid'] = $participant_role_tid;

  $ts = sirius_event_bullpen_ts($event_node);
  /*
  if ($ts['signin_opens_others'] > time()) {
    drupal_set_message("Sign-in opens at " . date('g:i A, l, F j', $ts['signin_opens_others']), 'warning');
    return array();
  } 
  if ($ts['signin_closes'] < time()) {
    drupal_set_message("Sign-in closed at " . date('g:i A, l, F j', $ts['signin_closes']), 'warning');
    return array();
  }
  */

	// $open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
  $form['worker'] = array(
    '#title' => t('Sign-in a Worker'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
		'#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Sign In',
  );

	return $form;
}


function sirius_event_register_bullpen_signin_form_submit($form, &$form_state) {
  $event_nid = $form_state['event_nid'];
  $rule = $form_state['rule'];

  $worker_str = $form_state['values']['worker'];
  if (!$worker_str) { 
    drupal_set_message("Please select a worker.", 'warning');
    return;
  }
  
  $matches = array();
  if (preg_match('/\s+\((\d+)\)/', $worker_str, $matches) && $matches[1]) { 
    $worker_nid = $matches[1];
  }
  if (!$worker_nid) {
    drupal_set_message("Please make sure that you have selected a a matching worker.", 'warning');
    return;
  }

  $worker_node = node_load($worker_nid);
  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) {
    drupal_set_message(t('There is no contact associated with that worker.'), 'warning');
    return;
  }

  $job_node = sirius_event_bullpen_job($event_nid);
  $result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, array('ignore_skill' => TRUE, 'ignore_department' => TRUE, 'ignore_dispatch_status' => TRUE, 'ignore_event' => TRUE));
  if (!$result['success']) {
    drupal_set_message($result['msg'], 'warning');
    drupal_set_message(t('The worker is NOT signed in to the bullpen.'), 'warning');
    $form_state['rebuild'] = TRUE;
    return;
  }

  foreach ($rule['register_as'] as $key) {
    list ($prole_tid, $pstatus_tid) = explode('|', $key);
    break;
  }
  if (!$prole_tid || !$pstatus_tid) {
    drupal_set_messge(t('The event is misconfigured; no prole or pstatus set.'), 'warning');
  }

  $result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
  if ($result['success']) { drupal_set_message(t('The worker has been signed in to the event.')); } else { drupal_set_message($result['msg'], 'error'); }
}


function sirius_event_register_bullpen_ajax() {
  
}