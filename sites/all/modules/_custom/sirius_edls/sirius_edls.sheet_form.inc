<?php

function sirius_edls_sheet_form($form, &$form_state, $sheet_node) {
	$form_state['sheet_nid'] = $sheet_node->nid;

	$sheet_json = sirius_json_get($sheet_node);
	$my_employer_node = sirius_employer_get();
	$sheet_employer_nid = $sheet_node->field_grievance_shop['und'][0]['target_id'];
	if (!$sheet_employer_nid) { $sheet_employer_nid = $my_employer_node->nid; }
	$employer_node = node_load($sheet_employer_nid);

  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css', 'file');

	$form['#tree'] = TRUE;

	if ($sheet_node) { $ts = sirius_fieldval($sheet_node, 'field_sirius_date_start', 'ts'); }
	else { $ts = time(); }

	$name = 'Sheet Info';
	if ($sheet_node->field_sirius_job_number['und'][0]['value']) {
		$name .= ' - Job #' . $sheet_node->field_sirius_job_number['und'][0]['value'];
	}
	$form['sheet'] = array(
		'#type' => 'fieldset',
		'#title' => $name,
		'#collapsible' => TRUE,
	);
	if ($sheet_node) { $form['sheet']['#collapsed'] = TRUE; }

	if ($my_employer_node) {
		$form['sheet']['grievance_shop'] = array(
			'#type' => 'value',
			'#value' => $my_employer_node->nid,
		);
	} else {
		$employer_options = sirius_employer_list(TRUE);
		$form['sheet']['grievance_shop'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
			'#default_value' => $sheet_node->field_grievance_shop['und'][0]['target_id'],
			'#required' => TRUE,
		);
	  if ($sheet_node->nid) { $form['sheet']['grievance_shop']['#disabled'] = TRUE; }
	}

	if (sirius_access_instance('edls')->access('sheet_change_supervisor', array('sheet_node' => $sheet_node))) {
		$supervisor_options = sirius_edls_supervisor_list($sheet_employer_nid);
		$form['sheet']['supervisor'] = array(
			'#type' => 'select',
			'#title' => t('Supervisor'),
			'#options' => array(NULL => t('-- Select --')) + $supervisor_options,
			'#default_value' => $sheet_node->field_grievance_rep_assignee['und'][0]['target_id'],
			'#required' => TRUE,
		);
	}

  $form['sheet']['date_start'] = array(
	  '#type' => 'date_popup',
	  '#title' => t('Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => $sheet_node->field_sirius_date_start['und'][0]['value'],
		'#required' => TRUE,
  );
  if ($sheet_node->nid) { $form['sheet']['date_start']['#disabled'] = TRUE; }

	$job_group_options = sirius_dispatch_group_list_active(NULL, $ts);
	$form['sheet']['sirius_dispatch_job_group'] = array(
		'#type' => 'select',
		'#title' => t('Event'),
		'#options' => array(NULL => t('-- Select --')) + $job_group_options,
		'#default_value' => $sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id'],
		'#required' => TRUE,
	);

	$form['sheet']['show_status'] = array(
		'#type' => 'select',
		'#title' => t('Show Status'),
		'#options' => array(NULL => t('-- Select --')) + array('Move-in' => 'Move-in', 'Show Day' => 'Show Day', 'Teardown' => 'Teardown'),
		'#default_value' => $sheet_json['edls']['sheet']['show_status'],
	);

	$department_options = sirius_edls_departments($my_employer_node);
	$form['sheet']['department'] = array(
		'#type' => 'select',
		'#title' => t('Department'),
		'#options' => array(NULL => t('-- Select --')) + $department_options,
		'#default_value' => $sheet_node->field_grievance_department_tid['und'][0]['tid'],
		'#id' => 'sirius_edls_sheet_department',
	);

  $form['sheet']['job_number'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Job Number'),
		'#default_value' => $sheet_node->field_sirius_job_number['und'][0]['value'],
		'#required' => TRUE,
  );

	$hall_ajax_id = 'sirius_edls_sheet_form_submit_ajax_halls';

	$facility_options = sirius_dispatch_facility_dropdown();
  $form['sheet']['facility'] = array(
  	'#type' => 'select',
  	'#options' => $facility_options,
  	'#title' => t('Facility'),
  	'#default_value' => $sheet_node->field_sirius_dispatch_facility['und'][0]['target_id'],
    '#ajax' => array(
      'callback' => $hall_ajax_id,
      'wrapper' => $hall_ajax_id,
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

	if ($form_state['values']['sheet']['facility']) { $facility_nid = $form_state['values']['sheet']['facility']; }
	else { $facility_nid = $sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']; }

	$hall_options = sirius_dispatch_facility_hall_dropdown($facility_nid);
	if (count($hall_options) > 1) {
		$form['sheet']['hall'] = array(
			'#type' => 'select',
			'#options' => $hall_options,
			'#title' => t('Hall'),
			'#prefix' => "<div id=\"$hall_ajax_id\">",
			'#suffix' => '</div>',
	  	'#default_value' => $sheet_node->field_sirius_dispatch_hall['und'][0]['target_id'],
		);
	} else {
		$form['sheet']['hall'] = array(
			'#prefix' => "<div id=\"$hall_ajax_id\">",
			'#suffix' => '</div>',
		);
	}

  $form['sheet']['count'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Worker Count'),
		'#default_value' => $sheet_node->field_sirius_count['und'][0]['value'],
		'#required' => TRUE,
		'#size' => 4,
  );

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$crews = sirius_edls_crews($sheet_node);
	if ($form_state['crew_count']) {
		$crew_count = $form_state['crew_count'];
	} else {
		$crew_count = count($crews);
	}
  $crew_count += $ajax_params['qty'];
  if ($crew_count < 1) { $crew_count = 1; }
  $form_state['crew_count'] = $crew_count;

  for ($i=count($crews); $i<$crew_count; ++$i) {
  	$crews[] = array();
  }

	$tasks_by_department = sirius_edls_tasks_by_department($employer_node);

  $form['sheet']['notes'] = array(
	  '#type' => 'textarea',
	  '#title' => t('Notes'),
		'#default_value' => sirius_edls_sheet_notes_get($sheet_node),
  );

  $form['#tree'] = TRUE;
	$form['crews']['prefix'] = array('#markup' => '<div id="crews">');
	foreach ($crews as $delta => $crew) {
		$uuid = $crew['uuid'];
    $default_weight = $crew['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_edls_crew_title($crew);
    $title .= "<span class=\"sirius_form_weight_indicator\">Sort Order: $default_weight</span>";

    $form['crews'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => FALSE,
    );

    $form['crews'][$delta]['weight'] = array(
      '#type' => 'textfield',
      // '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
      '#prefix' => '<div class="sirius_form_weightfield">',
      '#field_prefix' => t('Sort Order:'),
      '#suffix' => '</div>',
      '#attributes' => array('tabindex' => '-1'),
    );

    $form['crews'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $crew['uuid'],
    );

    $form['crews'][$delta]['name'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Name'),
    	'#default_value' => $crew['name'],
    );

   	$department = $sheet_node->field_grievance_department_tid['und'][0]['tid'];
		$task_options = $tasks_by_department[$department];
		if (!$task_options) { $task_options = array(); }
		$task_options = array_combine($task_options, $task_options);
		if (!$task_options[$crew['task']]) { $crew['task'] = NULL; }

    $form['crews'][$delta]['task'] = array(
    	'#type' => 'select',
    	'#title' => t('Task'),
    	'#default_value' => $crew['task'],
    	'#options' => array(NULL => t('-- Select --')) + $task_options,
    	'#attributes' => array('class' => array('sirius_edls_sheet_task')),
    );

    $form['crews'][$delta]['checkin_location'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Check-In Location'),
    	'#default_value' => $crew['checkin_location'],
    );

    $form['crews'][$delta]['count'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Count'),
    	'#size' => 4,
    	'#default_value' => $crew['count'],
    );

    $form['crews'][$delta]['start_time'] = array(
    	'#type' => 'date_popup',
    	'#title' => t('Start Time'),
    	'#size' => 4,
			'#attributes' => array(
				'autocomplete' => 'off',
			),
			'#date_format' => 'H:i',
			'#default_value' => date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])) . ' ' . $crew['start_time'],
    );

    $form['crews'][$delta]['end_time'] = array(
    	'#type' => 'date_popup',
    	'#title' => t('End Time'),
    	'#size' => 4,
			'#attributes' => array(
				'autocomplete' => 'off',
			),
			'#date_format' => 'H:i',
			'#default_value' => date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])) . ' ' . $crew['end_time'],
    );
  }
	$form['crews']['suffix'] = array('#markup' => '</div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add a Crew'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#crews',
    	'element' => 'crews',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_sheet_form_submit_ajax_halls($form, $form_state) {
  return $form['sheet']['hall'];
}

function sirius_edls_sheet_form_submit($form, &$form_state) {
	$sheet_node = node_load($form_state['sheet_nid']);

	$count = $form_state['values']['sheet']['count'] = intval($form_state['values']['sheet']['count']);

	if ($count < 0 || $count > 1000) {
		drupal_set_message("Count must be between 0 and 1000.", 'error');
		$form_state['rebuild'] = TRUE; 
		return;
	}

	$count_verify = 0;
	$crews_input = $form_state['values']['crews'];
	if (!$crews_input) { $crews_input = array(); }
	$crews_input_by_uuid = array();
	foreach ($crews_input as $delta => $crew) {
		$crew_count = $form_state['values']['crews'][$delta]['count'] = intval($crew['count']);
		$count_verify += $crew_count;
		$crews_input_by_uuid[$crew['uuid']] = $crew;
	}
	if ($count_verify != $count) {
		drupal_set_message("The count in all the crews ($count_verify) must add up to the total count ($count).", 'error');
		$form_state['rebuild'] = TRUE; 
		return;
	}

	if ($sheet_node) {
		$assignments = sirius_edls_assignment_fetch_sheet($sheet_node->field_sirius_date_start['und'][0]['value'], $sheet_node->nid);
		foreach ($assignments as $crew_uuid => $assignment_records) {
			if (!$crews_input_by_uuid[$crew_uuid]) {
				drupal_set_message("There is a worker assigned to a crew that does not appear on this sheet. Please remove the assignment.", 'error');
				$form_state['rebuild'] = TRUE;
				return;
			}

			foreach ($assignment_records as $assignment_record) {
				$position = $assignment_record['assignment_position'];
				if ($position+1 > $crews_input_by_uuid[$crew_uuid]['count']) {
					drupal_set_message("There is a worker assigned to position " . ($position+1) . " on a crew that only has " . $crews_input_by_uuid[$crew_uuid]['count'] . ' positions. Please remove the assignment or increase the number of positions.', 'error');
					$form_state['rebuild'] = TRUE;
					return;
				}
			}
		}
	}

	if (!$sheet_node) {
		global $user;
		$sheet_node = new stdClass();
		$sheet_node->type = 'sirius_edls_sheet';
		$sheet_node->title = 'EDSL Sheet';
		$sheet_node->language = LANGUAGE_NONE;
		$sheet_node->uid = $user->uid;
		$sheet_node->status = 1;
		$sheet_node->promote = 0;
		$sheet_node->comment = 0;
		$sheet_node = node_submit($sheet_node);
		node_object_prepare($sheet_node);

		if (sirius_domain_nid()) {
			$sheet_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
		}

		$sheet_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($form_state['values']['sheet']['date_start']));

		if ($form_state['values']['sheet']['grievance_shop']) {
			$sheet_node->field_grievance_shop['und'][0]['target_id'] = $form_state['values']['sheet']['grievance_shop'];
		} else {
			unset($sheet_node->field_grievance_shop['und'][0]);
		}
		$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = 'draft';
	}
	$sheet_json = sirius_json_get($sheet_node);

	if ($form_state['values']['sheet']['sirius_dispatch_job_group']) {
		$sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id'] = $form_state['values']['sheet']['sirius_dispatch_job_group'];
	} else {
		unset($sheet_node->field_sirius_dispatch_job_group['und'][0]);
	}

	if ($form_state['values']['sheet']['department']) {
		$sheet_node->field_grievance_department_tid['und'][0]['tid'] = $form_state['values']['sheet']['department'];
	} else {
		unset($sheet_node->field_grievance_department_tid['und'][0]);
	}

	$sheet_node->field_sirius_job_number['und'][0]['value'] = trim($form_state['values']['sheet']['job_number']);
	$sheet_node->field_sirius_count['und'][0]['value'] = $count;

	$sheet_json['edls']['sheet']['show_status'] = $form_state['values']['sheet']['show_status'];

	if ($form_state['values']['sheet']['facility']) {
		$sheet_node->field_sirius_dispatch_facility['und'][0]['target_id'] = $form_state['values']['sheet']['facility'];
	} else {
		unset($sheet_node->field_sirius_dispatch_facility['und'][0]);
	}

	if ($form_state['values']['sheet']['hall']) {
		$sheet_node->field_sirius_dispatch_hall['und'][0]['target_id'] = $form_state['values']['sheet']['hall'];
	} else {
		unset($sheet_node->field_sirius_dispatch_hall['und'][0]);
	}

	if (sirius_access_instance('edls')->access('sheet_change_supervisor', array('sheet_node' => $sheet_node))) {
		if ($form_state['values']['sheet']['supervisor']) {
			$sheet_node->field_grievance_rep_assignee['und'][0]['target_id'] = $form_state['values']['sheet']['supervisor'];
		} else {
			unset($sheet_node->field_grievance_rep_assignee['und'][0]);
		}
	} else {
		if (!$sheet_node->field_grievance_rep_assignee['und'][0]['target_id']) {
			global $user;
			$sheet_node->field_grievance_rep_assignee['und'][0]['target_id'] = $user->uid;
		}
	}

	$crews = array();

	foreach ($crews_input as $crew) {
		if (!$crew['start_time'] && !$crew['count']) { continue; }
    if ($crew['delete']) { continue; }
		$uuid = $crew['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$crew['uuid'] = $uuid;
		}
		$crews[$uuid] = $crew;
	}

	$sheet_json['edls']['crews'] = $crews;
	sirius_json_set($sheet_node, $sheet_json);

	$sheet_node->title = sirius_edls_sheet_title($sheet_node);

	node_save($sheet_node);

	sirius_edls_sheet_notes_set($sheet_node, $form_state['values']['sheet']['notes']);

	drupal_set_message("Sheet saved.");
	drupal_goto('node/' . $sheet_node->nid . '/sirius_edls_sheet_edit');
}
