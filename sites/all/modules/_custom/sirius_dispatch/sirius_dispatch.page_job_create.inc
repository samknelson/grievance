<?php

function sirius_dispatch_page_job_create_access($employer_node, $job_type_tid, $industry_tid = NULL) {
	// Staff can create jobs.
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Non-employers cannot create jobs.
	if (!user_access('sirius dispatch employer')) { return FALSE; }

	// Must be the correct employer
	$check_employer_node = sirius_employer_get();
	if (!$check_employer_node->nid) { return FALSE; }
	if ($check_employer_node->nid != $employer_node->nid) { return FALSE;}

	// If there's an industry, must be one of the employer's industries.
	if ($industry_tid) {
		if (!in_array($industry_tid, sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid'))) { return FALSE; }
	}

	// Employer must have access to this job type
	foreach ($employer_node->field_sirius_dispatch_job_types['und'] as $tmp) {
		if ($tmp['tid'] == $job_type_tid) {
			return TRUE;
		}
	}
	return FALSE;
}

function sirius_dispatch_page_job_create($employer_node, $job_type_tid, $industry_tid = NULL) {
	// Check for pending VLRIs
	if (!user_access('sirius dispatch staff')) {
		// Must not have any pending VLRIs
		$vlri_check = sirius_dispatch_vlri_block_check($employer_node->nid);
		if (!$vlri_check['success']) {
			$html = $vlri_check['msg'];
			$html .= " ";
			$html .= "No new jobs may be created until these layoff requests are resolved. ";
			$html .= '<br /><br />';
			$html .= l('Resolve layoff requests.', 'sirius/employer/dispatch/vlri');
			return $html;
		}
	}

	return drupal_get_form('sirius_dispatch_page_job_create_form', $employer_node, $job_type_tid, $industry_tid);
}


function sirius_dispatch_page_job_create_form($form, &$form_state, $employer_node, $job_type_tid, $industry_tid = NULL) {
	if (!$industry_tid) { $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid']; }
	$industry_term = taxonomy_term_load($industry_tid);
	$form_state['industry_tid'] = $industry_tid;

	$form['#tree'] = TRUE;

	global $user;
	$job_policy = sirius_dispatch_job_policy_from_tid($job_type_tid);


	// Do we need a "workers" region?
	$workers_visible = ($job_policy['workers']['cbn'] == 'Yes');

	// Some fields change the worker list.
	if ($workers_visible) {
		$workers_ajax = array(
      'callback' => 'sirius_dispatch_job_workerform_ajax',
      'wrapper' => 'workers_filtered',
      'method' => 'replace',
      'effect' => 'fade',
    );
	}


	// Get the last job created by this job at this employer with this type.
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch_job');
  $query->fieldCondition('field_grievance_shop', 'target_id', $employer_node->nid);
  $query->fieldCondition('field_sirius_dispatch_job_type', 'tid', $job_type_tid);
  $query->propertyOrderBy('nid', 'desc');
  $query->propertyCondition('uid', $user->uid);
  $query->range(0,1);
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$last_job_node = node_load(array_keys($stmt['node'])[0]);
  }

	$form_state['employer_node'] = $employer_node;
	$form_state['job_type_tid'] = $job_type_tid;

	$employer_json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$industry_json = json_decode($industry_term->field_sirius_json['und'][0]['value'], TRUE);

	$job_type_term = taxonomy_term_load($job_type_tid);
	$form['employer_markup'] = array('#markup' => '<div class="sirius_form_intro">Creating a job for <strong>' . 
		$employer_node->title . 
		' / ' .
		$job_type_term->field_sirius_name_display['und'][0]['value'] .
		'</strong></div>'
	);

	// Event
	$show_event = ($job_policy['event']['has_event'] == 'Yes' && $job_policy['event']['on_job_create'] == 'select');
	if ($show_event) {
		$event_options = array(NULL => t('-- Select --'));
		$query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_event');
	  $query->fieldCondition('field_sirius_event_type', 'tid', $job_policy['event']['event_type']);
	  $query->fieldCondition('field_sirius_daterepeat', 'value', gmdate('Y-m-d'), '>=');
	  $query->fieldCondition('field_sirius_active', 'value', 'Yes');
	  $query->fieldOrderBy('field_sirius_daterepeat', 'value');
	  $stmt = $query->execute();
	  if (!$stmt['node']) {
	  	drupal_set_message(t('There are no upcoming events.'), 'error');
	  	return array();
	  }
	  $event_nodes = node_load_multiple(array_keys($stmt['node']));
	  foreach ($event_nodes as $event_nid => $event_node) {
	  	$bullen_job_node = sirius_event_bullpen_job($event_nid);
	  	if (!$bullen_job_node) { continue; }
	  	$employer_node = node_load($bullen_job_node->field_grievance_shop['und'][0]['target_id']);
	  	$facility_node = node_load($bullen_job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	  	$event_options[$event_nid] = $employer_node->title . ': ' . sirius_date_date($event_node, 'field_sirius_daterepeat', 'Y-m-d h:i a') . ': ' . $facility_node->title;
	  }
		$form['event_nid'] = array(
			'#type' => 'select',
			'#title' => t('Event'),
			'#options' => $event_options,
			'#required' => TRUE,
		);
	}

	// Skill
	if ($job_policy['form']['hide_skill'] != 'Yes') {

		$skill_options = array(NULL => t('-- No specific skill required --')) + sirius_dispatch_skill_options($industry_tid);

	  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
	  $skill_count = $form_state['skill_count'];
	  $skill_count += $ajax_params['qty'];
	  if ($skill_count < 1) { $skill_count = 1; }
	  $form_state['skill_count'] = $skill_count;

	  $form['skills']['prefix'] = array('#markup' => '<div id="skills">');
	  for ($i=0; $i<$skill_count; ++$i) {
			$form['skills'][$i] = array(
				'#type' => 'select',
				'#options' => $skill_options,
				'#title' => 'Skill',
		    '#ajax' => $workers_ajax,
			);
	  }
	  $form['skills']['suffix'] = array('#markup' => '</div>');

	  $form['add_one'] = array(
	    '#type' => 'button',
	    '#value' => t('Add Another Skill'),
	    '#ajax' => array(
	      'callback' => 'sirius_ajax_generic_replace',
	    ),
	    '#sirius_ajax' => array(
	      'qty' => 1,
	      'wrapper' => '#skills',
	      'element' => 'skills',
	    ),
	    '#limit_validation_errors' => array(array('add_one')),
	  );
	}

	// Department
	if ($job_policy['form']['hide_department'] != 'Yes') {
		$departments = sirius_worker_department_by_industry(array($industry_tid));
		if ($departments) {
			$department_options = array(NULL => t('-- No specific department --'));
			foreach ($departments as $department_tid => $department_term) {
				$department_options[$department_tid] = $department_term->name;
			}
			$form['department'] = array(
				'#type' => 'select',
				'#options' => $department_options,
				'#title' => 'Department',
		    '#ajax' => $workers_ajax,
			);
		}
	}

	// Member status
	$show_closed_statuses = FALSE;
	if ($job_policy['event']['has_event'] == 'Yes') { $show_closed_statuses = TRUE; }
	$open_member_statuses = sirius_worker_member_status_get_open_statuses($industry_tid, $show_closed_statuses, $job_type_tid);
	$open_member_status_tids = array_keys($open_member_statuses);
	$open_member_status_options = array();
	foreach ($open_member_statuses as $tid => $term) {
		$open_member_status_options[$tid] = $term->name;
		if ($term->field_sirius_name_short['und'][0]['value']) {
			$open_member_status_options[$tid] .= ' [' . $term->field_sirius_name_short['und'][0]['value'] . ']';
		}
	}

	if (!$open_member_status_tids) { return sirius_form_error('There are no open member statuses.'); }
	
	if (count($open_member_status_tids) > 1) {
		$form['member_status'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Member Status',
			'#options' => $open_member_status_options,
			'#default_value' => array_keys($open_member_status_options),
	    '#ajax' => $workers_ajax,
		);
	} else {
		$form['member_status'] = array(
			'#type' => 'value',
			'#options' => $open_member_status_options,
			'#default_value' => array_keys($open_member_status_options),
		);
	}

	// EBA
	if ($job_policy['workers']['may_be_eba'] == 'Yes') {
	  $dates = sirius_dispatch_eba_dates_upcoming();
		$form['eba'] = array(
			'#title' => '',
			'#type' => 'select',
			'#options' => array(
				'No' => t('No, do not dispatch any currently employed workers'),
				'Yes' => t('Yes, dispatch employed-but-available workers.'),
			),
			'#default_value' => $worker_node->field_sirius_dispatch_eba['und'][0]['value'],
			'#prefix' => '<div class="sirius_form_intro">' . t('If this is a job which will take place within the next seven days, you can also dispatch workers who are currently employed, but have indicated that they are available on specific days.') . '</div>',
	    '#ajax' => $workers_ajax,
		);

		$form['eba_dates'] = array(
			'#title' => t('The job will take place on the following days only'),
			'#type' => 'checkboxes',
			'#options' => $dates,
			'#states' => array(
				'visible' => array(
					array(':input[name="eba"]' => array('value' => 'Yes')),
				)
			),
	    '#ajax' => $workers_ajax,
		);
	}

	if (!$show_event) {
		$form['datetime'] = array(
			'#type' => 'date_popup',
			'#title' => t('Report At Time'),
			'#timepicker' => 'timepicker',
			'#timepicker_options' => array(
				'rows' => 6,
				'minutes' => array(
				  'starts' => 0,
				  'ends' => 56,
				  'interval' => 5,
				),
			),
			'#attributes' => array(
				'autocomplete' => 'off',
			),
			// '#states' => $datetime_states,
		);
	}

	if ($job_policy['form']['shift_single'] == 'Yes') {
		$duration_options = array();
    for ($hour = 0; $hour <= 12; ++$hour) {
      for ($min = 0; $min < 60; $min += 30) {
        $secs = 60*$min + 3600*$hour;
        $duration_options[$secs] = sirius_event_duration_format($secs);
      }
    }
    unset($duration_options[0]);

		$form['duration'] = array(
			'#type' => 'select',
			'#title' => t('Duration'),
			'#options' => array(NULL => t('-- Select --')) + $duration_options,
		);
	}

	// Call Number
	$form['id'] = array(
		'#type' => 'textfield',
		'#title' => t('Call Number'),
		'#size' => 5,
	);

	// Count Policy
	$count_policy = $job_policy['workers']['count_policy'];
	if ($count_policy == 'optional' || $count_policy == 'required') {
		$form['count'] = array(
			'#type' => 'textfield',
			'#title' => t('Workers Required'),
			'#size' => 5,
		);
		if ($count_policy == 'required') { $form['count']['#required'] = TRUE; }
	}

	if (!$show_event) {
		// Group
		$group_options = sirius_dispatch_group_list_active($industry_tid);
		if ($group_options) { 
			// $group_options = array(NULL => t('-- Select --')) + $group_options;
			$form['job_group'] = array(
				'#type' => 'select',
				'#title' => t('Group or Show'),
				'#options' => $group_options,
			);
		}

		// Facility
		$hall_ajax_id = 'sirius_dispatch_page_job_create_ajax_halls';
		$facility_options = sirius_dispatch_facility_dropdown(array('employer_node' => $employer_node));
		$form['facility'] = array(
			'#type' => 'select',
			'#options' => $facility_options,
			'#title' => t('Location'),
	    '#ajax' => array(
	      'callback' => $hall_ajax_id,
	      'wrapper' => $hall_ajax_id,
	      'method' => 'replace',
	      'effect' => 'fade',
	    ),
		);
		$facility_nid = $form_state['values']['facility'];
		$hall_options = sirius_dispatch_facility_hall_dropdown($facility_nid);
		if (count($hall_options) > 1) {
			$form['hall'] = array(
				'#type' => 'select',
				'#options' => $hall_options,
				'#title' => t('Hall'),
				'#prefix' => "<div id=\"$hall_ajax_id\">",
				'#suffix' => '</div>',
			);
		} else {
			$form['hall'] = array(
				'#prefix' => "<div id=\"$hall_ajax_id\">",
				'#suffix' => '</div>',
			);
		}

		if ($job_policy['form']['hide_address_notes'] != 'Yes') {
			$form['address_notes_type'] = array(
				'#title' => t('Report Location'),
				'#type' => 'select',
				'#options' => array(
					NULL => t('-- Select --'),
					'door' => 'Door #',
					'booth' => 'Booth #',
					'other' => 'Other'
				),
				'#prefix' => '<div style="float: left;">',
				'#suffix' => '</div>',
			);

			$form['address_notes_door'] = array(
				'#type' => 'textfield',
				'#size' => 4,
				'#title' => 'Door #',
				'#states' => array(
			  	'visible' => array(
			  		array(':input[name="address_notes_type"]' => array('value' => 'door')),
			  	)
				),
				'#prefix' => '<div style="float: left; margin-left: 20px;">',
				'#suffix' => '</div>',
			);

			$form['address_notes_booth'] = array(
				'#type' => 'textfield',
				'#size' => 4,
				'#title' => 'Booth #',
				'#states' => array(
			  	'visible' => array(
			  		array(':input[name="address_notes_type"]' => array('value' => 'booth')),
			  	)
				),
				'#prefix' => '<div style="float: left; margin-left: 20px;">',
				'#suffix' => '</div>',
			);

			$form['address_notes_other'] = array(
				'#type' => 'textfield',
				'#size' => 30,
				'#title' => 'Other (please specify)',
				'#states' => array(
			  	'visible' => array(
			  		array(':input[name="address_notes_type"]' => array('value' => 'other')),
			  	)
				),
				'#prefix' => '<div style="float: left; margin-left: 20px;">',
				'#suffix' => '</div>',
			);
		}
	}

	$employer_json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$industry_json = json_decode($industry_term->field_sirius_json['und'][0]['value'], TRUE);

	$notes_default = $employer_json['dispatch']['default']['notes'];
	if (!$notes_default) { $notes_default = $industry_json['dispatch']['default']['notes']; }
	$form['notes'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
		'#description' => t('Enter any additional instructions or comments for workers.'),
		'#default_value' => $notes_default,
		'#prefix' => '<div style="clear: left;">',
		'#suffix' => '</div>',
	);

	if (!$show_event) {
		if ($job_policy['form']['hide_address_parking'] != 'Yes') {
			$address_parking_default = $employer_json['dispatch']['default']['address_parking'];
			if (!$address_parking_default) { $address_parking_default = $industry_json['dispatch']['default']['address_parking']; }
			$form['address_parking'] = array(
				'#type' => 'textarea',
				'#title' => t('Parking Instructions'),
				'#description' => t('Enter any parking instructions for workers.'),
				'#default_value' => $address_parking_default,
			);
		}
	}

	$job_node = sirius_dispatch_page_job_create_get_job_node($form_state);
	$form_state['job_node'] = $job_node;

	if ($workers_visible) {
		$form += sirius_dispatch_job_workerform($form, $form_state);
	}

	$default_job_node = $last_job_node;
	if (!$default_job_node) { $default_job_node = $job_node; }

	$form['notify'] = array(
		'#type' => 'select',
		'#title' => t('Send notifications?'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $default_job_node->field_sirius_notify['und'][0]['value'],
		'#default_value' => 'Yes',
	);

	$form['notify_details'] = array(
		'#type' => 'fieldset',
		'#states' => array(
	  	'visible' => array(
	  		array(':input[name="notify"]' => array('value' => 'Yes')),
	  	)
		),
	);

	for ($i=0; $i<8; ++$i) {
		$default_emails[$i] = $default_job_node->field_sirius_emails['und'][$i]['value'];
	}
	if (!$default_emails[0]) { $default_emails[0] = $user->mail; }

	for ($i=0; $i<8; ++$i) {
		$form['notify_details']["email_$i"] = array(
			'#type' => 'textfield',
			'#default_value' => $default_emails[$i],
		);
		$form['notify_details']['email_0']['#title'] = t('Notification Emails');
	}

	$dispatch_job_nfcns_field = field_info_field('field_sirius_dispatch_job_nfcns');
	$dispatch_job_nfcns_options = list_allowed_values($dispatch_job_nfcns_field);
	$dispatch_job_nfcns_default = array();
	if ($default_job_node->field_sirius_dispatch_job_nfcns['und']) {
		foreach ($default_job_node->field_sirius_dispatch_job_nfcns['und'] as $tmp) {
			if ($tmp['value']) {
				$dispatch_job_nfcns_default[] = $tmp['value'];
			}
		}
	}
	$form['notify_details']['dispatch_job_nfcns'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Notifications'),
		'#options' => $dispatch_job_nfcns_options,
		'#default_value' => $dispatch_job_nfcns_default,
		'#checkall' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('I have checked the form above, and all the information is correct. Please send worker notifications about this new job request.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Create Job Request',
	);

	return $form;
}

function sirius_dispatch_page_job_create_ajax_halls($form, $form_state) {
  return $form['hall'];
}

function sirius_dispatch_page_job_create_workers($form, $form_state) {
	return $form['fieldset_workers']['ajax']['workers'];
}

function sirius_dispatch_page_job_create_form_validate($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_policy = sirius_dispatch_job_policy_from_tid($job_type_tid);

	// Don't validate on ajax. This is kludgey, but limit_validation_errors() didn't work for me because it suppressed the values I needed.
	if ($form_state['triggering_element']['#value'] == 'Filter') { return; }

	// Report date must be in the future
	if (!$form_state['values']['event_nid']) {
		$report_ts = strtotime($form_state['values']['datetime']);
		if ($report_ts < time() + 60*60*1) {
	  	form_set_error('datetime', 'Report date must be at least one hour in the future.');
	  }
	}

	// Single-shift jobs must have a duration
	if ($job_policy['form']['shift_single'] == 'Yes') {
		if (!$form_state['values']['duration']) {
	  	form_set_error('duration', 'Please enter the shift duration.');
		}
	}


	for ($i=0; $i<10; ++$i) {
		if ($form_state['values']['notify_details']["email_$i"] && !sirius_email_validate($form_state['values']['notify_details']["email_$i"])) {
			form_set_error("notify_details][email_$i", 'Please enter a valid email addresss.');
		}
	}

	// If you want EBA, you have to specify the dates
	if ($form_state['values']['eba'] == 'Yes') {
		$found = FALSE;
		if ($form_state['values']['eba_dates']) {
			foreach ($form_state['values']['eba_dates'] as $key => $value) {
				if ($value) { 
					$found = TRUE;
				}
			}
		}
		if (!$found) {
	  	form_set_error('eba_dates', 'If you wish to dispatch EBA workers, you must specify the dates when the job will take place');
		}
	}

	// CBN must select workers
	$job_type_tid = $form_state['job_type_tid'];	
	$job_policy = sirius_dispatch_job_policy_from_tid($job_type_tid);
	$count = $form_state['values']['count'];
	$selected = 0;
	if ($form_state['values']['fieldset_workers']['ajax']['workers']) {
		foreach ($form_state['values']['fieldset_workers']['ajax']['workers'] as $ignore => $worker_nid) {
			if ($worker_nid) { ++$selected; }
		}
	}
	if ($job_policy['workers']['cbn'] == 'Yes' && $job_policy['workers']['fan'] != 'Yes') {
		if ($count && $selected < $count) {
			form_set_error('worker_id', t('For a call by name, please select at least @count workers. You have selected @selected.', array('@count' => $count, '@selected' => $selected)));
		}
	}

	// These should be simple required fields, but required fields interfere with ajax callbacks, dammit.
	if (!$form_state['values']['confirm']) { form_set_error('confirm', t('Please check the confirm box.')); }

	$count_policy = $job_policy['workers']['count_policy'];
	if ($count_policy == 'required') {
		if (! ($form_state['values']['count'] > 0)) { form_set_error('count', t('Please enter a nonzero number of workers.')); }
	}
	if (!$form_state['values']['event_nid']) {
		if (!$form_state['values']['facility']) { form_set_error('facility', t('Please select a facility.')); }
	}

}

function sirius_dispatch_page_job_create_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	// Build the job
	$job_node = sirius_dispatch_page_job_create_get_job_node($form_state);
	$job_node = node_submit($job_node);

	// Build and save the event (if any)
	$event_node = sirius_dispatch_page_job_create_get_event_node($form_state, $job_node);
	if ($event_node) {
		$event_node = node_submit($event_node);
		node_save($event_node);
		$event_nid = $event_node->nid;
		$job_node->field_sirius_events['und'][0]['target_id'] = $event_nid;
	}

	// Save the job
	node_save($job_node);
	$job_nid = $job_node->nid;

	// Build and save any CBN workers
	if ($form_state['values']['fieldset_workers']['ajax']['workers']) {
		foreach ($form_state['values']['fieldset_workers']['ajax']['workers'] as $worker_nid => $checked) {
			if (!$checked) { continue; }
			// drupal_set_message("Creating: $worker_nid");
			$result = sirius_dispatch_actions_create($job_node, $worker_nid, 'Yes');
			// drupal_set_message('<pre>' . print_r($result, 1) . '</pre>');
		}
	}

	drupal_set_message("Your job request has been submitted.");
	drupal_goto('node/' . $job_node->nid . '/sirius_dispatch_job_manage');
}


function sirius_dispatch_page_job_create_get_job_node($form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$job_policy = sirius_dispatch_job_policy_from_tid($job_type_tid);
	$employer_node = $form_state['employer_node'];

	global $user;
	$job_node = new stdClass();
	$job_node->title = $employer_node->title . ' - ' . $job_type_term->name . ' - ' . gmdate('Y-m-d', strtotime($form_state['values']['datetime']));
	$job_node->type = 'sirius_dispatch_job';
	node_object_prepare($job_node);
	$job_node->language = LANGUAGE_NONE;
	$job_node->uid = $user->uid;
	$job_node->status = 1;
	$job_node->promote = 0;
	$job_node->comment = 0;
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) { $job_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

	$job_node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;
	$job_node->field_sirius_dispatch_job_type['und'][0]['tid'] = $job_type_term->tid;

	$report_ts = strtotime($form_state['values']['datetime']);
 	$job_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', $report_ts);

 	if ($job_policy['form']['shift_single'] == 'Yes') {
 		// End TS comes from the "duration"
 		$end_ts = $report_ts + $form_state['values']['duration'];
	 	$job_node->field_sirius_datetime_completed['und'][0]['value'] = gmdate('Y-m-d H:i:s', $end_ts);
 	} else if ($job_policy['workers']['may_be_eba'] == 'Yes') {
 		// End TS may come from the EBA dates
 		if ($form_state['values']['eba_dates']) {
			$dates = array_filter(array_values($form_state['values']['eba_dates']));
			if ($dates) { 
		 		$end_ts = 0;
				foreach ($dates as $date) {
					$probe = strtotime('+1 day', strtotime($date));
					if ($probe > $end_ts) { $end_ts = $probe; }
				}
				if ($end_ts) {
					$job_node->field_sirius_datetime_completed['und'][0]['value'] = gmdate('Y-m-d H:i:s', $end_ts);
				}
			}
		}
	}

  $job_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s');
	$job_node->field_sirius_id['und'][0]['value'] = $form_state['values']['id'];
	$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Pending';


	$count_policy = $job_policy['workers']['count_policy'];
	if ($count_policy == 'optional' || $count_policy == 'required') {
		if ($form_state['values']['count']) {
			$job_node->field_sirius_count['und'][0]['value'] = $form_state['values']['count'];
		}
	} else if ($count_policy == 'fixed') {
		$job_node->field_sirius_count['und'][0]['value'] = $job_policy['workers']['count_fixed'];
	} else if ($count_policy == 'automatic') {
		$count = 0;
		if ($form_state['values']['fieldset_workers']['ajax']['workers']) {
			foreach ($form_state['values']['fieldset_workers']['ajax']['workers'] as $worker_nid => $checked) {
				if (!$checked) { continue; }
				++$count;
			}
		}
		$job_node->field_sirius_count['und'][0]['value'] = $count;
	}

	$job_node->field_sirius_dispatch_facility['und'][0]['target_id'] = $form_state['values']['facility'];
	if ($form_state['values']['hall']) {
		$job_node->field_sirius_dispatch_hall['und'][0]['target_id'] = $form_state['values']['hall'];
	}
	if ($form_state['industry_tid']) { $job_node->field_sirius_industry['und'][0]['tid'] = $form_state['industry_tid']; }

	$address_notes_type = $form_state['values']['address_notes_type'];
	if ($address_notes_type == 'booth') {
		if ($form_state['values']['address_notes_booth']) {
			$job_node->field_sirius_address_notes['und'][0]['value'] = 'Booth #' . $form_state['values']['address_notes_booth'];
		}
	}
	if ($address_notes_type == 'door') {
		if ($form_state['values']['address_notes_door']) {
			$job_node->field_sirius_address_notes['und'][0]['value'] = 'Door #' . $form_state['values']['address_notes_door'];
		}
	}
	if ($address_notes_type == 'other') {
		if ($form_state['values']['address_notes_other']) {
			$job_node->field_sirius_address_notes['und'][0]['value'] = $form_state['values']['address_notes_other'];
		}
	}

	if ($form_state['values']['address_notes']) {
		$job_node->field_sirius_address_notes['und'][0]['value'] = $form_state['values']['address_notes'];
	}
	if ($form_state['values']['address_parking']) {
		$job_node->field_sirius_address_parking['und'][0]['value'] = $form_state['values']['address_parking'];
	}
	if ($form_state['values']['notes']) {
		$job_node->field_sirius_notes['und'][0]['value'] = $form_state['values']['notes'];
	}

	if ($form_state['values']['skills']) {
		$skill_tids = array_filter(array_values($form_state['values']['skills']));
		if ($skill_tids) {
			foreach ($skill_tids as $skill_tid) {
				$job_node->field_sirius_skill['und'][] = array('tid' => $skill_tid);
			}
		}
	}

	if ($form_state['values']['department']) {
		$job_node->field_grievance_departments['und'][0]['tid'] = $form_state['values']['department'];
	}

	if ($form_state['values']['job_group']) {
		$job_node->field_sirius_dispatch_job_group['und'][0]['target_id'] = $form_state['values']['job_group'];
	}

	$event_nid = $form_state['values']['event_nid'];
	if ($event_nid) {
		$job_node->field_sirius_events['und'][0]['target_id'] = $event_nid;
		$event_node = node_load($event_nid);
		$sponsor_job_node = sirius_event_bullpen_job($event_nid);
		$job_node->field_sirius_datetime = $sponsor_job_node->field_sirius_datetime;
		$job_node->field_sirius_dispatch_facility = $sponsor_job_node->field_sirius_dispatch_facility;
	}

	if ($form_state['values']['notify']) {
		$job_node->field_sirius_notify['und'][0]['value'] = $form_state['values']['notify'];

		for ($i=0; $i<10; ++$i) {
			$email = sirius_email_format($form_state['values']['notify_details']["email_$i"]);
			$job_node->field_sirius_emails['und'][$i]['value'] = $email;
		}

		if ($form_state['values']['notify_details']['dispatch_job_nfcns']) {
			$values = array();
			foreach ($form_state['values']['notify_details']['dispatch_job_nfcns'] as $name => $value) {
				if ($value) {
					$values[] = array('value' => $value);
				}
			}
			$job_node->field_sirius_dispatch_job_nfcns['und'] = $values;
		}
		$job_node->field_sirius_notify['und'][0]['value'] = $form_state['values']['notify'];
	}

	if ($form_state['values']['member_status']) {
		$member_status_tids = array_values($form_state['values']['member_status']);
	}

	$job_node->field_sirius_dispatch_eba['und'][0]['value'] = $form_state['values']['eba'];
	if ($form_state['values']['eba'] == 'Yes') {
		if ($form_state['values']['eba_dates']) {
			$job_node->field_sirius_dispatch_eba_dates['und'] = array();
			foreach ($form_state['values']['eba_dates'] as $key => $value) {
				if ($value) {
					$job_node->field_sirius_dispatch_eba_dates['und'][] = array('value' => "$value");
				}
			}
		}
	}

	if (!$member_status_tids) {
		$member_status_tids = array_keys(sirius_worker_member_status_get_open_statuses($job_node->field_sirius_industry['und'][0]['tid'], FALSE, $job_type_term->tid));
	}
	foreach ($member_status_tids as $tid) {
		if ($tid) {
			$job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
		}
	}
	return $job_node;
}

function sirius_dispatch_page_job_create_get_event_node($form_state, $job_node) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_policy = sirius_dispatch_job_policy_from_tid($job_type_tid);
	if ($job_policy['event']['has_event'] != 'Yes') { return NULL; }
	if ($job_policy['event']['on_job_create'] != 'create') { return NULL; }

	$event_type_tid = $job_policy['event']['event_type'];
	if (!$event_type_tid) { return NULL; }
	$event_type_term = taxonomy_term_load($event_type_tid);

	$employer_node = $form_state['employer_node'];

	global $user;
	$event_node = new stdClass();
	$event_node->title = $employer_node->title . ' - ' . $event_type_term->name . ' - ' . gmdate('Y-m-d', strtotime($form_state['values']['datetime']));
	$event_node->field_sirius_active['und'][0]['value'] = 'Yes';
	$event_node->type = 'sirius_event';
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) { $event_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	node_object_prepare($event_node);
	$event_node->language = LANGUAGE_NONE;
	$event_node->uid = $user->uid;
	$event_node->status = 1;
	$event_node->promote = 0;
	$event_node->comment = 0;
 	$event_node->field_sirius_daterepeat['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['datetime']));
  $event_node->field_sirius_event_type['und'][0]['tid'] = $event_type_tid;
  $event_node->field_sirius_active['und'][0]['value'] = 'Yes';

	return $event_node;
}

