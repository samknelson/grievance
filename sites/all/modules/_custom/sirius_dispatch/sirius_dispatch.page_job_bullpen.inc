<?php

function sirius_node_tab_dispatch_job_bullpen_access($job_node) {
	// Must have an event
	if (!$job_node->field_sirius_events['und'][0]['target_id']) { return FALSE; }

	// Managers can access
	if (sirius_node_tab_dispatch_job_manage_access($job_node)) { return TRUE; }

	// Forepeople can access
	$worker_node = sirius_worker_get();
	$fore = sirius_dispatch_fore_list($job_node);
	if ($worker_node && $fore && in_array($worker_node->nid, array_keys($fore))) {
		return TRUE; 
	}

	return FALSE;
}

function sirius_node_tab_dispatch_job_bullpen($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_bullpen_form', $job_node);
}

function sirius_node_tab_dispatch_job_bullpen_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;
	$policy = sirius_dispatch_job_policy($job_node);
	$event_node = node_load($job_node->field_sirius_events['und'][0]['target_id']);

  $ts = sirius_event_bullpen_ts($event_node);
  if ($ts['job_ts'] > time()) {
    //drupal_set_message("The event opens at " . date('g:i A, l, F j', $ts['job_ts']), 'warning');
    //return array();
  }

	// Get the list of dispatchable workers
	$workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name', 0);
	$tmp = array('ignore_redispatch' => TRUE);
	if (!$workers) {
		drupal_set_message(t('There are no workers currently available for dispatch.'), 'warning');
		return;
	}

	// We need to find out if each worker is currently employed by this employer, and also their registration date and time. We're trying to avoid doing a node_load_multiple(),
	// since that could be expensive, so we use SQL. 
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$event_nids = sirius_fieldvals($job_node, 'field_sirius_events', 'target_id');
	$chunks = array_chunk(array_keys($workers), 50);
	foreach ($chunks as $chunk) {
		$sql = "select entity_id, field_grievance_shop_target_id ";
		$sql .= "from field_data_field_grievance_shop ";
		$sql .= "where entity_type = 'node' ";
		$sql .= "and entity_id in (:chunk) ";
		$stmt = db_query($sql, array(':chunk' => $chunk));
		while ($hr = $stmt->fetchAssoc()) {
			if ($hr['field_grievance_shop_target_id'] == $employer_nid) {
				$workers[$hr['entity_id']]['employed'] = TRUE;
			}
		}

		if (!$event_nids) { continue; }
		$contact_nids = array();
		foreach ($chunk as $worker_nid) {
			if ($workers[$worker_nid]['contact_nid']) { $contact_nids[$workers[$worker_nid]['contact_nid']] = $worker_nid; }
		}
		if (!$contact_nids) { continue; }

		$sql = "select field_sirius_contact_target_id as contact_nid, ";
		$sql .= "min(field_sirius_datetime_value) as d from node ";
		$sql .= "join field_data_field_sirius_datetime on node.nid = field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
		$sql .= "join field_data_field_sirius_contact on node.nid = field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
		$sql .= "join field_data_field_sirius_event on node.nid = field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_event_participant' ";
		$sql .= "and field_sirius_event_target_id in (:event_nids) ";
		$sql .= "and field_sirius_contact_target_id in (:contact_nids) ";
		$sql .= "group by field_sirius_contact_target_id ";
		$stmt = db_query($sql, array(':contact_nids' => array_keys($contact_nids), ':event_nids' => $event_nids));
		while ($hr = $stmt->fetchAssoc()) {
			$contact_nid = $hr['contact_nid'];
			$worker_nid = $contact_nids[$contact_nid];
			$date = date('Y-m-d H:i:s', strtotime($hr['d'] . ' GMT'));
			$workers[$worker_nid]['register_date'] = $date;
		}
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

	// Sort by register_date
	foreach (array_keys($workers_by_ms) as $ms) {
		uksort($workers_by_ms[$ms], function($a, $b) use ($workers) {
			return strcmp($workers[$a]['register_date'], $workers[$b]['register_date']);
		});
	}

	if (!$open_ms) {
		drupal_set_message("No member status is currently available for dispatch", 'warning');
		return array();
	}

  $rows = array();
  foreach ($workers_by_ms[$open_ms] as $worker_nid => $hr) {
   	$row = array();
		$row[] = $hr['given'] . ' ' . $hr['family'];
		$row[] = $hr['register_date'];
		if ($hr['employed']) { $row[] = t('Yes'); } else { $row[] = t('No'); }
		$row[] = $hr['id'];
		$rows[$worker_nid] = $row;
  }
  $header = array(
  	t('Worker'),
  	t('Sign in timestamp'),
  	t('Standby?'),
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

	foreach (array_keys($workers_by_ms) as $ms) {
		if ($ms == $open_ms) { continue; }
		$html .= '<h2>' . t('Not yet dispatching: ') . '<strong>' . $ms . '</strong>' . '</h2>';
	  $rows = array();
	  foreach ($workers_by_ms[$ms] as $worker_nid => $hr) {
	   	$row = array();
			$row[] = $hr['given'] . ' ' . $hr['family'];
			$row[] = $hr['register_date'];
			$row[] = $hr['id'];
			$rows[] = $row;
	  }
	  $header = array(
	  	t('Worker'),
	  	t('Sign in timestamp'),
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
					drupal_set_message(t("Notification failed for @name: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
				}
			} else {
				drupal_set_message(t("Dispatch failed for @name: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
			}
		}
	}
}