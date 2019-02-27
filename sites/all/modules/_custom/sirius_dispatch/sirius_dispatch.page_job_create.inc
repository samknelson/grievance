<?php

function sirius_dispatch_page_job_create($employer_node = NULL) {
	if (user_access('sirius dispatch staff')) {
		// Staff can receive an employer from the input. If not, they have to pick one.
		if ($employer_node) { 
			return drupal_get_form('sirius_dispatch_page_job_create_form', $employer_node);
		} else {
			return drupal_get_form('sirius_employer_select_form');
		}
	} else {
		// All other users are locked into their own employer
		global $user;
		if (!$user || !$user->uid) { return t('User not found.'); }
		$user_full = user_load($user->uid);
		$employer_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
		if (!$employer_nid) { return t('No employer found.'); }
		$employer_node = node_load($employer_nid);
		return drupal_get_form('sirius_dispatch_page_job_create_form', $employer_node);
	}
}

function sirius_employer_select_form($form, &$form_state) {
	// Get the list of shops.  Use SQL to avoid loading the full node
	$sql = "select title, nid from node ";
	$sql .= "where type = 'grievance_shop' ";
	$sql .= "order by title ";
	$stmt = db_query($sql, array());
	$options = array(
		NULL => t('-- Select --'),
	);
	while ($hr = $stmt->fetchAssoc()) {
		$options[$hr['nid']] = $hr['title'];
	}

	$form['employer_nid'] = array(
		'#title' => t('Employer'),
		'#type' => 'select',
		'#options' => $options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Go',
	);

	return $form;
}

function sirius_employer_select_form_submit($form, &$form_state) {
	$employer_nid = $form_state['values']['employer_nid'];
	drupal_goto(current_path() . '/' . $employer_nid);
}

function sirius_dispatch_page_job_create_form($form, &$form_state, $employer_node) {
	global $user;
	$form_state['employer_node'] = $employer_node;
	$worker_ajax_id = 'sirius_dispatch_page_job_create_workers';

	if (user_access('sirius dispatch staff')) {
		// Tell staff which employer they're looking at
		$form['employer_markup'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="sirius_form_intro"><h2>' . $employer_node->title . '</h2></div>',
		);
	}

	// Job Type
	$dispatch_job_type_options = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'field_sirius_name_display');
	$form['dispatch_job_type'] = array(
		'#type' => 'select',
		'#options' => $dispatch_job_type_options,
		'#title' => t('Request Type'),
		'#required' => TRUE,
	);

	// Skill
	$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE);
	$form['skill'] = array(
		'#type' => 'select',
		'#options' => $skill_options,
		'#title' => 'Skill',
    '#ajax' => array(
      'callback' => $worker_ajax_id,
      'wrapper' => $worker_ajax_id,
      'method' => 'replace',
      'effect' => 'fade',
    ),
	);

	// Member status
	$industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
	$open_member_statuses = sirius_worker_member_status_get_open_statuses($industry_tid);
	$open_member_status_tids = array_keys($open_member_statuses);
	$open_member_status_options = array();
	foreach ($open_member_statuses as $tid => $term) {
		$open_member_status_options[$tid] = $term->name;
		if ($term->field_sirius_name_short['und'][0]['value']) {
			$open_member_status_options[$tid] .= ' [' . $term->field_sirius_name_short['und'][0]['value'] . ']';
		}
	}
	if (count($open_member_status_tids) > 1) {
		$form['member_status'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Member Status',
			'#options' => $open_member_status_options,
			'#required' => TRUE, 
			'#default_value' => array_keys($open_member_status_options),
	    '#ajax' => array(
	      'callback' => $worker_ajax_id,
	      'wrapper' => $worker_ajax_id,
	      'method' => 'replace',
	      'effect' => 'fade',
	    ),
		);
	} else {
		$form['member_status'] = array(
			'#type' => 'value',
			'#options' => $open_member_status_options,
			'#default_value' => array_keys($open_member_status_options),
		);
	}


	// Report Date
	$form['datetime'] = array(
		'#type' => 'date_popup',
		'#title' => t('Report At Time'),
		'#required' => TRUE,
	);

	// Call Number
	$form['id'] = array(
		'#type' => 'textfield',
		'#title' => t('Call Number'),
		'#size' => 5,
		'#required' => TRUE,
	);

	// Call Number
	$form['count'] = array(
		'#type' => 'textfield',
		'#title' => t('Workers Required'),
		'#size' => 5,
		'#states' => array(
			'invisible' => array(
				array(':input[name="dispatch_job_type"]' => array('value' => sirius_get_special_tid('sirius_dispatch_job_type|BULL'))),
			)
		)
	);

	// Group
	$group_options = sirius_dispatch_group_list_active($industry_tid);
	if ($group_options) { 
		// $group_options = array(NULL => t('-- Select --')) + $group_options;
		$form['job_group'] = array(
			'#type' => 'select',
			'#title' => t('Group or Show'),
			'#options' => $group_options,
			'#required' => TRUE,
		);
	}

	// Facility
	$hall_ajax_id = 'sirius_dispatch_page_job_create_ajax_halls';
	$facility_options = sirius_dispatch_facility_dropdown();
	$form['facility'] = array(
		'#type' => 'select',
		'#options' => $facility_options,
		'#title' => t('Location'),
		'#required' => TRUE,
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

	$form['notes'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
		'#description' => t('Enter any additional instructions or comments for workers.'),
		'#default_value' => $employer_node->field_sirius_dispatch_notesdef['und'][0]['value'],
		'#prefix' => '<div style="clear: left;">',
		'#suffix' => '</div>',
	);

	$form['address_parking'] = array(
		'#type' => 'textarea',
		'#title' => t('Parking Instructions'),
		'#description' => t('Enter any parking instructions for workers.'),
	);

	$workers_visible = array(
  	'visible' => array(
  		array(':input[name="dispatch_job_type"]' => array('value' => sirius_get_special_tid('sirius_dispatch_job_type|CBN'))),
  		'or',
  		array(':input[name="dispatch_job_type"]' => array('value' => sirius_get_special_tid('sirius_dispatch_job_type|FAN'))),
  	)
	);
	$job_node = sirius_dispatch_page_job_create_get_job_node($form_state);
	$form += sirius_dispatch_job_get_dispatchable_workers_render_form($job_node, $workers_visible);

	$form['notify'] = array(
		'#type' => 'select',
		'#title' => t('Send notifications?'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $job_node->field_sirius_notify['und'][0]['value'],
	);

	$form['notify_details'] = array(
		'#type' => 'fieldset',
		'#states' => array(
	  	'visible' => array(
	  		array(':input[name="notify"]' => array('value' => 'Yes')),
	  	)
		),
	);

	$default_email = $job_node->field_sirius_email['und'][0]['value'];
	if (!$default_email) { $default_email = $user->mail; }
	$form['notify_details']['email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#default_value' => $default_email,
	);

	$form['notify_details']['email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#default_value' => $default_email,
	);

	$dispatch_job_nfcns_field = field_info_field('field_sirius_dispatch_job_nfcns');
	$dispatch_job_nfcns_options = list_allowed_values($dispatch_job_nfcns_field);
	$dispatch_job_nfcns_default = array();
	if ($job_node->field_sirius_dispatch_job_nfcns['und']) {
		foreach ($job_node->field_sirius_dispatch_job_nfcns['und'] as $tmp) {
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
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#required' => TRUE,
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
	return $form['fieldset_workers']['workers'];
}

function sirius_dispatch_page_job_create_form_validate($form, &$form_state) {
	// Report date must be in the future

	$report_ts = strtotime($form_state['values']['datetime']);
	if ($report_ts < time() + 60*60*2) {
  	form_set_error('datetime', 'Report date must be in the future.');
  }
}

function sirius_dispatch_page_job_create_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$employer_node = $form_state['employer_node'];

	$job_type_term = taxonomy_term_load($form_state['values']['dispatch_job_type']);

	global $user;
	$job_node = sirius_dispatch_page_job_create_get_job_node($form_state);
	$job_node = node_submit($job_node);
	node_save($job_node);
	$job_nid = $job_node->nid;

	if ($form_state['values']['workers']) {
		foreach ($form_state['values']['workers'] as $worker_nid => $checked) {
			if (!$checked) { continue; }
			sirius_dispatch_actions_create($job_node, $worker_nid, 'Yes');
		}
	}

	drupal_set_message("Your job request has been submitted.");
	drupal_goto('node/' . $job_node->nid . '/sirius_dispatch_job_manage');
}


function sirius_dispatch_page_job_create_get_job_node($form_state) {
	$job_type_term = taxonomy_term_load($form_state['values']['dispatch_job_type']);

	global $user;
	$employer_node = $form_state['employer_node'];
	$job_node = new stdClass();
	$job_node->title = $employer_node->title . ' - ' . $job_type_term->name . ' - ' . date('Y-m-d', strtotime($form_state['values']['datetime']));
	$job_node->type = 'sirius_dispatch_job';
	node_object_prepare($job_node);
	$job_node->language = LANGUAGE_NONE;
	$job_node->uid = $user->uid;
	$job_node->status = 1;
	$job_node->promote = 0;
	$job_node->comment = 0;

	$job_node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;
	$job_node->field_sirius_dispatch_job_type['und'][0]['tid'] = $form_state['values']['dispatch_job_type'];
	$job_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s', strtotime($form_state['values']['datetime']));
	$job_node->field_sirius_datetime_created['und'][0]['value'] = date('Y-m-d H:i:s');
	$job_node->field_sirius_id['und'][0]['value'] = $form_state['values']['id'];
	$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Pending';
	if ($form_state['values']['count']) {
		$job_node->field_sirius_count['und'][0]['value'] = $form_state['values']['count'];
	}
	$job_node->field_sirius_dispatch_facility['und'][0]['target_id'] = $form_state['values']['facility'];
	if ($form_state['values']['hall']) {
		$job_node->field_sirius_dispatch_hall['und'][0]['target_id'] = $form_state['values']['hall'];
	}
	$job_node->field_sirius_industry = $employer_node->field_sirius_industry;

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

	if ($form_state['values']['skill']) {
		$job_node->field_sirius_skill['und'][0]['tid'] = $form_state['values']['skill'];
	}

	if ($form_state['values']['job_group']) {
		$job_node->field_sirius_dispatch_job_group['und'][0]['target_id'] = $form_state['values']['job_group'];
	}

	if ($form_state['values']['notify']) {
		$job_node->field_sirius_notify['und'][0]['value'] = $form_state['values']['notify'];
		$job_node->field_sirius_email['und'][0]['value'] = $form_state['values']['email'];
		if ($form_state['values']['dispatch_job_nfcns']) {
			$values = array();
			foreach ($form_state['values']['dispatch_job_nfcns'] as $name => $value) {
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
	if (!$member_status_tids) {
		$member_status_tids = array_keys(sirius_worker_member_status_get_open_statuses($job_node->field_sirius_industry['und'][0]['tid']));
	}
	foreach ($member_status_tids as $tid) {
		if ($tid) {
			$job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
		}
	}

	return $job_node;
}

