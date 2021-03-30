<?php

function sirius_event_term_tab_interestlist_worker_access($event_type_term) {
	if (!sirius_event_interestlist_enabled()) { return FALSE; }
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }
	if (!sirius_event_interestlist_enabled_byterm($event_type_term)) { return FALSE; }
	return FALSE;
}

function sirius_event_term_tab_interestlist_worker($event_type_term) {
  sirius_backlink_add('sirius/event/interestlists', array('name' => 'All Interest Lists'));
	return drupal_get_form('sirius_event_term_tab_interestlist_worker_form', $event_type_term);
}

function sirius_event_term_tab_interestlist_worker_form($form, &$form_state, $event_type_term) {
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.term_tab_interestlist_worker');
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.register_worker_byname');
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#after_build'][] = 'sirius_event_register_worker_byname_worker_remove_format';

  $workers = sirius_event_register_worker_byname_workers_from_query();
  $worker_nids = array_keys($workers);
  
  $form_state['event_type_tid'] = $event_type_term->tid;

  // $open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
  $form['#tree'] = TRUE; 

  $form['table_open'] = array('#markup' => '<table class="columns_table">');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 66%; vertical-align: top;">');

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
  $worker_count = $form_state['worker_count'];
  $worker_count += $ajax_params['qty'];
  if ($worker_count < 1) { $worker_count = 1; }
  if ($worker_count < count($workers)) { $worker_count = count($workers); }
  $form_state['worker_count'] = $worker_count;

  $form['workers']['prefix'] = array('#markup' => '<div id="workers">');
  for ($i=0; $i<$worker_count; ++$i) {
    $form['workers'][$i] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
      '#ajax' => array(
        'callback' => 'sirius_event_register_worker_byname_form_ajax_preview',
        'wrapper' => 'sirius_worker_preview',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#sirius_ajax' => array(
        'delta' => $i,
      ),
    );
    if ($i == 0) { 
      $form['workers'][$i]['#title'] = t('Register a Worker');
      $form['workers'][$i]['#description'] = t('Enter or scan the worker\'s dispatch ID, SSN, or name.');
    }
    $worker_nid = $worker_nids[$i];
    if ($worker_nid) {
      $form['workers'][$i]['#default_value'] = $workers[$worker_nid] . ' (' . $worker_nid . ')';
    }
  }
  $form['workers']['suffix'] = array('#markup' => '</div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
      'qty' => 1,
      'wrapper' => '#workers',
      'element' => 'workers',
    ),
  );

  $medium_options = array(
    NULL => t('No notification'),
    'email' => t('Notify by Email'),
    'sms' => t('Notify by SMS'),
    'both' => t('Notify by both Email and SMS'),
  );

  $keys = array('email_subject', 'email_body', 'sms');
  $tokens = array('term' => $event_type_term);
  $tokenized = array();
  foreach ($keys as $key) {
  	$val = sirius_domain_variable_get('sirius_event_interestlist_notification_' . $key);
  	if (is_array($val)) { $val = $val['value']; }
    $tokenized[$key] = token_replace($val, $tokens, array('clear' => TRUE));
  }

  $form['notification']['prefix'] = array('#markup' => '<div id="sirius_ajax_generic_replace_notification">');
  $form['notification']['medium'] = array(
    '#title' => t('Send a Notification?'),
    '#type' => 'select',
    '#options' => $medium_options,
    '#default_value' => sirius_domain_variable_get('sirius_event_interestlist_notification_medium'),
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

  $form['interestlist']['comments'] = array(
    '#title' => t('Comments'),
    '#type' => 'textarea',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Register',
  );
  $form['col1_close'] = array('#markup' => '</td>');

  $form['col2_open'] = array('#markup' => '<td style="width: 33%; vertical-align: top;">');
  $delta = $ajax_params['delta'];
  if (!$delta) { $delta = 0; }
  $worker_str = $form_state['input']['workers'][$delta];
  $form['worker_preview'] = array(
    '#type' => 'markup',
    '#markup' => sirius_event_register_worker_byname_render_preview($worker_str),
  );
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

  return $form;
}

function sirius_event_term_tab_interestlist_worker_form_submit($form, &$form_state) {
  $event_type_tid = $form_state['event_type_tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);

  $count = 0;
  $count_success = 0;
  $count_failure = 0;
  foreach ($form_state['values']['workers'] as $worker_str) { 
    if (!$worker_str) { continue; }
    ++$count;
    $result = sirius_event_register_worker_byname_worker($worker_str);
    if (!$result['success']) {
      ++$count_failure;
      drupal_set_message($result['msg'], 'warning');
    } else {
      $contact_nid = $result['contact_nid'];
    	$register_json = array();
      $register_json['interestlist']['signup'] = $form_state['values']['interestlist'];
      $register_json['notification'] = $form_state['values']['notification'];
      $register_json['notification']['email_body'] = sirius_richtext_handle_input($form_state['values']['notification']['email_body']);
			$result = sirius_event_interestlist_signup($event_type_term, $contact_nid, $register_json);
      if ($result['success']) {
        ++$count_success;
        drupal_set_message(t($result['msg']));
      } else {
        ++$count_failure;
         drupal_set_message($result['msg'], 'error');
      }
    }
  }
  
  drupal_set_message("Registered $count_success workers with $count_failure failures.");

  if ($_REQUEST['worker_nids']) {
    // If we were passwed worker NIDs on the query string, redirect to a page without it to reset the form.
    drupal_goto(current_path(), array('query' => array('delta' => $_REQUEST['delta'], 'widget' => $_REQUEST['widget'])));
  }
}

