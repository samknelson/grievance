<?php

function sirius_event_register_bullpen_signin_form($form, &$form_state, $event_node, $rule) {
  $form_state['event_nid'] = $event_node->nid;
  $form_state['rule'] = $rule;
  $form_state['participant_role_tid'] = $participant_role_tid;

  $access = Sirius_Event_Bullpen_Access::getInstance();
  $context = array();
  $context['event_node'] = $event_node;
  if (!$access->access('register_open', $context)) { return sirius_form_error($access->error_msg()); }

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
  $event_node = node_load($form_state['event_nid']);
  $event_nid = $event_node->nid;
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
    drupal_set_message("Please make sure that you have selected a matching worker.", 'warning');
    return;
  }

  $worker_node = node_load($worker_nid);

  $access = Sirius_Event_Bullpen_Access::getInstance();
  $context = array();
  $context['event_node'] = $event_node;
  $context['worker_node'] = $worker_node;
  if (!$access->access('register_worker', $context)) {
    drupal_set_message($access->error_msg(), 'error');
    drupal_set_message(t('The worker is NOT signed in to the bullpen.'), 'warning');
    return;
  }

  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

  foreach ($rule['register_as'] as $key) {
    list ($prole_tid, $pstatus_tid) = explode('|', $key);
    break;
  }
  if (!$prole_tid || !$pstatus_tid) {
    drupal_set_messge(t('The event is misconfigured; no prole or pstatus set.'), 'warning');
  }

  $participant_json = array();
  $participant_json['bullpen']['self'] = TRUE;
  $participant_json['comments'] = 'Staff Sign-In';

  $result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid, $participant_json);
  if ($result['success']) { 
    drupal_set_message('The worker has been signed in to the event.');
  } else { 
    drupal_set_message($result['msg'], 'error');
  }
}


