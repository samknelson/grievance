<?php

function sirius_event_register_worker_byname_form($form, &$form_state, $event_node, $rule) {
  $form_state['event_nid'] = $event_node->nid;
  $form_state['rule'] = $rule;

  // $open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
  $form['#tree'] = TRUE; 
  $form['worker'] = array(
    '#title' => t('Register a Worker'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
    '#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
   );

  $form['participant']['comments'] = array(
    '#title' => t('Comments'),
    '#type' => 'textarea',
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Register',
  );

  return $form;
}


function sirius_event_register_worker_byname_form_submit($form, &$form_state) {
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
    drupal_set_message("Please make sure that you have selected a matching worker.", 'warning');
    return;
  }

  $worker_node = node_load($worker_nid);
  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) {
    drupal_set_message(t('There is no contact associated with that worker.'), 'warning');
    return;
  }

  foreach ($rule['register_as'] as $key) {
    list ($prole_tid, $pstatus_tid) = explode('|', $key);
    break;
  }
  if (!$prole_tid || !$pstatus_tid) {
    drupal_set_messge(t('The event is misconfigured; no prole or pstatus set.'), 'warning');
  }

  $result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid, $form_state['values']['participant']);
  if ($result['success']) { drupal_set_message(t('The worker has been registered.')); } else { drupal_set_message($result['msg'], 'error'); }
}


