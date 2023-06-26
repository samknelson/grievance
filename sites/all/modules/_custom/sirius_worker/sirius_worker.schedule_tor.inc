<?php

function sirius_worker_schedule_tor_list($params) {
	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql = "select distinct node.nid from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";

	$sql .= "left join field_data_field_sirius_log_handler dispatch_handler on dispatch_handler.entity_type = 'node' and dispatch_handler.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler worker_handler on worker_handler.entity_type = 'node' and worker_handler.entity_id = node.nid ";

	$sql .= "left join node worker_node on worker_node.type = 'sirius_worker' and worker_node.nid = worker_handler.field_sirius_log_handler_target_id ";

	$sql .= "left join node dispatch_node on dispatch_node.type = 'sirius_dispatch' and dispatch_node.nid = dispatch_handler.field_sirius_log_handler_target_id ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";

	$sql .= "left join node job_node on job_node.type = 'sirius_dispatch_job' and job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";

	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'tor' ";

	if ($params['worker_nid']) {
		$sql .= "and worker_node.nid = :worker_nid ";
		$sql_args[':worker_nid'] = $params['worker_nid']; 
	}

	if ($params['status']) {
		$sql .= "and field_sirius_type_value = :type ";
		$sql_args[':type'] = $params['status']; 
	}

	if ($params['statuses']) {
		$sql .= "and field_sirius_type_value in (:statuses) ";
		$sql_args[':statuses'] = $params['statuses']; 
	}
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id IS NULL ";
	}
	if ($params['employer_nid']) {
		$sql .= 'and field_grievance_shop_target_id = :employer_nid ';
		$sql_args[':employer_nid'] = $params['employer_nid'];
	}

	if ($params['start_date']) {
		$sql .= "and node.created >= :start_ts ";
		$sql_args[':start_ts'] = strtotime($params['start_date'] . ' GMT');
	}
	if ($params['end_date']) {
		$sql .= "and node.created <= :end_ts ";
		$sql_args[':end_ts'] = strtotime($params['end_date'] . ' GMT');
	}

	if ($params['type_tid']) {
		$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.tor.type_tid')) = :type_tid ";
		$sql_args[':type_tid'] = $params['type_tid'];
	}

	$sql .= "order by node.created desc ";
	$sql .= "limit 500";
	$stmt = sirius_sql_query($sql, $sql_args);

	$tor_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$tor_nids[] = $hr['nid'];
	}

	$tor_nodes = node_load_multiple($tor_nids);
	foreach ($tor_nodes as $tor_nid => $tor_node) {
		if (!sirius_worker_schedule_tor_access('view', NULL, $tor_node)) {
			unset($tor_nodes[$tor_nid]);
		}
	}

	return $tor_nodes;
}

function sirius_worker_schedule_tor_statuses($tor_type_tid) {
	$tor_type_term = taxonomy_term_load($tor_type_tid);
	if (!$tor_type_term) { return array(); }
	$tor_type_json = sirius_json_get($tor_type_term);

	$statuses = $tor_type_json['tor']['statuses'];
	if (!$statuses) { return array(); }
	uksort($statuses, function($a, $b) use ($statuses) { return $statuses[$a]['weight'] <=> $statuses[$b]['weight']; });
	return $statuses;
}

function sirius_worker_schedule_tor_status_options($tor_type_tid = NULL) {
	$statuses = sirius_worker_schedule_tor_statuses($tor_type_tid);
	$status_options = array();
	foreach ($statuses as $status_uuid => $status_record) {
		$status_options[$status_uuid] = $status_record['title'];
	}
	return $status_options;
}

function sirius_worker_schedule_tor_type_status_options() {
	$tor_types = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name');
	
	$result = array();
	foreach ($tor_types as $tor_type_tid => $tor_type_name) {
		$result[$tor_type_tid] = $tor_type_name;
		$status_options = sirius_worker_schedule_tor_status_options($tor_type_tid);
		foreach ($status_options as $status_uuid => $status_name) {
			$result["$tor_type_tid|$status_uuid"] = "$tor_type_name: $status_name";
		}
	}
	return $result;
}

function sirius_worker_schedule_tor_addedit_form($form, &$form_state, $worker_node, $tor_node = NULL, $tor_type_tid = NULL) {
	if ($tor_node) { $form_state['tor_nid'] = $tor_node->nid; }
	$tor_json = sirius_json_get($tor_node);
	if (!$tor_type_tid) { $tor_type_tid = $tor_json['tor']['type_tid']; }
	if (!$tor_type_tid) { 
		drupal_set_message('Please select a type for this TOR entry.', 'error'); 
		return array();
	}
	$tor_type_term = taxonomy_term_load($tor_type_tid);

	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;

	$dispatch_nid = NULL;
	if ($tor_node) {
		$dispatch_nid = $tor_json['tor']['dispatch_nid'];
		$dispatch_node = node_load($dispatch_nid);
	} else {
		$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
		$dispatch_nid = $dispatch_node->nid;
		if ($dispatch_node) {
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
			$form['tor']['dispatch_warning'] = array(
				'#markup' => 
				'<div class="sirius_form_intro">' . 
				t('This record will be forwarded to your employer, @employer_name. If there are confidential materials which you do not want the employer to see, do not include them in this record.', array('@employer_name' => $employer_node->title)) . 
				'</div>',
			);
		}
	}
	$form['tor']['dispatch_nid'] = array('#type' => 'hidden', '#value' => $dispatch_nid);

	// Can't change the type once it's created
	if (!$tor_node) {
		$type_options = array(NULL => t('-- Select --')) + sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name');
		$form['tor']['type_tid'] = array(
			'#type' => 'select',
			'#options' => $type_options,
			'#title' => t('Time-Off Type'),
			'#default_value' => $tor_type_tid,
			'#required' => TRUE,
		);
	} else {
		$form['tor']['type_tid'] = array(
			'#type' => 'value',
			'#value' => $tor_json['tor']['type_tid'],
		);
	}

	$tor_status_options = sirius_worker_schedule_tor_status_options($tor_type_tid);
	foreach ($tor_status_options as $status_key => $status_name) {
		if ($tor_node) {
			if ($status_key == $tor_node->field_sirius_type['und'][0]['value']) { continue; }
			if (!sirius_worker_schedule_tor_access("change_status_to_$status_key", $worker_node, $tor_node, $tor_type_tid)) {
				unset($tor_status_options[$status_key]);
			}
		} else {
			if (!sirius_worker_schedule_tor_access("add", $worker_node, NULL, $tor_type_tid, $status_key)) {
				unset($tor_status_options[$status_key]);
			}
		}
	}
	if (count($tor_status_options) < 1) { 
		drupal_set_message("You don't have access to any statuses.", 'error');
		return array();
	}
	if (count($tor_status_options) == 1) {
		$form['tor']['status'] = array(
			'#type' => 'hidden',
			'#value' => array_shift(array_keys($tor_status_options)),
		);
	} else {
		$form['tor']['status'] = array(
			'#title' => t('Status'),
			'#type' => 'select',
			'#options' => $tor_status_options,
			'#required' => TRUE,
			'#default_value' => $tor_node->field_sirius_type['und'][0]['value'],
		);
	}

	$form['tor']['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start Date:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#date_format' => 'm/d/Y',
    '#attributes' => array('autocomplete' => 'off'),
  );
	if ($tor_json['tor']['start_date']) { $form['tor']['start_date']['#default_value'] = $tor_json['tor']['start_date']; }

	$form['tor']['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End Date:'),
    /*
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    */
    '#date_format' => 'm/d/Y',
    '#attributes' => array('autocomplete' => 'off'),
  );
	if ($tor_json['tor']['end_date']) { $form['tor']['end_date']['#default_value'] = $tor_json['tor']['end_date']; }

	$form['tor']['summary'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for Absence'),
		'#required' => TRUE,
	);
	$default_summary = sirius_fieldval($tor_node, 'field_sirius_summary');
	if ($default_summary) { $form['tor']['summary']['#default_value'] = $default_summary; }

	if (sirius_worker_schedule_tor_access('approve', $worker_node, $tor_node)) {
		$form['tor']['comment'] = array(
			'#title' => t('Admin Comments'),
			'#type' => 'textarea',
			'#default_value' => $tor_json['tor']['comment'],
		);
	} else {
		$form['tor']['comment'] = array(
			'#type' => 'value',
			'#value' => $tor_json['tor']['comment'],
		);
	}

	if ($tor_node) {
		$renderable = field_view_field('node', $tor_node, 'field_sirius_attachments', 'full');
		if ($renderable) {
			$renderable['#title'] = 'Current File';
			$form["attach_current"] = array('#markup' => '<div><br />' . drupal_render($renderable) . '</div>');
		}
	}

	$form["attach"] = array(
		'#type' => 'file',
		'#title' => t('Attachment'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_worker_schedule_tor_addedit_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("attach", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["attach"] = $file;
    }
    else {
      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
    }
	}

	if ($form_state['values']['tor']['start_date'] && $form_state['values']['tor']['end_date']) {
		if (strtotime($form_state['values']['tor']['start_date']) > strtotime($form_state['values']['tor']['end_date'])) {
			form_set_error('tor][start_date', t('Start date must be before end date.'));
		}
	}

	/*
	if (!$form_state['tor_nid']) {
		if (strtotime($form_state['values']['tor']['start_date']) < time() - 60*60*24) {
			form_set_error('tor][start_date', t('Start date must be in the future.'));
		}
	}
	*/
}

function sirius_worker_schedule_tor_addedit_form_submit($form, &$form_state) {
	$files = array();
  $file = $form_state['storage']["attach"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach"]);
		unset($file['destination']);
		unset($file['uri']);
		$files[] = $file;
  }

  $tor_type_tid = $form_state['values']['tor']['type_tid'];
  $tor_type_term = taxonomy_term_load($tor_type_tid);
  $tor_type_json = sirius_json_get($tor_type_term);

	$worker_node = node_load($form_state['worker_nid']);
	if (!$worker_node) { $worker_node = sirius_log_handler($tor_node, 'sirius_worker'); }
	$status = $form_state['values']['tor']['status'];
	$status_options = sirius_worker_schedule_tor_status_options();
	$tor_node = node_load($form_state['tor_nid']);
	$title = $worker_node->title . ': ' . 
		$tor_type_term->name . ': ' . 
		$form_state['values']['tor']['start_date'];


	if ($tor_node) {
		$log_type = 'update';
		$tor_json = sirius_json_get($tor_node);
		$old_status = $tor_json['tor']['status'];
		$tor_json['tor'] = $form_state['values']['tor'];
		sirius_jsonfield_util_node_json_set($tor_node, $tor_json);
		$tor_node->field_sirius_summary['und'][0]['value'] = $form_state['values']['tor']['summary'];
		$tor_node->field_sirius_type['und'][0]['value'] = $status;
		if ($files) { $tor_node->field_sirius_attachments['und'] = $files; }
		$tor_node->title = $title;
		$log_msg = "Time-off record edited. ";
		drupal_set_message("Time-off record edited. ");

		if ($old_status && $old_status != $status) {
			$old_status_name = $status_options[$old_status];
			$new_status_name = $status_options[$status];

			$log_msg .= "Status changed from $old_status_name to $new_status_name. ";

			$body = "The time-off record below has been changed from $old_status_name to $new_status_name.<br /><hr>";
			$body .= sirius_worker_schedule_tor_email_body($worker_node, $tor_node);
			$sms = drupal_html_to_text($body);
			$subject = "Time-off record status change: $new_status_name";

			$result = sirius_worker_notify(
				$worker_node, 
				array(
					'subject' => $subject, 
					'body' => $body,
					'sms' => $sms,
				)
			);

			if ($result['success']) {
				$log_msg .= "Worker notification sent. ";
			} else {
				$log_msg .= "Worker notification failed: " . $result['msg'] . " ";
			}
		}

		node_save($tor_node);
		sirius_log('tor_update', 'update', $log_msg, $log_msg, NULL, array($worker_node->nid, $tor_node->nid), NULL, NULL);
	} else {
		$is_new = TRUE;
		$tor_json = array();
		$tor_json['tor'] = $form_state['values']['tor'];
		$summary = $form_state['values']['tor']['summary'];
		$category = 'tor';
		$handlers = array($worker_node->nid);
		if ($form_state['values']['tor']['dispatch_nid']) {
			$handlers[] = $form_state['values']['tor']['dispatch_nid'];
		}
		$tor_node = sirius_log($category, $status, $summary, $summary, NULL, $handlers, $files, $tor_json);
		$tor_node->title = $title;
		node_save($tor_node); 
		drupal_set_message(t('Time-off record added.'));

		// Get the notification emails from the member statuses
		$notify_emails = array();
		$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
		$ms_terms = taxonomy_term_load_multiple($ms_tids);
		$notify_emails = array();
		foreach ($ms_terms as $ms_tid => $ms_term) {
			$tor_json = sirius_jsonfield_util_term_json_get($ms_term);
			if ($tor_json['tor']['notify']) {
				foreach ($tor_json['tor']['notify'] as $email) {
					if ($email) {$notify_emails[$email] = TRUE; }
				}
			}
		}

		// Also add the notification emails from the job
		$dispatch_nid = $form_state['values']['tor']['dispatch_nid'];
		if ($dispatch_nid) {
			$dispatch_node = node_load($dispatch_nid);
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$job_emails = sirius_fieldvals($job_node, 'field_sirius_emails', 'value');
			foreach ($job_emails as $job_email) {
				if ($job_email) { $notify_emails[$job_email] = TRUE; }
			}
		}

		$subject = "Time-off record submitted by " . $worker_node->title;
		$body = "A new time-off record has been submitted<br /><hr>";
		$body .= sirius_worker_schedule_tor_email_body($worker_node, $tor_node);
		foreach (array_keys($notify_emails) as $notify_email) {
			$result = sirius_email_send(
				array(
					'to' => $notify_email,
					'subject' => $subject,
					'body' => $body,
					'handler_nids' => array($tor_node->nid),
				)
			);

			if ($result['success']) {
				drupal_set_message("Employer notification sent to $notify_email.");
			} else {
				drupal_set_message("Employer notification to $notify_email failed: " . $result['msg'], 'warning');
			}
		}

		drupal_goto('node/' . $tor_node->nid . '/sirius_worker_schedule_tor');
	}

}

function sirius_worker_schedule_tor_email_body($worker_node, $tor_node) {
	$tor_json = sirius_json_get($tor_node);

	$dispatch_nid = $tor_json['tor']['dispatch_nid'];
	if ($dispatch_nid) { 
		$dispatch_node = node_load($dispatch_nid);
		$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
		$job_node = node_load($job_nid);
		$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
		$employer_node = node_load($employer_nid);
		$html .= "<strong>Employer:</strong> " . $employer_node->title . '<br />';
		$html .= "<strong>Job:</strong> " . $job_node->title . '<br />';
		$html .= "<strong>Dispatch:</strong> " . $dispatch_node->title . '<br />';
	}

	$status_options = sirius_worker_schedule_tor_status_options();
	$html .= "<strong>Start Date:</strong> " . $tor_json['tor']['start_date'] . '<br />';
	$html .= "<strong>End Date:</strong> " . $tor_json['tor']['end_date'] . '<br />';
	$html .= "<strong>Reason for Absence:</strong> " . sirius_text_to_html(sirius_fieldval($tor_node, 'field_sirius_summary')) . '<br />';
	$html .= "<strong>Status:</strong> " . $status_options[$tor_json['tor']['status']] . '<br />';
	$comment = sirius_text_to_html($tor_json['tor']['comment']);
	if ($comment) { $html .= "<strong>Admin Comments:</strong> " . $comment . '<br />'; }

	global $base_url;
	$view_url = $base_url . '/node/' . $worker_node->nid . '/sirius_worker_schedule/tor/view/' . $tor_node->nid;
	$edit_url = $base_url . '/node/' . $worker_node->nid . '/sirius_worker_schedule/tor/edit/' . $tor_node->nid;
	$html .= "<strong>Link:</strong> <a href=\"$view_url\">View</a> / <a href=\"$edit_url\">Edit</a><br />";
	return $html;
}


function sirius_worker_schedule_tor_access($action, $worker_node = NULL, $tor_node = NULL, $tor_type_tid = NULL, $status_key = NULL) {
	// Admins skip the access check, can do anything.
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	if (user_access('sirius worker admin')) { return TRUE; }

	// Pre-load all the tor type terms. 
 	$lookup = &drupal_static(__FUNCTION__);
  if (!$lookup['tor_type_terms']) {
  	$lookup['tor_type_terms'] = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'full');
  }

  // Figure out the type and settings for the type. It might come from the tor node (i.e. "edit") or it might be passed in (i.e. "create")
  $tor_json = sirius_json_get($tor_node);
  if (!$tor_type_tid) { $tor_type_tid = $tor_json['tor']['type_tid']; }
  $tor_type_term = $lookup['tor_type_terms'][$tor_type_tid];
  $tor_type_json = sirius_json_get($tor_type_term);

  // Figure out the worker node. Might be passed in explicitly. We always need view access to the underlying worker node.
	if (!$worker_node) { $worker_node = sirius_log_handler($tor_node, 'sirius_worker'); }
	if (!$worker_node) { return FALSE; }
	if (!node_access('view', $worker_node)) { return FALSE; }

	if (!$status_key) { $status_key = $tor_node->field_sirius_type['und'][0]['value']; }

  if ($action == 'list') {
	  // In order to list, there:
	  // -- Must be a worker node
	  // -- The user must be able to see it
  	return TRUE;
  }

  if ($action == 'add') {
  	if ($status_key) {
  		// Check add for a specific status
			$grants = $tor_type_json['tor']['status'][$status_key]['permission'][$action];
			return sirius_worker_schedule_tor_grant_check($grants);
  	} else {
  		// Check add for *any* status
			$status_options = sirius_worker_schedule_tor_status_options();
			foreach ($status_options as $status_key => $status_name) {
				$grants = $tor_type_json['tor']['status'][$status_key]['permission'][$action];
				if (sirius_worker_schedule_tor_grant_check($grants)) { return TRUE; }
			}
			return FALSE;
		}
  }

  // All the other actions require a TOR node, a worker node, a type, and a status
	if (!$tor_node) { return FALSE; }
	if (!$tor_type_term) { return FALSE; }
	if (!$status_key) { return FALSE; }

	// Check the explicit permission grants.
	$grants = $tor_type_json['tor']['status'][$status_key]['permission'][$action];
	return sirius_worker_schedule_tor_grant_check($grants);
}

function sirius_worker_schedule_tor_grant_check($grants) {
	if (!$grants) { return FALSE; }
	$my_worker = sirius_worker_get();
	if ($my_worker) {
		$my_ms_tids = sirius_fieldvals($my_worker, 'field_sirius_member_status', 'tid');
		foreach ($my_ms_tids as $my_ms_tid) {
			if ($grants["ms_tid:$my_ms_tid"]) { return TRUE; }
		}
	}
	if ($grants['employer'] && user_access("sirius dispatch employer")) {
		$my_employer = sirius_employer_get();
		if ($my_employer == $worker_node->field_grievance_shop['und'][0]['target_id']) { return TRUE; }
	}

	return FALSE;
}


function sirius_worker_schedule_tor_download_access($tor_node) {
	return sirius_worker_schedule_tor_access('view', NULL, $tor_node);
}

function sirius_schedule_worker_tor_type($tor_node) {
	$tor_json = sirius_json_get($tor_node);
	$tor_type_tid = $tor_json['tor']['type_tid'];
	return taxonomy_term_load($tor_type_tid);
}

function sirius_worker_schedule_tor_status($tor_node) {
	$tor_json = sirius_json_get($tor_node);
	$tor_status_uuid = $tor_json['tor']['status'];

	$tor_type_term = sirius_schedule_worker_tor_type($tor_node);
	$tor_type_json = sirius_json_get($tor_type_term);

	return $tor_type_json['tor']['statuses'][$tor_status_uuid];
}