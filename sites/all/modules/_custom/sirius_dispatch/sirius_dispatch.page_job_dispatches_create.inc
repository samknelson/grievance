<?php

function sirius_node_tab_dispatch_job_dispatches_create($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_dispatches_create_form', $job_node);
}

function sirius_node_tab_dispatch_job_dispatches_create_form($form, &$form_state, $job_node) {
	form_load_include($form_state, 'inc', 'sirius_dispatch', 'sirius_dispatch.page_job_dispatches_create');
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.register_worker_byname');
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#after_build'][] = 'sirius_event_register_worker_byname_worker_remove_format';

  $workers = sirius_event_register_worker_byname_workers_from_query();
  $worker_nids = array_keys($workers);
  
  $form_state['job_nid'] = $job_node->nid;

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
      $form['workers'][$i]['#title'] = t('Dispatch a Worker');
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

  $form['datetime_completed'] = array(
		'#type' => 'date_popup',
		'#title' => t('Start Date'),
		'#date_format' => 'Y-m-j',
		// '#default_value' => date('Y-m-d 00:00:00'),
  	'#attributes' => array(
			'autocomplete' => 'off',
		),
	);

	$status_field = field_info_field('field_sirius_dispatch_status');
	$status_options = list_allowed_values($status_field);

  $form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => $status_options,
		'#default_value' => 'requested',
		'#description' => t('Warning: selecting any value except "requested" may cause unexpected side effects. Use with caution.'),
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create Dispatch',
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

function sirius_node_tab_dispatch_job_dispatches_create_form_submit($form, &$form_state) {
  $job_node = node_load($form_state['job_nid']);

  $completed_ts = NULL;
  if ($form_state['values']['datetime_completed']) {
  	$completed_ts = strtotime($form_state['values']['datetime_completed']);
  }
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
    	$worker_nid = $result['worker_nid'];
			$result = sirius_dispatch_actions_create($job_node, $worker_nid, 'No', $form_state['values']['status'], $completed_ts);
      if ($result['success']) {
        ++$count_success;
        drupal_set_message(t($result['msg']));
      } else {
        ++$count_failure;
         drupal_set_message($result['msg'], 'error');
      }
    }
  }
  
  drupal_set_message("Created $count_success dispatches with $count_failure failures.");

  if ($_REQUEST['worker_nids']) {
    // If we were passwed worker NIDs on the query string, redirect to a page without it to reset the form.
    drupal_goto(current_path(), array('query' => array('delta' => $_REQUEST['delta'], 'widget' => $_REQUEST['widget'])));
  }
}