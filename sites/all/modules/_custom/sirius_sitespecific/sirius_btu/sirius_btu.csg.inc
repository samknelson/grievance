<?php

function sirius_btu_csg_notify_scan($mode = 'test', $force = FALSE) {
	return Sirius_Btu_Csg::getInstance()->notify_scan($mode, $force);
}

class Sirius_Btu_Csg extends Sirius_Singleton {
	public function access($action, $context = array()) {
		return Sirius_Btu_Csg_Access::getInstance()->access($action, $context);
	}

	public function notify_scan($mode = 'test', $force = FALSE) {
		$emails = $this->notify_emails();
		if (!$emails) {
			sirius_minilog("No valid notification emails found. Nothing to do.", 'warning');
			return array('success' => TRUE);
		}

		$search_params['start_ts'] = strtotime(date('Y-m-d 00:00:00', strtotime('-1 day')));
		$search_params['end_ts'] = strtotime(date('Y-m-d 23:59:59', strtotime('-1 day')));
		sirius_minilog("Notifying class size grievances between " . date('Y-m-d H:i', $search_params['start_ts']) . " and " . date('Y-m-d H:i', $search_params['end_ts']));
		$csg_nodes = $this->search($search_params);
		if (!$csg_nodes) {
			sirius_minilog("No new class size grievances found.");
			return array('success' => TRUE, 'csg_count' => 0, 'email_count' => 0);
		}

		$count = count($csg_nodes);

		global $base_url;
		$email_params = array();
		$email_params['subject'] = "BTU - Class size grievances received on  " . date('Y-m-d', $search_params['start_ts']);
		$email_params['body'] = 'The following class size grievances have been received:<br />';
		$email_params['body'] .= '<hr>';
		foreach ($csg_nodes as $csg_nid => $csg_node) {
			$csg_json = sirius_json_get($csg_node);
			$email_params['body'] .= $csg_json['csg']['first_name'] . ' ' . $csg_json['csg']['last_name'] . ' - ' . $csg_nid;
			$email_params['body'] .= '<br />';
		}
		$email_params['body'] .= '<br />';
		$email_params['body'] .= "Please sign in at $base_url/user to view the grievances.";

		foreach ($emails as $email) {
			++$count;
			sirius_minilog("Sending a notification email to [$email].");
			$email_params['to'] = $email;
			if ($mode == 'live') {
				$result = sirius_email_send($email_params);
				if (!$result['success']) {
					sirius_minilog("... Message failed: $result[msg].", 'error');
				}
			} else {
				sirius_minilog("... (Skipping send because mode = $mode)");
			}
		}

		return array('success' => TRUE, 'count' => $count);
	}

	public function notify_emails() {
		$input = sirius_domain_variable_get('sirius_btu_csg_notify_emails', '');
		$candidates = preg_split('/\n/', $input);

		$result = array();
		foreach ($candidates as $candidate) {
			$candidate = trim($candidate);
			if (!sirius_email_validate($candidate)) { continue; }
			$result[] = $candidate;
		}

		return $result;
	}

	public function switch_into_main_domain() {
		$domain_nid = variable_get('sirius_btu_csg_main_domain_nid');
		if (!$domain_nid) { return; }
		sirius_domain($domain_nid);
	}

	public function form($form, &$form_state, $csg_node = NULL, $options = array()) {
		if ($csg_node) { $form_state['csg_nid'] = $csg_node->nid; }

		$form_state['options'] = $options;

		$form['#tree'] = TRUE;

		$is_staff = $this->access('staff');

		$form['csg']['bps_id'] = array(
			'#title' => 'BPS ID',
			'#type' => 'textfield',
			'#size' => 12,
			'#required' => TRUE,
		);

		$form['csg']['first_name'] = array(
			'#title' => 'First Name',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['last_name'] = array(
			'#title' => 'Last Name',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['phone'] = array(
			'#title' => 'Phone',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['email'] = array(
			'#title' => 'Non-BPS Email',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$employers = sirius_employer_list();
		$form['csg']['employer_nid'] = array(
			'#title' => 'School',
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $employers,
		);

		$form['csg']['principal'] = array(
			'#title' => 'Principal/Headmaster',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['role'] = array(
			'#title' => 'What is your role?',
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => array(
				NULL => t('-- Select --'),
				'Elementary Classroom Teacher' => t('Elementary Classroom Teacher'),
				'Elementary Teacher Specialist' => t('Elementary Teacher Specialist'),
				'Secondary Teacher' => t('Secondary Teacher'),
			),
		);

		$form['csg']['class_type'] = array(
			'#title' => 'Type of Class',
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => array(
				NULL => t('-- Select --'),
				'Regular Ed' => t('Regular Ed'),
				'Inclusion' => t('Inclusion'),
				'SEI/ESL/Bilingual (no para)' => t('SEI/ESL/Bilingual (no para)'),
				'SEI/ESL/Bilingual (with para)' => t('SEI/ESL/Bilingual (with para)'),
				'Substantially Separate Speacial Ed (no para)' => t('Substantially Separate Speacial Ed (no para)'),
				'Substantially Separate Speacial Ed (with para)' => t('Substantially Separate Speacial Ed (with para)'),
			),
		);

		$form['csg']['course'] = array(
			'#title' => 'Course',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['section'] = array(
			'#title' => 'Section',
			'#type' => 'textfield',
			'#size' => 30,
			'#required' => TRUE,
		);

		$form['csg']['count'] = array(
			'#title' => 'Number of Students',
			'#type' => 'textfield',
			'#size' => 5,
			'#required' => TRUE,
		);

		$form['csg']['comments'] = array(
			'#title' => 'Comments',
			'#type' => 'textarea',
			'#required' => TRUE,
		);

		if ($is_staff) {
			$form['csg-admin'] = array(
				'#type' => 'fieldset',
				'#title' => 'Tracking',
			);

			$form['csg-admin']['status'] = array(
				'#title' => t('Status'),
				'#type' => 'select',
				'#options' => $this->status_options(),
				'#default_value' => $this->get_status($csg_node),
				'#required' => TRUE,
			);

			$form['csg-admin']['admin_notes'] = array(
				'#title' => t('Admin Notes'),
				'#type' => 'textarea',
			);
		}

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);

		if ($csg_node) {
			$csg_json = sirius_json_get($csg_node);
			sirius_jsonfield_util_set_defaults($form['csg'], $csg_json['csg']);
			sirius_jsonfield_util_set_defaults($form['csg-admin'], $csg_json['csg-admin']);
		}

		return $form;
	}

	public function form_validate($form, &$form_state) {
		if (!sirius_phone_validate($form_state['values']['csg']['phone'])) {
			form_set_error('csg][phone', 'Please enter a valid phone number.');
		}

		if (!sirius_email_validate($form_state['values']['csg']['email'])) {
			form_set_error('csg][email', 'Please enter a valid email address.');
		}

		if (!sirius_worker_get_by_id($form_state['values']['csg']['bps_id'], TRUE)) {
			form_set_error('csg][bps_id', 'Please enter a valid BPS ID.');
		}
	}

	public function form_submit($form, &$form_state) {
		$csg_node = node_load($form_state['csg_nid']);

		$options = $form_state['options'];

		if (!$csg_node) {
			$csg_node = sirius_log(
				'btu:csg',
				'open',
				'Class Size Grievance',
				'Class Size Grievance',
				NULL,
				array()
			);
		}

		$csg_json = sirius_json_get($csg_node);
		$csg_json['csg'] = $form_state['values']['csg'];
		$is_staff = $this->access('staff');
		if ($is_staff) {
			$csg_node->field_sirius_type['und'][0]['value'] = $form_state['values']['csg-admin']['status'];
			$csg_json['csg-admin'] = $form_state['values']['csg-admin'];
		}

		sirius_json_set($csg_node, $csg_json);

		$worker_nid = sirius_worker_get_by_id($form_state['values']['csg']['bps_id'], TRUE);
		$old_worker_nids = sirius_log_handlers($csg_node, 'sirius_worker');
		foreach ($old_worker_nids as $old_worker_nid => $ignore) { sirius_log_handler_clear($csg_node, $old_worker_nid); }
		sirius_log_handler_set($csg_node, $worker_nid);

		$csg_node->title = $this->title($csg_node);

		node_save($csg_node);

		drupal_set_message("Grievance saved.");

		if ($options['send_confirmation_msg']) {
			$params = array();
			$params['to'] = $form_state['values']['csg']['email'];
			$params['subject'] = "Thank you, " . $form_state['values']['csg']['first_name'] . " for your Class Size Grievance submissions.";
			$params['body'] = sirius_richtext_handle_input(variable_get('sirius_btu_csg_public_confirmation_msg_top', NULL));
			$params['body'] .= sirius_jsonfield_util_render_all_values_new($form['csg'], $form_state['values']['csg'], array());
			$params['body'] .= sirius_richtext_handle_input(variable_get('sirius_btu_csg_public_confirmation_msg_bottom', NULL));
			$params['handler_nids'] = array($worker_nid, $csg_node->nid);
			sirius_email_send($params);
		}

		return $csg_node;
	}

	public function title($csg_node) {
		$title = "Class Size Grievance #";
		$title .= $csg_node->nid;
		$worker_node = sirius_log_handler($csg_node, 'sirius_worker');
		if ($worker_node) {
			$title .= ' - ' . $worker_node->title;
		}
		return $title;
	}

  public function status_options() {
    return array(
      'open' => 'Open',
      'resolved' => 'Resolved',
    );
  }

  public function get_status($csg_node) {
    return $csg_node->field_sirius_type['und'][0]['value'];
  }

  public function search($search_params) {
    $domain_nid = sirius_domain_nid();

    $sql_args = array();
    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_log' ";
    $sql .= "and field_sirius_category_value = 'btu:csg' ";
    if ($search_params['worker_nid']) {
      $sql .= "and field_sirius_log_handler_target_id = :worker_nid ";
      $sql_args[':worker_nid'] = $search_params['worker_nid'];
    }
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is null ";
    }

    if ($search_params['start_ts']) {
			$sql .= "and created >= :start_ts ";
			$sql_args[':start_ts'] = $search_params['start_ts'];
    }

    if ($search_params['end_ts']) {
			$sql .= "and created <= :end_ts ";
			$sql_args[':end_ts'] = $search_params['end_ts'];
    }

    $limit = $search_params['limit'];
    if (!$limit) { $limit = 1000; }
    $offset = $search_params['offset'];
    if (!$offset) { $offset = 0; }
    $sql .= "order by nid desc ";
    $sql .= "limit $limit offset $offset";

    $stmt = sirius_sql_query($sql, $sql_args);
    $nids = array();
    while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }
    if ($search_params['nids_only']) { return $nids; }
    return node_load_multiple($nids);
  }

  public function render_list($csg_nodes, $options = array()) {
  	if (!$csg_nodes) {
  		return '<p>' . t('No records found.') . '</p>';
  	}

  	$access = Sirius_Btu_Csg_Access::getInstance();
  	$status_options = $this->status_options();
  	foreach ($csg_nodes as $csg_node) {
  		$worker_node = sirius_log_handler($csg_node, 'sirius_worker');

  		$row = array();

  		if (!$options['hide_tools']) {
		  	$tools = array();
		  	$paths = array();
		  	$paths['node/' . $csg_node->nid . '/sirius_btu_csg_view'] = 'View';
		  	$paths['node/' . $csg_node->nid . '/sirius_btu_csg_edit'] = 'Edit';
		  	if (!$options['hide_worker']) { $paths['node/' . $worker_node->nid] = 'Worker'; }
		  	$tools = array();
		  	foreach ($paths as $path => $label) {
		  		if (!drupal_valid_path($path)) { continue; }
		  		$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
		  	}
		  }

	  	if (!$options['hide_worker']) { $row[] = $worker_node->title; }
	  	$row[] = date('m/d/Y', $csg_node->created);
  		$row[] = $status_options[$this->get_status($csg_node)];
  		$row[] = $worker_node->title;
  		if (!$options['hide_tools']) { $row[] = join('', $tools); }
  		$rows[] = $row;
  	}


  	$header = array();

  	if (!$options['hide_worker']) { $header[] = t('Member'); }
  	$header[] = t('Submitted');
  	$header[] = t('Status');
  	$header[] = t('Member Name');
  	if (!$options['hide_tools']) { $header[] = t('Tools');  }

  	return sirius_table_render($header, $rows);
  }

  public function export($csg_nodes, $options = array()) {
  	$access = Sirius_Btu_Csg_Access::getInstance();
  	$status_options = $this->status_options();

  	$header = array();
		$header['nid'] = t('Grievance ID'); 
		$header['created'] = t('Created'); 
		$header['bps_id'] = t('BPS ID');
		$header['first_name'] = t('First Name');
		$header['last_name'] = t('Last Name');
		// $header['phone'] = t('Phone');
		// $header['email'] = t('Email');
		$header['employer_nid'] = t('School');
		$header['principal'] = t('Principal');
		$header['role'] = t('Role');
		$header['class_type'] = t('Class Type');
		$header['course'] = t('Course / Homeroom');
		$header['section'] = t('Section');
		$header['count'] = t('Count');
		$header['comments'] = t('Comments');

    // drupal_add_http_header('Content-Type', 'text/plain');
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=class_size_grievances.csv');
    $fp = fopen('php://output', 'w');
    fputcsv($fp, array_values($header));;

  	foreach ($csg_nodes as $csg_node) {
  		$csg_json = sirius_json_get($csg_node);
  		$row = array();
  		foreach ($header as $key => $value) {
  			$row[$key] = $csg_json['csg'][$key];
  		}
  		$row['nid'] = $csg_node->nid;
  		$row['created'] = date('m/d/Y h:i a', $csg_node->created);
  		$row['employer_nid'] = sirius_node_title($row['employer_nid']);
  		fputcsv($fp, array_values($row));
  	}
  	drupal_exit();

  	return sirius_table_render($header, $rows);
  }

  public function render_view($csg_node, $options = array()) {
		$csg_json = sirius_json_get($csg_node);

  	$form = $this->form(array(), $form_state);

  	$html .= '<div class="sirius_btu_csg_view">';

  	$html .= '<h2>' . t('Grievance Information') . '</h2>';
  	$html .= sirius_jsonfield_util_render_all_values_new($form['csg'], $csg_json['csg'], array());

  	if ($this->access('staff')) {
		  $html .= '<h2>' . t('Tracking') . '</h2>';
  		$html .= sirius_jsonfield_util_render_all_values_new($form['csg-admin'], $csg_json['csg-admin'], array());
  	}

  	$html .= '</div>';
  	return $html;
  }

}

class Sirius_Btu_Csg_Access extends Sirius_Access {
  public $type = 'btu:csg';
  public $name = 'Boston Teachers\' Union Class Size Grievances';
  public $description = '';

  public function access($action, $context = array()) {
  	// Must be a csg
  	if ($context['csg_node']) {
  		if ($context['csg_node']->field_sirius_category['und'][0]['value'] != 'btu:csg') { return FALSE; }
  	}

    global $user;

    $actions = array(
      // No parameters
      'admin',
      'list',

      // csg_node
      'view',
      'edit',
    );

    // Who am I?
    // We're doing something weird here. BTU doesn't really have "employers". There is one employer, BPS, who 
    // manages *all* the schools. We don't expect individual schools to log in to see their grievances; it's handled by the system.
    // So: we have a special permission called "BPS". We really should 
    if (user_access('sirius admin') || user_access('sirius trust admin')) {
      $is_admin = $is_staff = $is_bps = TRUE;
    } else if (user_access('sirius trust staff')) {
      $is_staff = $is_bps = TRUE;
    } else if (user_access('sirius employer user manager')) {
    	$is_bps = TRUE;
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // Staff can do anything
    if ($is_staff) { return TRUE; }

    // BPS can access "list" and "view"
    if (in_array($action, array('list', 'view'))) {
    	if ($is_bps) { return TRUE; }
    }

    return $this->deny("Permission denied for $action", $action, $context);
  }
}

