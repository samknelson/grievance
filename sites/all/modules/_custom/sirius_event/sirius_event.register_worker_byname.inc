<?php

function sirius_event_register_worker_byname_form($form, &$form_state, $event_node, $rule) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#after_build'][] = 'sirius_event_register_worker_byname_worker_remove_format';

  $form_state['event_nid'] = $event_node->nid;
  $form_state['rule'] = $rule;

  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

  // $open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
  $form['#tree'] = TRUE; 

  $form['table_open'] = array('#markup' => '<table class="columns_table">');
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


  if (!$rule['register_as']) {
    drupal_set_message(t('The event is misconfigured; no prole or pstatus set.'), 'warning');
    return array();
  }
  $pstatus_name_lookup = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
  $prole_name_lookup = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');
  $paccess_options = array();
  foreach ($rule['register_as'] as $key) {
    list ($prole_tid, $pstatus_tid) = explode('|', $key);
    $paccess_options[$key] = $prole_name_lookup[$prole_tid] . ': ' . $pstatus_name_lookup[$pstatus_tid];
  }
  $form['paccess'] = array(
    '#title' => t('Participant Status'),
    '#type' => 'select',
    '#options' => $paccess_options,
    '#ajax' => array('callback' => 'sirius_ajax_generic_replace'),
    '#sirius_ajax' => array(
      'wrapper' => '#sirius_ajax_generic_replace_notification',
      'element' => 'notification',
    ),
  );

  $medium_options = array(
    NULL => t('No notification'),
    'email' => t('Notify by Email'),
    'sms' => t('Notify by SMS'),
    'both' => t('Notify by both Email and SMS'),
  );

  list($prole_tid, $pstatus_tid) = explode('|', $form_state['values']['paccess']);

  if ($pstatus_tid) {
    $pstatus_term = taxonomy_term_load($pstatus_tid);
    $pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);

    if ($form_state['triggering_element']['#name'] == 'paccess') {
      // If we changed the paccess, update the default notification text
      $keys = array('medium', 'email_subject', 'email_body', 'sms');
      $tokens = array('node' => $event_node);
      $tokenized = array();
      foreach ($keys as $key) {
        $tokenized[$key] = token_replace($pstatus_json['notification']['register'][$key], $tokens, array('clear' => TRUE));
      }

      foreach ($keys as $key) {
        if ($pstatus_json['notification']['register'][$key]) {
          $form_state['input']['notification'][$key] = $tokenized[$key];
        }
      }
    }
  }

  $form['notification']['prefix'] = array('#markup' => '<div id="sirius_ajax_generic_replace_notification">');
  $form['notification']['medium'] = array(
    '#title' => t('Send a Notification?'),
    '#type' => 'select',
    '#options' => $medium_options,
    '#default_value' => $tokenized['medium'],
  );

  $form['notification']['email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Subject'),
    '#default_value' => $tokenized['email_subject'],
    '#states' => array(
      'visible' => array(
        ":input[name=\"notification[medium]\"]" => array(
          array('value' => 'email'), 
          array('value' => 'both')
        )
      )
    ),
  );

  $form['notification']['email_body'] = array(
    '#type' => 'text_format',
    '#title' => 'Email Body',
    '#default_value' => $tokenized['email_body'],
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
    '#states' => array(
      'visible' => array(
        ":input[name=\"notification[medium]\"]" => array(
          array('value' => 'email'), 
          array('value' => 'both')
        )
      )
    ),
  );

  $form['notification']['sms'] = array(
    '#type' => 'textarea',
    '#title' => t('SMS'),
    '#default_value' => $tokenized['sms'],
    '#states' => array(
      'visible' => array(
        ":input[name=\"notification[medium]\"]" => array(
          array('value' => 'sms'), 
          array('value' => 'both')
        )
      )
    ),
  );
  $form['notification']['suffix'] = array('#markup' => '</div>');

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
  
  $paccess = $form_state['values']['paccess'];
  list ($prole_tid, $pstatus_tid) = explode('|', $paccess);
  $contact_nid = $result['contact_nid'];

  $register_json = $form_state['values']['participant'];
  $register_json['notification'] = $form_state['values']['notification'];
  $register_json['notification']['email_body'] = sirius_richtext_handle_input($form_state['values']['notification']['email_body']);
  $result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid, $register_json);
  if ($result['success']) { drupal_set_message(t($result['msg'])); } else { drupal_set_message($result['msg'], 'error'); }
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


function sirius_event_register_worker_byname_worker_remove_format(&$form) {
  $fields = array('xprefix', 'xsuffix', 'markup', 'description');
  if ($form['notification']['email_body']['format']) {
    $form['notification']['email_body']['format']['#access'] = FALSE;
  }
  return $form;
} 
