<?php

/*** 
 * tos = "Time-Off Sick".
 */

function sirius_worker_page_schedule_tos_access() {
	// Admins can access
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	if (user_access('sirius worker admin')) { return TRUE; }

	// Employers can access
	$my_employer = sirius_employer_get();
	if (user_access('sirius dispatch employer') && $my_employer) { return TRUE; }

	return FALSE;
}

function sirius_worker_page_schedule_tos() {
	return drupal_get_form('sirius_worker_page_schedule_tos_form');
}

function sirius_worker_page_schedule_tos_form($form, &$form_state) {
	$form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => sirius_worker_schedule_tos_status_options(),
		'#default_value' => 'requested',
	);

	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius worker admin')) { 
		$employer_options = array(NULL => t('-- Select --')) + sirius_employer_list();
		$form['employer_nid'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => $employer_options,
		);
	} else {
		$my_employer = sirius_employer_get();
		$form['employer_nid'] = array(
			'#type' => 'hidden',
			'#value' => $my_employer->nid,
		);
	}

	$form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Submitted after:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array('autocomplete' => 'off'),
  );

	$form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Submitted before:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array('autocomplete' => 'off'),
  );


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']); 
	}

	return $form;
}

function sirius_worker_page_schedule_tos_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUe;
	$params = array();
	$params['employer_nid'] = $form_state['values']['employer_nid'];
	$params['status'] = $form_state['values']['status'];
	$params['start_date'] = $form_state['values']['start_date'];
	$params['end_date'] = $form_state['values']['end_date'];
	$toss = sirius_worker_schedule_tos_list_all($params);

	$rows = array();
	foreach ($toss as $tos_nid => $tos_node) {
		$handler_nid = $tos_node->field_sirius_log_handler['und'][0]['target_id'];
		$sql = "select title from node where nid = :handler_nid";
		$stmt = db_query($sql, array(':handler_nid' => $handler_nid)); 
		$hr = $stmt->fetchAssoc();
		$handler_title = $hr['title'];

		$tools = '';
		/*
		if (sirius_worker_schedule_tos_access('view', $entity, $tos_node)) {
			$tools .= l('View', 'node/' . $handler_nid . '/sirius_worker_schedule/tos/view/' . $tos_nid, array('attributes' => array('class' => array('tool-button'))));
		}
		*/
		if (sirius_worker_schedule_tos_access('edit', $entity, $tos_node)) {
			$tools .= l('Edit', 'node/' . $handler_nid . '/sirius_worker_schedule/tos/edit/' . $tos_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		if (sirius_worker_schedule_tos_access('delete', $entity, $tos_node)) {
			$tools .= l('Delete', 'node/' . $handler_nid . '/sirius_worker_schedule/tos/delete/' . $tos_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		if (drupal_valid_path("node/" . $handler_nid)) {
			$tools .= l('Worker', 'node/' . $handler_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}

		$json = sirius_jsonfield_util_node_json_get($tos_node);
		$status_options = sirius_worker_schedule_tos_status_options();

		$row = array();
		$row[] = $handler_title;
		$row[] = $json['tos']['start_date'];
		$row[] = $json['tos']['end_date'];
		$row[] = sirius_text_to_html(sirius_fieldval($tos_node, 'field_sirius_summary'));
		$row[] = $status_options[$json['tos']['status']];
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($toss) {
		$header = array('Worker', 'Start', 'End', 'Description', 'Status', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_worker_schedule_toss'))));
	} else {
		$html .= '<p>No time-off requests found.</p>';
	}

	$html = '<hr><h2>' . t('Results') . '</h2>' . $html;
	$form_state['results'] = $html;
}

function sirius_worker_schedule_tos_list_all($params) {
	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql = "select  distinct node.nid from node ";
	// $sql = "select job_node.* from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";

	$sql .= "left join node dispatch_node on dispatch_node.type = 'sirius_dispatch' and dispatch_node.nid = field_sirius_log_handler_target_id ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";

	$sql .= "left join node job_node on job_node.type = 'sirius_dispatch_job' and job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";

	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'tos' ";
	if ($params['status']) {
		$sql .= "and field_sirius_type_value = :type ";
		$sql_args[':type'] = $params['status']; 
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
	$sql .= "order by node.created desc ";
	$sql .= "limit 500";
	$stmt = sirius_sql_query($sql, $sql_args);
	$tos_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$tos_nids[] = $hr['nid'];
	}
	return node_load_multiple($tos_nids);
}


function sirius_worker_schedule_tos_download_access($file_item, $entity_type, $tos_node) {
	if ($entity_type != 'node') { return FALSE; }

	$node = node_load($tos_node->field_sirius_log_handler['und'][0]['target_id']);

	if ($node->type == 'sirius_taxonomy_proxy') { $entity = taxonomy_term_load($node->field_sirius_term_proxy['und'][0]['target_id']); } else { $entity = $node; }

	return sirius_worker_schedule_tos_access('view', $entity, $tos_node);
}

function sirius_worker_schedule_tos_list($entity) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'tos');
	$query->fieldCondition('field_sirius_log_handler', 'target_id', $node->nid);
  $query->propertyOrderBy('created', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }

  $all_nodes = node_load_multiple(array_keys($stmt['node']));
  $good_nodes = array();
  foreach ($all_nodes as $nid => $node) {
  	if (!sirius_worker_schedule_tos_access('view', $entity, $node)) { continue; }
  	$good_nodes[$nid] = $node;
  }
  return $good_nodes;
}

function sirius_worker_schedule_tos_access($action, $entity, $tos_node = NULL) {
	global $user; 

	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	// Verify that this tos belongs to this node or entity
	if ($tos_node) { 
		$good = FALSE; 
		if ($tos_node->field_sirius_log_handler['und'][0]['target_id'] == $node->nid) { $good = TRUE; }
		if (!$good) {
			$real_parent = node_load($tos_node->field_sirius_log_handler['und'][0]['target_id']);
			if ($real_parent->nid != $node->nid) { 
				return sirius_worker_schedule_tos_access($action, $real_parent, $tos_node); 
			}
		}
		if (!$good) { return FALSE; }
	}

	// Admins can do anything
	if (user_access('sirius admin')) { return TRUE; }

	if ($node->type == 'sirius_worker') {
		// Admins can do anything
		if (user_access('sirius domain admin')) { return TRUE; }
		if (user_access('sirius worker admin')) { return TRUE; }

		// Must have permission to view the underlying worker node, or be an employer
		if (!user_access('sirius dispatch employer') && !node_access('view', $node)) { return FALSE; }

		// Anybody can list
		if ($action == 'list') { return TRUE; }

		$my_worker = sirius_worker_get();
		$my_employer = sirius_employer_get();

		// Workers can view
		if ($action == 'view' && ($my_worker->nid == $node->nid)) { return TRUE; }

		// Workers can add, if they have one of the approved member statuses
		if ($action == 'add' && ($my_worker->nid == $node->nid)) { 
			$ms_tids = sirius_fieldvals($node, 'field_sirius_member_status', 'tid');
			$ms_terms = taxonomy_term_load_multiple($ms_tids);
			foreach ($ms_terms as $ms_tid => $ms_term) {
				$json = sirius_jsonfield_util_term_json_get($ms_term);
				if ($json['tos']['worker_can_submit'] == 'Yes') { return TRUE; }
			}
		}

		// Employers can view, edit, or approve, but only their own
		if (($action == 'view' || $action == 'edit' || $action == 'approve') && user_access('sirius dispatch employer') && $my_employer) {
			$json = sirius_jsonfield_util_node_json_get($tos_node);
			$dispatch_nid = $json['tos']['dispatch_nid'];
			$dispatch_node = node_load($dispatch_nid);
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			if ($my_employer->nid == $job_node->field_grievance_shop['und'][0]['target_id']) { return TRUE; }
		}

		return FALSE;
	}

	return FALSE;
}

function sirius_worker_node_tab_schedule_tos_access($worker_node) {
	return sirius_worker_schedule_tos_access('list', $worker_node);
}

function sirius_worker_node_tab_schedule_tos($entity) {
	$action = sirius_node_tab_arg(0);
	if (!$action) { $action = 'list'; }
	$tos_nid = sirius_node_tab_arg(1);
	if ($tos_nid) { $tos_node = node_load($tos_nid); }

	$access = sirius_worker_schedule_tos_access($action, $entity, $tos_node);
	if (!$access) { return 'Access denied.'; }

	if ($action == 'add') {
		return drupal_get_form('sirius_worker_node_tab_schedule_tos_addedit_form', $entity);
	} else if ($action == 'view') {
		return sirius_worker_node_tab_schedule_tos_view($entity, $tos_node);
	} else if ($action == 'edit') {
		return drupal_get_form('sirius_worker_node_tab_schedule_tos_addedit_form', $entity, $tos_node);
	} else if ($action == 'delete') {
		return drupal_get_form('sirius_worker_node_tab_schedule_tos_delete_form', $entity, $tos_node);
	} else {
		return sirius_worker_node_tab_schedule_tos_list($entity);
	}
}

function sirius_worker_node_tab_schedule_tos_list($entity, $options = array()) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	$toss = sirius_worker_schedule_tos_list($entity);
	if (!$toss && $options['hide_empty']) { return ''; }

	$rows = array();
	foreach ($toss as $tos_nid => $tos_node) {

		$tools = '';
		if (sirius_worker_schedule_tos_access('view', $entity, $tos_node)) {
			$tools .= l('View', current_path() . '/view/' . $tos_nid, array('attributes' => array('class' => array('tool-button'))));
		}
		if (sirius_worker_schedule_tos_access('edit', $entity, $tos_node)) {
			$tools .= l('Edit', current_path() . '/edit/' . $tos_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		if (sirius_worker_schedule_tos_access('delete', $entity, $tos_node)) {
			$tools .= l('Delete', current_path() . '/delete/' . $tos_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}

		if (sirius_worker_schedule_tos_access('view', $entity, $tos_node)) {
			$renderable = field_view_field('node', $tos_node, 'field_sirius_attachments', 'full');
			if ($renderable) {
				$renderable['#label_display'] = 'hidden';
				$attachment_render = render($renderable);
			} else {
				$attachment_render = '';
			}
		} else {
			$attachment_render = '';
		}

		$json = sirius_jsonfield_util_node_json_get($tos_node);
		$status_options = sirius_worker_schedule_tos_status_options();

		$row = array();
		$row[] = $json['tos']['start_date'];
		$row[] = $json['tos']['end_date'];
		$row[] = sirius_text_to_html(sirius_fieldval($tos_node, 'field_sirius_summary'));
		$row[] = $status_options[$json['tos']['status']];
		$row[] = $attachment_render;	
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($toss) {
		$header = array('Start', 'End', 'Description', 'Status', 'Attachment', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_worker_schedule_toss'))));
	} else {
		$html .= '<p>No time-off requests found.</p>';
	}

	if (!$options['hide_add']) {
		if (sirius_worker_schedule_tos_access('add', $entity)) {
			$html .= l('Add a Time-Off Request', current_path() . '/add', array('attributes' => array('class' => 'tool-button'), 'query' => array('destination' => current_path())));
		}
	}

	return $html;
}

function sirius_worker_node_tab_schedule_tos_view($entity, $tos_node) {
	$json = sirius_jsonfield_util_node_json_get($tos_node);
	$html = '<h2>' . $json['tos']['title'] . '</h2>';

	$dispatch_nid = $json['tos']['dispatch_nid'];
	if ($dispatch_nid) { 
		$dispatch_node = node_load($dispatch_nid);
		if (drupal_valid_path("node/$dispatch_nid/sirius_dispatch_worker_view")) {
			$html .= sirius_fakerender_field('Dispatch', l($dispatch_node->title, "node/$dispatch_nid/sirius_dispatch_worker_view"));
		} else {
			$html .= sirius_fakerender_field('Dispatch', $dispatch_node->title);
		}

		$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
		$job_node = node_load($job_nid);
		if (drupal_valid_path("node/$job_nid")) {
			$html .= sirius_fakerender_field('Job', l($job_node->title, "node/$job_nid"));
		} else {
			$html .= sirius_fakerender_field('Job', $job_node->title);
		}

		$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
		$employer_node = node_load($employer_nid);
		if (drupal_valid_path("node/$employer_nid")) {
			$html .= sirius_fakerender_field('Employer', l($employer_node->title, "node/$employer_nid"));
		} else {
			$html .= sirius_fakerender_field('Employer', $employer_node->title);
		}
	}

	$status_options = sirius_worker_schedule_tos_status_options();
	$html .= sirius_fakerender_field('Start Date', $json['tos']['start_date']);
	$html .= sirius_fakerender_field('End Date', $json['tos']['end_date']);
	$html .= sirius_fakerender_field('Reason for Absence', sirius_text_to_html(sirius_fieldval($tos_node, 'field_sirius_summary')));
	$html .= sirius_fakerender_field('Status', $status_options[$json['tos']['status']]);
	$comment = sirius_text_to_html($json['tos']['comment']);
	if ($comment) { $html .= sirius_fakerender_field('Comment', $comment); }
	$html .= drupal_render(field_view_field('node', $tos_node, 'field_sirius_attachments', 'full'));

	return $html;
}

function sirius_worker_node_tab_schedule_tos_view_email($entity, $tos_node) {
	$json = sirius_jsonfield_util_node_json_get($tos_node);

	$dispatch_nid = $json['tos']['dispatch_nid'];
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

	$status_options = sirius_worker_schedule_tos_status_options();
	$html .= "<strong>Start Date:</strong> " . $json['tos']['start_date'] . '<br />';
	$html .= "<strong>End Date:</strong> " . $json['tos']['end_date'] . '<br />';
	$html .= "<strong>Reason for Absence:</strong> " . sirius_text_to_html(sirius_fieldval($tos_node, 'field_sirius_summary')) . '<br />';
	$html .= "<strong>Status:</strong> " . $status_options[$json['tos']['status']] . '<br />';
	$comment = sirius_text_to_html($json['tos']['comment']);
	if ($comment) { $html .= "<strong>Comment:</strong> " . $comment . '<br />'; }

	global $base_url;
	$view_url = $base_url . '/node/' . $entity->nid . '/sirius_worker_schedule/tos/view/' . $tos_node->nid;
	$edit_url = $base_url . '/node/' . $entity->nid . '/sirius_worker_schedule/tos/edit/' . $tos_node->nid;
	$html .= "<strong>Link:</strong> <a href=\"$view_url\">View</a> / <a href=\"$edit_url\">Edit</a><br />";
	return $html;
}

function sirius_worker_schedule_tos_status_options() {
	return array(
		'requested' => t('Requested'),
		'approved' => t('Approved'),
		'declined' => t('Declined'),
	);
}

function sirius_worker_node_tab_schedule_tos_addedit_form($form, &$form_state, $entity, $tos_node = NULL) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	if ($tos_node) { $form_state['tos_nid'] = $tos_node->nid; }

	$json = sirius_jsonfield_util_node_json_get($tos_node);

	$form_state['nid'] = $node->nid;

	$form['#tree'] = TRUE;

	$dispatch_nid = NULL;
	if ($tos_node) {
		$dispatch_nid = $json['tos']['dispatch_nid'];
		$dispatch_node = node_load($dispatch_nid);
	} else {
		$dispatch_node = sirius_dispatch_get_current($node->nid);
		$dispatch_nid = $dispatch_node->nid;
		if ($dispatch_node) {
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
			$form['tos']['dispatch_warning'] = array(
				'#markup' => 
				'<div class="sirius_form_intro">' . 
				t('This request will be forwarded to your employer, @employer_name. If there are confidential materials which you do not want the employer to see, do not include them in this request.', array('@employer_name' => $employer_node->title)) . 
				'</div>',
			);
		}
	}
	$form['tos']['dispatch_nid'] = array('#type' => 'hidden', '#value' => $dispatch_nid);

	$form['tos']['start_date'] = array(
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
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
  );
	if ($json['tos']['start_date']) { $form['tos']['start_date']['#default_value'] = $json['tos']['start_date']; }

	$form['tos']['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End Date:'),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
  );
	if ($json['tos']['end_date']) { $form['tos']['end_date']['#default_value'] = $json['tos']['end_date']; }

	$form['tos']['summary'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for Absence'),
		'#required' => TRUE,
	);
	$default_summary = sirius_fieldval($tos_node, 'field_sirius_summary');
	if ($default_summary) { $form['tos']['summary']['#default_value'] = $default_summary; }

	if (sirius_worker_schedule_tos_access('approve', $entity, $tos_node)) {
		$form['tos']['status'] = array(
			'#title' => t('Status'),
			'#type' => 'select',
			'#options' => sirius_worker_schedule_tos_status_options(),
			'#requred' => TRUE,
		);

		$form['tos']['comment'] = array(
			'#title' => t('Comment'),
			'#type' => 'textarea',
		);
		if ($json['tos']['comment']) { $form['tos']['comment']['#default_value'] = $json['tos']['comment']; }

	} else {
		$form['tos']['status'] = array(
			'#type' => 'hidden',
		);
	}
	$default_status = $json['tos']['status'];
	if (!$default_status) { $default_status = 'requested'; }
	$form['tos']['status']['#default_value'] = $default_status;

	if ($tos_node) {
		$renderable = field_view_field('node', $tos_node, 'field_sirius_attachments', 'full');
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

function sirius_worker_node_tab_schedule_tos_addedit_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("attach", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['stosage']["attach"] = $file;
    }
    else {
      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
    }
	}

	if (strtotime($form_state['values']['tos']['start_date']) >= strtotime($form_state['values']['tos']['end_date'])) {
		form_set_error('tos][start_date', t('Start date must be before end date.'));
	}

	if (!$form_state['tos_nid']) {
		if (strtotime($form_state['values']['tos']['start_date']) < time() - 60*60*24) {
			form_set_error('tos][start_date', t('Start date must be in the future.'));
		}
	}
}

function sirius_worker_node_tab_schedule_tos_addedit_form_submit($form, &$form_state) {
	$files = array();
  $file = $form_state['stosage']["attach"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['stosage']["attach"]);
		unset($file['destination']);
		unset($file['uri']);
		$files[] = $file;
  }

	$node = node_load($form_state['nid']);
	$status = $form_state['values']['tos']['status'];
	$status_options = sirius_worker_schedule_tos_status_options();
	$tos_node = node_load($form_state['tos_nid']);
	$title = $node->title . ': ' . $form_state['values']['tos']['start_date'] . ' - ' . $form_state['values']['tos']['end_date'];


	if ($tos_node) {
		$json = sirius_jsonfield_util_node_json_get($tos_node);
		$old_status = $json['tos']['status'];
		$json['tos'] = $form_state['values']['tos'];
		sirius_jsonfield_util_node_json_set($tos_node, $json);
		$tos_node->field_sirius_summary['und'][0]['value'] = $form_state['values']['tos']['summary'];
		$tos_node->field_sirius_type['und'][0]['value'] = $status;
		if ($files) { $tos_node->field_sirius_attachments['und'] = $files; }
		$tos_node->title = $title;
		sirius_jsonfield_log($tos_node, 'update', 'Time-off request updated.');
		node_save($tos_node);
		drupal_set_message("Time-off request edited.");

		if ($old_status && $old_status != $status) {
			$old_status_name = $status_options[$old_status];
			$new_status_name = $status_options[$status];

			$body = "The time-off request below has been changed from $old_status_name to $new_status_name.<br /><hr>";
			$body .= sirius_worker_node_tab_schedule_tos_view_email($node, $tos_node);
			$sms = drupal_html_to_text($body);
			$subject = "Time-off request status change: $new_status_name";

			sirius_worker_notify(
				$node, 
				array(
					'subject' => $subject, 
					'body' => $body,
					'sms' => $sms,
				)
			);
		}

	} else {
		$json = array();
		$json['tos'] = $form_state['values']['tos'];
		$summary = $form_state['values']['tos']['summary'];
		$category = 'tos';
		$handlers = array($node->nid);
		if ($form_state['values']['tos']['dispatch_nid']) {
			$handlers[] = $form_state['values']['tos']['dispatch_nid'];
		}
		$tos_node = sirius_log($category, $status, $summary, $summary, NULL, $handlers, $files, $json);
		$tos_node->title = $title;
		sirius_jsonfield_log($tos_node, 'create', 'Time-off request created.');
		node_save($tos_node); 
		drupal_set_message(t('Time-off request added.'));

		// Get the notification emails from the member statuses
		$notify_emails = array();
		$ms_tids = sirius_fieldvals($node, 'field_sirius_member_status', 'tid');
		$ms_terms = taxonomy_term_load_multiple($ms_tids);
		$notify_emails = array();
		foreach ($ms_terms as $ms_tid => $ms_term) {
			$json = sirius_jsonfield_util_term_json_get($ms_term);
			if ($json['tos']['notify']) {
				foreach ($json['tos']['notify'] as $email) {
					if ($email) {$notify_emails[$email] = TRUE; }
				}
			}
		}

		// Also add the notification emails from the job
		$dispatch_nid = $form_state['values']['tos']['dispatch_nid'];
		if ($dispatch_nid) {
			$dispatch_node = node_load($dispatch_nid);
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$job_emails = sirius_fieldvals($job_node, 'field_sirius_emails', 'value');
			foreach ($job_emails as $job_email) {
				if ($job_email) { $notify_emails[$job_email] = TRUE; }
			}
		}

		$subject = "Time-off request submitted by " . $node->title;
		$body = "A new time-off request has been submitted<br /><hr>";
		$body .= sirius_worker_node_tab_schedule_tos_view_email($node, $tos_node);
		foreach (array_keys($notify_emails) as $notify_email) {
			sirius_email_send(
				array(
					'to' => $notify_email,
					'subject' => $subject,
					'body' => $body,
					'handler_nids' => array($tos_node->nid),
				)
			);
		}
	}

}

function sirius_worker_node_tab_schedule_tos_delete_form($form, &$form_state, $entity, $tos_node) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$form_state['tos_nid'] = $tos_node->nid;

	$form['view'] = array(
		'#markup' => sirius_worker_node_tab_schedule_tos_view($entity, $tos_node)
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Really Delete?'),
	);

	return $form;
}

function sirius_worker_node_tab_schedule_tos_delete_form_submit($form, &$form_state) {
	node_delete($form_state['tos_nid']);
	drupal_set_message(t('Timw-off request deleted.'));
}
