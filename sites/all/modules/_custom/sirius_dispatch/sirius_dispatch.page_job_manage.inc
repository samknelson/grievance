<?php

function sirius_node_tab_dispatch_job_manage_access($job_node) {
	// Staff can view any job
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employer can view only their own shop's jobs.
	global $user;
	$user_full = user_load($user->uid);
	if ($job_node->field_grievance_shop['und'][0]['target_id'] && 
		($user_full->field_grievance_shops['und'][0]['target_id'] == $job_node->field_grievance_shop['und'][0]['target_id'])) {
		return TRUE;
	}

	// Oh well.
	return FALSE;
}

function sirius_node_tab_dispatch_job_manage($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_manage_form', $job_node);
}

function sirius_node_tab_dispatch_job_manage_form($form, &$form_state, $job_node) {
	$form_state['job_node'] = $job_node;

	// drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.employer_job_view.js', 'file');
	// drupal_add_css(drupal_get_path('module', 'sirius_dispatch') . '/css/sirius_dispatch.css');
  // drupal_add_library('system', 'ui.tabs');
  // drupal_add_js(array('sirius_dispatch' => array('job_nid' => $job_node->nid)), array('type' => 'setting'));

  $html .= '<div class="sirius_dispatch_job_intro">';
  $count = $job_node->field_sirius_count['und'][0]['value'];
  if (!$count) { $count = 0; }
  $count_yes = $job_node->field_sirius_count_yes['und'][0]['value'];
  if (!$count_yes) { $count_yes = 0; }
  if ($count) {
  	$percent = 100 * $count_yes / $count;
  	if ($percent > 100) { $percent = 100; }
  	$percent = round($percent, 0);
	  $html .= theme_progress_bar(array('percent' => $percent, 'message' => "$count_yes / $count workers have accepted."));
  }
  $html .= t('To contact the dispatch operator regarding this job request, reference job ID <strong>#@job_nid</strong>.', array('@job_nid' => $job_node->nid));
  $html .= '<br /><br />';
  $html .= '</div>';
	$renderable = node_view($job_node, 'full');
	$html .= drupal_render($renderable);
	$form['intro'] = array('#markup' => $html);

	$status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];

	if (user_access('sirius dispatch staff')) {
		// Staff form
		$options = array();
		$options[NULL] = t('-- Select --');
		$options['refresh'] = t('Refresh this page');

		if ($status == 'Pending') {
			$options['approve'] = t('Approve (move to "Unfilled")');
			$options['member_status'] = t('Change Member Statuses (which "lists" are available)');
			$options['archive'] = t('Archive (move to "Archived")');
		}

		if ($status == 'Unfilled') {
			$options['member_status'] = t('Change Member Statuses (which "lists" are available)');
			$options['run'] = t('Run (dispatches will be created automatically until the job is filled.)');
			$options['create_dispatches_auto'] = t('Create Dispatches - Automatic');
			$options['create_dispatches_manual'] = t('Create Dispatches - Manual');
			$options['notify'] = t('Send notifications');
			$options['close'] = t('Close (mark as "Filled")');
			$options['archive'] = t('Archive (move to "Archived")');
		}

		if ($status == 'Filled') {
			$options['reopen'] = t('Re-open (mark as "Unfilled")');
			$options['archive'] = t('Archive (move to "Archived")');
		}

		if ($status == 'Running') {
			$options['poll'] = t('Poll Manually (will run automatically)');
			$options['pause'] = t('Pause');
		}

		if ($status == 'Archived') {
			$options['reopen'] = t('Re-open (mark as "Unfilled")');
		}

		$options['explain'] = t('Determine whether a specific worker is dispatchable to this job.');

		$form['action'] = array(
			'#type' => 'select',
			'#title' => 'Action',
			'#required' => TRUE,
			'#options' => $options,
		);

		$form['wtf'] = array(
			'#title' => t('Dispatch ID'),
			'#type' => 'textfield',
			'#size' => 5,
			'#states' => array(
		  	'visible' => array(
		  		array(':input[name="action"]' => array('value' => 'explain')),
		  	)
			),
		);

		$form['run_settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Running Job Settings'),
			'#states' => array(
		  	'visible' => array(
		  		array(':input[name="action"]' => array('value' => 'run')),
		  	)
			),
		);

		$html = '<div style="width: 40%; float: right; padding-left: 10px;">';
		$html .= '<h2>Examples</h2>';
		$html .= '<table>';
		$html .= '<tr><th>Expire Time</th><th>Max</th><th>Min</th><th>Ratio</th><th>Open Slots</th><th></th></tr>';
		$html .= '<tr><td>5</td><td>50</td><td>10</td><td>2</td><td>10</td><td><strong>20 notifications every 5 minutes</strong></td></tr>';
		$html .= '<tr><td>5</td><td>50</td><td>10</td><td>2</td><td>100</td><td><strong>50 notifications every 5 minutes</strong></td></tr>';
		$html .= '<tr><td>5</td><td>50</td><td>10</td><td>2</td><td>1</td><td><strong>10 notifications every 5 minutes</strong></td></tr>';
		$html .= '<tr><td>30</td><td>50</td><td>10</td><td>1.5</td><td>10</td><td><strong>15 notifications every 30 minutes</strong></td></tr>';
		$html .= '<tr><td>30</td><td>50</td><td>1</td><td>1</td><td>5</td><td><strong>5 notifications every 30 minutes</strong></td></tr>';
		$html .= '</table>';
		$html .= '</div>';

		$form['run_settings']['description'] = array('#markup' => $html);


		$form['run_settings']['timelimit'] = array(
			'#title' => 'Expire After',
			'#type' => 'textfield',
			'#size' => 5,
			'#default_value' => $job_node->field_sirius_timelimit['und'][0]['value'],
			'#description' => t('Enter the number of minutes before a dispatch notification should expire.'),
		);

		$form['run_settings']['batch_max'] = array(
			'#title' => 'Maximum Batch Size',
			'#type' => 'textfield',
			'#size' => 5,
			'#default_value' => $job_node->field_sirius_batch_max['und'][0]['value'],
			'#description' => t('Enter the maxminum number ofnotifications that will be sent in any given batch'),
		);

		$form['run_settings']['batch_min'] = array(
			'#title' => 'Minimum Batch Size',
			'#type' => 'textfield',
			'#size' => 5,
			'#default_value' => $job_node->field_sirius_batch_min['und'][0]['value'],
			'#description' => t('Enter the minimum numer of notifications that will be sent in any given batch. If this is greater than 1, then it is possible that more notifications will be sent than the number of open slots, meaning that there is no guarantee that the job will be filled in seniority order.'),
		);

		$form['run_settings']['batch_ratio'] = array(
			'#title' => 'Ratio of Notifications to Slots',
			'#type' => 'textfield',
			'#size' => 5,
			'#default_value' => $job_node->field_sirius_batch_ratio['und'][0]['value'],
			'#description' => t('Enter the number of notifications to be sent for each open slot. If this is greater than 1, then there will be more than one notification sent for each slot, meaning that there is no guarantee that the job will be filled in seniority order.'),
		);

		$description = '';
		if (!$options) {
			$description = t('There are no dispatchable workers. You might want to open some additional members statuses for this job.');
		}

		$workers_visible = array(
	  	'visible' => array(
	  		array(':input[name="action"]' => array('value' => 'create_dispatches_manual')),
	  	)
		);

		$form['sirius_dispatch_cbn'] = array(
			'#title' => 'Call by Name?',
			'#type' => 'select',
			'#options' => array(
				'No' => 'No',
				'Yes' => 'Yes',
			),
			'#states' => $workers_visible,
		);

		$form += sirius_dispatch_job_get_dispatchable_workers_render_form($job_node, $workers_visible);

		$form['count'] = array(
			'#title' => t('Number of Workers'),
			'#type' => 'textfield',
			'#size' => 5,
			'#states' => array(
		  	'visible' => array(
		  		array(':input[name="action"]' => array('value' => 'create_dispatches_auto')),
		  	)
			),
		);

		$member_status_terms = sirius_worker_member_status_get_open_statuses($job_node->field_sirius_industry['und'][0]['tid']);
		$options = array();
		foreach ($member_status_terms as $member_status_term) {
			$options[$member_status_term->tid] = $member_status_term->name;
		}
		$defaults = array();
		if ($job_node->field_sirius_member_status['und'][0]['tid']) {
			foreach ($job_node->field_sirius_member_status['und'] as $tmp) {
				$defaults[$tmp['tid']] = $tmp['tid'];
			}
		}
		$form['member_status'] = array(
			'#title' => t('Member Statuses'),
			'#type' => 'checkboxes',
			'#options' => $options,
			'#default_value' => $defaults,
			'#description' => t('Specify which member statuses (or "lists") may be dispatched for this job.'),
			'#states' => array(
		  	'visible' => array(
		  		array(':input[name="action"]' => array('value' => 'member_status')),
		  	)
			),
		);

		$form['notes'] = array(
			'#type' => 'textarea',
			'#title' => t('Notes'),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
		);

		$form += sirius_log_render_form($job_node->nid);
	} else {
		// Employer form
		$options = array();
		$options[NULL] = t('-- Select --');
		$options['refresh'] = t('Refresh this page');
		$options['explain'] = t('Determine whether a specific worker is dispatchable to this job.');
		$form['action'] = array(
			'#type' => 'select',
			'#title' => 'Action',
			'#required' => TRUE,
			'#options' => $options,
		);

		$form['wtf'] = array(
			'#title' => t('Dispatch ID'),
			'#type' => 'textfield',
			'#size' => 5,
			'#states' => array(
		  	'visible' => array(
		  		array(':input[name="action"]' => array('value' => 'explain')),
		  	)
			),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
		);
	}

	$html = '';
	if (user_access('sirius dispatch staff')) {
		$html .= views_embed_view('sirius_dispatches', 'block', $job_node->nid);
	} else {
		$html .= views_embed_view('sirius_dispatches', 'block_1', $job_node->nid);
	}
	$form['conclusion'] = array('#markup' => $html);

	return $form;
}

function sirius_node_tab_dispatch_job_manage_form_submit($form, &$form_state) {
	$job_node = $form_state['job_node'];
	$action = $form_state['values']['action'];
	$notes = $form_state['values']['notes'];

	if ($action == 'refresh') { 
		drupal_set_message(t("Page refreshed."));
		return;
	}

	if ($action == 'explain') {
		$result = sirius_dispatch_job_get_dispatchable_workers_explain_by_worker_id($job_node, $form_state['values']['wtf']);
		if ($result['success']) {
			drupal_set_message($result['msg']);
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
		return;
	}

	if ($action == 'approve' || $action == 'reopen' || $action == 'pause') {
		$status = "Unfilled";
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = $status;
		node_save($job_node);
		drupal_set_message(t("Job status has been set to \"@status\".", array('@status' => $status)));
		sirius_log('job:status', $status, "Status set to \"$status\"", $notes, $notes, array($job_node->nid));
		return;
	}

	if ($action == 'close') {
		$status = "Filled";
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = $status;
		node_save($job_node);
		drupal_set_message(t("Job status has been set to \"@status\".", array('@status' => $status)));
		sirius_log('job:status', $status, "Status set to \"$status\"", $notes, $notes, array($job_node->nid));
		return;
	}

	if ($action == 'run') {
		$timelimit = $form_state['values']['timelimit'];
		if ($timelimit < 1) { drupal_set_message(t('Time limit must be at least one minute.'), 'error'); $is_error = TRUE; }

		$system_max = variable_get('sirius_dispatch_run_create_count', 25);
		$batch_min = $form_state['values']['batch_min'];
		if (!$batch_min) { $batch_min = 1; }
		if ($batch_min < 1) { drupal_set_message(t('Minimum batch size must be at least 1.'), 'error');  $is_error = TRUE; }
		if ($batch_min > $system_max) { drupal_set_message(t('Minimum batch size cannot be greater than @max.', array('@max' => $system_max)), 'error'); $is_error = TRUE; }

		$batch_max = $form_state['values']['batch_max'];
		if (!$batch_max) { $batch_max = $system_max; }
		if ($batch_max < 1) { drupal_set_message(t('Maximum batch size must be at least 1.'), 'error'); $is_error = TRUE; }
		if ($batch_max > $system_max) { drupal_set_message(t('Maxium batch size cannot be greater than @max.', array('@max' => $system_max)), 'error'); $is_error = TRUE; }

		$batch_ratio = $form_state['values']['batch_ratio'];
		if (!$batch_ratio) { $batch_ratio = 1; }
		if ($batch_ratio < 0.5) { drupal_set_message(t('Batch ratio must be at least 0.5'), 'error'); $is_error = TRUE; }
		if ($batch_ratio > 5) { drupal_set_message(t('Batch ratio cannot be larger than 5.'), 'error'); $is_error = TRUE; }

		if ($is_error) {
			$form_state['rebuild'] = TRUE;
			return;
		}

		$status = "Running";
		$job_node->field_sirius_timelimit['und'][0]['value'] = $timelimit;
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = $status;
		$job_node->field_sirius_summary['und'][0]['value'] = '';
		$job_node->field_sirius_batch_min['und'][0]['value'] = $batch_min;
		$job_node->field_sirius_batch_max['und'][0]['value'] = $batch_max;
		$job_node->field_sirius_batch_ratio['und'][0]['value'] = $batch_ratio;
		node_save($job_node);
		drupal_set_message(t("Job status has been set to \"@status\" with time limit @timelimit minutes", array('@status' => $status, '@timelimit' => $timelimit)));
		sirius_log('job:status', $status, "Status set to \"$status\" with time limit $timelimit minutes", $notes, $notes, array($job_node->nid));
		return;
	}

	if ($action == 'poll') {
		sirius_dispatch_run($job_node);
		drupal_set_message(t("Poll complete; see the log for details."));
		return;
	}

	if ($action == 'archive') {
		$status = "Archived";
		$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = $status;
		node_save($job_node);
		drupal_set_message(t("Job status has been set to \"@status\".", array('@status' => $status)));
		sirius_log('job:status', $status, "Status set to \"$status\"", $notes, $notes, array($job_node->nid));
		return;
	}

	if ($action == 'member_status') {
		unset($job_node->field_sirius_member_status);
		foreach ($form_state['values']['member_status'] as $ignore => $tid) {
			if ($tid) {
				$job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
			}
		}
		node_save($job_node);
		drupal_set_message(t("The dispatchable member statuses for this job have been updated."));
		sirius_log('job:lists', 'changed', "The available lists for this job have been changed", $notes, $notes, array($job_node->nid));
	}

	if ($action == 'create_dispatches_manual') {
		foreach ($form_state['values']['workers'] as $ignore => $worker_nid) {
			if ($worker_nid) {
				$worker_node = node_load($worker_nid);
				$result = sirius_dispatch_actions_create($job_node, $worker_node->nid, $form_state['values']['sirius_dispatch_cbn']);
				if ($result['success']) {
					drupal_set_message(t("Dispatched: @name", array('@name' => $worker_node->title)));
				} else {
					drupal_set_message(t("Dispatch failed for @name: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
				}
			}
		}
	}

	if ($action == 'create_dispatches_auto') {
		$count = $form_state['values']['count'];
		if ($count < 1) {
			drupal_set_message(t("Please enter a number greater than 0."), 'error');
			return;
		}

		if ($count > 100) {
			drupal_set_message(t("Please enter a number less than 100."), 'error');
			return;
		}

		if ($count > $job_node->field_sirius_count['und'][0]['value'] - $job_node->field_sirius_count_yes['und'][0]['value']) {
			drupal_set_message(t("Please enter a number less than the number of unfilled positions."), 'error');
			return;
		}
		$workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'dispatch', $count);
		$created = 0;
		foreach ($workers as $hr) {
			$worker_node = node_load($hr['nid']);
			sirius_dispatch_actions_create($job_node, $worker_node->nid, 'No');
			drupal_set_message(t("Dispatched: @name", array('@name' => $worker_node->title)));
			++$created;
		}

		$msg = '';
		if ($created < $count) {
			$msg = t("Only created @created out of @count dispatches; presumably there are no more available workers.", array('@created' => $created, '@count' => $count));
			drupal_set_message($msg, 'warning');
		} else {
			$msg = t("Created @created dispatch(es).", array('@created' => $created));
			drupal_set_message($msg);
		}
		sirius_log('job:create_dispatches', 'auto', $msg, $notes, $notes, array($job_node->nid));
	}

	if ($action == 'notify') {
		$query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_dispatch');
	  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
	  $query->fieldCondition('field_sirius_dispatch_status', 'value', 'requested');
	  $stmt = $query->execute();
	  $dispatch_nids = array();
	  if ($stmt['node']) {
	  	$dispatch_nids = array_keys($stmt['node']);
	  }

	 	$count = 0;
	  foreach ($dispatch_nids as $dispatch_nid) {
	  	++$count;
	  	$dispatch_node = node_load($dispatch_nid);
	  	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	  	$result = sirius_dispatch_notify($dispatch_node);
	  	if ($result['success']) {
				drupal_set_message(t("Notified: @name", array('@name' => $worker_node->title)));
			} else {
				drupal_set_message(t("Notification to @name failed: @msg", array('@name' => $worker_node->title, '@msg' => $result['msg'])), 'warning');
			}
	  }
		sirius_log('job:notify', 'notify', "Sent $count notifications", $notes, $notes, array($job_node->nid));
	}
}
