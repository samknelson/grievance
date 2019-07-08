<?php

function sirius_node_tab_dispatch_view($dispatch_node) {
	if (variable_get('sirius_dispatch_view_css', '')) {
		drupal_add_css(variable_get('sirius_dispatch_view_css', ''), 'inline');
	}

	// All users can "access" this page. But only certain users can access the details.
	if (sirius_node_tab_dispatch_worker_view_access_details($dispatch_node)) {
		return drupal_get_form('sirius_node_tab_dispatch_worker_view_form', $dispatch_node);
	} 

	return sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node);
}

function sirius_node_tab_dispatch_worker_view_access_details($dispatch_node) {
	global $user;

	// Admins can get in
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employers can get in. Though they might not see everything.
	if (user_access('sirius dispatch employer')) { 
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$employer_node = sirius_employer_get($user->uid);
		if ($employer_node->nid == $job_node->field_grievance_shop['und'][0]['target_id']) { return TRUE; }
	}

	// Access token gets us in
	if (sirius_aat_check($dispatch_node)) { return TRUE; }

	// This user must be a worker
	if (!$user) { return FALSE; }
	if (!$user->uid) { return FALSE; }
	$user_full = user_load($user->uid);
	$worker_nid = $user_full->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return FALSE; }

	// The worker must be associated with this dispatch, and it must have a status besides "requested"
	if ($worker_nid == $dispatch_node->field_sirius_worker['und'][0]['target_id'] && $dispatch_node->field_sirius_dispatch_status['und'][0]['value'] != 'requested') { return TRUE; }

	return FALSE;
}


/* ***************************************
 * Verify Only
 * ***************************************/

function sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node) {
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);

	drupal_set_title("Dispatch Status: $status");

	$form['qr'] = array(
		'#type' => 'markup',
		'#markup' => sirius_qr_url(),
	);

	$html .= '<section class="field field-name-dispatch-valid field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'This dispatch is:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . 'Valid' . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-id field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'Dispatch ID:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . $dispatch_node->nid . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-' . $status . ' field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'Dispatch Status:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . $status . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-signin field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= "To see the dispatch details, please " . l('sign in.', 'user/login', array('query' => array('destination' => current_path())));
	$html .= '</h2></section>';

	return $html;
}

/* ***************************************
 * Full
 * ***************************************/

function sirius_node_tab_dispatch_worker_view_form($form, &$form_state, $dispatch_node) {
	drupal_set_title("Dispatch #" . $dispatch_node->nid);
	$form_state['dispatch_node'] = $dispatch_node;
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_policy = sirius_dispatch_job_policy($job_node);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$facility_node = node_load($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);

	// Get our permission type. We might have multiple. We're a worker if we either are logged in, or if we have an access token.
	if (user_access('sirius dispatch employer')) { $is_employer = TRUE; }
	if (user_access('sirius dispatch staff')) { $is_staff = TRUE; }
	if (user_access('sirius dispatch worker')) { $is_worker = TRUE; }
	if (sirius_aat_check($dispatch_node)) { $is_worker = TRUE; }

	if (variable_get('sirius_dispatch_view_print_header', '')) { $form['print_header'] = array('#markup' => '<div class="sirius_dispatch_view_print_header">' . variable_get('sirius_dispatch_view_print_header', '') . '</div>'); }
	if (variable_get('sirius_dispatch_view_screen_header', '')) { $form['screen_header'] = array('#markup' => '<div class="sirius_dispatch_view_screen_header">' . variable_get('sirius_dispatch_view_screen_header', '') . '</div>'); }

	$actions = array(NULL => t('-- Select --'));
	if ($is_staff || $is_worker) {
		// @todo: Can only accept or decline if you are not UEB!
		if (sirius_dispatch_accept_possible($dispatch_node)['success']) { $actions['accept'] = t('Accept this dispatch'); }
		if (sirius_dispatch_decline_possible($dispatch_node)['success']) { $actions['decline'] = t('Decline this dispatch'); }
	}

	if ($is_staff || $is_employer) {
		if (sirius_dispatch_terminate_possible($dispatch_node)['success']) { $actions['terminate'] = t('Layoff this dispatch'); }
	}

	if ($is_staff || $is_worker) {
		if (sirius_dispatch_resign_possible($dispatch_node)['success']) { $actions['resign'] = t('Voluntary layoff this dispatch'); }
	}

	$is_accepted = FALSE;
	$is_accepted = in_array($dispatch_node->field_sirius_dispatch_status['und'][0]['value'], array('accepted', 'accepted_secondary', 'bullpen', 'resigned', 'terminated'));

	if (count($actions) > 1) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => $actions,
			'#prefix' => '<div class="sirius_noprint">',
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
			'#suffix' => '</div>',
		);
	}

	$form['dispatch_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_dispatch_section">');
	$form['dispatch_section_qr'] = array('#markup' => sirius_qr_url());
	$form['dispatch_section_a'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_dispatch_status', t('Dispatch Status')));
	$form['dispatch_section_b'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_id', t('Request ID')));
	$form['dispatch_section_c'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_dispatch_cbn', t('Call by Name?')));
	$form['dispatch_section_d'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_payrate', t('Pay Rate')));
	$stupid_html = '<section class="field field-name-nid field-label-inline clearfix view-mode-full"><h2 class="field-label">Report #:&nbsp;</h2><div class="field-items"><div class="field-item even">' . $dispatch_node->nid . '</div></div></section>';
	$form['dispatch_section_e'] = array('#markup' => $stupid_html);
	$form['dispatch_section_close'] = array('#markup' => '</div>');

	$form['worker_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_worker_section">');
	$form['worker_section_g'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_headshot'));
	$form['worker_section_a'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_name', t('Worker Name')));
	$form['worker_section_b'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_phone', t('Phone')));
	$form['worker_section_c'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_email', t('Email')));
	$form['worker_section_d'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_id', t('Dispatch ID')));
	$form['worker_section_e'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_id2', t('Titan ID')));
	$form['worker_section_f'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_ssn', t('SSN')));

	// Need to fake the worker node -- show only the member status(es) associated with the industry of this job.
	$job_industry_tids = sirius_fieldvals($job_node, 'field_sirius_industry', 'tid');
	$member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	$member_status_terms = taxonomy_term_load_multiple($member_status_tids);
	$new_field = array();
	foreach ($member_status_terms as $status_tid => $status_term) {
		$status_industry_tids = sirius_fieldvals($status_term, 'field_sirius_industry', 'tid');
		if (array_intersect($job_industry_tids, $status_industry_tids)) {
			$new_field['und'][]['tid'] = $status_tid;
		}
	}
	$orig_field = $worker_node->field_sirius_member_status;
	$worker_node->field_sirius_member_status = $new_field;
	$form['worker_section_h'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_member_status'));	
	$worker_node->field_sirius_member_status = $orig_field;
	$form['worker_section_close'] = array('#markup' => '</div>');


	$form['job_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_section">');

	$form['job_employer_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_employer_section">');
	$form['job_employer_section_a'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_grievance_shop'));
	$form['job_employer_section_close'] = array('#markup' => '</div>');

	$form['job_location_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_location_section">');

	$job_tags = sirius_fieldvals($job_node, 'field_sirius_dispatch_job_tags', 'tid');
	if (in_array(sirius_get_special_tid('sirius_dispatch_job_tags|ASAP'), $job_tags)) {
		$stupid_html = '<section class="field field-name-datetime field-label-inline clearfix view-mode-full"><h2 class="field-label">Report Date</h2><div class="field-items"><div class="field-item">As Soon As Possible</div></div></section>';
		$form['job_location_section_a'] = array('#markup' => $stupid_html);
	} else {
		$form['job_location_section_a'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_datetime'));
	}

	if ($dispatch_node->field_sirius_dispatch_status['und'][0]['value'] == 'accepted_secondary' && $job_policy['workers']['must_be_rollover'] == 'Yes') {
		$rollover_log_nid = sirius_dispatch_rollover_nid($employer_node->nid, $worker_node->nid, sirius_date_ts($dispatch_node, 'field_sirius_datetime'));
		$rollover_log_node = node_load($rollover_log_nid);
		if ($rollover_log_node) {
			$json = json_decode($rollover_log_node->field_sirius_json['und'][0]['value'], TRUE);
			$rollover_end_ts = strtotime($json['end_date']);
			if ($rollover_end_ts > 1) {
				$rollover_end_date = gmdate('l, F j, Y', $rollover_end_ts);
				$stupid_html = '<section class="field field-name-datetime field-label-inline clearfix view-mode-full"><h2 class="field-label">Rollover End Date</h2><div class="field-items"><div class="field-item">' . $rollover_end_date . '</div></div></section>';
				$form['job_location_section_aa'] = array('#markup' => $stupid_html);
			}
		}
	}

	if ($is_accepted) {
		$form['job_location_section_b'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_facility'));
		$form['job_location_section_c'] = array('#markup' => sirius_dispatch_view_render_field($facility_node, 'field_sirius_address'));
		$form['job_location_section_d'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_hall'));
		$form['job_location_section_e'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_address_notes'));
		$form['job_location_section_f'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_address_parking'));
	}
	$form['job_location_section_close'] = array('#markup' => '</div>');

	$form['job_detail_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_detail_section">');
	$form['job_detail_section_a'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_skill'));
	$form['job_detail_section_b'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_grievance_departments'));
	$form['job_detail_section_c'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_job_type'));
	$form['job_detail_section_d'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_notes'));
	$form['job_location_section_close'] = array('#markup' => '</div>');

	$form['job_section_close'] = array('#markup' => '</div>');

	if (variable_get('sirius_dispatch_view_print_footer', '')) { $form['print_footer'] = array('#markup' => '<div class="sirius_dispatch_view_print_footer">' . variable_get('sirius_dispatch_view_print_footer', '') . '</div>'); }
	if (variable_get('sirius_dispatch_view_screen_footer', '')) { $form['screen_footer'] = array('#markup' => '<div class="sirius_dispatch_view_screen_footer">' . variable_get('sirius_dispatch_view_screen_footer', '') . '</div>'); }

	return $form;
}

function sirius_node_tab_dispatch_worker_view_form_submit($form, &$form_state) {
	$dispatch_node = $form_state['dispatch_node'];
	$action = $form_state['values']['action'];
	if ($action == 'accept') { $result = sirius_dispatch_accept($dispatch_node); }
	if ($action == 'decline') { $result = sirius_dispatch_decline($dispatch_node); }
	if ($action == 'terminate') { $result = sirius_dispatch_terminate($dispatch_node); }
	if ($action == 'resign') { $result = sirius_dispatch_resign($dispatch_node); }

	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message(t('Error: ') . $result['msg'], 'error');
	}
}


function sirius_dispatch_view_render_field($node, $field_name, $title = '') {
	if (!$node) { return ''; }
	$mode = 'full';
	if ($field_name == 'field_sirius_ssn') { $mode = 'sirius_worker'; }
	$renderable = field_view_field('node', $node, $field_name, $mode);
	if ($title) {
		$renderable['#title'] = $title;
	}
	return render($renderable);
}

///////////////////////////////////////////////////////////////////
// Admin
///////////////////////////////////////////////////////////////////

function sirius_dispatch_view_admin() {
  $form = array();

  $var = 'sirius_dispatch_view_print_header';
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Print Header'),
    '#default_value' => variable_get($var, ''),
  );

  $var = 'sirius_dispatch_view_screen_header';
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Screen Header'),
    '#default_value' => variable_get($var, ''),
  );

  $var = 'sirius_dispatch_view_print_footer';
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Print Footer'),
    '#default_value' => variable_get($var, ''),
  );

  $var = 'sirius_dispatch_view_screen_footer';
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Screen Footer'),
    '#default_value' => variable_get($var, ''),
  );

  $var = 'sirius_dispatch_view_css';
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View CSS'),
    '#default_value' => variable_get($var, ''),
  );

  return system_settings_form($form);
}