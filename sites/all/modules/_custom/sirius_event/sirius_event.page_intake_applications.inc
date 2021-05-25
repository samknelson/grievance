<?php

function sirius_node_tab_event_intake_applications_access($event_node) {
	// If it's disabled at the event type level, not enabled
	if (!sirius_event_intake_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Admin participant roles can access
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['intake']['admin_prole_tids']);
	if (in_array($prole_tid, $admin_prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_intake_applications($event_node) {
	$domain_nid = sirius_domain_nid();
	$action = sirius_node_tab_arg(0);
	if ($action == 'add') { return drupal_get_form('sirius_node_tab_event_intake_applications_add_form', $event_node); }
	return drupal_get_form('sirius_node_tab_event_intake_applications_list_form', $event_node);
}

function sirius_node_tab_event_intake_applications_list_form($form, &$form_state, $event_node) {
	$form['#tree'] = TRUE;

	$intake_status_options = sirius_event_intake_application_status_options();
	$form['filters'] = array(
		'#title' => t('Filters'),
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['filters']['name'] = array(
		'#title' => t('Name'),
		'#type' => 'textfield',
		'#description' => t('Enter all or part of the participant\'s first or last name.'),
	);

	$form['filters']['status'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Status'),
		'#options' => $intake_status_options,
	);

	$assignment_options = sirius_event_intake_application_assignment_options($event_node);
	if ($assignment_options) {
		$form['filters']['assignment_name'] = array(
			'#type' => 'select',
			'#title' => t('Assignment'),
			'#options' => array(NULL => t('-- All Assignments --')) + $assignment_options,
		);
		if ($_GET['assignment_name']) { $form['filters']['assignment_name']['#default_value'] = $_GET['assignment_name']; }
	}

	$display_field_options = array();
	$display_field_options['status'] = t('Status');
	$display_field_options['date'] = t('Date Submitted');
	$display_field_options['name'] = t('Full Name');
	$display_field_options['name_first'] = t('First Name');
	$display_field_options['name_last'] = t('Last Name');
	$display_field_options['ssn'] = t('SSN');
	$display_field_options['dob'] = t('Date of Birth');
	$display_field_options['phone'] = t('Phone');
	$display_field_options['email'] = t('Email');
	$display_field_options['address'] = t('Address');
	if ($assignment_options) { $display_field_options['assignment'] = t('Assignment'); }

	$display_fields = array();
	if ($form_state['values']['filters']['display_fields']) { $display_fields = array_filter(array_values($form_state['values']['filters']['display_fields'])); }
	if (!$display_fields) { 
		$display_fields = array();
		$display_fields[] = 'status';
		$display_fields[] = 'date';
		$display_fields[] = 'name';
		$display_fields[] = 'phone';
		$display_fields[] = 'email';
		if ($assignment_options) { $display_fields[] = 'assignment'; }
	}

	$form['filters']['display_fields'] = array(
		'#title' => 'Display Fields',
		'#type' => 'checkboxes',
		'#options' => $display_field_options,
		'#default_value' => $display_fields,
	);

	$form['filters']['output'] = array(
		'#title' => 'Output',
		'#type' => 'select',
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('CSV Export'),
		),
		'#default_value' => 'screen',
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Filter'),
	);


  $domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->fieldCondition('field_sirius_category', 'value', 'event:intake_application');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $event_node->nid);
  $status_filter = array();
  if ($form_state['values']['filters']['status']) { 
  	$status_filter = array_filter(array_values($form_state['values']['filters']['status']));
  }
  if ($status_filter) {
	  $query->fieldCondition('field_sirius_type', 'value', $status_filter, 'in');
  }
  $query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();

  if ($stmt['node']) {
  	$application_nodes = node_load_multiple(array_keys($stmt['node']));
		$is_csv = ($form_state['values']['filters']['output'] == 'csv'); 
		$rows = array();

		foreach ($application_nodes as $application_nid => $application_node) {
			$worker_node = sirius_log_handler($application_node, 'sirius_worker');
			$participant_node = sirius_log_handler($application_node, 'sirius_event_participant');

			$application_json = sirius_jsonfield_util_node_json_get($application_node);
			$submitted_ts = $application_json['wizard']['intake']['submitted_ts'];
			if ($submitted_ts) {
				$datestr = date('l, F d Y - g:ia', $submitted_ts);
			} else {
				$datestr = '';
			}

			$assignment = sirius_event_intake_application_assignment_get($application_node);

			$name = $application_json['wizard']['intake']['identity']['name_first'] . ' ' . $application_json['wizard']['intake']['identity']['name_last'];
			$filter_name = $form_state['values']['filters']['name'];
			if ($filter_name) {
				$filter_name = preg_replace('/[^a-zA-Z0-9_]/', '', $filter_name);
				$filter_name = strtolower($filter_name);
				$filter_name = trim($filter_name);
				if (!preg_match('/' . $filter_name . '/i', $name)) { continue; }
			}

			$filter_assignment_name = $form_state['values']['filters']['assignment_name'];
			if (!isset($filter_assignment_name)) { $filter_assignment_name = $_GET['assignment_name']; }
			if ($filter_assignment_name) {
				if ($filter_assignment_name != $assignment['name']) { continue; }
			}

			$status = $application_node->field_sirius_type['und'][0]['value'];

			$tools = '';
			$tools .= l('View', "node/$application_nid/sirius_event_intake_application_wizard", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Manage', "node/$application_nid/sirius_event_intake_application_manage", array('attributes' => array('class' => array('tool-button'))));
			if ($worker_node) { $tools .= l('Worker', "node/" . $worker_node->nid, array('attributes' => array('class' => array('tool-button')))); }
			if ($participant_node) { $tools .= l('Participant', "node/" . $participant_node->nid . '/sirius_event_participant_comments', array('attributes' => array('class' => array('tool-button')))); }

			if ($is_csv) {
				$phone_render = $application_json['wizard']['intake']['phone'];
				$email_render = $application_json['wizard']['intake']['email'];
				$addr_render = sirius_addr_format_oneline($application_json['wizard']['intake']['addr']);
			} else {
				$phone_render = sirius_phone_render($application_json['wizard']['intake']['phone']);
				$email_render = sirius_email_render($application_json['wizard']['intake']['email']);
				$addr_render = sirius_addr_format_html($application_json['wizard']['intake']['addr']);
			}

			$row = array();
			if (in_array('status', $display_fields)) { $row[] = $intake_status_options[$status]; }
			if (in_array('date', $display_fields)) { $row[] = $datestr; }
			if (in_array('name', $display_fields)) { $row[] = $name; }
			if (in_array('name_first', $display_fields)) { $row[] = $application_json['wizard']['intake']['identity']['name_first']; }
			if (in_array('name_last', $display_fields)) { $row[] = $application_json['wizard']['intake']['identity']['name_last']; }
			if (in_array('ssn', $display_fields)) { $row[] = $application_json['wizard']['intake']['identity']['ssn'];  } 
			if (in_array('dob', $display_fields)) { $row[] = $application_json['wizard']['intake']['identity']['dob'];  } 
			if (in_array('phone', $display_fields)) { $row[] = $phone_render; }
			if (in_array('email', $display_fields)) { $row[] = $email_render;  }
			if (in_array('address', $display_fields)) { $row[] = $addr_render; }
			if (in_array('assignment', $display_fields)) { $row[] = $assignment['name']; } 
			if (!$is_csv) { $row[] = $tools; }
			$rows[] = $row;
		}

		$header = array();
		foreach ($display_fields as $display_field) {
			$header[] = $display_field_options[$display_field];
		}
		if (!$is_csv) { $header[] = 'Tools'; }

		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_materials'))));
  }

	$html .= l('Add', current_path() . '/add', array('attributes' => array('class' => array('tool-button'))));


	if ($is_csv) {
		$fp = fopen('php://output', 'w');
		$filename = $event_node->title . '_applications';
		$filename = strtolower($filename);
		$filename = preg_replace('/[^a-z0-9_]/', '_', $filename);
		$filename = preg_replace('/_+/', '_', $filename);
		$filename .= '.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, $header);
		foreach ($rows as $row) { fputcsv($fp, $row); }
		fclose($fp); 
		drupal_exit();
	}

	$form['results'] = array('#markup' => $html);

	return $form;
}

function sirius_node_tab_event_intake_applications_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}

function sirius_node_tab_event_intake_applications_add_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Create a New Application'),
	);

	return $form;
}

function sirius_node_tab_event_intake_applications_add_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);

	$application_json = array();
	$application_json['esignature'] = $event_json['intake']['esignature'];
	$application_node = sirius_log('event:intake_application', 'draft', 'New', 'New', NULL, array($event_nid), array(), $application_json);
	sirius_event_intake_set_title($application_node);

	drupal_set_message(t('Application added.'));
	drupal_goto('node/' . $application_node->nid);
}
