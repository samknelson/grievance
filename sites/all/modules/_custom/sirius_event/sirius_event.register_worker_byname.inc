<?php

function sirius_event_register_worker_byname_form($form, &$form_state, $event_node, $rule) {
  $form_state['event_nid'] = $event_node->nid;
  $form_state['rule'] = $rule;

  // $open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
  $form['#tree'] = TRUE; 

  $form['table_open'] = array('#markup' => '<table>');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 66%; vertical-align: top;">');
  $form['worker'] = array(
    '#title' => t('Register a Worker'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
    '#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
    '#ajax' => array(
      'callback' => 'sirius_event_register_worker_byname_form_ajax_preview',
      'wrapper' => 'sirius_worker_preview',
      'method' => 'replace',
      'effect' => 'fade',
    )
  );

  $form['participant']['comments'] = array(
    '#title' => t('Comments'),
    '#type' => 'textarea',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Register',
  );
  $form['col1_close'] = array('#markup' => '</td>');

  $form['col2_open'] = array('#markup' => '<td style="width: 33%; vertical-align: top;">');
  $worker_str = $form_state['input']['worker'];
  if ($worker_str) {
    $result = sirius_event_register_worker_byname_worker($worker_str);
    $preview = '<h2>' . t('Worker Selected') . '</h2>';
    if (!$result['success']) {
      $preview .= '<em>' . $result['msg'] . '</em>';
    } else {
      $preview .= sirius_dispatch_view_render_field($result['worker_node'], 'field_sirius_name', 'Name');
      $preview .= sirius_dispatch_view_render_field($result['worker_node'], 'field_sirius_id');
      $preview .= sirius_dispatch_view_render_field($result['worker_node'], 'field_sirius_ssn');
      $preview .= sirius_dispatch_view_render_field($result['worker_node'], 'field_sirius_phone');
      $preview .= sirius_dispatch_view_render_field($result['worker_node'], 'field_sirius_headshot');

      $path = 'node/' . $result['worker_nid'];
      if (drupal_valid_path($path)) {
        $preview .= l('Worker', $path, array('attributes' => array('class' => array('tool-button'))));
      }
    }
  }
  $form['worker_preview'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="sirius_worker_preview">' . $preview . '</pre>' . '</div>',
  );
 
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

  return $form;
}

function sirius_event_register_worker_byname_form_ajax_preview($form, $form_state) {
  return $form['worker_preview'];
}

function sirius_event_register_worker_byname_form_submit($form, &$form_state) {
  $event_nid = $form_state['event_nid'];
  $rule = $form_state['rule'];

  $result = sirius_event_register_worker_byname_worker($form_state['values']['worker']);
  if (!$result['success']) {
    drupal_set_message($result['msg'], 'warning');
    return;
  }
  
  $contact_nid = $result['contact_nid'];

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

function sirius_event_register_worker_byname_worker($worker_str) {
  if (!$worker_str) { return array('success' => FALSE, 'msg' => 'Please enter the name, ID, or SSN of a worker.'); }
  $matches = array();
  if (preg_match('/\s+\((\d+)\)/', $worker_str, $matches) && $matches[1]) { 
    $worker_nid = $matches[1];
  }
  if (!$worker_nid) { return array('success' => FALSE, 'msg' =>'Please make sure that you have selected a matching worker.'); }

  $worker_node = node_load($worker_nid);
  if ($worker_node->type != 'sirius_worker') { return array('success' => FALSE, 'msg' => 'That is not a worker.'); }

  $domain_nid = sirius_domain_nid();
  if ($domain_nid != $worker_node->field_sirius_domain['und'][0]['target_id']) { return array('success' => FALSE, 'msg' => 'That worker is not in the current domain.'); }

  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) { return array('success' => FALSE, 'msg' => 'There is no contact associated with that worker.'); }

  return array(
    'success' => TRUE,
    'worker_nid' => $worker_nid,
    'worker_node' => $worker_node,
    'contact_nid' => $contact_nid,
  );
}