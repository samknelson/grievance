<?php

function sirius_node_tab_dispatch_job_bullpen_access($job_node) {
	if (!sirius_node_tab_dispatch_job_manage_access($job_node)) { return FALSE; }

	$event_node = node_load($job_node->field_sirius_event['und'][0]['target_id']);
	if (!$event_node) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_dispatch_job_bullpen($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_bullpen_form', $job_node);
}

function sirius_node_tab_dispatch_job_bullpen_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;
	$policy = sirius_dispatch_job_policy($job_node);
	$event_node = node_load($job_node->field_sirius_event['und'][0]['target_id']);

  $ts = sirius_event_bullpen_ts($event_node);
  /*
  if ($ts['job_ts'] > time()) {
    drupal_set_message("The event opens at " . date('g:i A, l, F j', $ts['job_ts']), 'warning');
    return array();
  }
  */

	// Get the list of dispatchable workers
	$workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name', 0);
	$tmp = array('ignore_redispatch' => TRUE);
	if (!$workers) {
		drupal_set_message(t('There are no workers currently available for dispatch.'), 'warning');
		return;
	}

	// Break up by member status. Only the highest-ranked member status is dispatchable.
	$workers_by_ms = array();
	$weights_by_ms = array();
	$open_weight = 999999;
	$open_ms = '';
	foreach ($workers as $worker) {
		if ($worker['member_status_weight'] < $open_weight) {
			$open_weight = $worker['member_status_weight'];
			$open_ms = $worker['member_status_name'];
		}
		$workers_by_ms[$worker['member_status_name']][$worker['nid']] = $worker;
	}

	if (!$open_ms) {
		drupal_set_message("No member status is currently available for dispatch", 'warning');
		return array();
	}

  $rows = array();
  foreach ($workers_by_ms[$open_ms] as $worker_nid => $hr) {
   	$row = array();
		$row[] = $hr['given'] . ' ' . $hr['family'];
		$row[] = $hr['id'];
		$rows[$worker_nid] = $row;
  }
  $header = array(
  	t('Worker'),
  	t('ID'),
  );

  $form['worker_nid'] = array(
  	'#type' => 'tableselect',
  	'#header' => $header,
  	'#options' => $rows,
  	'#prefix' => '<h2>' . t('Now dispatching: ') . '<strong>' . $open_ms . '</strong>' . '</h2>',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Dispatch These Workers'),
	);

	foreach ($workers_by_ms as $ms => $workers) {
		if ($ms == $open_ms) { continue; }
		$html .= '<h2>' . t('Not yet dispatching: ') . '<strong>' . $ms . '</strong>' . '</h2>';
	  $rows = array();
	  foreach ($workers as $worker_nid => $hr) {
	   	$row = array();
			$row[] = $hr['given'] . ' ' . $hr['family'];
			$row[] = $hr['id'];
			$rows[] = $row;
	  }
	  $header = array(
	  	t('Worker'),
	  	t('ID'),
	  );
	  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	}

	if ($html) {
		$form['not_yet_dispatching'] = array('#markup' => '<hr>' . $html);
	}

	return $form;
}

function sirius_node_tab_dispatch_job_bullpen_form_submit($form, &$form_state) {
	$job_node = node_load($form_state['job_nid']);

	foreach ($form_state['values']['worker_nid'] as $ignore => $worker_nid) {
		if ($worker_nid) {
			$worker_node = node_load($worker_nid);
			$result = sirius_dispatch_actions_create($job_node, $worker_node->nid);
			if ($result['success']) {
				$dispatch_node = $result['dispatch_node'];
				$result = sirius_dispatch_notify($dispatch_node);
				if ($result['success']) {
					drupal_set_message(t("Dispatched: @name", array('@name' => $worker_node->title)));
				} else { 
					drupal_set_message(t("Dispatch failed for @name: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
				}
			} else {
				drupal_set_message(t("Dispatch failed for @name: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
			}
		}
	}
}